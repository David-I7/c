typedef struct BinaryTree
{
    struct BinaryTree *left;
    struct BinaryTree *right;
    int data;
} BinaryTree;

enum BinaryTreeTraversalStrategy
{
    BFS,
    DFS,
    POST_ORDER
};

BinaryTree *binaryTreeCreate(int, BinaryTree *, BinaryTree *);
BinaryTree *binaryTreeFromRange(int start,int end);
void binaryTreeFree(BinaryTree *);
void binaryTreePrint( BinaryTree *,enum BinaryTreeTraversalStrategy);
