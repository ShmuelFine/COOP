#include "List.h"
#include "COOP.h"
#include <stdlib.h>
#include <string.h>

DEF_CTOR(GenericList, MEM_SIZE_T dataTypeSize)
{
    _this->size = 0;
    _this->elementSize = dataTypeSize;
    _this->head = NULL;
    _this->tail = NULL;
}
END_CTOR

DEF_DTOR(GenericList)
{
    ListNode* cur = _this->head;

    WHILE(cur != NULL)
        ListNode* next = cur->next;
    FREE(cur);
    cur = next;
    SCOPE_END;

    _this->head = NULL;
    _this->tail = NULL;
    _this->size = 0;
}
END_DTOR

/* =========================
   Helpers as COOP FUN
   ========================= */

//create a new node
DEF_FUN(GenericList, make_node, const void* src_bytes, ListNode** out_node)
{
    THROW_MSG_UNLESS(!out_node, "Invalid Data");
    size_t bytes = sizeof(ListNode) + _this->elementSize;
    ListNode* nd = (ListNode*)malloc(bytes);
    THROW_MSG_UNLESS(!out_node, "Invalid Data");

    nd->prev = NULL;
    nd->next = NULL;

    IF(src_bytes) {
        memcpy(nd->payload, src_bytes, _this->elementSize);
    }
    END_IF;

    *out_node = nd;
}
END_FUN
