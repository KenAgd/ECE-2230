/* ens_support.h
 * Prof. Russell        <<-- some changes needed including your name
 * harlanr
 * ECE 2230 Fall 2021
 * MP2
 *
 * Propose: A template for MP2 
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignmnet.  
 *
 *              I have provided prototypes of some of the functions
 *              and you CANNOT change these prototypes
 *
 *              All function prototypes in this file must start with the prefix
 *              ens_ and are public.  
 *
 * Bugs:
 *
 * (You CANNOT modify any of the details of the linkedlist.h interface, or use any 
 *  of the private variables outside of linkedlist.c.)
 */

/* prototype function definitions */

/* function to compare EP ID records */
int ens_compare_epid(const epid_rec_t *soc_a, const epid_rec_t *soc_b);
int ens_compare_recnum(const epid_rec_t *rec_a, const epid_rec_t *rec_b);

/* functions to create and cleanup EP ID lists */
linked_t *ens_create(const char *);
void ens_cleanup(linked_t *);

/* Functions to get and print EP ID information */
void ens_print(linked_t *list_ptr, const char *); /* print list of records */
void ens_stats(linked_t *, linked_t *);            /* prints size of each list */

/* functions for sorted list */
void ens_add(linked_t *, const int, const int);
void ens_print_epid(linked_t *, int);
void ens_list_ip(linked_t *, int);
void ens_remove_old(linked_t *, int);
void ens_trim(linked_t *);
int ens_match(linked_t *, linked_t *, int);
void ens_list_sort(linked_t *list_ptr, int sort_type);
void ens_appexp(linked_t *list_ptr, int id);

/* functions for unsorted list to insert at the rear and remove at the front.
 * Duplicate alerts with the same destination IP address are not
 * permitted.  If a new alert is inserted the old one must be
 * recycled
 */
void ens_list_exposures(linked_t *, int);
void ens_remove_exposures(linked_t *);

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
