/* driver.c

Writing and debugging MP2 is challenging, and it is particularly hard if you
try and write all the code before you do any testing. It is much easier to
break the problem into small pieces and test each piece.

For MP2, one approach is to write linked_insert first, since it might be the
easiest algorithm and it depends only on linked_construct.

   For testing, we use a simple main function in this file.  This file is for
   testing only, and you will NOT submit it as it is for your personal testing
   during development.

Below are a few simple examples that allocates a few structures and inserts 
them into a list.

With this file, you can compile and run linkedlist.c on its own using 

   gcc -Wall -g driver.c linkedlist.c ens_support.c -o driver

The compiling command is also in the makefile. Do
   make driver 

Do this testing before you make ANY changes to ens_support.c.  The only 
reason we need to compile ens_support.c is for the ens_compare function.
The linker needs to find the compare_fun function and it is found in the template
   ens_support.c file (with no changes) 
*/

#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"
#include "ens_support.h"

/* prototype for list debug print */
void linked_debug_print(linked_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    linked_t *Lptr = NULL;
    int seq_num = 0;

    Lptr = linked_construct(ens_compare_epid);

    // create one item to test linked_insert
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->ep_id = 555;
    datap->rec_num = ++seq_num;
    linked_insert(Lptr, datap, 0);
    datap = NULL;

    // test linked_access with one item in list
    datap = linked_access(Lptr, LLPOSITION_FRONT);
    if (datap != NULL) {
        printf("should find 555 and found (%d)\n", datap->ep_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }
    datap = NULL;

    // add a second item 
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->ep_id = 333;
    datap->rec_num = ++seq_num; 
    linked_insert(Lptr, datap, LLPOSITION_BACK);
    datap = NULL;

    // add a third item
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->ep_id = 444;
    datap->rec_num = ++seq_num;
    linked_insert(Lptr, datap, LLPOSITION_BACK);
    datap = NULL;

    // find all three and print 
    datap = linked_access(Lptr, 0);   /* same as LLPOSITION_FRONT */
    if (datap != NULL) {
        printf("Position 0 should find 555 and found (%d)\n", datap->ep_id);
        datap = linked_access(Lptr, 1);
        printf("Position 1 should find 333 and found (%d)\n", datap->ep_id);
        datap = linked_access(Lptr, LLPOSITION_BACK);
        printf("Position 2 should find 444 and found (%d)\n", datap->ep_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }

    //Next try to use linked_debug_print 
    printf("\nTest of list debug print function\n\n");
    linked_debug_print(Lptr);

    // you should repeat the above tests with differents insertion orders
    //
    // a test for linked_elem_remove_extremum
   
    datap = linked_elem_remove_extremum(Lptr, ens_compare_recnum);
    if (datap != NULL) {
       printf("Removed rec_num extremum %d with id %d\n", datap->rec_num, datap->ep_id);
    } else {
       printf("only case don't remove is if list is empty\n");
    }
    


    datap = linked_access(Lptr, 0);   /* same as LLPOSITION_FRONT */
    if (datap != NULL) {
        printf("Position 0 should find 555 and found (%d)\n", datap->ep_id);
        datap = linked_access(Lptr, 1);
        printf("Position 1 should find 333 and found (%d)\n", datap->ep_id);
        datap = linked_access(Lptr, LLPOSITION_BACK);
        printf("Position 2 should find 444 and found (%d)\n", datap->ep_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }




    // next you may want to work on linked_insert_sorted, so replace the above
    // linked_insert commands with linked_insert_sorted
 
    // after the insertion functions work, you can try the find command
 
    // Uncomment this section to test linked_elem_find 
/*
    int position = -9876;
    data_t my_template;
    my_template.ep_id = 444;
    my_template.rec_num = ++seq_num;
    data_t *foundp = linked_elem_find(Lptr, &my_template, &position, ens_compare_epid);
    printf("\nTest of list elem find\n");
    if (foundp != NULL) {
        printf("looked for %d and found %d in position %d\n",
            my_template.ep_id, foundp->ep_id, position);
        if (my_template.ep_id != foundp->ep_id) {
            printf("ERROR: returned pointer to wrong record\n");
        }
        if (position < -1) {
            printf("ERROR: did not set position\n");
        }
    } else {
        printf("looked for %d and did not find. Is this an error? \n", 
            my_template.ep_id);
    }

	
    linked_destruct(Lptr);
    // End of tests with unsorted list
 */
 /*
    // Uncomment this section to try some tests on a sorted list 
    
    linked_t *Lsortptr = linked_construct(ens_compare_epid);

    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->ep_id = 23;
    datap->rec_num = ++seq_num;
    linked_insert_sorted(Lsortptr, datap);

    // add a second item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->ep_id = 7;
    datap->rec_num = ++seq_num;
    linked_insert_sorted(Lsortptr, datap);

    // add a third item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->ep_id = 15;
    datap->rec_num = ++seq_num;
    linked_insert_sorted(Lsortptr, datap);

    printf("\nHere is the sorted list\n\n");
    linked_debug_print(Lsortptr);

    linked_destruct(Lsortptr);
    */
    return 0;
}

/*

   Next you will want to write your own linked_debug_print function to print a
   list. Then you can do "before and after" testing. That is, print the list
   before a change, and print the list after the change to verify that the change
   worked.

   Something like
*/

void linked_debug_print(linked_t *list_ptr) {
    linked_elem_t *rover;
    int count;

    printf("list size: %d\n", list_ptr->current_list_size);
    rover = list_ptr->list_head;
    count = 0;
    while(rover != NULL) {
        printf("Pos: %d contains ID: %d and seq# %d\n", count, 
                rover->data_ptr->ep_id, rover->data_ptr->rec_num);
        rover = rover->fwd_link;
        count++;
    }
    if (count != list_ptr->current_list_size)
        printf("\nERROR: the list size is wrong! count is %d\n", count);
}

