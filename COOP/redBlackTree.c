#include "redBlackTree.h"
#include <stdio.h>

DEF_CTOR(redBlackTree, void(*compFunc)(void*, void*, bool *))
{
	_this->root = NULL;
    _this->size = 0;
    //_this->elementSize = elemSize;

    _this->comparisonFunctionPtr = compFunc;
    
    //construct header node
    NEW(_this->head, node);
    //could be anything
    //_this->head->data = -10;
    _this->head->data = NULL;
    _this->head->parent = _this->head->right = _this->head->left = NULL;
    _this->head->color = 'B';
    _this->head->isHead = true;
}
END_CTOR

//TODO: can I delete the memory with this pointer-nodePtr?
DEF_DTOR(redBlackTree)
{
    FUN(_this, destroyRecursive), _this->root CALL
    DELETE_OBJ(_this->head);
}
END_DTOR

MEM_FUN_IMPL(redBlackTree, destroyRecursive, node * nodePtr)
{
    if (nodePtr != NULL)
    {
        FUN(_this, destroyRecursive), nodePtr->left CALL
        FUN(_this, destroyRecursive), nodePtr->right CALL
        DELETE_OBJ(nodePtr);
    }
}END_FUN;

MEM_FUN_IMPL(redBlackTree, LeftRotate, node * x)
{
    if (!x || !x->right)
        RETURN_NONE;

    //y stored pointer of right child of x
    node* y = x->right;

    //store y's left subtree's pointer as x's right child
    x->right = y->left;

    //update parent pointer of x's right
    if (x->right != NULL)
        x->right->parent = x;

    //update y's parent pointer
    y->parent = x->parent;

    // if x's parent is null make y as root of tree
    if (x->parent == NULL || x->parent->isHead == true)
        (_this->root) = y;
       
        

    // store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    // make x left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}
END_FUN;

MEM_FUN_IMPL(redBlackTree, RightRotate, node* y)
{
    if (!y || !y->left)
        RETURN_NONE;
    node* x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent = y->parent;
    if (x->parent == NULL || x->parent->isHead == true)
        _this->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}END_FUN;

MEM_FUN_IMPL(redBlackTree, insertFixUp, node* pt)
{
    node* parent_pt = NULL;
    node* grand_parent_pt = NULL;

    while ((pt != _this->root) && (pt->color != 'B') &&
        (pt->parent->color == 'R'))
    {
        SCOPE_START;
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {

            node* uncle_pt = grand_parent_pt->right;

            /* Case : 1
               The uncle of pt is also red
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == 'R')
            {
                grand_parent_pt->color = 'R';
                parent_pt->color = 'B';
                uncle_pt->color = 'B';
                pt = grand_parent_pt;
            }

            else
            {
                /* Case : 2
                   pt is right child of its parent
                   Left-rotation required */
                if (pt == parent_pt->right)
                {
                    FUN(_this, LeftRotate), parent_pt CALL
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is left child of its parent
                   Right-rotation required */
                FUN(_this, RightRotate), grand_parent_pt CALL

                char c;
                c = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = c;

                pt = parent_pt;
            }
        }

        /* Case : B
           Parent of pt is right child of Grand-parent of pt */
        else
        {
            node* uncle_pt = grand_parent_pt->left;

            /*  Case : 1
                The uncle of pt is also red
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == 'R'))
            {
                grand_parent_pt->color = 'R';
                parent_pt->color = 'B';
                uncle_pt->color = 'B';
                pt = grand_parent_pt;
            }
            else
            {
                /* Case : 2
                   pt is left child of its parent
                   Right-rotation required */
                if (pt == parent_pt->left)
                {
                    FUN(_this, RightRotate), parent_pt CALL
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }

                /* Case : 3
                   pt is right child of its parent
                   Left-rotation required */
                FUN(_this, LeftRotate), grand_parent_pt CALL
                char c;
                c = parent_pt->color;
                parent_pt->color = grand_parent_pt->color;
                grand_parent_pt->color = c;
                pt = parent_pt;
            }
        }
        END_SCOPE;
    }

    _this->root->color = 'B';
   
}END_FUN;

MEM_FUN_IMPL(redBlackTree, insert, void * data, node ** insertednode, bool * retBool)
{
    //if we prefer code cleanliness to avoiding extra allocation
    // Allocate memory for new node
    //node* z = (node*)malloc(sizeof(node));
    //node* z = NULL;
    //NEW(z, node);
    //z->data = data;
    //z->left = z->right = z->parent = NULL;

     node* z = NULL;
     //used for comparison function ptr return value
     bool compBool = false, compBool1 = false;

    //if root is null make z the root
    if (_this->root == NULL)
    {
        //if we want to allocate only if the element is not already present
        NEW(_this->root, node);
        _this->root->data = data;
        _this->root->left = _this->root->right = NULL;
        _this->root->color = 'B';
        _this->root->isHead = false;
        _this->root->parent = _this->head;

        _this->head->left = _this->head->right = _this->root;

        *insertednode = _this->root;
    }
    else
    {
        node* y = NULL;
        node* x = (_this->root);

        // Follow standard BST insert steps to first insert the node
        while (x != NULL)
        {
            y = x;
            //only allow unique elements
            //equivalence => !comp(a, b) && !comp(b, a).
            //if (data == x->data)
            (*(_this->comparisonFunctionPtr))(data, x->data, &compBool);
            (*(_this->comparisonFunctionPtr))(x->data, data, &compBool1);
            if (!compBool && !compBool1)
            {
                *insertednode = x;
                *retBool = false;
                RETURN_NONE;
            }
                
            //(*functionPtr)(2, 3)
            //data < x->data
            (*(_this->comparisonFunctionPtr))(data, x->data, &compBool);
            if (compBool)
                x = x->left;
            else
                x = x->right;
        }
        //if we want to allocate only if the element is not already present
        
        NEW(z, node);
        z->data = data;
        z->isHead = false;
        z->left = z->right = NULL;

        z->parent = y;
        //if (z->data > y->data)
        (*(_this->comparisonFunctionPtr))(y->data, z->data, &compBool);
        if (compBool)
            y->right = z;
        else
            y->left = z;
        z->color = 'R';

        // call insertFixUp to fix red-black tree's property if it
        // is violated due to insertion.
        FUN(_this, insertFixUp), z CALL
        * insertednode = z;

        //if (z->data > _this->head->right->data)
        (*(_this->comparisonFunctionPtr))(_this->head->right->data, z->data, &compBool);
        if (compBool)
            _this->head->right = z;
    }
    
    *retBool = true;
    _this->size++;
}END_FUN;

MEM_FUN_IMPL(redBlackTree, getRootNode, node ** retRootnode)
{
    *retRootnode = _this->root;
}END_FUN;

//TODO: we need to pass a function ptr for printing if we want to print
//only for int data!
MEM_FUN_IMPL(redBlackTree, inOrderTraversal, node * rootnode)
{
        static int last = 0;
        if (rootnode == NULL || rootnode->isHead == true)
            RETURN_NONE;
        FUN(_this, inOrderTraversal), rootnode->left CALL
        printf("Data: %d ", *(int*)(rootnode->data));
        printf("Color: %c ", rootnode->color);
        if (rootnode->parent->isHead)
            printf("root node ");
        if(rootnode->parent != NULL && !rootnode->parent->isHead)
           printf("Parent: %d ", *(int *)(rootnode->parent->data));
        if (rootnode->right != NULL)
            printf("Right: %d ", *(int*)(rootnode->right->data));
        if (rootnode->left != NULL)
            printf("Left: %d ", *(int *)(rootnode->left->data));
        printf("\n");
        //last = rootnode->data;
        FUN(_this, inOrderTraversal), rootnode->right CALL
        
}END_FUN;

MEM_FUN_IMPL(redBlackTree, begin, redBlackTreeIterator * beginNode)
{
    //find smallest value in tree
    node* minNode = _this->root;

    while (minNode->left != NULL)
    {
        SCOPE_START;
        minNode = minNode->left;
        END_SCOPE;
    }
        

    CREATE(redBlackTreeIterator, it), minNode CALL;

    *beginNode = it;
}END_FUN;

//returns the header node
MEM_FUN_IMPL(redBlackTree, end, redBlackTreeIterator * endNode)
{
    CREATE(redBlackTreeIterator, it), _this->head CALL;

    *endNode = it;
}END_FUN;

MEM_FUN_IMPL(redBlackTree, size, int * _size)
{
    *_size = _this->size;
}END_FUN;

//TODO: Question on last line
MEM_FUN_IMPL(redBlackTree, find, void * val, redBlackTreeIterator* foundVal)
{
    bool compBool = false, compBool1 = false;
    if (_this->root == NULL)
    {
        (*foundVal).nodePtr = _this->head;
        RETURN_NONE;
    }
    node* temp = _this->root;
    while (temp != NULL) {
        SCOPE_START;
        //if (val < temp->data) 
        (*(_this->comparisonFunctionPtr))(val, temp->data, &compBool);
        if (compBool)
        {
            if (temp->left == NULL)
            {
                BREAK;
            } 
            else
                temp = temp->left;
        }
        //else if (val == temp->data) 
        (*(_this->comparisonFunctionPtr))(val, temp->data, &compBool);
        (*(_this->comparisonFunctionPtr))(temp->data, val, &compBool1);
        if (!compBool && !compBool1)
        {
            BREAK;
        }
        else {
            if (temp->right == NULL) {
                BREAK;
            }
            else
                temp = temp->right;
        }
        END_SCOPE;
    }

    //(temp->data == val)
    (*(_this->comparisonFunctionPtr))(temp->data, val, &compBool);
    (*(_this->comparisonFunctionPtr))(val, temp->data, &compBool1);
    //is it okay that we access the nodePtr directly? Is this a breach
    //of the priniciple of abstraction? 
    //If we use  getContensOf will the changes persist?
    (*foundVal).nodePtr = (!compBool && !compBool1) ? temp : _this->head;
}END_FUN;

MEM_FUN_IMPL(redBlackTree, erase, void * val, int* numElemsErased)
{
    //just for initialization purposes
    node* valNode = _this->head;
    bool compBool = false, compBool1 = false;
    
    CREATE(redBlackTreeIterator, rbIt), NULL CALL;

    FUN(_this, find), val, & rbIt CALL

    FUN(&rbIt, getContentsOf), & valNode CALL

    //if they were not equal, we set nodePtr in the iterator that find returns
    //to the header node, whose data is NULL
    //if (valNode->data != val) 
    if(valNode->data == NULL)
    {
        *numElemsErased = 0;
        RETURN_NONE;
    }

    FUN(_this, deleteNode), valNode CALL
    * numElemsErased = 1;
    _this->size--;
}END_FUN;

//https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/
//TODO: Here we also delete some nodes. Can I use these pointers to deallocate them?
//deleted nodes- dNode, replaceNode
MEM_FUN_IMPL(redBlackTree, deleteNode, node * dNode)
{
    
    node* replaceNode = NULL;
    FUN(_this,findReplacingNode), dNode, &replaceNode CALL

    // True when replaceNode and dNode are both black 
    bool uvBlack = ((replaceNode == NULL || replaceNode->color == 'B') && (dNode->color == 'B'));
    node* parent = dNode->parent, *sibling = NULL;

    //find dNode's sibling
    if (dNode->parent == NULL)
        sibling = NULL;
    else if (dNode == dNode->parent->left)
        sibling = parent->right;
    else
        sibling = parent->left;

    if (replaceNode == NULL) {
        // replaceNode is NULL therefore dNode is leaf 
        if (dNode == _this->root) {
            // dNode is _this->root, making _this->root null 
            _this->root = NULL;
        }
        else {
            if (uvBlack) {
                // replaceNode and dNode both black 
                // dNode is leaf, fix double black at dNode 
                FUN(_this, fixDoubleBlack), dNode CALL
            }
            else {
                // replaceNode or dNode is red 
                if (sibling != NULL)
                    // sibling is not null, make it red" 
                    sibling->color = 'R';
            }

            // delete dNode from the tree 
            if (dNode == dNode->parent->left) {
                parent->left = NULL;
            }
            else {
                parent->right = NULL;
            }
        }
        DELETE_OBJ(dNode);
        RETURN_NONE;
    }

    if (dNode->left == NULL || dNode->right == NULL) {
        // dNode has 1 child 
        if (dNode == _this->root) {
            // dNode is _this->root, assign the value of replaceNode to dNode, and delete replaceNode 
            dNode->data = replaceNode->data;
            dNode->left = dNode->right = NULL;
            DELETE_OBJ(replaceNode);
        }
        else {
            // Detach dNode from tree and move replaceNode up 
            if (dNode == dNode->parent->left) {
                parent->left = replaceNode;
            }
            else {
                parent->right = replaceNode;
            }
            DELETE_OBJ(dNode);
            replaceNode->parent = parent;
            if (uvBlack) {
                // replaceNode and dNode both black, fix double black at replaceNode 
                FUN(_this, fixDoubleBlack), replaceNode CALL
            }
            else {
                // replaceNode or dNode red, color replaceNode black 
                replaceNode->color = 'B';
            }
        }
        RETURN_NONE;
    }

    // dNode has 2 children, swap values with successor and recurse 
    void * temp;
    temp = replaceNode->data;
    replaceNode->data = dNode->data;
    dNode->data = temp;
    FUN(_this, deleteNode), replaceNode CALL
}END_FUN;

MEM_FUN_IMPL(redBlackTree, fixDoubleBlack, node* node_p)
{
        if (node_p == _this->root)
            // Reached root 
            RETURN_NONE;

        node* sibling = NULL, * parent = node_p->parent;

        //find node_p's sibling
        if (node_p->parent == NULL)
            sibling = NULL;
        else if (node_p == node_p->parent->left)
            sibling = parent->right;
        else
            sibling = parent->left;

        if (sibling == NULL) {
            // No sibling, double black pushed up 
            FUN(_this, fixDoubleBlack), parent CALL
        }
        else {
            if (sibling->color == 'R') {
                // Sibling red 
                parent->color = 'R';
                sibling->color = 'B';
                if (sibling == sibling->parent->left) {
                    // left case 
                    FUN(_this, RightRotate), parent CALL
                }
                else {
                    // right case 
                    FUN(_this, LeftRotate), parent CALL
                    
                }
                FUN(_this, fixDoubleBlack), node_p CALL
            }
            else {
                // Sibling black 
                if((sibling->left != NULL && sibling->left->color == 'R') ||
                    (sibling->right != NULL && sibling->right->color == 'R')) {
                    // at least 1 red child
                    if (sibling->left != NULL && sibling->left->color == 'R') {
                        if (sibling == sibling->parent->left){
                            // left left 
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            FUN(_this, RightRotate), parent CALL
                        }
                        else {
                            // right left 
                            sibling->left->color = parent->color;
                            FUN(_this, RightRotate), sibling CALL
                            FUN(_this, LeftRotate), parent CALL
                        }
                    }
                    else {                  
                        if (sibling == sibling->parent->left) {
                            // left right 
                            sibling->right->color = parent->color;
                            FUN(_this, LeftRotate), sibling CALL
                            FUN(_this, RightRotate), parent CALL
                        }
                        else {
                            // right right 
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            FUN(_this, LeftRotate), parent CALL
                        }
                    }
                    parent->color = 'B';
                }
                else {
                    // 2 black children 
                    sibling->color = 'R';
                    if (parent->color == 'B')
                        FUN(_this, fixDoubleBlack), parent CALL
                    else
                        parent->color = 'B';
                }
            }
        }
}END_FUN;

//TODO: should we use the algorithm that uses the deleted node's predecessor
//or successor as the replacing node? I don't think it makes much difference
//practically? Both implementations are below.
MEM_FUN_IMPL(redBlackTree, findReplacingNode, node* what, node** withWhat)
{
    // if the node has 2 children 
    if (what->left != NULL && what->right != NULL)
    {
        //this algorightm uses the deleted node's successor as the replacing node
        //node* temp = what->right;

        //while (temp->left != NULL)
        //    temp = temp->left;

        //this algorithm uses the deleted node's predecessor as the replacing node
        node* temp = what->left;

        while (temp->right != NULL)
            temp = temp->right;

        *withWhat = temp;
        RETURN_NONE;
    }

    // when leaf 
    if (what->left == NULL && what->right == NULL)
        *withWhat = NULL;

    // when the node has a single child 
    if (what->left != NULL)
        *withWhat = what->left;
    else
        *withWhat = what->right;
}END_FUN;

INIT_CLASS(redBlackTree)
BIND(redBlackTree, LeftRotate);
BIND(redBlackTree, RightRotate);
BIND(redBlackTree, insertFixUp);
BIND(redBlackTree, insert);
BIND(redBlackTree, getRootNode);
BIND(redBlackTree, inOrderTraversal);
BIND(redBlackTree, begin);
BIND(redBlackTree, end);
BIND(redBlackTree, size);
BIND(redBlackTree, find);
BIND(redBlackTree, erase);
BIND(redBlackTree, deleteNode);
BIND(redBlackTree, fixDoubleBlack);
BIND(redBlackTree, findReplacingNode);
BIND(redBlackTree, destroyRecursive);
END_INIT_CLASS(redBlackTree)