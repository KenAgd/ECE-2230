/* linkedlist.c            <<-- A template to update and 
 * Kendric Agdaca
 * Kagdaca
 * ECE 2230 Fall 2021
 * MP3
 *
 * Propose: A template for linkedlist.c. You will make many changes.
 *
 * Assumptions: Many details are incomplete.  
 *
 * Bugs: Many details have not been implemented.
 *
 */
 #include<stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "linkedlist.h"        // defines public functions for list ADT

// definitions for private constants used in linkedlist.c only

#define SORTED_LLIST    999999
#define UNSORTED_LLIST -888888

// prototypes for private functions used in linkedlist.c only 

void my_debug_validate(linked_t *L);
void list_recursive_selection_sort(linked_t *list_ptr);
void recurseSelect(linked_t *list_ptr, linked_elem_t *list_head, linked_elem_t *list_tail);
linked_elem_t *findPosition(linked_t *list_ptr, linked_elem_t *list_head, linked_elem_t *list_tail);
void list_recursive_selection_sort(linked_t *list_ptr);
void recurseSelect(linked_t *list_ptr, linked_elem_t *list_head, linked_elem_t *list_tail);
void list_iterative_selection_sort(linked_t *list_ptr);
void list_merge_sort(linked_t *list_ptr);
linked_elem_t *mergeSort(linked_t *list_ptr, linked_elem_t *head);
linked_elem_t *Split(linked_elem_t *head);
linked_elem_t *sortMerge(linked_t *list_ptr, linked_elem_t *A, linked_elem_t *B);
/* ----- below are the functions  ----- */



/* Sorts a list used a specific sorting algorithm and direction.
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * sort_type: sorting algorithm used during sorting: 1) Insertion Sort; 
 *            2)Extremum Sort; 3) Recursive Selection Sort; 
 *            4) Iterative Selection Sort; and 5) Merge Sort
 *
 * return value: None.
 */
void linked_sort(linked_t *list_ptr, int sort_type, int (*fcomp)(const data_t *, const data_t *))
{
        list_ptr -> comp_proc = fcomp;
		
//insertion sort call. Make a copy of current node into sorted list and then remove it from unsorted list.
	if (sort_type == 1)
	{
	
            linked_t *sorted_list = linked_construct(fcomp);
            
            int i = 0;
            data_t *new_ptr;
            int list_size = linked_entries(list_ptr);
            
            while (i < list_size)
            {
                new_ptr = linked_remove(list_ptr, 0);
                linked_insert_sorted(sorted_list, new_ptr);
                i++;
            }
            
            *list_ptr = *sorted_list;
            free(sorted_list);
	}


//extremum sort call. Make a copy of the smallest node into sorted list and then remove it from unsorted list
	else if (sort_type == 2)
	{
            linked_t *sorted_list = linked_construct(fcomp);
            
            int i = 0;
            data_t *new_ptr;
            int list_size = linked_entries(list_ptr);
            
            while (i < list_size)
            {
                new_ptr = linked_elem_remove_extremum(list_ptr, fcomp);
                linked_insert_sorted(sorted_list, new_ptr);
                i++;
            }
            
            *list_ptr = *sorted_list;
            free(sorted_list);	
	}


//recursive selection sort call
	else if (sort_type == 3)
	{
		list_recursive_selection_sort(list_ptr);
	}


//iterative selection sort call
	else if (sort_type == 4)
	{
		list_iterative_selection_sort(list_ptr);
	}


//merge sort call.
	else if (sort_type == 5)
	{
		list_merge_sort(list_ptr);
	}

    list_ptr -> list_sorted_state = SORTED_LLIST;
    my_debug_validate(list_ptr);

}



/*
    Purpose: recursive version of selection sort. Repeatedly finds the smallest node from unsorted list and moves it into the sorted list
    
    Inputs: 
        -list_ptr: pointer to the unsorted list
*/
void list_recursive_selection_sort(linked_t *list_ptr)
{
    recurseSelect(list_ptr, list_ptr -> list_head, list_ptr -> list_tail);
}


/*
    Purpose: 1st auxiliary function to recursive selection sort that actual recursing function.
    
    Input:
        -list_ptr: pointer to the unsorted list
        -list_head: pointer to the unsorted list head
        -list_tail: pointer to the unsorted list tail
*/
void recurseSelect(linked_t *list_ptr, linked_elem_t *list_head, linked_elem_t *list_tail)
{
    linked_elem_t *position;
    data_t *temp;
    
    if (list_head != NULL)
    {
        position = findPosition(list_ptr, list_head, list_tail);
        temp = list_head -> data_ptr;
        list_head -> data_ptr = position -> data_ptr;
        position -> data_ptr = temp;
        
        recurseSelect(list_ptr, list_head -> fwd_link, list_tail);
    }
}


/*
    Purpose: 2nd auxiliary function to recursive selection sort that finds the smallest node and returns it to recurseSelect function
    
    Input:
        -list_ptr: pointer to the unsorted list
        -list_head: pointer to the unsorted list head
        -list_tail: pointer to the unsorted list tail
        
    Output:
        -position: position of the smallest node to be swapped
*/
linked_elem_t *findPosition(linked_t *list_ptr, linked_elem_t *list_head, linked_elem_t *list_tail)
{
    linked_elem_t *temp = list_head;
    linked_elem_t *position = list_head;
    
    do
    {
        if (list_ptr -> comp_proc(temp -> data_ptr, position -> data_ptr) == 1)
        {
            position = temp;
        }
        
        temp = temp -> fwd_link;
    
    }while (temp != NULL);
    
    return position;
}



/*
    Purpose: Essentially the same as recursive selection sort but does it all within one function instead of multiple functions
    
    Input:
        -list_ptr: pointer to the unsorted list
*/
void list_iterative_selection_sort(linked_t *list_ptr)
{
    linked_elem_t *temp = list_ptr -> list_head;
    linked_elem_t *position;
    linked_elem_t *rove;
    data_t *A;

    
    while (temp != NULL)
    {
        position = temp;
        rove = temp -> fwd_link;
        
        while (rove != NULL)
        {
            if (list_ptr -> comp_proc(rove -> data_ptr, position -> data_ptr) == 1)
            {
                position = rove;
            }
            rove = rove -> fwd_link;
        }
            A = temp -> data_ptr;
    temp -> data_ptr = position -> data_ptr;
    position -> data_ptr = A;
    temp = temp -> fwd_link;
    }
}


/*
    Purpose: Repeatedly breaks down list into multiple sublists until each sublist consists of a single node then merges sublists all back together in a sorted manner
    
    Input: 
        -list_ptr: pointer to the unsorted list
*/
void list_merge_sort(linked_t *list_ptr)
{
    linked_elem_t *temp = list_ptr -> list_head;
    list_ptr -> list_head = mergeSort(list_ptr, list_ptr -> list_head);
    list_ptr -> list_head -> back_link = NULL;
    
    while (temp -> fwd_link != NULL)
    {
        temp = temp -> fwd_link;
    }
    
    list_ptr -> list_tail = temp;
    list_ptr -> list_tail -> fwd_link = NULL;
}


/*
    Purpose: 1st auxiliary function to list_merge_sort. Splits unsorted list into two sub lists and sorts them in ascending order. Once sorted, merge both sub lists together
    
    Input: 
        -list_ptr: pointer to the unsorted list
        -head: pointer to the unsorted list head
        
    Output:
        -returns final merge sorted list to the main merge function
*/
linked_elem_t *mergeSort(linked_t *list_ptr, linked_elem_t *head)
{
    if (head == NULL || head -> fwd_link == NULL)
    {
        return head;
    }
    
    linked_elem_t *secondSplit = Split(head);
    
    head = mergeSort(list_ptr, head);
    secondSplit = mergeSort(list_ptr, secondSplit);
    
    return sortMerge(list_ptr, head, secondSplit);
}



/*
    Purpose: 2nd auxiliary function to merge sort that splits the unsorted list into 2 separate lists by front and back end
    
    Input:
        -head: pointer to unsorted list head
        
    Output:
        -temp: pointer to the second half of the list
*/
linked_elem_t *Split(linked_elem_t *head)
{
    linked_elem_t *A = head;
    linked_elem_t *B = head;
    
    while (A -> fwd_link && A -> fwd_link -> fwd_link)
    {
        A = A -> fwd_link -> fwd_link;
        B = B -> fwd_link;
    }
    
    linked_elem_t *temp = B -> fwd_link;
    B -> fwd_link = NULL;
    return temp;
}


/*
    Purpose: 3rd auxiliary function to merge sort. Splits list A and B into smaller sublists
    
    Input:
        list_ptr: pointer to the unsorted list
        A: first half of the unsorted list
        B: second half of the unsorted list
    
    Output:
*/
linked_elem_t *sortMerge(linked_t *list_ptr, linked_elem_t *A, linked_elem_t *B)
{
    linked_elem_t temp;
    temp.fwd_link = NULL;
    
    linked_elem_t *tail = &temp;
    
    while (1)
    {
        if (A == NULL)
        {
            tail -> fwd_link = B;
            B -> back_link = tail;
            break;
        }
        
        else if (B == NULL)
        {
            tail -> fwd_link = A;
            A -> back_link = tail;
            break;
        }
        
        if (list_ptr -> comp_proc(A -> data_ptr, B -> data_ptr) == 1)
        {
            tail -> fwd_link = A;
            tail -> fwd_link -> back_link = tail;
            A -> back_link = tail;
            A = A -> fwd_link;
        }
        
        else
        {
            tail -> fwd_link = B;
            tail -> fwd_link -> back_link = tail;
            B -> back_link = tail;
            B = B -> fwd_link;
        }
        
        tail = tail -> fwd_link;
    }
    
    return temp.fwd_link;
}




/* Allocates a new, empty list 
 *
 * If the comparison function is NULL, then the list is unsorted.
 *
 * Otherwise, the list is initially assumed to be sorted.  Note that if 
 * list_insert is used the list is changed to unsorted.  
 *
 * The field sorted can only take values SORTED_LLIST or UNSORTED_LLIST
 *
 * Use linked_destruct to remove and deallocate all elements on a list 
 * and the header block.
 */
linked_t * linked_construct(int (*compare_function)(const data_t *, const data_t *))
{
	linked_t *L;
    	L = (linked_t *) malloc(sizeof(linked_t));
    	L->list_head = NULL;
    	L->list_tail = NULL;
    	L->current_list_size = 0;
    	L->comp_proc = compare_function;
    
	if (compare_function == NULL)
	{
		L->list_sorted_state = UNSORTED_LLIST;
    	}

	else
	{
        	L->list_sorted_state = SORTED_LLIST;
	}

//    	my_debug_validate(L);
    	return L;
}





/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from linked_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.
 */
data_t * linked_access(linked_t *list_ptr, int pos_index)
{
	int count=0;
    	linked_elem_t *L;
 
// debugging function to verify that the structure of the list is valid
//    	my_debug_validate(list_ptr);

    /* handle special cases.
     *   1.  The list is empty
     *   2.  Asking for the head 
     *   3.  Asking for the tail
     *   4.  specifying a position that is out of range.  This is not defined
     *       to be an error in this function, but instead it is assumed the 
     *       calling function correctly specifies the position index
     */
    	if (linked_entries(list_ptr) == 0) 
	{
        	return NULL;  // list is empty
    	}


    	else if (pos_index == LLPOSITION_FRONT || pos_index == 0) 
	{
        	return list_ptr->list_head->data_ptr;
    	}


    	else if (pos_index == LLPOSITION_BACK || pos_index == linked_entries(list_ptr) - 1) 
	{
        	return list_ptr->list_tail->data_ptr;
    	}
    
	
	else if (pos_index < 0 || pos_index >= linked_entries(list_ptr))
	{
        	return NULL;   // does not correspond to position in list
	}


// loop through the list until find correct position index
    	L=list_ptr->list_head;
    	
	
	for (count = 0; count < pos_index; count++) 
	{
        	L = L->fwd_link;
    	}


// already verified that pos_index should be valid so L better not be null
    	assert(L != NULL);
    	assert(L->data_ptr != NULL);
    	return L->data_ptr;
}





/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 */
void linked_destruct(linked_t *list_ptr)
{
//    my_debug_validate(list_ptr);

    linked_elem_t *L = list_ptr -> list_head;
    linked_elem_t *nextNode;
	
//First go thru and clear each node's data   	    
    while (L != NULL)
    {
        nextNode = L;
        L = L -> fwd_link;
        free(nextNode -> data_ptr);
        nextNode -> data_ptr = NULL;
    }
	
//Once each node's data is cleared, clear the node's themselves
    L = list_ptr -> list_head;	
    while (L != NULL)
    {
        nextNode = L;
        L = L -> fwd_link;
	    
        if (nextNode != NULL)
        {
            free(nextNode);
            nextNode = NULL;
        }
    }

//Finally, clear the list itself	
    free(list_ptr);
    list_ptr = NULL;
}





/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * linked_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the 
 *            list.  Index starts at 0 at head of the list, and incremented by 
 *            one until the tail is reached.  The index can also be equal
 *            to LLPOSITION_FRONT or LLPOSITION_BACK (these are special negative 
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list, 
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the linked_ptr->list_sorted_state must be equal 
 * to UNSORTED_LLIST.
 */
void linked_insert(linked_t *list_ptr, data_t *elem_ptr, int pos_index)
{
	assert(list_ptr != NULL);


	int i;
	linked_elem_t *L = list_ptr -> list_head;
	linked_elem_t *newNode = (linked_elem_t*)malloc(sizeof(linked_elem_t));
	newNode -> data_ptr = elem_ptr;
        

//If list empty, push node @ head
	if (list_ptr -> current_list_size == 0)
	{
		list_ptr -> list_head = newNode;
		list_ptr -> list_tail = newNode;
		newNode -> fwd_link = NULL;
		newNode -> back_link = NULL;
		list_ptr -> current_list_size++;
	}


//If insert location is @ head, push node
	else if (pos_index == 0 ||  pos_index == LLPOSITION_FRONT)
	{
		list_ptr -> list_head -> back_link = newNode;
		newNode -> fwd_link = list_ptr -> list_head;
//		newNode -> back_link = NULL;
		list_ptr -> list_head = newNode;
		list_ptr -> list_head -> back_link = NULL;
		list_ptr -> current_list_size++;
	}


//If insert location is @ tail, append node
	else if (pos_index >= LLPOSITION_BACK)//pos_index >= list_ptr -> current_list_size || pos_index == LLPOSITION_BACK
	{/*
		list_ptr -> list_tail -> fwd_link = newNode;
		newNode -> fwd_link = NULL;
		newNode -> back_link = list_ptr -> list_tail;
		list_ptr -> list_tail = newNode;
		list_ptr -> current_list_size++;*/
		
		list_ptr -> list_tail = newNode;
		newNode -> data_ptr = elem_ptr;
		newNode -> fwd_link = NULL;
		
		while (L -> fwd_link != NULL)
		{
		    L = L -> fwd_link;
		}
		
		L -> fwd_link = newNode;
		newNode -> back_link = L;
		list_ptr -> current_list_size++;
	}


//Insert at specified inputted location if location isn't head or tail
else
{
//first iterate to the specified insert location
        for (i = 0; i < pos_index - 1; i++)
	{
		if (L != NULL)
		{
			break;
		}

		L = L -> fwd_link;
	}

//once iteration is at proper location, rewire the next and prev for the nodes in front/back of the newNode, then insert
	L -> fwd_link -> back_link = newNode;
	newNode -> fwd_link = L -> fwd_link;
	newNode -> back_link = L;
	L -> fwd_link = newNode;
	list_ptr -> current_list_size++;
}


    if (list_ptr->list_sorted_state == SORTED_LLIST)
	{ 
        list_ptr->list_sorted_state = UNSORTED_LLIST;
	}
//    my_debug_validate(list_ptr);

}





/* Inserts the element into the specified sorted list at the proper position,as defined by the comp_proc function pointer found in the header block.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use linked_insert_sorted, the list preserves its sorted nature.
 *
 * If you use linked_insert, the list will be considered to be unsorted, even if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call linked_insert_sorted, this subroutineshould generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both pointers to elements of type data_t.  The comparison procedure returns an integer code which indicates the precedence relationship between the two elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 *
 * Note: if the element to be inserted is equal in rank to an element already in the list, the newly inserted element will be placed after all the elements of equal rank that are already in the list.
 */
void linked_insert_sorted(linked_t *list_ptr, data_t *elem_ptr)
{
	assert(list_ptr != NULL); //if list empty/doesn't exist, quit
	assert(list_ptr->list_sorted_state == SORTED_LLIST); //if list isn't sorted, quit

        linked_elem_t *L = list_ptr -> list_head;	
	linked_elem_t *newNode = (linked_elem_t*)malloc(sizeof(linked_elem_t));	
	newNode -> data_ptr = elem_ptr;
	newNode -> fwd_link = NULL;
	newNode -> back_link = NULL;


//If the list is empty, push node @ head
	if (list_ptr -> list_head == NULL)
	{
	    list_ptr -> list_head = newNode;
	    list_ptr -> list_tail = newNode;
	}
	
//Insert @ head	
	else if (list_ptr -> comp_proc(elem_ptr, linked_access(list_ptr, LLPOSITION_FRONT)) == 1)
	{
	    list_ptr -> list_head -> back_link = newNode;
	    newNode -> fwd_link = list_ptr -> list_head;
	    list_ptr -> list_head = newNode;
	}	

//Insert @ tail	
	else if (list_ptr -> comp_proc(elem_ptr, linked_access(list_ptr, LLPOSITION_BACK)) == -1 || list_ptr -> comp_proc(elem_ptr, linked_access(list_ptr, LLPOSITION_BACK)) == 0)
	{
            list_ptr -> list_tail -> fwd_link = newNode;
            newNode -> back_link = list_ptr -> list_tail;
            list_ptr -> list_tail = newNode;
	}
	

//If reach this point, iterate to the correct position and insert	
	else
	{
	    
	    while (list_ptr -> comp_proc(elem_ptr, L -> data_ptr) != 1)
	    {/*
	        if (L != NULL)
	        {
	            L = L -> fwd_link;
	        }*/
	        
	        if (L -> fwd_link != NULL)
	        {
	            L = L -> fwd_link;
	        }
	    }
	    
	    	newNode -> fwd_link = L;
	        newNode -> back_link = L -> back_link;
	        newNode -> fwd_link -> back_link = newNode;
	        newNode -> back_link -> fwd_link = newNode;
	}
	
	list_ptr -> current_list_size++;


// the last line checks if the new list is correct */
//	my_debug_validate(list_ptr);
}





/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
data_t * linked_remove(linked_t *list_ptr, int pos_index)
{

    // the first line must validate the list 
//    my_debug_validate(list_ptr);
    int num_in_list = linked_entries(list_ptr);
    int i = 0;
    linked_elem_t *L = list_ptr -> list_head;
    data_t *remove;


//First iterate to the correct position   
    while (i != pos_index)
    {
        L = L -> fwd_link;
        i++;
    }
    
//If index is @ head, remove
    if (L -> back_link == NULL)
    {
        list_ptr -> list_head = list_ptr -> list_head -> fwd_link;
        
        if (num_in_list != 1)
        {
            list_ptr -> list_head -> back_link = NULL;
        }
        if (num_in_list == 1)
        {
            list_ptr -> list_tail = list_ptr -> list_head;
        }
        
        list_ptr -> current_list_size--;
        remove = L -> data_ptr;
        free(L);
        L = NULL;
        return remove;
    }

//If index is @ tail, remove    
    else if(L -> fwd_link == NULL)
    {
        list_ptr -> list_tail = list_ptr -> list_tail -> back_link;
        list_ptr -> list_tail -> fwd_link = NULL;
        list_ptr -> current_list_size--;
        remove = L -> data_ptr;
        free(L);
        L = NULL;
        return remove;
    }

//Normally remove node from list    
    else
    {
        L -> back_link -> fwd_link = L -> fwd_link;
        L -> fwd_link -> back_link = L -> back_link;
        list_ptr -> current_list_size--;
        remove = L -> data_ptr;
        free(L);
        L = NULL;
        return remove;     
    }
    
    return NULL;
}





/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 * 
 * compare_function: function to test if elem_ptr is a match to an item in the list. Returns 0 if they match and any value not equal to 0 if they do not match.
 *
 * NOTICE: pos_index is returned and is not an input value!
 *
 * The function returns a pointer to the matching element with lowest index if a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the lowest index.  If a matching element is not found, the position index that is returned should be -1. 
 *
 * pos_index: used as a return value for the position index of matching element
 *
 */
data_t * linked_elem_find(linked_t *list_ptr, data_t *elem_ptr, int *pos_index,
        int (*compare_function)(const data_t *, const data_t *))
{

    // the first line must validate the list 
//    my_debug_validate(list_ptr);



        if (list_ptr -> current_list_size == 0)
        {
            return NULL;
        }
	int i;
	linked_elem_t *L = list_ptr -> list_head;

	for (i = 0; i < list_ptr -> current_list_size; i++)
	{
		if (list_ptr -> comp_proc(L -> data_ptr, elem_ptr) == 0)//  (compare_function)(L -> data_ptr, elem_ptr) == 0
		{
			*pos_index = i;
			return L -> data_ptr;
		}
		
		L = L -> fwd_link;
	}
    
	*pos_index = -1;
    return NULL;

}





/* Removes an extremum element from the list and returns a pointer to it. If multiple elements are deemed to have equal extremum values then the element closer to the head of the linked list should be the one removed
 * 
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * compare_function: function to test if an element has the extremum value. Returns 1 if the first argumnet is more extreme than the second.  
 *
 * The function returns a pointer to the first extremum element if the list contains one or more items.  If the list is empty the return value is NULL.
 *
 * Note if the list is sorted and the objective is to move the first or last item then this function should not be used.  Instead remove the head or tail!
 */
data_t * linked_elem_remove_extremum(linked_t *list_ptr, int (*compare_function)(const data_t *, const data_t *))
{
    // the first line must validate the list 
//    my_debug_validate(list_ptr);

    int i = 0;
    int pos_index = 0;
    
    
    //checks if list exists or populated
    if (list_ptr -> current_list_size == 0 )
    {
        return NULL;
    }
    
    
    linked_elem_t *L = list_ptr -> list_head;
    data_t *extreme = L -> data_ptr;
   
  

//need to find a way to mark multiple nodes if there are duplicates. Once all dupes identified, remove the one closest to the head
    for (i = 0; i < list_ptr -> current_list_size; i++)
    {
   
  
        if ((compare_function)(L -> data_ptr, extreme) == 1)
        {
            extreme = L -> data_ptr;  
            pos_index = i; 

        }

        L = L -> fwd_link;
    }


    return linked_remove(list_ptr, pos_index);
}




/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int linked_entries(linked_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->current_list_size >= 0);
    return list_ptr->current_list_size;
}











/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void my_debug_validate(linked_t *L)
{
    assert(NULL != L); 
    assert(SORTED_LLIST == L->list_sorted_state || UNSORTED_LLIST == L->list_sorted_state);
    if (0 == L->current_list_size) assert(NULL == L->list_head && L->list_tail == NULL);
    if (NULL == L->list_tail) 
	assert(NULL == L->list_head && 0 == L->current_list_size);
    else
	assert(NULL == L->list_tail->fwd_link);
    if (NULL == L->list_head) 
	assert(NULL == L->list_tail && 0 == L->current_list_size);
    else
	assert(NULL == L->list_head->back_link);
    if (NULL != L->list_tail && L->list_tail == L->list_head) assert(1 == L->current_list_size);
    if (1 == L->current_list_size) {
        assert(L->list_head == L->list_tail && NULL != L->list_tail);
        assert(NULL != L->list_tail->data_ptr);
        assert(NULL == L->list_tail->fwd_link && NULL == L->list_head->back_link);
    }
    if (1 < L->current_list_size) {
        assert(L->list_head != L->list_tail && NULL != L->list_tail && NULL != L->list_head);
        linked_elem_t *R = L->list_head;
        int tally = 0;
        while (NULL != R) {
            if (NULL != R->fwd_link) assert(R->fwd_link->back_link == R);
            else assert(R == L->list_tail);
            assert(NULL != R->data_ptr);
            ++tally;
            R = R->fwd_link;
        }
        assert(tally == L->current_list_size);
    }
    if (NULL != L->list_head && SORTED_LLIST == L->list_sorted_state) {
        linked_elem_t *R = L->list_head;
        while (NULL != R->fwd_link) {
            assert(-1 != L->comp_proc(R->data_ptr, R->fwd_link->data_ptr));
            R = R->fwd_link;
        }
    }
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
