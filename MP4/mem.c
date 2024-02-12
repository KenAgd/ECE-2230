/* mem.c A template
 * Kendric Agdaca
 * Kagdaca
 * Lab4: Dynamic Memory Allocation
 * ECE 2230, Fall 2021
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "mem.h"

// Global variables required in mem.c only
// NEVER use DummyChunk in your allocation or free functions!!
static mem_chunk_t DummyChunk = {0, &DummyChunk};
static mem_chunk_t * Rover = &DummyChunk;   // one time initialization



typedef struct heap_stats 
{
	int numItems;    /* number of chunks in the free list    */
	int min;         /* size of the smallest chunk, in bytes */
	int max;         /* size of the largest chunk, in bytes  */
	int average;     /* average size of the chunks, in bytes */
	int totalBytes;  /* total size of all chunks, in bytes   */


/* the following two fields are updated in morecore() */
	int numSbrkCalls;  /* number of successful calls to sbrk()  */
	int numPages;      /* number of pages allocated with sbrk() */
}heap_stats_t;
static heap_stats_t stats;


// private function prototypes
void mem_validate(void);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 *
 * You can update this function to match your design.  But the method
 * to test sbrk much not be changed.  
 */
mem_chunk_t *morecore(int new_bytes) 
{
    char *cp;
    mem_chunk_t *new_p;
    // preconditions that must be true for all designs
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(mem_chunk_t) == 0);
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (mem_chunk_t *) cp;
    // You should add some code to count the number of calls
    // to sbrk, and the number of pages that have been requested
    stats.numSbrkCalls++; 
    stats.numPages += new_bytes/PAGESIZE;
    return new_p;
}

/* deallocates the space pointed to by return_ptr; it does nothing if
 * return_ptr is NULL.  
 *
 * This function assumes that the Rover pointer has already been 
 * initialized and points to some memory block in the free list.
 */
void Mem_free(void *return_ptr)
{
    // precondition
    assert(Rover != NULL && Rover->next != NULL);
//    assert(p->size_units > 1);   // verify that the size field is valid


    mem_chunk_t *ptr = ((mem_chunk_t *)return_ptr) - 1;
    
    if (return_ptr == NULL)
    {
        return;
    }
    
    
    if (Coalescing == TRUE)
    {
        while (Rover -> next < ptr && Rover -> next != &DummyChunk)
        {
            Rover = Rover -> next;
        }
    }
    ptr -> next = Rover -> next;
    Rover -> next = ptr;
    
    
    if (Coalescing == TRUE)
    {
        if (ptr + ptr -> size_units == ptr -> next)
        {
            ptr -> size_units += ptr -> next -> size_units;
            ptr -> next = ptr -> next -> next;
        }
        
        if (Rover + Rover -> size_units == Rover -> next)
        {
            Rover -> size_units += Rover -> next -> size_units;
            Rover -> next = Rover -> next -> next;
            ptr = Rover;
            
            while (Rover -> next != ptr)
            {
                Rover = Rover -> next;
            }
        }
    }

    assert(ptr -> size_units > 1);
}



/* returns a pointer to space for an object of size nbytes, or NULL if the
 * request cannot be satisfied.  The memory is uninitialized.
 *
 * This function assumes that there is a Rover pointer that points to
 * some item in the free list.  
 */
void *Mem_alloc(const int nbytes)
{
    // precondition
    assert(nbytes > 0);
    assert(Rover != NULL && Rover->next != NULL);

    // The first step is to calculate the number of units needed to satisfy the users request of nbytes.  See the figure on page 3 of the MP4.pdf 
    // Say you calcualte nunits.  To verify it is correct include these tests:
    // assert((nunits-1)*sizeof(mem_chunk_t) >= nbytes);
    // assert((nunits-1)*sizeof(mem_chunk_t) < nbytes + sizeof(mem_chunk_t));



	int sizeUnits = nbytes / sizeof(mem_chunk_t) + 1;
	int flags = 0;
	int numPage = 0;
	int newBytes = 0;
	int bytes;
	int diff;
	int small;

	mem_chunk_t *temp_ptr = Rover;
	mem_chunk_t *bestFit = NULL;

	mem_chunk_t *qtr;


	if (nbytes % sizeof(mem_chunk_t) != 0)
	{
		sizeUnits++;
	}


//First-fit
	if (SearchPolicy == FIRST_FIT)	
	{

	//Iterates thru the free list and finds the first available free mem block thats big enough to hold the upcoming alloc
		do
		{
			if (Rover -> size_units >= sizeUnits)
			{
				flags = 1;
				bestFit = Rover;
				break;
			}

			Rover = Rover -> next;
		}while (Rover != temp_ptr);
	}


//Best-fit
	else
	{
		small = Rover -> size_units - sizeUnits;

	//Iterates thru the free list and finds the smallest mem block that can hold the upcoming alloc
		do
		{
			diff = Rover -> size_units - sizeUnits;

			if (diff >= 0 && flags == 0)
			{
				flags = 1;
				small = diff;
				bestFit = Rover;
			}


			if (diff >= 0 && diff < small && flags == 1)
			{
				bestFit = Rover;
				
				if (diff == 0)
				{
					break;
				}
			}


			Rover = Rover -> next;
		}while (temp_ptr != Rover);


		if (flags == 1)
		{
			Rover = bestFit;
		}

	}

	mem_chunk_t *ptr = NULL;

//If flags == 0, no more free mem blocks. Make more in increments of PAGESIZE
	if (flags == 0)
	{
		if (sizeUnits / (PAGESIZE / sizeof(mem_chunk_t)) == 0)
		{
			numPage = 0;
		}

		else
		{
			numPage = sizeUnits / (PAGESIZE / sizeof(mem_chunk_t));
		}


		if (sizeUnits % PAGESIZE != 0)
		{
			numPage++;
		}


		bytes = numPage * PAGESIZE;
		newBytes = bytes / sizeof(mem_chunk_t);	
		temp_ptr = morecore(bytes);
		temp_ptr -> size_units = newBytes;
		ptr = temp_ptr;
		temp_ptr += sizeUnits;
		temp_ptr -> size_units = newBytes - sizeUnits;
		ptr -> size_units = sizeUnits;
		mem_chunk_t *origRove = Rover;


		do
		{
			if ((Rover < ptr && Rover -> next > temp_ptr) || (Rover < temp_ptr && Rover -> next == &DummyChunk))
			{
				temp_ptr -> next = Rover -> next;
				Rover -> next = temp_ptr;
				break;
			}

			Rover = Rover -> next;
		}while (Rover != origRove);

		ptr -> next = NULL;
	}


//Worst fit. Finds the largest free mem block for the upcoming alloc
	else
	{
		ptr = Rover;


		if ((Rover -> size_units - sizeUnits) > 0)
		{
			ptr += (Rover -> size_units - sizeUnits);
			ptr -> size_units = sizeUnits;
			Rover -> size_units = Rover -> size_units - sizeUnits;
			ptr -> next = NULL;
		}


		else
		{
			while (temp_ptr -> next != Rover)
			{
				temp_ptr = temp_ptr -> next;
			}

			temp_ptr -> next = ptr -> next;
			Rover = ptr -> next;
			ptr -> next = NULL;
		}
	}

	qtr = ptr + 1;
	return qtr;
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
	mem_chunk_t* temp_ptr = Rover;
	stats.min = DummyChunk.next -> size_units * sizeof(mem_chunk_t);
	stats.max = stats.min;
	stats.totalBytes = 0;


//Iterates thru the list and grabs/calculates data values of the heap list and preps it to be printed. 
	do
	{
		stats.numItems++;

		if (Rover -> size_units * sizeof(mem_chunk_t) > stats.max)
		{
			stats.max = Rover -> size_units * sizeof(mem_chunk_t);
		}

		if (Rover -> size_units * sizeof(mem_chunk_t) < stats.min && Rover != &DummyChunk)
		{
			stats.min = Rover -> size_units * sizeof(mem_chunk_t);
		}


		stats.totalBytes += Rover -> size_units * sizeof(mem_chunk_t);
		Rover = Rover -> next;
	}while (Rover != temp_ptr);

	stats.average = stats.totalBytes / (stats.numItems - 1);
 
    printf("\n\t\tMP4 Heap Memory Statistics\n"
            "----------------------------------------------------\n\n"
            "\tNumber of blocks in free list: %d\n"
            "\tMin: %d\n"
            "\tMax: %d\n"
            "\tAverage: %d\n"
            "\tTotal bytes in free list: %d\n"
            "\tNumber of sbrk calls: %d\n"
            "\tNumber of requested pages: %d\n"
            "\tHeap status:  %s\n",
            stats.numItems, stats.min, stats.max, stats.average, stats.totalBytes,
            stats.numSbrkCalls, stats.numPages,
            stats.totalBytes == stats.numPages*PAGESIZE ? \
            "all memory is in the heap -- no leaks are possible\n"\
            : "heap is in-use -- leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 *
 * A unit is the size of one mem_chunk_t structure
 */
void Mem_print(void)
{
    // note position of Rover is not changed by this function
    assert(Rover != NULL && Rover->next != NULL);
    mem_chunk_t *p = Rover;
    mem_chunk_t *start = p;
    do {
        // example format.  Modify for your design
        printf("p=%p, size=%d (units), end=%p, next=%p %s\n", 
                p, p->size_units, p + p->size_units, p->next, 
                p->size_units!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start);
    mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    // note position of Rover is not changed by this function
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size_units >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    mem_chunk_t *p, *largest, *smallest;

    // for validate begin at DummyChunk
    p = &DummyChunk;
    do {
        if (p->size_units == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size_units > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &DummyChunk);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                // this is not good unless at the one wrap around
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   // stop and use gdb
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size_units < p->next);
            }
            p = p->next;
        } while (p != &DummyChunk);
        assert(wrapped == TRUE);
    }
}
/* vi:set ts=8 sts=4 sw=4 et: */

