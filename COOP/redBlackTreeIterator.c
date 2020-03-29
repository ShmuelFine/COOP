#include "redBlackTreeIterator.h"
//#include "redBlackTree.h"


DEF_CTOR(redBlackTreeIterator, node * node_ptr)
{
	_this->nodePtr = node_ptr;
}
END_CTOR

DEF_DTOR(redBlackTreeIterator)
{
    //nothing needed here
}
END_DTOR

MEM_FUN_IMPL(redBlackTreeIterator, increment)
{
    //if (_Ptr->_Right->_Isnil) { // climb looking for right subtree
    //    _Nodeptr _Pnode;
    //-while the parent is not the header and nodePtr is the right child
    //    while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Right) {
    //        _Ptr = _Pnode; // ==> parent while right subtree
    //    }

    //-parent last time nodePtr is right subtree, or head if nodePtr was the biggest
    //    _Ptr = _Pnode; // ==> parent (head if end())
    //}
    //else {
    //    _Ptr = _Mytree::_Min(_Ptr->_Right); // ==> smallest of right subtree
    //}

    //if nodePtr does not have a right child, set it to its parent or grandparent
    if (_this->nodePtr->right == NULL)
    {
        node* pNode = NULL;
        
        //while the parent is not the header and nodePtr is the right child
        while (!(pNode = _this->nodePtr->parent)->isHead && _this->nodePtr == pNode->right)
            _this->nodePtr = pNode; 

        _this->nodePtr = pNode; //equal to the parent (header if end())
    }
    //if nodePtr has a right child, find the smallest value in the right subtree
    else
    {
        node* pNode = _this->nodePtr->right;

        while (pNode->left != NULL)
            pNode = pNode->left;
        _this->nodePtr = pNode;
    }
}
END_FUN;


MEM_FUN_IMPL(redBlackTreeIterator, decrement)
{
    //if (_Ptr->_Isnil) {
    //    _Ptr = _Ptr->_Right; // end() ==> rightmost
    //}
    //else if (_Ptr->_Left->_Isnil) { // climb looking for left subtree
    //    _Nodeptr _Pnode;
    //    while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Left) {
    //        _Ptr = _Pnode; // ==> parent while left subtree
    //    }

    //    if (!_Ptr->_Isnil) { // decrement non-begin()
    //        _Ptr = _Pnode; // ==> parent if not head
    //    }
    //}
    //else {
    //    _Ptr = _Mytree::_Max(_Ptr->_Left); // ==> largest of left subtree
    //}

    //if this is the head node, go to the node with the largest value
    if (_this->nodePtr->isHead)
        _this->nodePtr = _this->nodePtr->right; 
  
    //if nodePtr does not have a left child
    else if (_this->nodePtr->left == NULL)
    {
        node* pNode = NULL;
        while (!(pNode = _this->nodePtr->parent)->isHead && _this->nodePtr == pNode->left) 
                 _this->nodePtr = pNode; // ==> parent while left subtree

        if (!_this->nodePtr->isHead)
            _this->nodePtr = pNode;
    }
    else {
        node* pNode = _this->nodePtr->left;

        while (pNode->right != NULL)
            pNode = pNode->right;
        _this->nodePtr = pNode;
    }
}
END_FUN;

MEM_FUN_IMPL(redBlackTreeIterator, equals, redBlackTreeIterator other, bool* retVal)
{
	*retVal = (_this->nodePtr == other.nodePtr);
}
END_FUN;

MEM_FUN_IMPL(redBlackTreeIterator, getContentsOf, node** retVal)
{
	*retVal = _this->nodePtr;
}
END_FUN;


INIT_CLASS(redBlackTreeIterator)
BIND(redBlackTreeIterator, increment);
BIND(redBlackTreeIterator, decrement);
BIND(redBlackTreeIterator, equals);
BIND(redBlackTreeIterator, getContentsOf);
END_INIT_CLASS

