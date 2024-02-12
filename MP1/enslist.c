/* enslist.c
 * Kendric Agdaca
 * Kagdaca
 * ECE 2230 Fall 2021
 * MP1
 *
 * Purpose: Performs all the menu commands for lab1.c
 *
 * Assumptions:
 * 
 * Bugs:
 */

// The ONLY members you can access in a epid_record_t block is 
// ep_id, rec_num, or time_received.  All other members in this structure cannot be used
// in this file. 

#include <stdlib.h>
#include "enslist.h"
#include <limits.h>


/*
 * Purpose: Creates "size" amount of nodes when called
 * Input: 
 *  -size: amount of nodes to be created
 * Output: Returns a pointer to the header block of the newly created list
 */
struct enslist_t *enslist_construct(int size)
{
	int i;
	
//This chunk handles initializing the list and its list stats
	struct enslist_t *list_ptr = (struct enslist_t*)malloc(size*sizeof(struct epid_record_t*));
	list_ptr -> epids_ptr = (struct epid_record_t**)malloc(size*sizeof(struct enslist_t));
	list_ptr -> ens_size_list = size;
	list_ptr -> num_epids = 0;

//Handles creating each individual node
	for (i = 0; i < size - 1; i++)
	{
		list_ptr -> epids_ptr[i] = NULL;
	}


    return list_ptr;
}





/*
 * Purpose: frees all nodes in the linked list
 * Input:
 *  -ptr: current list of nodes
 * Output: None
 */
void enslist_destruct(struct enslist_t *ptr)
{
	int i;


//Goes down the linked list and individually frees each node. Then it frees the ptr to the list, then frees the list itself
	for (i = 0; i > ptr -> num_epids; i++)
	{
		free(ptr -> epids_ptr[i]);
		ptr -> epids_ptr[i] = NULL;
	}
	free(ptr -> epids_ptr);
	free(ptr);
}





/*
 * Purpose: sequentially inserts a new node into the list. If entry already exists, update that node with any new info. If list is full and entry isn't a duplicate, remove smallest rec_num and sequentially insert.
 * Input:
    -list_ptr: pointer to the current list
    -rec_ptr: pointer to the node to be inserted
 * Output: 
    -If normal insert, return 1
    -If duplicate replacement, return 2
    -If list is full and not a duplicate entry, return 3
    -If list doesn't exist, return 0
 */
int enslist_add(struct enslist_t *list_ptr, struct epid_record_t *rec_ptr)
{
	int i;
	int compare = INT_MAX;
	int index;

//if list doesn't exist, return 0
	if (list_ptr == NULL)
	{
		return 0;
	}


//If list is empty, push node
	if (list_ptr -> num_epids == 0)
	{
		list_ptr -> epids_ptr[0] = rec_ptr;
		list_ptr -> num_epids++;
		return 1;
	}


//check for dupes. If theres a dupe, update old info
	for (i = 0;i < list_ptr -> num_epids - 1; i++)
	{
		if ((rec_ptr -> ep_id == list_ptr -> epids_ptr[i] -> ep_id ))
		{
			list_ptr -> epids_ptr[i] = rec_ptr;
			return 2;
		}
	}


//If list is full and no dupes, find node position of smallest rec_num
	if ((list_ptr -> num_epids == list_ptr -> ens_size_list))
	{

		for (i = 0; i < list_ptr -> num_epids; i++)
		{
		//while list is being searched thru, find the smallest rec_num node to prep for removal if there's no dupes in the list	
			if (compare > list_ptr -> epids_ptr[i] -> rec_num)		
			{
				compare  = list_ptr -> epids_ptr[i] -> rec_num;
				index = i;
			}
		}
		list_ptr -> epids_ptr[index] = rec_ptr;
		return 3;
	}


//if it reaches this point, means list isn't full and no dupes. Append new node  and return 1
	list_ptr -> epids_ptr[list_ptr -> num_epids] = rec_ptr;
	list_ptr -> num_epids++;
	return 1;
}


/*
 * Purpose: auxiliary function that sorts the current list
 * Input: 
 *  -list_ptr: pointer to the current list
 * Output: None
 */

void sort(struct enslist_t *list_ptr)
{
	int i, j, index;
	
//Iterate thru the list
	for (j = 0; j < list_ptr -> num_epids - 1; j++)
	{
		for (i = 0; i < list_ptr -> num_epids - 1; i++)
		{
			//if the next node that would be used for comparison is node, break the loop
			if (list_ptr -> epids_ptr[i+1] == NULL)
			{
				break;
			}
			
			//If current node epid > next node epid, swap their info
			if (list_ptr -> epids_ptr[i] -> ep_id > list_ptr -> epids_ptr[i+1] -> ep_id)
			{
				index = i;
				swapNodes(list_ptr, index);
			}
		}
	}
}



/*
 * Purpose: auxiliary function that swaps two nodes info
 */
void swapNodes(struct enslist_t *list_ptr, int index)
{
	struct epid_record_t *temp;
	temp = list_ptr -> epids_ptr[index];
	list_ptr -> epids_ptr[index] = list_ptr -> epids_ptr[index+1];
	list_ptr -> epids_ptr[index+1] = temp;
}


/*
 * Purpose: returns a pointer to the specified ep_id
 * Input:
    -list_ptr: pointer to the current list
 * Output: 
    -If node is found, return pointer to the node
    -If node isn't found, return NULL
 */
struct epid_record_t *enslist_lookup(struct enslist_t *list_ptr, int id)
{
	int i;

//Iterate thru list and compare each node's id to the id to be found. If found, return pointer to specific index. If not, return NULL
	for (i = 0;i < list_ptr -> num_epids; i++)
	{
		if (list_ptr -> epids_ptr[i] -> ep_id == id)
		{
			return list_ptr -> epids_ptr[i];
		}
	}

    return NULL;
}




/*
 * Purpose: returns a pointer to the node position of the specified input
 * Input:
 *  -list_ptr: current linked list
 *  -index: node to be found
 * Output: pointer to the specified index position
 */
struct epid_record_t *enslist_access(struct enslist_t *list_ptr, int index)
{
	//checks if index search is greater than the list size or if its a negative. Also checks if the node to be found is empty or not. If any are true, return NULL
	if (index > list_ptr -> ens_size_list || index < 0 ) 
	{
		return NULL;
	}

	
	//Checks if the node is populated. If empty, return NULL
	else if (list_ptr -> epids_ptr[index] == NULL)
	{
		return NULL;
	}


	//if safeguard if isn't triggered, return pointer to the specified node
	else
	{
		return list_ptr -> epids_ptr[index];
	}
}




/*
 * Purpose: Finds the first node whose epid matches the input. The matched node is removed and the gap in the list is closed
 * Input:
 *  -list_ptr: pointer to the current list
 *  -id: the epid ID to be matched
 * Output: If a match is found, return found node. If no match found, return NULL
 *
 */
struct epid_record_t *enslist_remove_epid(struct enslist_t *list_ptr, int id)
{
	int i;
	struct epid_record_t *ens_remove = NULL;


//iterates thru the list and tries to find a matching ep_id.
	for (i = 0; i < list_ptr -> num_epids -1; i++)
	{
		//if a match is found, remove the links from the node, remove its data, and close the gap
		if (list_ptr -> epids_ptr[i] -> ep_id == id)
		{
			ens_remove = list_ptr -> epids_ptr[i];


		//Moves node to be removed to the back of the list and NULL its data
			for (i = i; i < list_ptr -> num_epids - 1; i++)
			{
				list_ptr -> epids_ptr[i] = list_ptr -> epids_ptr[i + 1];
			}
			list_ptr -> epids_ptr[list_ptr -> num_epids + 1] = NULL;
			list_ptr -> num_epids--;


		    	return ens_remove;	
		}
	}
	return NULL;
}




/*
 * Purpose: removes all nodes with a "time_received" value smaller than the "time" input
 * Input: 
 *  -list_ptr: pointer to the current list
 *  -time: value which defines the which nodes should be deleted (if smaller)
 * Output: # of nodes removed
 */
int enslist_remove_old(struct enslist_t *list_ptr, int time)
{
	int i, j; 
	int count = 0;

//if list is empty or doesn't exist, return 0
	if (list_ptr -> num_epids == 0 || list_ptr == NULL)
	{
		return 0;
	}
	
//If list is populated, iterate thru and compare current node and inputted "time"
	for (i = 0; i < list_ptr -> num_epids; i++)
	{
	//If theres only 1 node in the list and it matches, just free it and NULL its data
		if (list_ptr -> num_epids == 1 && list_ptr -> epids_ptr[0] -> time_received < time)
		{
			free(list_ptr -> epids_ptr[0]);
			list_ptr -> epids_ptr[0] = NULL;
			list_ptr -> num_epids--;
			count++;
			return count;
		}

	//if the current node's time received < inputted time, free the node and its data then move it to the back of the list to keep the list sequential
		if (list_ptr -> epids_ptr[i] -> time_received < time)
		{
			free(list_ptr -> epids_ptr[i]);
			list_ptr -> epids_ptr[i] = NULL;
			count++;
		
			for (j = i; j < list_ptr -> num_epids; j++)
			{
				list_ptr -> epids_ptr[j] = list_ptr -> epids_ptr [j + 1];
			}
			list_ptr -> num_epids--;
			i--;
		}
	}
	return count;
}



/*
 * Purpose: removes the node with the smallest rec_num value
 * Input: 
 *  -list_ptr: pointer to the current list
 * Output: 
 *  -If a node is removed, return its epid value
 *  -If no node is removed, return -1
 */
int enslist_remove_smallest(struct enslist_t *list_ptr)
{
	int i, compare, index, ep_val;
	compare = INT_MAX;


//checks if the list empty or not even created. If so, return -1
	if (list_ptr == NULL || list_ptr -> ens_size_list == 0)
	{
		return -1;
	}


//Iterates thru the list and finds the index position of the smallest rec_num
	for (i = 0; i < list_ptr -> num_epids /**/; i++)
	{
		if (compare > list_ptr -> epids_ptr[i] -> rec_num)
		{
			compare = list_ptr -> epids_ptr[i] -> rec_num;
			index = i;
			ep_val = list_ptr -> epids_ptr[i] -> ep_id;
		}
	}


//Once node with smallest rec_num has been identified, send to back of list to be deleted. Once deleted, return its ep_id value to be printed in lab1.c
	for (i = index; i < list_ptr -> num_epids /**/; i++)    
	{
		list_ptr -> epids_ptr[i] = list_ptr -> epids_ptr[i + 1];
	}
	list_ptr -> epids_ptr[list_ptr -> num_epids/**/] = NULL;
	list_ptr -> num_epids--;

	return ep_val;
}



/*
 * Purpose: returns the size of the list
 * Input:
 *  -list_ptr: pointer to the current list
 * Output: If the list doesn't exist, return 0. If list exists, return ens_size_list
 */
int enslist_list_size(struct enslist_t *list_ptr)
{
	if (list_ptr -> ens_size_list == 0)
	{
		return 0;
	}

    return list_ptr -> ens_size_list;
}



/*
 * Purpose: returns the current number of EPID records in the list
 * Input:
    -list_ptr: pointer to the current list
 * Output: If the list doesn't exist, return 0. If list exists, returns num_epids
 */
int enslist_id_count(struct enslist_t *list_ptr)
{
	if (list_ptr -> num_epids == 0)
	{
		return 0;
	}

    return list_ptr -> num_epids;
}

