
#include "Cache.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define BLOCK_METADATA_SIZE (3*sizeof(MEM_SIZE_T))

#define BLOCK_SIZE(i) *((MEM_SIZE_T*)(_this->buffer + i))
#define BLOCK_SIZE_WITH_METADATA(i) (BLOCK_METADATA_SIZE + BLOCK_SIZE(i))

#define JUMP_TILL_NEXT_BLOCK(i) *((MEM_SIZE_T*)(_this->buffer + i + sizeof(MEM_SIZE_T)))
#define NEXT_BLOCK_LOCATION(i) (i + JUMP_TILL_NEXT_BLOCK(i))

#define JUMP_TILL_PREV_BLOCK(i) *((MEM_SIZE_T*)(_this->buffer + i + 2*sizeof(MEM_SIZE_T)))
#define PREV_BLOCK_LOCATION(i) (i - JUMP_TILL_PREV_BLOCK(i))

#define BLOCK_MEM_START(i) (_this->buffer + i + BLOCK_METADATA_SIZE )
#define BLOCK_MEM_END(i) (_this->buffer + i + BLOCK_METADATA_SIZE + BLOCK_SIZE(i))

#define END_OF_BLOCKS_IDX (_this->size - BLOCK_METADATA_SIZE)
void print_mem_metadata(char* buf, long idx)
{
	MEM_SIZE_T* ibuf = (MEM_SIZE_T*)(buf + idx);
	printf("B[%d]:Size=%d,Next=%d,Prev=%d\n", idx, ibuf[0], ibuf[1], ibuf[2]);
}

DEF_DERIVED_CTOR(InMemoryCache, iCache, MEM_SIZE_T size) SUPER ME
{
	_this->size = size;
	_this->buffer = (char*)malloc(sizeof(char) * size);
	if (NULL == _this->buffer)
	{
		THROW("Could not allocate buffer");
	}
	memset(_this->buffer, 0, _this->size);

	// anchor and suffix: (a.k.a "begin" and "end")
	long anchor_idx = 0, suffix_idx = END_OF_BLOCKS_IDX;
	BLOCK_SIZE(anchor_idx) = 0;
	JUMP_TILL_NEXT_BLOCK(anchor_idx) = suffix_idx - anchor_idx;
	JUMP_TILL_PREV_BLOCK(anchor_idx) = 0;

	BLOCK_SIZE(suffix_idx) = 0;
	JUMP_TILL_NEXT_BLOCK(suffix_idx) = 0;
	JUMP_TILL_PREV_BLOCK(suffix_idx) = suffix_idx - anchor_idx;

}
END_DERIVED_CTOR

DEF_DERIVED_DTOR(InMemoryCache, iCache)
{
	if (_this->buffer)
		free(_this->buffer);
}
END_DERIVED_DTOR

FUN_OVERRIDE_IMPL(InMemoryCache, iCache, AddNewBlock, MEM_SIZE_T num_bytes_to_alloc, void** returned)
{
	*returned = NULL;

	for (long mem_idx = 0; mem_idx < END_OF_BLOCKS_IDX; mem_idx = NEXT_BLOCK_LOCATION(mem_idx))
	{
		char* this_block_end_ptr = BLOCK_MEM_END(mem_idx);
		long this_block_end_idx = this_block_end_ptr - _this->buffer;
		long space_between_blocks = NEXT_BLOCK_LOCATION(mem_idx) - this_block_end_idx;
		if (space_between_blocks >= num_bytes_to_alloc + BLOCK_METADATA_SIZE)
		{
			long new_block_idx = mem_idx + BLOCK_SIZE_WITH_METADATA(mem_idx);
			BLOCK_SIZE(new_block_idx) = num_bytes_to_alloc;
			JUMP_TILL_NEXT_BLOCK(new_block_idx) = NEXT_BLOCK_LOCATION(mem_idx) - (mem_idx + BLOCK_SIZE_WITH_METADATA(mem_idx));
			JUMP_TILL_PREV_BLOCK(new_block_idx) = BLOCK_SIZE_WITH_METADATA(mem_idx);

			JUMP_TILL_NEXT_BLOCK(mem_idx) = BLOCK_SIZE_WITH_METADATA(mem_idx);
			*returned = BLOCK_MEM_START(new_block_idx);
			RETURN;
		}
	}
	THROW_MSG("Could not allocate new block");
}
END_FUN

FUN_OVERRIDE_IMPL(InMemoryCache, iCache, RemoveBlock, void* toDelete)
{
	long mem_idx = (((char*)toDelete) - _this->buffer) - BLOCK_METADATA_SIZE;
	
	// prev block should now point to next block: (anchor is never removed)
	long prev_block_idx = PREV_BLOCK_LOCATION(mem_idx);
	long next_block_idx = NEXT_BLOCK_LOCATION(mem_idx);

	JUMP_TILL_PREV_BLOCK(next_block_idx) = 
		JUMP_TILL_NEXT_BLOCK(prev_block_idx) = next_block_idx - prev_block_idx;
}
END_FUN


INIT_DERIVED_CLASS(InMemoryCache, iCache);
BIND_OVERIDE(InMemoryCache, iCache, AddNewBlock);
BIND_OVERIDE(InMemoryCache, iCache, RemoveBlock);
END_INIT_CLASS(InMemoryCache)
