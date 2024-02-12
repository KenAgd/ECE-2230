/* enslist.h 
 * Prof. Russell 
 * ECE 2230 Fall 2021 
 * MP1
 *
 * Purpose: The interface definition for MP1.  DO NOT CHANGE THIS FILE
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  There are no extensions or modifications permitted.
 *
 * Bugs:
 */

struct enslist_t {
    struct epid_record_t **epids_ptr;  // pointer to Ephemeral ID list
    int num_epids; // number of EPID records in ENS list
    int ens_size_list;  // size of ENS array given when list created
};

struct epid_record_t {
    unsigned int ep_id;         // ephemeral ID 
    unsigned int rec_num;       // EPID sequential record number
    unsigned int time_received; // time in seconds when record recevied
    int source_type;    // wireless type: 0 bluetooth 1 wifi 2 LTE 3 other 
    unsigned int mac_addr;      // MAC address of source device 
    int authenticated;  // true or false 
    int privacy;        // mode 0 for none, 1 for encrypted
    int channel;        // 1-10 
    float rssi;         // received signal strength in dB
    float band;         // received frequency band in GHz
};

//  The sequential list ADT must have the following interface:

struct enslist_t *enslist_construct(int);
void enslist_destruct(struct enslist_t *);
int enslist_add(struct enslist_t *, struct epid_record_t *);
struct epid_record_t *enslist_lookup(struct enslist_t *, int);
struct epid_record_t *enslist_access(struct enslist_t *, int);
struct epid_record_t *enslist_remove_epid(struct enslist_t *, int);
int enslist_remove_old(struct enslist_t *, int);
int enslist_remove_smallest(struct enslist_t *);
int enslist_list_size(struct enslist_t *);
int enslist_id_count(struct enslist_t *);
void sort(struct enslist_t *list_ptr);
void swapNodes(struct enslist_t *list_ptr, int index);

