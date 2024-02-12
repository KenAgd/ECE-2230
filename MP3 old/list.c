/* list.c 
 * Kendric Agdaca
 * kagdaca
 * Prof. Calhoun
 * ECE 2230 Spring 2021
 * MP3
 *
 * Propose: serves as a driver for all the important background functions necessary for all atom_support.c functions to work. Also holds sorting algos
 *
 * Assumptions:  
 *
 * Bugs: 
 *
 * The interface definition for the two-way linked list ADT is based on one 
 * defined by OPNET, Inc. http://www.opnet.com/
 */

#include <stdlib.h>
#include <assert.h>

#include "datatypes.h"   /* defines data_t */
#include "list.h"        /* defines public functions for list ADT */

/* definitions for private constants used in list.c only */

#define LIST_SORTED_ASCENDING   -29634
#define LIST_SORTED_DESCENDING  -29631
#define LIST_UNSORTED           -29630
//int VALIDATE_STATUS = 1;

/* prototypes for private functions used in list.c only */
void list_debug_validate(list_t *L);
void list_bubble_sort(list_t** L, int sort_order);
void list_insertion_sort(list_t** L, int sort_order);
void list_recursive_selection_sort(list_t** L, int sort_order);
void list_selection_sort(list_t** L, int sort_order);
void list_merge_sort(list_t** L, int sort_order);
void recurseSelect(list_t *L, list_node_t *head, list_node_t *tail, int sort_order);
list_node_t* findPosition(list_t *L, list_node_t *head, list_node_t *tail, int sort_order);
list_node_t *mergeSort(list_t *lst, list_node_t *head, int sort_order);
list_node_t *subMergeSort(list_t *lst, list_node_t *A, list_node_t *B, int sort_order);
list_node_t *Split(list_node_t *head);


/* Sorts a list used a specific sorting algorithm and direction.
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * sort_type: sorting algorithm used during sorting: 1) Bubble Sort; 
 *            2)Insertion Sort; 3) Recursive Selection Sort; 
 *            4) Iterative Selection Sort; and 5) Merge Sort
 *
 * sort_order: if 1 then the items are to be sorted in descending order.
 *             if 2 then the items are to be sorted in accending order.
 *
 * return value: None.
 */
void list_sort(list_t **list_ptr, int sort_type, int sort_order)
{

//bubble sort call
	if (sort_type == 1)
	{
		list_bubble_sort(list_ptr, sort_order);			
	}


//insertion sort call
	else if (sort_type == 2)
	{
		list_insertion_sort(list_ptr, sort_order);
	}


//recursive selection sort call
	else if (sort_type == 3)
	{
		list_recursive_selection_sort(list_ptr, sort_order);
	}


//iterative selection sort call
	else if (sort_type == 4)
	{
		list_selection_sort(list_ptr, sort_order);
	}


//merge sort call
	else if (sort_type == 5)
	{
		list_merge_sort(list_ptr, sort_order);
	}
	
   (*list_ptr)->list_sorted_state = (sort_order == 2) ? LIST_SORTED_ASCENDING : LIST_SORTED_DESCENDING; 
    list_debug_validate(*list_ptr);
}



/* Sorting the list via the insertion sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_bubble_sort(list_t** L, int sort_order)
{
	int swap;
	list_node_t *current;
	list_node_t *reverseList = NULL;
	data_t *temp;


//returns if list if empty
	if ((*L) -> head == NULL)
	{
		return;
	}


//Iterates thru the list and compares a node to the node next to it. 
	do
	{
		swap = 0;
		current = (*L) -> head;

		while (current -> next != reverseList)
		{
		//ascending order
			if (sort_order == 2)
			{
			//If node A > node B, swap their data
				if ((*L) -> comp_proc(current -> data_ptr, current -> next -> data_ptr) == -1)
				{
					temp = current -> data_ptr;
					current -> data_ptr = current -> next -> data_ptr;
					current -> next -> data_ptr = temp;
					swap = 1;
				}
			
			}
	
		//descending order
			else
			{
			//If node A < node B, swap their data
				if ((*L) -> comp_proc(current -> data_ptr, current -> next -> data_ptr) == 1)
				{
					temp = current -> data_ptr;
					current -> data_ptr = current -> next -> data_ptr;
					current -> next -> data_ptr = temp;
					swap = 1;
				}
			}
	
			current = current -> next;
		}

		reverseList = current;
	}while (swap); //Function will keep iterating until there has been at least 1 swap
}


/* Sorting the list via the insertion sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_insertion_sort(list_t** L, int sort_order)
{
	list_node_t *temp = (*L) -> head;
//Creates a new list for the sorted list
	list_t *sorted = list_construct((*L) -> comp_proc, (*L) -> data_clean);
	sorted -> list_sorted_state = (sort_order == 2) ? LIST_SORTED_ASCENDING : LIST_SORTED_DESCENDING;


//Feeds list L nodes into the new list in a sorted manner. Once iteration is done, set list L = the new sorted list
	while (temp != NULL)
	{
		list_insert_sorted(sorted, temp -> data_ptr);
		temp = temp -> next;
	}

	*L = sorted;
}



/* Sorting the list via the recursive selection sort algorithm
 * 
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
*/ 
void list_recursive_selection_sort(list_t **L, int sort_order)

{
	recurseSelect((*L), (*L) -> head, (*L) -> tail, sort_order);
}



/*1st auxiliary function to recursive select sort that serves as the recursion for selection sort
 * Input: 
 * 	L: Unsorted list ptr
 * 	head: Unsorted list ptr head
 * 	tail: Unsorted list ptr tail
 * 	sort_order: the type of sort order (ascending: 2, descending: 1)
 * Output: N/A
 */
void recurseSelect(list_t *L, list_node_t *head, list_node_t *tail, int sort_order)
{
	list_node_t *position;
	data_t *temp;


//As function iterates thru the list, it finds the lowest/minimum node from list L and moves it into sorted list "temp"
	if (head != NULL)
	{
		position = findPosition(L, head, tail, sort_order);
		temp = head -> data_ptr;
		head -> data_ptr = position -> data_ptr;
		position -> data_ptr = temp;

		recurseSelect(L, head -> next, tail, sort_order);
	}
}


/*2nd auxiliary function to recursive select sort that finds greatest or least node (depending on sort_order) and returns it to recurseSelect
 * Inputs: 
 * 	L: Unsorted list ptr
 *	head: Unsorted list ptr head
 *	tail: Unsorted list ptr tail
 *	sort_order: the type of sort order (ascending: 2, descending: 1)
 * Outputs:
 * 	position: position of the node to be swapped
 */
list_node_t* findPosition(list_t *L, list_node_t *head, list_node_t *tail, int sort_order)
{
	list_node_t *temp = head;
	list_node_t *position = head;

	do
	{
	//If list is to be sorted in ascending order, find the least/lowest node and return its position back to recurseSelect
		if (sort_order == 2)
		{
			if (L -> comp_proc(temp -> data_ptr, position -> data_ptr) == 1)
			{
				position = temp;
			}
		}


	//If list is to be sorted in descending order, find the greatest/highest node and return its position back to recurseSelect
		else
		{
			if (L -> comp_proc(temp -> data_ptr, position -> data_ptr) == -1)
			{
				position = temp;
			}
		}


		temp = temp -> next;
	}while (temp != NULL);

	return position;
}



/* Sorting the list via the selection sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_selection_sort(list_t** L, int sort_order)
{
	list_node_t *temp = (*L) -> head;
	list_node_t *position;
	list_node_t *rove;
	data_t *A;


//Ascending sort order
	if (sort_order == 2)
	{
		position = temp;
		rove = temp -> next;

	//As the list is iterated thru, the function finds the lowest/least node position and moves the node into the new sorted list
		while (rove != NULL)
		{
			if ((*L) -> comp_proc(rove -> data_ptr, position -> data_ptr) == 1)
			{
				position = rove;
			}

			rove = rove -> next;
		}


		A = temp -> data_ptr;
		temp -> data_ptr = position -> data_ptr;
		position -> data_ptr = A;
		temp = temp -> next;
	}


//Descending sort order
	else
	{

	//As the list is iterated thru, the function finds the highest/greatest node position and moves the node into the new sorted list
		while (temp != NULL)
		{
			position = temp;
			rove = temp -> next;

			while (rove != NULL)
			{
				if ((*L) -> comp_proc(rove -> data_ptr, position -> data_ptr) == -1)
				{
					position = rove;
				}

				rove = rove -> next;
			}


			A = temp -> data_ptr;
			temp -> data_ptr = position -> data_ptr;
			position -> data_ptr = A;
			temp = temp -> next;
		}
	}
}




/* Sorting the list via the merge sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 *
void list_merge_sort(list_t** L, int sort_order)
{
	list_node_t* temp = (*L) -> head;
	(*L) -> head = mergeSort((*L), (*L) -> head, sort_order);
	(*L) -> head -> prev = NULL;

	while (temp -> next != NULL)
	{
		temp = temp -> next;
	}

	(*L) -> tail = temp;
	(*L) -> tail -> next = NULL;
}
*/

/*1st auxiliary function to list_merge_sort. Function splits unsorted list into two sub lists and sorts them depending on sort_order. Once sorted, merge both sub lists together.
 * Input:
 * 	lst: Unsorted list ptr
 * 	head: Unsorted list ptr head
 * 	sort_order: the type of sort order (ascending: 2, descending: 1)
 * Output: returns final merge sorted list to main merge sort function
 *
list_node_t *mergeSort(list_t *lst, list_node_t *head, int sort_order)
{

//Base case: if list length = 1 or 0, exit recursion
	if (head == NULL || head -> next == NULL)
	{
		return head;
	}

//Split list into 2 sub lists
	list_node_t *lst2 = Split(head);
	head = mergeSort(lst, head, sort_order);
	lst2 = mergeSort(lst, head, sort_order);

	return subMergeSort(lst, head, lst2, sort_order);
}
*/

/*2nd auxiliary function that splits list 1 and 2 into smaller groups
 * Input:
 * 	lst: Unsorted list ptr
 * 	A: First half of unsorted list ptr
 * 	B: Second half of unsorted list ptr
 * 	sort_order: order to sort the list
 *Output: 
 *
list_node_t *subMergeSort(list_t *lst, list_node_t *A, list_node_t *B, int sort_order)
{
	list_node_t rec_ptr;
	rec_ptr.next = NULL;
	list_node_t *tail = &rec_ptr;

	while(1)
	{
		if (A == NULL)
		{
			tail -> next = B;
			B -> prev = tail;
			break;
		}

		else if (B == NULL)
		{
			tail -> next = A;
			A -> prev = tail;
			break;
		}

		if (sort_order == 2)
		{
			if (lst -> comp_proc(A -> data_ptr, B -> data_ptr) == 1)
			{
				tail -> next = A;
				tail -> next -> prev = tail;
				A -> prev = tail;
				A = A -> next;
			}

			else
			{
				tail -> next = B;
				tail -> next -> prev = tail;
				B -> prev = tail;
				B = B -> next;
			}

			tail = tail -> next;
		}

		else
		{
			if (lst -> comp_proc(A -> data_ptr, B -> data_ptr) == -1)
			{
				tail -> next = A;
				tail -> next -> prev = tail;
				A -> prev = tail;
				A = A -> next;
			}

			else
			{
				tail -> next = B;
				tail -> next -> prev = tail;
				B -> prev = tail;
				B = B -> next;
			}

			tail = tail -> next;
		}
	}

	return rec_ptr.next;
}
*/

/*3rd auxiliary function to merge sory that splilts the unsorted list into 2 separate lists by front and back end
 * Input: 
 * 	head: Unsorted list ptr head
 * Output: 
 *
 *
list_node_t *Split(list_node_t *head)
{
	list_node_t *fast = head;
	list_node_t *slow = head;

	while (fast -> next && fast -> next -> next)
	{
		fast = fast -> next -> next;
		slow = slow -> next;
	}

	list_node_t *temp = slow -> next;
	slow -> next = NULL;
	return temp;
}
*/


/* Sorting the list via the merge sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_merge_sort(list_t** L, int sort_order)
{
  list_node_t* temp = (*L)-> head;

  (*L)-> head = mergeSort((*L), (*L)-> head, sort_order);

  (*L)-> head-> prev = NULL; //fix the pointers in the header block
    while(temp-> next != NULL){
      temp = temp-> next;
    }
    (*L)-> tail = temp;
    (*L)-> tail-> next = NULL;
}


/*1st auxiliary function to list_merge_sort. Function splits unsorted list into two sub lists and sorts them depending on sort_order. Once sorted, merge both sub lists together.
 * Input:
 * 	lst: Unsorted list ptr
 * 	head: Unsorted list ptr head
 * 	sort_order: the type of sort order (ascending: 2, descending: 1)
 * Output: returns final merge sorted list to main merge sort function
 */
list_node_t* mergeSort(list_t* list, list_node_t *head, int sort_order){

  if(head == NULL || head-> next == NULL) //base case
    return head;

  list_node_t* list2 = Split(head);

  head = mergeSort(list, head, sort_order);
  list2 = mergeSort(list, list2, sort_order);

  return subMergeSort(list, head, list2, sort_order);
}

/*2nd auxiliary function that splits list 1 and 2 into smaller groups
 * Input:
 * 	lst: Unsorted list ptr
 * 	A: First half of unsorted list ptr
 * 	B: Second half of unsorted list ptr
 * 	sort_order: order to sort the list
 *Output: 
 */
list_node_t* subMergeSort(list_t* list, list_node_t* a, list_node_t* b, int sort_order){

  list_node_t dummy;
  dummy. next = NULL;

  list_node_t* tail = &dummy;

    while (1)
	{
      		if(a == NULL)
		{
        		tail-> next = b;
        		b-> prev = tail;
        		break;
      		}
      
		else if(b == NULL)
		{
        		tail-> next = a;
        		a-> prev = tail;
        		break;
      		}
    		
		if(sort_order == 2)
		{
      			if(list-> comp_proc(a->data_ptr, b->data_ptr) == 1)
			{
        			tail-> next = a;
        			tail-> next-> prev = tail;
        			a-> prev = tail;
        			a = a-> next;
      			}
      			
			else
			{
        		tail-> next = b;
        		tail-> next-> prev = tail;
        		b-> prev = tail;
        		b = b-> next;
      			}
      
			tail = tail-> next;
    		}
    
		else
		{
      			if(list-> comp_proc(a->data_ptr, b->data_ptr) == -1)
			{
        			tail-> next = a;
        			tail-> next-> prev = tail;
        			a-> prev = tail;
        			a = a-> next;
      			}
      			
			else
			{
        			tail-> next = b;
        			tail-> next-> prev = tail;
        			b-> prev = tail;
        			b = b-> next;
      			}
      
			tail = tail-> next;
    		}
	  }

    return dummy. next;
}

/* This function splits the list into smaller lists of equal size. if the list has
 * an odd number of elements, the list with +1 elements is returned by temp.
 *
 * head = pointer to the front of the list
 */

 list_node_t* Split(list_node_t* head){

   list_node_t *fast = head;
   list_node_t *slow = head;

   while (fast-> next && fast-> next-> next)
   {
       fast = fast-> next-> next;
       slow = slow-> next;
   }
   list_node_t *temp = slow-> next;
   slow-> next = NULL;
   return temp;
 }




/* ----- below are the functions  ----- */

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.  For example, if the list is empty, NULL is returned.
 */
data_t * list_access(list_t *list_ptr, int pos_index)
{
	int count;
	list_node_t *L = list_ptr -> head;
	assert(list_ptr != NULL);
	list_debug_validate(list_ptr);


//Checks if list is empty
	if (list_ptr -> current_list_size == 0)
	{
		return NULL;
	}


//Checks for invalid or negative pos_index
	else if (list_ptr -> current_list_size < pos_index || pos_index < 0)
	{
		return NULL;
	}

//Checks if the node to access is the head
	else if (pos_index == LISTPOS_HEAD || pos_index == 0)
	{
		return list_ptr -> head -> data_ptr;
	}


//Checks if the node to access is the tail
	else if (pos_index == LISTPOS_TAIL || pos_index == list_ptr -> current_list_size - 1)
	{
		return list_ptr -> tail -> data_ptr;
	}


//Iterates thru list, checking head to tail. Loop will stop when it reaches the correct node
	else
	{
		for (count = 0; count < pos_index; count++)
		{
			L = L -> next;
		}
	}

	assert(L != NULL);
	return L -> data_ptr;
}



/* Allocates a new, empty list 
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values LIST_SORTED_ASCENDING LIST_SORTED_DESCENDING or 
 * LIST_UNSORTED
 *
 * Use list_free to remove and deallocate all elements on a list (retaining the
 * list itself).
 *
 * comp_proc = pointer to comparison function
 *
 * Use the standard function free() to deallocate a list which is no longer
 * useful (after freeing its elements).
 */
list_t * list_construct(int (*fcomp)(const data_t *, const data_t *),
        void (*dataclean)(data_t *))
{
    list_t *L;
    L = (list_t *) malloc(sizeof(list_t));
    L->head = NULL;
    L->tail = NULL;
    L->current_list_size = 0;
    L->list_sorted_state = LIST_SORTED_ASCENDING;
    L->comp_proc = fcomp;
    L->data_clean = dataclean;

    /* the last line of this function must call validate */
    list_debug_validate(L);
    return L;
}



/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 *           Note: use the comp_proc function pointer found in the list_t 
 *           header block. 
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1. 
 *
 * pos_index: used as a return value for the position index of matching element
 */
data_t * list_elem_find(list_t *list_ptr, data_t *elem_ptr, int *pos_index)
{
	list_debug_validate(list_ptr);
	int i;
	list_node_t *find = list_ptr -> head;


//Iterates thru the list. If the atom id of the current iterated node and the node compare match, then change the value of pos_index to the node position and return a pointer to the node position
	for (i = 0; i < list_ptr -> current_list_size; i++)
	{
		if (list_ptr -> comp_proc(elem_ptr, find -> data_ptr) == 0)
		{
			*pos_index = i;
			return find -> data_ptr;
		}

		find = find -> next;
	}

	*pos_index = -1;
 
    return NULL;
}



/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list, and the header block.  Use the data_clean
 * function point to free the data_t items in the list.
 */
void list_destruct(list_t *list_ptr)
{
	list_debug_validate(list_ptr);
	list_node_t *L = list_ptr -> head, *nextNode;
	int i;

//Iterates thru the lsit and frees each node
	for (i = 0; i < list_ptr -> current_list_size; i++)
	{
		nextNode = L -> next;
		free(L);
		L = nextNode;
	}

//Once each node has been freed, reset what the head and tail point to, set the list size to 0, then free the list
	list_ptr -> head = NULL;
	list_ptr -> tail = NULL;
	list_ptr -> current_list_size = 0;
	free(list_ptr);
}



/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the 
 *            list.  Index starts at 0 at head of the list, and incremented by 
 *            one until the tail is reached.  The index can also be equal
 *            to LISTPOS_HEAD or LISTPOS_TAIL (these are special negative 
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list, 
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->list_sorted_state must be equal 
 * to LIST_UNSORTED.
 */
void list_insert(list_t *list_ptr, data_t *elem_ptr, int pos_index)
{
 	assert(list_ptr != NULL);
 	int count;
	list_node_t *L;
	list_node_t *newNode = (list_node_t *)malloc(sizeof(list_node_t));
	newNode -> data_ptr = elem_ptr;

//If list is empty, starts the list with the node to be inserted and increments current list size to match
	if (list_ptr -> current_list_size == 0)
  	{
		list_ptr -> head = newNode;
		list_ptr -> tail = newNode;
		newNode -> prev = NULL;
		newNode -> next = NULL;
		list_ptr -> current_list_size++;
	}
 

//Push function (insert @ head)
	else if (pos_index == LISTPOS_HEAD || pos_index == 0)
	{
		list_ptr -> head -> prev = newNode;
		newNode -> prev = NULL;
		newNode -> next = list_ptr -> head;
		list_ptr -> head = newNode;
		list_ptr -> current_list_size++;
	}


//Append function (insert @ tail)
	else if (pos_index == LISTPOS_TAIL || pos_index >= list_ptr -> current_list_size)
	{
		list_ptr -> tail -> next = newNode;
		newNode -> next = NULL;
		newNode -> prev = list_ptr -> tail;
		list_ptr -> tail = newNode;
		list_ptr -> current_list_size++;
	}


//Insert at a certain point within list 
	else
	{
		L = list_ptr -> head;
	//Iterates till it reaches the node previous to the position to insert
		for (count = 0; count < pos_index - 1; count++)
		{
			if (L != NULL)
			{
				break;
			}

			L = L -> next;
		}

	//Rewires the next and prev for the nodes in front and behind the node to be inserted, then inserts the node
		L -> next -> prev = newNode;
		newNode -> next = L -> next;
		newNode -> prev = L;
		L -> next = newNode;
	}


	if (list_ptr->list_sorted_state != LIST_UNSORTED)
	{	
		list_ptr->list_sorted_state = LIST_UNSORTED;
	}
	list_debug_validate(list_ptr);


}



/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the comp_proc function pointer found in the header block.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use list_insert_sorted, the list preserves its sorted nature.
 *
 * If you use list_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call list_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 * This definition results in the list being in ascending order.  To insert
 * in decending order, change the sign of the value that is returned.
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 */
void list_insert_sorted(list_t *list_ptr, data_t *elem_ptr)
{
	assert(list_ptr != NULL);
	assert(list_ptr->list_sorted_state != LIST_UNSORTED);


	int count = 0;
	data_t *compA, *compB;
	list_node_t *newNode = (list_node_t*)malloc(sizeof(list_node_t));
	newNode -> data_ptr = elem_ptr;
	list_node_t *L;

   
//Pushes new node to the head if list is empty
	if(list_ptr -> current_list_size == 0)
	{ 
		list_ptr -> head = newNode; 
		list_ptr -> tail = newNode;
		newNode -> next = NULL;
		newNode -> prev = NULL;
		list_ptr -> current_list_size++;
	}

//Adds new node if list isn't empty
	else
	{
		compA = newNode-> data_ptr;
		L = list_ptr-> head;


	//Ascending order search
	        if(list_order(list_ptr) == 1)
		{
		//Checks to see if the new node goes at the head
			if(list_ptr-> comp_proc(compA, list_ptr-> head-> data_ptr) == 1)
			{ 

				list_ptr-> head-> prev = newNode; 
				newNode-> prev = NULL;            
				newNode-> next = list_ptr-> head; 
			        list_ptr-> head = newNode;        
			        list_ptr-> current_list_size++;
      			}
      		
		//compA goes somewhere else in the list	
			else
			{ 
			//iterates thru the list and locates the insert location
        			while(L -> next != NULL)
				{
	          			compB = L-> data_ptr;
	          			count = list_ptr-> comp_proc(compA, compB);
	          		
				//compA goes after current node	
					if(count == 0 || count == -1)
					{ 
	            				L = L-> next;
	          			}
	          			
				//compA goes before current node
					else
					{ 
	            				break;
	          			}
        			}

        		
			//If iteration reached end of list, append at end of list
				if(L-> next == NULL)
				{
					if (list_ptr -> comp_proc(compA, list_ptr -> tail -> data_ptr) == 1)
					{
						newNode -> prev = list_ptr -> tail -> prev;
						list_ptr -> tail -> prev = newNode;
						newNode -> next = list_ptr -> tail;

						if (newNode -> prev != NULL)
						{
							newNode -> prev -> next = newNode;
						}
					}

					else 
					{
						list_ptr -> tail -> next = newNode;
						newNode -> next = NULL;
						newNode -> prev = list_ptr -> tail;
						list_ptr -> tail = newNode;
					}

					list_ptr -> current_list_size++;
				}

			//L rover is currently at the correct insertion position and will insert new node before L
	        		else
				{ 
			        	L-> prev-> next = newNode; 
					newNode-> prev = L-> prev;
					newNode-> next = L;        
					L-> prev = newNode;        
					list_ptr-> current_list_size++;
        			}
      			}	
		}
		

	//Descending order search
			else if(list_order(list_ptr) == -1)
			{
			//Checks to see if new node goes at head
	      			if(list_ptr-> comp_proc(compA, list_ptr-> head-> data_ptr) == -1)
				{
				        list_ptr-> head-> prev = newNode;
				        newNode-> prev = NULL;           
				        newNode-> next = list_ptr-> head; 
				        list_ptr-> head = newNode;        
				        list_ptr-> current_list_size++;
	      			}
	      
			//compA goes somewhere else in the list
				else
				{ 
				//Iterates thru the list and finds the corrent insert position
				        while(L-> next != NULL)
					{ 
	          				compB = L-> data_ptr;
	          				count = list_ptr-> comp_proc(compA, compB);
	          			
					//compA goes after current node
						if(count == 0 || count == 1)
						{
					        	L = L-> next;
	          				}

					//compA goes before current node
	          				else
						{ 
							break;
	          				}
	        			}
	
	        		//If iteration reaches end of list, append new node at end of list
					if(L-> next == NULL)
					{ 
						if (list_ptr -> comp_proc(compA, list_ptr -> tail -> data_ptr) == -1)
						{
							newNode -> prev = list_ptr -> tail -> prev;
							list_ptr -> tail -> prev = newNode;
							newNode -> next = list_ptr -> tail;

							if (newNode -> prev != NULL)
							{
								newNode -> prev -> next = newNode;
							}
						}

						else
						{
							list_ptr -> tail -> next = newNode;
							newNode -> next = NULL;
							newNode -> prev = list_ptr -> tail;
							list_ptr -> tail = newNode;
						}

						list_ptr -> current_list_size++;
	        			}
	        			
				//L rover is at correct insertion point. Insert new node before L
					else
					{ 
	          				L-> prev-> next = newNode; 
	          				newNode-> prev = L-> prev; 
	          				newNode-> next = L;        
	          				L-> prev = newNode;       
	          				list_ptr-> current_list_size++;
	        			}
	      			}
	    		}
	  	}

    
    list_debug_validate(list_ptr);
}



/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
data_t * list_remove(list_t *list_ptr, int pos_index)
{
	int count;
	list_node_t *L = list_ptr -> head;
	data_t *atom_remove = NULL;




//Checks if list is empty
	if (list_ptr -> current_list_size < pos_index ||list_ptr -> current_list_size == 0)
	{
		return NULL;
	}


//Checks if theres only one node in the list. If so, completely empty the list and set list size = 0
	else if (list_ptr -> current_list_size == 1)
	{
		atom_remove = list_ptr -> head -> data_ptr;
		list_ptr -> head = NULL;
		list_ptr -> tail = NULL;
		list_ptr -> current_list_size = 0;
	}


//If pos_index is the head, then remove head and re-adjust new head
	else if (pos_index == LISTPOS_HEAD || pos_index == 0)
	{
		atom_remove = list_ptr -> head -> data_ptr;
		list_ptr -> head -> next -> prev = NULL;
		list_ptr -> head = list_ptr -> head -> next;
		list_ptr -> current_list_size--;
	}


//If pos_index is the tail, then remove tail and re-adjust new tail
	else if (pos_index == list_ptr -> current_list_size -1 || pos_index == LISTPOS_TAIL)
	{
		atom_remove = list_ptr -> tail -> data_ptr;
		list_ptr -> tail -> prev -> next = NULL;
		list_ptr -> tail = list_ptr -> tail -> prev;
		list_ptr -> current_list_size--;
	}



//Iterates through the list until it reaches pos_index. When it does, it switches the previous and next nodes to point to each other, then removes pos_index	
	else 
	{
		for (count = 0; count < pos_index; count++)
		{
			L = L -> next;
		}

		atom_remove = L -> data_ptr;
		L -> prev -> next = L -> next;
		L -> next -> prev = L -> prev;
		list_ptr -> current_list_size--;
	}

 
	list_debug_validate(list_ptr);
	return atom_remove;
}



/* Reverse the order of the elements in the list.  Also change the 
 * list_sorted_state flag.  This function can only be called on a list
 * that is sorted.
 *
 * list_ptr: pointer to list-of-interest.  
 */
void list_reverse(list_t *list_ptr)
{
	assert(list_order(list_ptr) != 0);

	list_node_t *temp = NULL;
	list_node_t *current = list_ptr -> head;


//Switches the head and tail first before reversing middle nodes
	temp = list_ptr -> head;
	list_ptr -> head = list_ptr -> tail;
	list_ptr -> tail = temp;


//Reverses what each node is pointing to as it iterates through the list
	while(current != NULL)
	{
		temp = current -> prev;
		current -> prev = current -> next;
		current -> next = temp;
		current = current -> prev;
	}

	list_ptr -> list_sorted_state = list_order(list_ptr) == 1 ? LIST_SORTED_DESCENDING : LIST_SORTED_ASCENDING;
	 
	list_debug_validate(list_ptr);
}



/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int list_size(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->current_list_size >= 0);
    return list_ptr->current_list_size;
}



/* Obtains the sort status and order of the specified list. 
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns 
 *    1: the list is sorted in ascending order
 *   -1: descending order
 *    0: the list is not sorted but a queue
 */
int list_order(list_t *list_ptr)
{
    assert(list_ptr != NULL);
    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING)
        return 1;
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING)
        return -1;
    else if (list_ptr->list_sorted_state == LIST_UNSORTED)
        return 0;
    else 
        exit(5);  // this should not happen
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
void list_debug_validate(list_t *L)
{
#ifdef VALIDATE
    list_node_t *N;
    int count = 0;
    assert(L != NULL);
//    if (VALIDATE_STATUS != 1)
//        return;

    if (L->head == NULL)
        assert(L->tail == NULL && L->current_list_size == 0);
    if (L->tail == NULL)
        assert(L->head == NULL && L->current_list_size == 0);
    if (L->current_list_size == 0)
        assert(L->head == NULL && L->tail == NULL);
    assert(L->list_sorted_state == LIST_SORTED_ASCENDING 
            || L->list_sorted_state == LIST_SORTED_DESCENDING
            || L->list_sorted_state == LIST_UNSORTED);

    if (L->current_list_size == 1) {
        assert(L->head == L->tail && L->head != NULL);
        assert(L->head->next == NULL && L->head->prev == NULL);
        assert(L->head->data_ptr != NULL);
    }
    if (L->head == L->tail && L->head != NULL)
        assert(L->current_list_size == 1);
    if (L->current_list_size > 1) {
        assert(L->head != L->tail && L->head != NULL && L->tail != NULL);
        N = L->head;
        assert(N->prev == NULL);
        while (N != NULL) {
            assert(N->data_ptr != NULL);
            if (N->next != NULL)
                assert(N->next->prev == N);
            else
                assert(N == L->tail);
            count++;
            N = N->next;
        }
        assert(count == L->current_list_size);
    }
    if (L->list_sorted_state != LIST_UNSORTED && L->head != NULL) {
        N = L->head;
        int comp_val = -1 * list_order(L);
        while (N->next != NULL) {
            assert((L->comp_proc)(N->data_ptr, N->next->data_ptr) != comp_val);
            N = N->next;
        }
    }
#endif
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */

