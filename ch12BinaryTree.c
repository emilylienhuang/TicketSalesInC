
// Creating and traversing a binary tree inorder
#include <stdio.h>
#include <stdlib.h>


// Create structure to hold integers for a tree node
struct treeNode
{
	struct treeNode *leftPtr; // pointer to left subtree
	int data; // node value
	struct treeNode *rightPtr; // pointer to right subtree
};

typedef struct treeNode TreeNode; // synonym for struct treeNode

// prototypes
void insertTreeNode (TreeNode **treePtr, int number);
void inOrder (TreeNode *treePtr);
void preOrder(TreeNode *treePtr);
void postOrder(TreeNode *treePtr);


int main(void)
{
	// Intialize a rootPtr for the tree
	TreeNode *rootPtr = NULL;
	// number to be added
	int number = 0;



	// add some numbers to tree
	for(int i=0; i<5; i++)
	{
		// prompt for number
		printf ("\n%s","Enter a value for node to add to tree ");
		scanf ("%d", &number);

		// rootPtr address is necessary
		// Providing the address enables the value stored in rootPtr to be modified
		insertTreeNode(&rootPtr, number);


	}
	
	puts("inOrder");
	inOrder(rootPtr);
	puts("\npreOrder");
	preOrder(rootPtr);
	puts("\npostOrder");
	postOrder(rootPtr);
	


	return 0;
}

//Recursive function to add node 
void insertTreeNode (TreeNode **treePtr, int number)
{

	// If the tree is empty
	if (*treePtr == NULL)
	{
		// Base case to stop recursion!
		// Allocate memory in the heap for the node
		*treePtr = malloc(sizeof(struct treeNode));

		// Setup the tree node
		if (*treePtr != NULL)
		{
			(*treePtr)->data = number;
			(*treePtr)->leftPtr = NULL;
			(*treePtr)->rightPtr = NULL;
		}
		else
		{
			printf ("No memory available to insert %d\n", number);
		}
	}
	else
	{
		// Tree is not empty
		// Determine if inserting on left or right side of tree
		if (number < (*treePtr)->data)
		{
			insertTreeNode( &((*treePtr)->leftPtr), number );
		}
		else if (number > (*treePtr)->data)
		{
			insertTreeNode( &((*treePtr)->rightPtr), number );
		}
		else
		{
			printf ("%d is a duplicate value so don't insert\n", number);
		}

	} // insert into tree with nodes

} // treeInsertNode


// InOrder traversal means:
//   Traverse the left subtree inOrder
//   Process the value in the node
//   Traverse the right subtree inOrder
// For binary search tree prints the values in ascending order
void inOrder (TreeNode *treePtr)
{

	// If tree is not empty, then traverse
	if (treePtr != NULL)
	{
		inOrder(treePtr->leftPtr);
		printf("%3d", treePtr->data);
		inOrder(treePtr->rightPtr);
	}
}

// begin preorder traversal of tree
void preOrder(TreeNode *treePtr)
{ 
   // if tree is not empty, then traverse
   if (treePtr != NULL) {                
      printf("%3d", treePtr->data);      
      preOrder(treePtr->leftPtr);        
      preOrder(treePtr->rightPtr);       
   }                           
}

// begin postorder traversal of tree
void postOrder(TreeNode *treePtr)
{ 
   // if tree is not empty, then traverse
   if (treePtr != NULL) {                
      postOrder(treePtr->leftPtr);       
      postOrder(treePtr->rightPtr);      
      printf("%3d", treePtr->data);      
   }                           
} 










