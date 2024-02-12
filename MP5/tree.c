/* tree.c 
 * Kendric Agdaca
 * KAGDACA
 * ECE 2230 Fall 2021
 * MP 5
 *
 * Implementation of tree interface for basic binary tree
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "tree.h"
#define MYMAX(a, b) (a > b ? a : b)


int CompCalls = 0;
int NumRotations = 0;


int rec_height(tree_node_t *N);
int children(tree_node_t *N);
int tree_path_find(tree_node_t *node, int temp) ;
void tree_recurse_destruct(tree_node_t *rec_ptr);
int checkbalance(tree_node_t* node);
tree_node_t* left_rotate(tree_node_t* A);
tree_node_t* right_rotate(tree_node_t* A);
tree_node_t* insert_new_node(tree_node_t *node, tree_key_t key, data_t elem_ptr, int policy);
int max_find(int a, int b);
int height(tree_node_t* node);
tree_node_t* node_remove(tree_node_t* node, tree_key_t key, int policy);
data_t tree_remove(tree_t *T, tree_key_t key);
tree_node_t* least_node(tree_node_t *node);



/* Creates the header block for the tree with the provided management policy and initializes it with default 'blank' data.
 *
 * tree_policy - tree management policy to use either AVL or BST.
 *
 * RETURNS pointer to the newly created tree
 */
tree_t *tree_construct(int tree_policy)
{
    tree_t *header = (tree_t*)malloc(sizeof(tree_t));
    header -> root = NULL;
    header -> policy = tree_policy;      
    header -> size = 0;
    header -> num_recent_key_comparisons = 0;
    header -> num_recent_rotations = 0;

    return header;
}


/* Free all items stored in the tree including the memory block with the data and the bst_node_t structure.  Also frees the header block.  
 *
 * T - tree to destroy
 */
void tree_destruct(tree_t *T)
{
//First delete subtrees
    tree_recurse_destruct(T -> root);
    
//Then delete the node
    free(T);
}


void tree_recurse_destruct(tree_node_t *rec_ptr)
{
//Recursion base case. Once entire tree is deleted, exit recursion
	if (rec_ptr == NULL)
	{
		return;
	}

//Recursively go thru the left and right branches and destruct the tree, going thru one by one clearing each node's data and their connections
	tree_recurse_destruct (rec_ptr -> left);
	tree_recurse_destruct (rec_ptr -> right);
	free(rec_ptr -> data_ptr);
	rec_ptr -> data_ptr = NULL;
	rec_ptr -> left = NULL;
	rec_ptr -> right = NULL;
	free(rec_ptr);
}



/* Insert data_t into the tree with the associated key. Insertion MUST
 * follow the tree's property (i.e., AVL or BST)
 *
 * T - tree to insert into
 * key - search key to determine if key is in the tree
 * elem_ptr - data to be stored at tree node indicated by key
 *
 * RETURNS 0 if key is found and element is replaced, and 1 if key is not found and element is inserted
 */
int tree_insert(tree_t *T, tree_key_t key, data_t elem_ptr)
{
    int insert = -1;
    CompCalls = 0;
    NumRotations = 0;
    tree_node_t *node = T -> root;
	

//First check for dupes
	while (insert == -1 && node != NULL )
	{
		CompCalls++;
		
	//If match is found, free data within node and replace	
		if (node -> key == key)
		{
			free(node -> data_ptr);
			node -> data_ptr = elem_ptr;
			insert = 0;
		}
        
        //Searches thru tree looking for a key match
		else
		{
			CompCalls++;
			
			if (node -> key > key)
			{
				node = node -> left;
			}
			
			else
			{
				node = node -> right;
			}
		}
	}

	//If no dupes found, insert a new node. First check tree policy to see if the current tree is BST or AVL
	if (insert == -1)
	{
	
		if (T -> policy == AVL)
		{
			insert = tree_avl_insert(T, key, elem_ptr);
		}
		
		//BST insert
		else
		{
			insert = 1;
			T -> root = insert_new_node(T -> root, key, elem_ptr, T -> policy);
		}
		// Update tree size
		(T -> size)++;
	}

	//After insertion, free node dangling pointer and update the tree stats 
	node = NULL;
	T -> num_recent_key_comparisons = CompCalls;
	T -> num_recent_rotations = NumRotations;


    return insert;
}



/* Insert data_t into the tree with the associated key. Insertion MUST follow the tree's property AVL. This function should be called from bst_insert for AVL tree's inserts.
 *
 * T - tree to insert into
 * key - search key to determine if key is in the tree
 * elem_ptr - data to be stored at tree node indicated by key
 *
 * Precondition: there is not already a node in tree T with the given key
 *
 * RETURNS 0 if key is found and element is replaced, and 1 if key is not found
 * and element is inserted
 */
int tree_avl_insert(tree_t *T, tree_key_t key, data_t elem_ptr)
{
    T -> root = insert_new_node(T -> root, key, elem_ptr, T -> policy);
    return 1;
}



/*
    Purpose: auxiliary function to avl and bst insert. Handles in inserting part of the original function
    Input: 
        -node: the current node position
        -key: the key to be attached to the node to be inserted
        -elem_ptr: the data to the attached to the node to be inserted
        -policy: determines insert method (standard BST or AVL)
    Output: returns a pointer to the newly inserted node
 */
tree_node_t* insert_new_node(tree_node_t *node, tree_key_t key, data_t elem_ptr, int policy)
{
    int balance;

//Checks if tree is empty. If so, insert new node as the root/start of the tree
	if (node == NULL)
	{
		tree_node_t *new_node = (tree_node_t*)malloc(sizeof(tree_node_t));
		new_node -> data_ptr = elem_ptr;
		new_node -> key = key;
		new_node -> height = 0;
		new_node->right = NULL;		
		new_node->left = NULL;

		return new_node;
	}

//If tree not empty, recursively move to the correct position for insertion of new node
	if (node -> key > key)
	{
		node -> left = insert_new_node(node -> left, key, elem_ptr, policy);
	}
	
	else
	{
		node -> right = insert_new_node(node -> right, key, elem_ptr, policy);
	}

//Update node height stat
	node -> height = 1 + max_find(height(node -> left), height(node -> right));


//If tree policy is AVL, double check tree balance after insertion
	if (policy == AVL)
	{
		balance = checkbalance(node);


		//LL rotation case
		if (key < node -> left -> key && balance > 1 )
		{
			return right_rotate(node);
		}
			
		//RR rotation case
		if (key > node -> right -> key && balance < -1 )
		{	
			return left_rotate(node);
		}
			
		//LR rotation case
		if (key > node -> left-> key && balance > 1)
		{
			node -> left = left_rotate(node -> left);
			return right_rotate(node);
		}
		//RL rotation case
		if (key < node -> right -> key && balance < -1 )
		{
			node -> right = right_rotate(node -> right);
			return left_rotate(node);
		}
	}


	return node;
}



 /*
 * Removes the item in the tree with the matching key.
 * 
 * T - pointer to tree
 * key - search key for particular node in the tree 'T'
 *
 * RETURNS the pointer to the data memory block and free the bst_node_t memory block. If the key is not found in the tree, return NULL.  If the tree's policy is AVL, then ensure all nodes have the AVL property.
 */
data_t tree_remove(tree_t *T, tree_key_t key)
{

    data_t dp = NULL;
    CompCalls = 0;
    NumRotations = 0;

    tree_node_t *rec_ptr = T -> root;
    tree_node_t *parent = NULL;
    
    
//first find parent node of node to be removed
    while (dp == NULL && rec_ptr != NULL)
    {
        CompCalls++;
        
        if (rec_ptr -> key == key)
        {
            dp = rec_ptr -> data_ptr;
        }
        
        else
        {
            parent = rec_ptr;
            CompCalls++;
            
            if (rec_ptr -> key > key)
            {
                rec_ptr = rec_ptr -> left;
            }
            
            else
            {
                rec_ptr = rec_ptr -> right;
            }
        }
    }
    

//Once parent node of node to be removed is found, proceed to remove node
    if (dp != NULL)
    {
        if (parent == NULL)
        {
            T -> root = node_remove(rec_ptr, key, T -> policy);
        }
        
        else if (parent -> key > key)
        {
            parent -> left = node_remove(parent -> left, key, T -> policy);
        }
        
        else if (parent -> key < key)
        {
            parent -> right= node_remove(parent -> right, key, T -> policy);
        }
        
        (T -> size)--;
    }



//update tree stats
    rec_ptr = NULL;
    T -> num_recent_key_comparisons = CompCalls;
    T -> num_recent_rotations = NumRotations;


    return dp;
    
}


/* Recursive auxiliary function for tree_remove. Will remove node and then check the balance of the tree. If policy is AVL, also do rotations to balance
 *
 * node - node to be removed
 * key - the key of the node to be removed
 * polcy - the type of tree policy to keep
 *
 * RETURNS pointer to the node removed
*/
tree_node_t* node_remove(tree_node_t* node, tree_key_t key, int policy)
{

    if (node == NULL)
    {
        return NULL;
    }

    if (key < node->key)
    {
        node->left = node_remove(node->left, key, policy);
    }
    
    else if (key > node->key)
    {
        node->right = node_remove(node->right, key, policy);
    }
    
    else
    {
   
        if (node->left == NULL || node->right == NULL)
        {
            tree_node_t* temp = node->left;
    
            if (temp == NULL)
            {
                temp = node->right;
            }
   
            if (temp == NULL)
            {
                temp = node;
                node = NULL;
            }
   
        else
        {
            *node = *temp;
        }

        free(temp);
        }
   
   
        else
        {
            tree_node_t* temp = least_node(node->right);
            node->key = temp->key;
            node->data_ptr = temp->data_ptr;
            node->right = node_remove(node->right, temp->key, policy);
        }
    }

    if (node == NULL)
    {
        return node;
    }

    node->height = 1 + max_find(height(node->left), height(node->right));

    if (policy == AVL)
    {
        int balance = checkbalance(node);
    

    if (balance > 1 && checkbalance(node->left) >= 0)
    {
        return right_rotate(node);
    }
    
    if (balance < -1 && checkbalance(node->right) <= 0)
    {
        return left_rotate(node);
    }
   
    if (balance > 1 && checkbalance(node->left) < 0)
    {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && checkbalance(node->right) > 0)
    {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    }

    return node;
}



/* Finds the tree element with the matching key and returns the data that is
 * stored in this node in the tree.
 *
 * T - tree to search in key - key to find inside T
 *
 * RETURNS pointer to the data stored in the found node or NULL if no match is
 * found
 */
data_t tree_search(tree_t *T, tree_key_t key)
{
    CompCalls = 0;
    tree_node_t *find = T -> root;

//Iterates thru tree till it reaches the end    
    while (find != NULL)
    {
        CompCalls++;
    
    //If matched key is found, return the node's data
        if (find -> key == key)
        {
            T -> num_recent_key_comparisons++;
            return find -> data_ptr;
        }
        
        
        CompCalls++;
    //key is > current node, search left
        if (find -> key > key)
        {
            find = find -> left;
            T -> num_recent_key_comparisons++;
        }
    
    //key is < current node, search right    
        else
        {
            find = find -> right;
            T -> num_recent_key_comparisons++;
        }
    }
    
    
//Update tree stats. If code reaches this point, no matching node key was found
    T -> num_recent_key_comparisons = CompCalls; 
    return NULL;
}


/* RETURNS the number of keys in the tree */
int tree_count(tree_t *T)
{
    return T -> size; 
}


/* RETURNS the number of key comparisons  */
int tree_stats(tree_t *T)
{
    return T -> num_recent_key_comparisons; 
}


/* RETURNS the computed internal path length of the tree T */
int tree_internal_path_len(tree_t *T)
{
    return tree_path_find(T -> root, 0);
}


int tree_path_find(tree_node_t *node, int temp) 
{
 
//Recursive base case. If current node = NULL, its reached the end, so end recursion
    if (node == NULL)
    {
        return 0;
    }

    return temp + tree_path_find(node -> right, temp + 1) + tree_path_find(node -> left, temp + 1);
}







//////AUX FUNCTIONS

 /*
    Purpose: Finds the returns the higher int value between 2 input param
    Input: 
        -A: first int input param to be compared
        -B: second int input param to be compared
    Output: returns the input param that has a higher int value
 */
int max_find(int A, int B)
{
	if (A > B)
	{
		return A;
	}
	
	else
	{
		return B;
        }
}


/*
    Purpose: auxiliary function to insert_new_node when the tree policy is AVl. Rotates node "A" to the right
    Input: 
        -A: pointer to the node that needs to be rotated to the right
    Output: returns a pointer to the node in its new location on the tree
*/
tree_node_t* right_rotate(tree_node_t* A)
{
	int Rheight, Lheight;
	
//Handles the right rotation of "A" node
	NumRotations++;
	tree_node_t *B = A -> left;
	tree_node_t *tmp = B -> right;
	B -> right = A;
	A -> left = tmp;


//Update the new "A" node height stat after rotation. First find the height of the left and right side
	if (A -> left == NULL)
	{
		Lheight = -1;
	}
	
	else
	{
		Lheight = A -> left -> height;
	}
	
	if (A -> right == NULL)
	{
		Rheight = -1;
	}
	
	else
	{
		Rheight = A -> right -> height;	
	}
	
//Then update "A"'s height 	
	A -> height = 1 + max_find(Lheight, Rheight);
	
	
	
//Does the same for "B", which is the node to the left of "A". First find the height of the left and right sides
	if (B -> left == NULL)
	{
		Lheight = -1;
	}
	
	else
	{
		Lheight = B -> left -> height;
	}
	
	if (B -> right == NULL)
	{
		Rheight = -1;
	}
	
	else
	{
		Rheight = B -> right -> height;	
	}
	
	
//Then update "B"'s height and return new root, "B"	
	B -> height = 1 + max_find(Lheight, Rheight);

	return B;
}


/*
    Purpose: auxiliary function to insert_new_node when the tree policy is AVl. Rotates node "A" to the left
    Input: 
        -A: pointer to the node that needs to be rotated to the right
    Output: returns a pointer to the node in its new location on the tree
 */
tree_node_t* left_rotate(tree_node_t* A)
{
	int Rheight, Lheight;

//Handles the left rotation of "A" node	
	NumRotations++;
	tree_node_t *B = A -> right;
	tree_node_t *tmp = B -> left;
	B -> left = A;
	A -> right = tmp;


//Update the new "A" node height stat after rotation. First find the height of the left and right side
	if (A -> left == NULL)
	{
		Lheight = -1;
	}
	
	else
	{
		Lheight = A -> left -> height;
	}
	
	if (A -> right == NULL)
	{
		Rheight = -1;
	}
	
	else
	{
		Rheight = A -> right -> height;	
	}
	
	
	A -> height = 1 + max_find(Lheight, Rheight);
	
	
//Does the same for "B", which is the node to the left of "A". First find the height of the left and right sides
	if (B -> left == NULL)
	{
		Lheight = -1;
	}
	
	else
	{
		Lheight = B -> left -> height;
	}
	
	if (B -> right == NULL)
	{
		Rheight = -1;
	}
	
	else
	{
		Rheight = B -> right -> height;	
	}
	
//Then update "B"'s height and return new root, "B"
	B -> height = 1 + max_find(Lheight, Rheight);

	return B;
}


 /*
    Purpose: auxiliary function to just assist in updating height stats
    Input:
        -node: pointer to the node whose height is to be found
    Output: returns the height of the inputted node. If the node is empty, just return -1
 */
int height(tree_node_t* node)
{
	if (node == NULL)
	{
		return -1;
	}
	
	return node -> height;
}


/*
    Purpose: auxiliary function to insert_new_node if the tree policy is AVl. Simply determines if the AVL tree still maintains the properties of an AVL after a new node has been inserted. Balance is determined if the height of each subtree of each node has only a difference of 1 height
    Inputs:
	-node: pointer to the node to determine if it's balanced or not
    Output: find the height difference between the left and right subtree and return its value. Returns 0 if the inputted null is NULL
*/
int checkbalance(tree_node_t* node)
{
	if (node == NULL)
	{
		return 0;
	}
	
	return (height(node -> left) - height(node -> right));
}


tree_node_t* least_node(tree_node_t *node)
{
    tree_node_t *least = node;
    while (least->left != NULL)
least = least->left;
    return least;
}


/* RETURNS the number of rotations from the last remove MIGHT NOT NEED num_recent_rotations only used 4 times*/
int tree_rotations(tree_t *T)
{
    return T -> num_recent_rotations;
}


/* A tree validation function based on node position
 *
 * N - node to validate tree
 * min - temp min for it's left subtree min
 * max - temp max for it's right subtree max
 */
int tree_debug_validate_rec(tree_node_t *N, int min, int max, int *size)
{
    if (N == NULL)
        return TRUE;
    if (N->key <= min || N->key >= max)
        return FALSE;
    assert(N->data_ptr != NULL);
    *size += 1;
    return tree_debug_validate_rec(N->left, min, N->key, size) &&
        tree_debug_validate_rec(N->right, N->key, max, size);
}


/* Basic validation function for tree T */
void tree_debug_validate(tree_t *T)
{
    int size = 0;
    assert(tree_debug_validate_rec(T->root, INT_MIN, INT_MAX, &size) == TRUE);
    assert(size == T->size);
//    if (T->policy == AVL)
//	    rec_height(T->root);
}


/* basic print function for a binary tree 
 *
 * N - node of tree to print
 * level - level in which the node resides
 * policy - BST or AVL
 */
void ugly_print(tree_node_t *N, int level)
{
    int i;
    if (N == NULL) return;
    ugly_print(N->right, level+1);
    for (i = 0; i<level; i++) printf("     ");
    printf("%5d\n", N->key);
    ugly_print(N->left, level+1);
}

/* prints the tree T */
void tree_debug_print_tree(tree_t *T)
{
    printf("Tree with %d keys\n", T -> size);
    ugly_print(T->root, 0);
    printf("\n");
    tree_debug_validate(T);
}




/* EVERYTHING BELOW THIS DONT NEED*/
/* Verifies AVL tree properties */
int rec_height(tree_node_t *N)
{
    if (N == NULL)
	    return 0;
    int lh = rec_height(N->left);
    int rh = rec_height(N->right);
    int lean = lh - rh;
    assert(lean == 0 || lean == 1 || lean == -1);
    return 1 + MYMAX(lh, rh); 

}


/* Recursive function to count children */
int children(tree_node_t *N)
{
    if (N == NULL) return 0;
    return 1 + children(N->left) + children(N->right);
}


/* Prints the tree to the terminal in ASCII art*/
void pretty_print(tree_t *T)
{
    typedef struct queue_tag {
	    tree_node_t *N;
	    int level;
	    int list_sum;
    } queue_t;

    queue_t Q[T->size];
    int q_head = 0;
    int q_tail = 0;
    int i, j;
    int current_level = 0;
    int col_cnt = 0;
    tree_node_t *N;

    Q[q_tail].N = T->root;
    Q[q_tail].level = 0;
    Q[q_tail].list_sum = 0;
    q_tail++;
    for (i = 0; i < T->size; i++)
    {
	assert(q_head < T->size);
	N = Q[q_head].N;
	if (Q[q_head].level > current_level) {
	    printf("\n");
	    current_level++;
	    col_cnt = 0;
	}
	int left_ch = children(N->left);
	int my_pos = 1 + left_ch + Q[q_head].list_sum;
	int left_child_pos = my_pos;
	if (N->left != NULL)
	    left_child_pos = 1 + Q[q_head].list_sum + children(N->left->left);
	int right_child_pos = my_pos;
	if (N->right != NULL)
	    right_child_pos = my_pos + 1 + children(N->right->left);
	for (j = col_cnt + 1; j <= right_child_pos; j++)
	{
	    if (j == my_pos)
		if (left_child_pos < my_pos)
		    if (N->key < 10)
			printf("--%d", N->key);
		    else if (N->key < 100)
			printf("-%d", N->key);
		    else
			printf("%d", N->key);
		else
		    printf("%3d", N->key);
	    else if (j == left_child_pos)
		//printf("  |");
		printf("  /");
	    else if (j > left_child_pos && j < my_pos)
		printf("---");
	    else if (j > my_pos && j < right_child_pos)
		printf("---");
	    else if (j == right_child_pos)
		//printf("--|");
		printf("-\\ ");
	    else
		printf("   ");
	}
	col_cnt = right_child_pos;

	if (N->left != NULL) {
	    Q[q_tail].N = N->left;
	    Q[q_tail].level = Q[q_head].level + 1;
	    Q[q_tail].list_sum = Q[q_head].list_sum;
	    q_tail++;
	}
	if (N->right != NULL) {
	    Q[q_tail].N = N->right;
	    Q[q_tail].level = Q[q_head].level + 1;
	    Q[q_tail].list_sum = Q[q_head].list_sum + left_ch + 1;
	    q_tail++;
	}
	q_head++;
    }
    printf("\n");
}

/* vi:set ts=8 sts=4 sw=4 et: */
