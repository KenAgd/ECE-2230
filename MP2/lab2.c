/* lab2.c
 * Kendric Agdaca
 * Kagdaca
 * ECE 2230 Fall 2021
 * MP2
 *
 * NOTE:  You must update the comments in the file header comment block!
 *
 * Purpose: A template for MP2 
 *
 * Assumptions: The main function collects input commands and
 *              calls the appropriate ens function.
 *
 *              Other than updating comments you will not change this file.
 *
 * See the ECE 2230 programming guide
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
 *
 * Comment for future redesign: don't mix work of collecting list of 
 *     match ids with doing work on ENS lists.  Instead collect all ids
 *     into another list.  Then pass the list to ens_support to process.
 *     Advantage: keeps collecting input separate from work on list
 *     Disadvantage: requires storing a list that is only processed once.  This
 *                   adds a lot of code that does not make implementation easier.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "linkedlist.h"
#include "ens_support.h"

#define MAXLINE 256

int main(int argc, char *argv[])
{
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int command_number;   // number found with command
    int num_items;
    linked_t *unsorted_list = NULL;  // Pointer to unsorted list
    linked_t *sorted_list = NULL;  // Pointer to sorted list

    if (argc != 1) {
        printf("Usage: ./lab2 \n");
        exit(1);
    }
    printf("Welcome to lab2.\n");
    printf("Contacts   : CREATE size; ADDID id; QUERY id; CLEAROLD time; TRIM; PRINT\n");
    printf("           : MATCH thresh (end list of ids with -1)\n");
    printf("Exposures  : RECEXP time; CLEAREXP; PRINTEXP\n");
    printf("           : STATS; QUIT\n");

    // this list is sorted and must be constructed with CREATE command
    sorted_list = NULL;
    int create_size = -1;

    // this list is unsorted 
    unsorted_list = ens_create("Exposure");

    // remember fgets includes newline \n unless line too long 
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s%d%s", command, &command_number, junk);
        if (num_items == 2 && strcmp(command, "CREATE") == 0) {
            create_size = command_number;
            if (create_size < 1) {
                printf("Cannot create list: invalid size %d giving up!\n", create_size);
                exit(1);
            } 
            // clean up old list if necessary 
            ens_cleanup(sorted_list);
            sorted_list = ens_create("Contact");
            printf("New list with size: %d\n", create_size);
        } else if (num_items == 2 && strcmp(command, "ADDID") == 0) {
            ens_add(sorted_list, command_number, create_size);
        } 
        
        else if (num_items == 2 && strcmp(command, "QUERY") == 0) {
            ens_print_epid(sorted_list, command_number);
        } 
        
        else if (num_items == 2 && strcmp(command, "CLEAROLD") == 0) {
            ens_remove_old(sorted_list, command_number);
        } 
        
        else if (num_items == 1 && strcmp(command, "TRIM") == 0) {
            ens_trim(sorted_list);
        } 
        
        else if (num_items == 2 && strcmp(command, "MATCH") == 0) {
            int check_count = 0, match_count = 0, match_id = -1;
            printf("Starting exposure scan of contact list with threshold %d\n", command_number);
            while (fgets(line, MAXLINE, stdin) != NULL) {
                num_items = sscanf(line, "%d%s", &match_id, junk);
                if (num_items == 1 && match_id > -1) {
                    check_count++;
                    match_count += ens_match(sorted_list, unsorted_list, match_id);
                } else if (num_items == 1 && match_id == -1) {
                    printf("Checked %d IDs and found %d matches\n", check_count, match_count);
                    break;
                } else {
                    printf("BAD INPUT! Not a valid ID: %s", line);
                    printf("Cannot recover from this error so giving up!\n");
                    exit(1);
                }
            }
            // check the size of the confirmed exposure list.  
            int exposure_size = linked_entries(unsorted_list);
            if (exposure_size >= command_number) {
                printf("Notification %d accumulated exposures\n", exposure_size);
            } 
        } 
        
        else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            ens_print(sorted_list, "Contact");
        } 
        
        else if (num_items == 2 && strcmp(command, "RECEXP") == 0) {
            ens_list_exposures(unsorted_list, command_number);
        } 
        
        else if (num_items == 1 && strcmp(command, "CLEAREXP") == 0) {
            ens_remove_exposures(unsorted_list);
        } 
        
        else if (num_items == 1 && strcmp(command, "PRINTEXP") == 0) {
            ens_print(unsorted_list, "Exposure");
        } 
        
        else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            ens_stats(sorted_list, unsorted_list);
        } 
        
        else if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            ens_cleanup(sorted_list);
            ens_cleanup(unsorted_list);
            printf("Goodbye\n");
            break;
        } 
        
        else {
            printf("# %s", line);
        }
    }
    exit(0);
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
