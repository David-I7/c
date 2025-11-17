#include "binaryTree.h"
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

// Internal API
void binaryTreeFreeNode(BinaryTree *);
void binaryTreePrintNode( BinaryTree *);
void binaryTreeTraverse(BinaryTree *, enum BinaryTreeTraversalStrategy, void (*mapper)( BinaryTree *));
void binaryTreeTraverseDFS(BinaryTree *, void (*mapper)( BinaryTree *));
void binaryTreeTraverseBFS(BinaryTree *, void (*mapper)( BinaryTree *));
void binaryTreeTraversePostOrder(BinaryTree *, void (*mapper)( BinaryTree *));

// API

BinaryTree *binaryTreeCreate(int data,BinaryTree* left,BinaryTree* right){
    BinaryTree* node = malloc(sizeof(BinaryTree));
    node->data = data;
    node->left = left;
    node->right = right;
    return node;
};

BinaryTree *binaryTreeFromRange(int start,int end){
    if (start - end >= 0) return NULL;

    Vector* q = vectorCreate();
    BinaryTree* root = binaryTreeCreate(start++,NULL,NULL);
    vectorPushback(q,root);

    BinaryTree* node;
    while(start < end){
        int size = q->size;
        for(int i = 0 ;i < size; i++){
            node = (BinaryTree*)vectorFront(q);
            vectorPopfront(q);
            if (end - start >= 2){
                node->left = binaryTreeCreate(start++,NULL,NULL);
                node->right = binaryTreeCreate(start++,NULL,NULL);
                vectorPushback(q,node->left);
                vectorPushback(q,node->right);
            }else if(end - start == 1){
                node->left = binaryTreeCreate(start++,NULL,NULL);
                break;
            }else break;
        }
    }
    while (q->size > 0) vectorPopback(q);
    return root;
};

void binaryTreeTraverse(BinaryTree *node, enum BinaryTreeTraversalStrategy strategy, void (*mapper)( BinaryTree *)){
    switch (strategy)
    {
    case BFS:
        binaryTreeTraverseBFS(node,mapper);
        break;
    case POST_ORDER:
        binaryTreeTraversePostOrder(node,mapper);
        break;
    case DFS:
        binaryTreeTraverseDFS(node,mapper);
        break;
    default:
        break;
    }
};

// INTERNAL API IMPL

void binaryTreeFree(BinaryTree* root){
    binaryTreeTraverse(root,POST_ORDER,binaryTreeFreeNode);
};
void binaryTreePrint(BinaryTree * node, enum BinaryTreeTraversalStrategy strategy){
    binaryTreeTraverse(node,strategy,binaryTreePrintNode);
    printf("\n");
};

void binaryTreeTraverseBFS(BinaryTree* node,void (*mapper)( BinaryTree *)){
    if (node == NULL) return;
    Vector* q = vectorCreate();
    vectorPushback(q,node);

    while(q->size > 0){
        BinaryTree* node = q->buf[0];
        vectorPopfront(q);
        mapper(node);
        if (node->left) vectorPushback(q,node->left);
        if (node->right) vectorPushback(q,node->right);
    }
}
void binaryTreeTraversePostOrder(BinaryTree* node,void (*mapper)( BinaryTree *)){
    if (node == NULL) return;
    if (node->left) binaryTreeTraverseBFS(node->left,mapper); 
    if (node->right) binaryTreeTraverseBFS(node->right,mapper);
    mapper(node);
}

void binaryTreeTraverseDFS(BinaryTree* node,void (*mapper)( BinaryTree *)){
    if (node == NULL) return;
    if (node->left) binaryTreeTraverseBFS(node->left,mapper); 
    mapper(node);
    if (node->right) binaryTreeTraverseBFS(node->right,mapper);
}

void binaryTreeFreeNode(BinaryTree* node){
    free(node);
};

void binaryTreePrintNode( BinaryTree * node){
    printf("%d ",node->data);
};