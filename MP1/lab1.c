/* lab1.c
 * Kendric Agdaca
 * Kagdaca
 * ECE 2230 Fall 2021
 * MP1
 *
 * NOTE:  You must update all of the following comments!
 *
 * Purpose: A template for MP1 
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * An outline for the interactive menu input is provided.  Details need to be
 * completed but format of the commands and the output generated by the
 * print statements must not be changed. You have to call the corrent 
 * print statement in response to the input
 *
 * Bugs: 
 * 	-Struggles to do clearold when only 1 node left
 * 	-Getting a valgrind error from make_epid_record
 *
 * See the ECE 2230 programming guide
 *
 * NOTE: if it forbidden to access any of the members in the enslist_t 
 * structure.   The members that in this structure 
 * MUST NOT be found in this file or it is a design violation.  Instead you 
 * must utilize the enslist_ fuctions found in the enslist header file to
 * access any details of the list.
 *
 * One of the requirements is to verify you program does not have any 
 * memory leaks or other errors that can be detected by valgrind.  Run with
 * your test script(s):
 *      valgrind --leak-check=full ./lab1 < your_test_script
 * 
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 l
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "enslist.h"

#define MAXLINE 256

// function prototypes for lab1.c
struct epid_record_t *make_epid_record(int id, int seq_num);
void print_epid_rec(struct epid_record_t *rec);

int main(int argc, char *argv[])
{
	char line[MAXLINE];
	char command[MAXLINE];
	char junk[MAXLINE];
	int sequence_num = 0;
	int list_size = -1;
	int num_items;
	int input_1;
	int i;
	struct epid_record_t *rec_ptr = NULL;
	struct enslist_t *my_list;

	if (argc != 1) 
	{
		printf("Usage: ./lab1\n");
        	exit(1);
    	}
    
    
	printf("Welcome to lab1. Possible commands:\n");
	printf("CREATE x\nADDID x\nQUERY x\nCLEAROLD x\nTRIM\n");
	printf("MATCH x (end list with -1)\nSTATS\nPRINT\nQUIT\n");
	// remember fgets includes newline \n unless line too long 

//controls menu commands and runs necessary functions
	while (fgets(line, MAXLINE, stdin) != NULL) 
   	{
        	num_items = sscanf(line, "%s%d%s", command, &input_1, junk);

	
        	if (num_items == 2 && strcmp(command, "CREATE") == 0) 
		{
            		list_size = input_1;

	    		//validates that list size is isn't negative or zero
	    		if (list_size < 1) 
	    		{
                		printf("Cannot create list: invalid size %d\n", list_size);
            		}
	   

	    		//if list size isn't negative or 0, create new list of "list_size" size. If list already exists, destruct old list and create new list
	    		else 
	    		{
/*                		if (my_list -> num_epids > 0)
                		{
                		    enslist_destruct(my_list);
                		}
  */              		
				my_list = enslist_construct(list_size); 
                		printf("New list with size: %d\n", list_size);
            		}
        	}













       		else if (num_items == 1 && strcmp(command, "QUIT") == 0) 
		{
			enslist_destruct(my_list); //destruct will handle the iterating and thru each index and free 
			free(rec_ptr);
			printf("Goodbye\n");
        	    	break;
        	}














       		else if (num_items == 2 && strcmp(command, "ADDID") == 0) 
		{
            		sequence_num++;
            		rec_ptr = make_epid_record(input_1, sequence_num);
	    		num_items++;



            		// you have to figure out what goes here
            		// and call the correct printf command
            		int added_return = enslist_add(my_list, rec_ptr);
			sort(my_list);
            		if (added_return == 1) 
	    		{
                		printf("Inserted: %d\n", rec_ptr->ep_id);
            		}
	   
	   
	    		else if (added_return == 2) 
	    		{
                		printf("Updated: %d\n", rec_ptr->ep_id);
            		}
	   
	   
	    		else if (added_return == 3) 
	    		{
                		printf("Removed old record and added: %d\n", rec_ptr->ep_id);
            		}
	   
	   
	    		else if (added_return == 0) 
	    		{
                		printf("The list does not exist.  Record rejected: %d\n", rec_ptr->ep_id);
				free(rec_ptr);
            		}
	   
	   
	    		else 
	    		{
                		printf("Error with return value! Fix your code.\n");
            		}
            

	    		rec_ptr = NULL;
			free(rec_ptr);
        	}
       











		else if (num_items == 2 && strcmp(command, "QUERY") == 0) 
		{
            		int num_in_list = enslist_id_count(my_list); 
           
	   
	    		if (num_in_list == 0) 
	    		{
				printf("List empty. Did not find ephemeral ID %d\n", input_1);
	    		}
	   
	   
	    		else 
	    		{
                		printf("List has %d records.  Looking for EPID %d\n", num_in_list, input_1);
		

				// lookup EPID and print if found
       	        		rec_ptr = enslist_lookup(my_list, input_1);
       //  			rec_ptr = enslist_remove_epid(my_list, input_1);       
		
				if (rec_ptr == NULL) 
				{
					printf("Did not find ephemeral ID %d\n", input_1);
//                    			print_epid_rec(rec_ptr);
                		}
	       
	
				else 
				{
                    			print_epid_rec(rec_ptr);
                		}
            		}
            		rec_ptr = NULL;
        	} 
	
	












		else if (num_items == 2 && strcmp(command, "CLEAROLD") == 0) 
		{
            		int num_removed = enslist_remove_old(my_list, input_1);
 
	            	// remove all records older than input_1
            		if (num_removed == 0) 
	    		{
                		printf("No old records found: %d\n", input_1);
            		}
	   

	    		else 
	    		{
                		printf("Removed %d records older than %d\n", num_removed, input_1);
            		}
        	}
       














		else if (num_items == 1 && strcmp(command, "TRIM") == 0) 
		{
            		int rec_removed = enslist_remove_smallest(my_list);

            		
            		if (rec_removed == -1) 
	    		{
				printf("The list is empty and no ID removed\n");
	    		}
	    
	    
	    		else 
	    		{
                		printf("Removed oldest EPID %d\n", rec_removed);
            		}
        	}		 
	


	








		else if (num_items == 2 && strcmp(command, "MATCH") == 0) 
		{
            		int matches = 0;
            		int match_id = -1;
            		int check_count = 0;
            
	    
	    		while (fgets(line, MAXLINE, stdin) != NULL) 
	    		{
                		num_items = sscanf(line, "%d%s", &match_id, junk);
                
		
				if (num_items == 1 && match_id > -1) 
				{
                    			check_count++;
                    			rec_ptr = enslist_remove_epid(my_list, match_id);
        //  				rec_ptr = enslist_lookup(my_list, match_id);          
		    
		    			if (rec_ptr != NULL) 
		    			{
                        			matches++;
                        			printf("%d: ", matches);
                        			print_epid_rec(rec_ptr);
                    			}
                		}



	       			else if (num_items == 1 && match_id == -1) 
				{
                    			printf("Checked %d IDs and found %d matches\n", check_count, matches);
                    			break;
                		}
	       	
		
				else 
				{
                    			printf("BAD INPUT! Not a valid ID: %s", line);
                    			printf("Cannot recover from this error so giving up!\n");
                    			exit(1);
                		}
            		}
            
	    
	    		if (matches >= input_1) 
	    		{
                		printf("Exposure notification\n");
            		}
        	} 
	
	
	











		else if (num_items == 1 && strcmp(command, "STATS") == 0) 
		{
            		// get the number in list and size of the list
            		int num_in_list = enslist_id_count(my_list);
            		int size_of_list = enslist_list_size(my_list);
            		printf("Number records: %d, Array size: %d\n", num_in_list, size_of_list);
        	}
       	
	







		else if (num_items == 1 && strcmp(command, "PRINT") == 0) 
		{
            		int num_in_list = enslist_id_count(my_list);
            
	    
	    		if (num_in_list == 0) 
	    		{
				printf("List empty\n");
	    		}
	   
	   
	    		else 
	    		{
                		printf("List has %d records\n", num_in_list);
                
		
				for (i = 0; i < num_in_list; i++) 
				{
                    			printf("%d: ", i+1);
					rec_ptr = NULL;
                    			rec_ptr = enslist_access(my_list, i);

					if (rec_ptr != NULL)
					{
                    				print_epid_rec(rec_ptr);
					}
                		}
            		}
            		rec_ptr = NULL;
        	} 
	
	
	

		else 
		{
            		printf("# %s", line);
        	}
	}



    exit(0);
}

/* Prompts user for data associated with ephemeral ID
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function is the EPID and the sequence number.
 *
 * There output is the dynamic memory block with the data filled in.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
struct epid_record_t *make_epid_record(int id, int seq_num)
{
    char line[MAXLINE];
    char str[MAXLINE];
    struct epid_record_t *new; 

    new = (struct epid_record_t *) calloc(1, sizeof(struct epid_record_t));
    assert(new != NULL);

    new->ep_id = id;
    new->rec_num = seq_num;
    printf("time received:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->time_received);
    printf("source type:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->source_type);
    printf("MAC address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->mac_addr);

    printf("Authenticated (T/F):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%s", str);
    if (strcmp(str, "T")==0 || strcmp(str, "t")==0)
	new->authenticated = 1;
    else
	new->authenticated = 0;

    printf("Privacy (none|standard|strong|NSA):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%s", str);
    if (strcmp(str, "standard")==0)
	new->privacy = 1;
    else if (strcmp(str, "strong")==0)
	new->privacy = 2;
    else if (strcmp(str, "NSA")==0)
	new->privacy = 3;
    else
	new->privacy = 0;

    printf("Channel:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->channel);

    printf("RSSI:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%f", &new->rssi);

    printf("Band (2.4|3.5|5.0):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%f", &new->band);
    printf("\n");
    return new;
}

/* print the information for a particular secondary user record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_epid_rec(struct epid_record_t *rec)
{
    const char *pri_str[] = {"none", "standard", "strong", "NSA"};
    assert(rec != NULL);
    printf("EPID: %d, Rec# %d, Time: %d", rec->ep_id, rec->rec_num, rec->time_received);
    printf(" Src: %d, MAC: %d,", rec->source_type, rec->mac_addr);
    printf(" Auth: %s,", rec->authenticated ? "T" : "F"); 
    printf(" Pri: %s, C: %d, RSSI: %g, B: %g\n", pri_str[rec->privacy], 
            rec->channel, rec->rssi, rec->band);
}


/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
