/* datatypes.h 
 * Prof. Russell 
 * harlanr
 * ECE 2230 Fall 2021 
 * MP2
 * 
 * Propose: The data type that is stored in the linked list ADT is defined here.  We
 *          define a single mapping that allows the linked list ADT to be defined in
 *          terms of a generic data_t.
 *
 * Assumptions: 
 *     For MP2 there are no changes to this file.  For all other MP's
 *     this file must be updated
 *
 *     data_t: The type of data that we want to store in the list
 *
 * Bugs:
 */

typedef struct epid_record_tag {
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
} epid_rec_t;

/* the list ADT works on data of this type */
typedef epid_rec_t data_t;

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
