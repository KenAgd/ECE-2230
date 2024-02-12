/* linkedlist.h            <<-- no changes to this file
 * Prof. Russell
 * harlanr
 * ECE 2230 Fall 2021
 * MP2
 *
 * Public functions for two-way linked list
 *
 * You should not need to change anything in this file.  If you do you
 * must get permission from the instructor.
 */

#include "datatypes.h"   // defines data_t 

// flags for fast access to front and back of a list
#define LLPOSITION_FRONT -98765
#define LLPOSITION_BACK  -23456

typedef struct linked_element_tag {
    // private members for linkedlist.c only
    data_t *data_ptr;
    struct linked_element_tag *back_link;
    struct linked_element_tag *fwd_link;
} linked_elem_t;

typedef struct list_header_tag {
    // private members for linkedlist.c only
    linked_elem_t *list_head;
    linked_elem_t *list_tail;
    int current_list_size;
    int list_sorted_state;
    // Private procedure for linkedlist.c only
    int (*comp_proc)(const data_t *, const data_t *);
} linked_t;

/* prototype definitions for functions in linkedlist.c */
linked_t * linked_construct(int (*fcomp)(const data_t *, const data_t *));
data_t *  linked_access(linked_t *list_ptr, int pos_index);
void      linked_destruct(linked_t *list_ptr);
void      linked_insert(linked_t *list_ptr, data_t *elem_ptr, int pos_index);
void      linked_insert_sorted(linked_t *list_ptr, data_t *elem_ptr);
data_t *  linked_remove(linked_t *list_ptr, int pos_index);
data_t *  linked_elem_find(linked_t *list_ptr, data_t *elem_ptr, int *pos_index,
        int (*fcomp)(const data_t *, const data_t *));
data_t *  linked_elem_remove_extremum(linked_t *list_ptr, int (*fcomp)(const data_t *, const data_t *));
int       linked_entries(linked_t *list_ptr);

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
