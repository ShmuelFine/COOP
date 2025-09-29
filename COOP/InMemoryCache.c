#include "InMemoryCache.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

enum BLOCK_METADATA_PARTS {BLOCK_SIZE_IDX = 0, JMP_TO_NEXT_IDX, JMP_TO_PREV_IDX, NUM_MD_PARTS};

#define BLOCK_METADATA_SIZE (NUM_MD_PARTS*sizeof(MEM_SIZE_T))

#define BLOCK_SIZE(i)				(*((MEM_SIZE_T*)(void*)(_this->buffer + (size_t)(i) + (size_t)BLOCK_SIZE_IDX*sizeof(MEM_SIZE_T))))
#define BLOCK_SIZE_WITH_METADATA(i) (BLOCK_METADATA_SIZE + BLOCK_SIZE(i))

#define JUMP_TILL_NEXT_BLOCK(i)		(*((MEM_SIZE_T*)(void*)(_this->buffer + (size_t)(i) + (size_t)JMP_TO_NEXT_IDX*sizeof(MEM_SIZE_T))))
#define NEXT_BLOCK_LOCATION(i)		((MEM_SIZE_T)((MEM_SIZE_T)(i) + (MEM_SIZE_T)JUMP_TILL_NEXT_BLOCK(i)))

#define JUMP_TILL_PREV_BLOCK(i)		(*((MEM_SIZE_T*)(void*)(_this->buffer + (size_t)(i) + (size_t)JMP_TO_PREV_IDX*sizeof(MEM_SIZE_T))))
#define PREV_BLOCK_LOCATION(i)		((MEM_SIZE_T)((MEM_SIZE_T)(i) - (MEM_SIZE_T)JUMP_TILL_PREV_BLOCK(i)))

#define BLOCK_MEM_START(i) (_this->buffer + (size_t)(i) + (size_t)BLOCK_METADATA_SIZE)
#define BLOCK_MEM_END(i) (BLOCK_MEM_START(i) + (size_t)BLOCK_SIZE(i))

#define END_OF_BLOCKS_IDX (_this->size - BLOCK_METADATA_SIZE)


DEF_DERIVED_CTOR(InMemoryCache, ICache, MEM_SIZE_T size) SUPER ME
{
	_this->size = size;
	_this->buffer = (char*)malloc(sizeof(char) * size);
	if (NULL == _this->buffer)
	{
		THROW("Could not allocate buffer");
	}
	memset(_this->buffer, 0, _this->size);

	// anchor and suffix: (a.k.a "begin" and "end")
	MEM_SIZE_T anchor_idx = 0, suffix_idx = END_OF_BLOCKS_IDX;
	BLOCK_SIZE(anchor_idx) = 0;
	JUMP_TILL_NEXT_BLOCK(anchor_idx) = suffix_idx - anchor_idx;
	JUMP_TILL_PREV_BLOCK(anchor_idx) = 0;

	BLOCK_SIZE(suffix_idx) = 0;
	JUMP_TILL_NEXT_BLOCK(suffix_idx) = 0;
	JUMP_TILL_PREV_BLOCK(suffix_idx) = suffix_idx - anchor_idx;
}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(InMemoryCache, ICache)
{
	if (_this->buffer)
		free(_this->buffer);
}
END_DERIVED_DTOR


MEM_FUN_IMPL(InMemoryCache, print_block, MEM_SIZE_T offset)
{
	printf("B[%d]:Size=%d,Next=%d,Prev=%d\n", offset,
		BLOCK_SIZE(offset),
		NEXT_BLOCK_LOCATION(offset),
		PREV_BLOCK_LOCATION(offset)
	);
	fflush(stdout);
}
END_FUN

MEM_FUN_IMPL(InMemoryCache, print_all)
{
	printf("***********************\n");
	FOR(MEM_SIZE_T mem_idx = 0; mem_idx < END_OF_BLOCKS_IDX; mem_idx = NEXT_BLOCK_LOCATION(mem_idx))
	{
		MFUN(_this, print_block), mem_idx CALL;
	}END_LOOP;
	printf("***********************\n");
	fflush(stdout);
}
END_FUN;

FUN_OVERRIDE_IMPL(InMemoryCache, ICache, AddNewBlock, MEM_SIZE_T num_bytes_to_alloc, void** returned)
{
	*returned = NULL;
	FOR(MEM_SIZE_T mem_idx = 0; mem_idx < END_OF_BLOCKS_IDX; mem_idx = NEXT_BLOCK_LOCATION(mem_idx))
	{
		char* this_block_end_ptr = BLOCK_MEM_END(mem_idx);
		MEM_SIZE_T this_block_end_idx = (MEM_SIZE_T)(this_block_end_ptr - _this->buffer);
		MEM_SIZE_T space_between_blocks = NEXT_BLOCK_LOCATION(mem_idx) - this_block_end_idx;
		if (space_between_blocks >= num_bytes_to_alloc + BLOCK_METADATA_SIZE)
		{
			MEM_SIZE_T original_next_idx = NEXT_BLOCK_LOCATION(mem_idx);             
			MEM_SIZE_T new_block_idx = mem_idx + BLOCK_SIZE_WITH_METADATA(mem_idx);
			
			BLOCK_SIZE(new_block_idx) = num_bytes_to_alloc;
			JUMP_TILL_NEXT_BLOCK(new_block_idx) = original_next_idx - (mem_idx + BLOCK_SIZE_WITH_METADATA(mem_idx));
			JUMP_TILL_PREV_BLOCK(new_block_idx) = BLOCK_SIZE_WITH_METADATA(mem_idx);

			JUMP_TILL_PREV_BLOCK(original_next_idx) = original_next_idx - new_block_idx; 
			JUMP_TILL_NEXT_BLOCK(mem_idx) = BLOCK_SIZE_WITH_METADATA(mem_idx);
			*returned = BLOCK_MEM_START(new_block_idx);

			RETURN;
		}
	}END_LOOP;
	THROW_MSG("Could not allocate new block");
}
END_FUN

FUN_OVERRIDE_IMPL(InMemoryCache, ICache, RemoveBlock, void* toDelete)
{
	MEM_SIZE_T mem_idx = (MEM_SIZE_T)((((char*)toDelete) - _this->buffer) - BLOCK_METADATA_SIZE);

	// prev block should now point to next block: (anchor is never removed)
	MEM_SIZE_T prev_block_idx = PREV_BLOCK_LOCATION(mem_idx);
	MEM_SIZE_T next_block_idx = NEXT_BLOCK_LOCATION(mem_idx);

	JUMP_TILL_PREV_BLOCK(next_block_idx) = 
		JUMP_TILL_NEXT_BLOCK(prev_block_idx) = next_block_idx - prev_block_idx;
}
END_FUN

FUN_OVERRIDE_IMPL(InMemoryCache, ICache, getTotalFreeBytes, MEM_SIZE_T* out_count)
{
	*out_count = 0;
	FOR(MEM_SIZE_T mem_idx = 0; mem_idx < END_OF_BLOCKS_IDX; mem_idx = NEXT_BLOCK_LOCATION(mem_idx))
	{
		char* this_block_end_ptr = BLOCK_MEM_END(mem_idx);
		MEM_SIZE_T this_block_end_idx = (MEM_SIZE_T)(this_block_end_ptr - _this->buffer);
		MEM_SIZE_T space_between_blocks = NEXT_BLOCK_LOCATION(mem_idx) - this_block_end_idx;

		*out_count += space_between_blocks;
	}END_LOOP;
}
END_FUN


INIT_DERIVED_CLASS(InMemoryCache, ICache);
BIND_OVERIDE(InMemoryCache, ICache, AddNewBlock);
BIND_OVERIDE(InMemoryCache, ICache, RemoveBlock);
BIND_OVERIDE(InMemoryCache, ICache, getTotalFreeBytes);
BIND(InMemoryCache, print_block);
BIND(InMemoryCache, print_all);
END_INIT_CLASS(InMemoryCache)
