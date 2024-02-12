/* tree.h 
 * Kendric Agdaca
 * kagdaca
 * ECE 2230 Fall 2021
 * MP 5
 *
 * Interface and tree definition for basic binary tree
 */

enum balanceoptions {BST, AVL, TWOTHREET};

#define TRUE 1
#define FALSE 0

typedef void *data_t;
typedef int tree_key_t;

typedef struct tree_node_tag {
    data_t data_ptr;
    tree_key_t key;
    int height;
    struct tree_node_tag *left;
    struct tree_node_tag *right;
    struct tree_node_tag *parent;
} tree_node_t;

typedef struct tree_tag {
    tree_node_t *root;
    int size;       // number of keys in tree
    int num_recent_rotations;  // number of rotations in last operation
    int policy;     // must be BST or AVL
    int num_recent_key_comparisons; // number of comparisons in last operation
} tree_t;


/* prototype definitions for functions in bst.c */

//int tree_avl_insert(tree_t *tree_ptr, tree_key_t key, data_t elem_ptr);

tree_t *tree_construct(int);//
void tree_destruct(tree_t *tree_ptr);//
int tree_insert(tree_t *tree_ptr, tree_key_t key, data_t elem_ptr);//
data_t tree_remove(tree_t *tree_ptr, tree_key_t key);//
data_t tree_search(tree_t *tree_ptr, tree_key_t key);//
int tree_count(tree_t *tree_ptr);
int tree_stats(tree_t *tree_ptr);
int tree_internal_path_len(tree_t *);
int tree_rotations(tree_t *tree_ptr);
void tree_debug_print_tree(tree_t *tree_ptr);
void tree_debug_validate(tree_t *T);
int tree_avl_insert(tree_t *T, tree_key_t key, data_t elem_ptr);

/* vi:set ts=8 sts=4 sw=4: */
