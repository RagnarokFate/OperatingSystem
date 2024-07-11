/**
 * virtmem.c 
 * Written by Michael Ballantyne 
 */

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define TLB_SIZE 16
#define PAGES 256
#define PAGE_MASK 255

#define PAGE_SIZE 256
#define OFFSET_BITS 8
#define OFFSET_MASK 255

#define MEMORY_SIZE PAGES * PAGE_SIZE


// Max number of characters per line of input file to read.
#define BUFFER_SIZE 10

typedef struct tlbentry {
    unsigned char logical;
    unsigned char physical;
}Tlbentry;

// TLB is kept track of as a circular array, with the oldest element being overwritten once the TLB is full.
struct tlbentry tlb[TLB_SIZE];

// number of inserts into TLB that have been completed. Use as tlbindex % TLB_SIZE for the index of the next TLB line to use.
int tlbindex = 0;

// pagetable[logical_page] is the physical frame number for logical page. Value is -1 if that logical page isn't yet in the table.
int pagetable[PAGES];

signed char main_memory[MEMORY_SIZE];

// Pointer to memory mapped backing file
signed char *backing;

// -----------------------------------------------------------------
// -------- YOUR CODE HERE: helper functions of your choice --------


//checks if the given logical_page exist in the tlb if it is returns 
//the physical_frame else returns -1
int CheckInTlb(int logical_page)
{
    int i=0, j=0;

    if ((tlbindex - TLB_SIZE) > 0)
    {
        i = tlbindex - TLB_SIZE;
    }

    j = i % TLB_SIZE;

    for (i; i < tlbindex; i++) {
        j = i % TLB_SIZE;
        if (tlb[j].logical == logical_page) {
            return tlb[j].physical;
        }
    }
    return -1;
}

char* CalculatePgeBegining(char* memoryBegining, int pageNum)
{
    return memoryBegining + (pageNum * PAGE_SIZE);
}

/*
void InsertToTlb(unsigned char physical, unsigned char logical)
{
    Tlbentry* temp = &tlb[tlbindex % TLB_SIZE];
    temp->logical = logical;
    temp->physical = physical;
    tlbindex++;
    return;
}

*/



// -----------------------------------------------------------------

int main(int argc, const char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage ./virtmem backingstore input\n");
        exit(1);
    }

    const char *backing_filename = argv[1]; 
    int backing_fd = open(backing_filename, O_RDONLY);
    backing = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, backing_fd, 0); 

    const char *input_filename = argv[2];
    FILE *input_fp = fopen(input_filename, "r");

    // Fill page table entries with -1 for initially empty table.
    int i;
    for (i = 0; i < PAGES; i++) 
    {
        pagetable[i] = -1;
    }

    // Character buffer for reading lines of input file.
    char buffer[BUFFER_SIZE];

    // Data we need to keep track of to compute stats at end.
    int total_addresses = 0;
    int tlb_hits = 0;
    int page_faults = 0;

    // Number of the next unallocated physical frame in main memory
    unsigned char free_page = 0;

    while (fgets(buffer, BUFFER_SIZE, input_fp) != NULL) 
    {
        int logical_address = atoi(buffer);

        // -------------------------------------------------------------------------------------  
        //-------------variables--------------
        int  logical_page, physical_frame, offset;
        int physical_address;
        signed char value;

        
        
        //-------------- stage 1 ----------------
        //promote by 1 the number of addresses we are translating 
        total_addresses++;

        //-------------- stage 2 ----------------
        //calculate logical_page using logical_address

        //extract the page number from the logical address by:
        //1:do shift right 8 times (/2) to extract the upper 8 bits(logical_page num)
        //2:do bitwise and with the PAGE_MASK which is 11111111 (255 in decimal)
        //to keep the relevant bits of the logical_page number 
        logical_page = (logical_address >> OFFSET_BITS) & PAGE_MASK;

        


        //check if logical_page exist in the tlb table 
        physical_frame = CheckInTlb(logical_page);

        //tlb miss
        if (physical_frame == -1)
        {
           
            physical_frame = pagetable[logical_page];
            
            //the logical_page is not mapped by the bagetable 
            //there is a page fault exception
            if (physical_frame == -1)
            {
                //promote page_faults by 1
                page_faults++;

                //---------------------------------------------
                //allocate the next free page for this physical_frame 
                //---------------------------------------------
                physical_frame = free_page;
                free_page++;

                //---------------------------------------------
                //copy the page from BACKING_STORE to the main memory
                //---------------------------------------------
                char *logical_page_begining, *physical_frame_begining;
                //compute the begining(pointer) of the page in the baking using logical_page number  
                logical_page_begining = CalculatePgeBegining(backing, logical_page); 

                //compute the begining(pointer) of the frame in the main memory using physical_frame number 
                physical_frame_begining = CalculatePgeBegining(main_memory ,physical_frame);

                //copy the page from baking to the main memory bit by bit 

                for (int i = 0; i < PAGE_SIZE; i++)
                {
                    physical_frame_begining[i] = logical_page_begining[i];
                }

                //----------------------------------------------
                //update pagetable such that the logical_page is mapped to the physical_frame
                //----------------------------------------------
                pagetable[logical_page] = physical_frame;
            }
             
            //step on the data in the tlb[tlbindex] by inserting the
            //current physical_frame and logical_page 
            //promote the tlbindex by 1
            tlb[tlbindex % TLB_SIZE].logical = logical_page;
            tlb[tlbindex % TLB_SIZE].physical = physical_frame;
            tlbindex++;
        }
        //tlb hit 
        else
        {
            tlb_hits++;
        }
        

        //calculate the offset directly from the logical_address by extracting the lower 8 bits
        //using bitwise and with 0000000011111111 (255)
        offset = logical_address & PAGE_MASK;

        //calculate the physical address by 
        //1.adding 8 zeros to the phyzical_frame from the right then 
        //2.copy the offset instead of those zeros by doing bitwise or with the offset(8 bits)
        physical_address = (physical_frame<<OFFSET_BITS)|offset ;

        //read the value of the physical_address from the memory 
        //calculate the frame place in the memory by:
        //1.multiply the frame number at the frime_size which is equal to PAGE_SIZE
        //2go to the appropriate entry in the frame using the offset 

        value =main_memory[(physical_frame*PAGE_SIZE)+offset] ;

        // -------------------------------------------------------------------------------------



        printf("Virtual address: %d Physical address: %d Value: %d\n", logical_address, physical_address, value);
    }

    printf("Number of Translated Addresses = %d\n", total_addresses);
    printf("Page Faults = %d\n", page_faults);
    printf("Page Fault Rate = %.3f\n", page_faults / (1. * total_addresses));
    printf("TLB Hits = %d\n", tlb_hits);
    printf("TLB Hit Rate = %.3f\n", tlb_hits / (1. * total_addresses));

    return 0;
}
