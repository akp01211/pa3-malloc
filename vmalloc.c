#include "vm.h"
#include "vmlib.h"



/**
 * Allocate a memory block with the given minimum size on the simulated heap.
 * If allocation succeeds, return a pointer to the start of the new block's
 * payload. If allocation fails due to insufficient free space, return NULL.
 *
 * Read the section for vmalloc in the PA writeup for how to implement this
 * function.
 */
static struct b_header *block_n(struct b_header *b){
    return (struct b_header *)((char *)b+BLKSZ(b));
}
static void footer_w(struct b_header *b){
    struct block_footer *f =(struct block_footer *)((char *)b + BLKSZ(b) -sizeof(struct block_footer));
    f->size = BLKSZ(b);
}



 void *vmalloc(size_t size){
     if(size<=0)
        return NULL;
 
    // Rounding up multiple of 16
    size_t nec =ROUND_UP(size +sizeof(struct b_header),BLKALIGN);
 
    // Best fit alg
    struct b_header *best = NULL;
    for(struct b_header *b = heapstart; b->size_status !=VM_ENDMARK; b =block_n(b)){



        if(!(b->size_status &VM_BUSY) &&BLKSZ(b)>= nec)





            if(best== NULL|| BLKSZ(b)< BLKSZ(best)){



                best =b;
            }
    
 


    if(best == NULL)
        return NULL;
 



    size_t rem= BLKSZ(best)-nec;
 
    if(rem >=2* BLKALIGN){


        best->size_status=nec | (best->size_status & VM_PREVBUSY) |VM_BUSY;
 
        struct b_header *split= block_n(best);
        split->size_status =rem |VM_PREVBUSY;
        footer_w(split);
 
        struct b_header *next =block_n(split);


        if(next->size_status!= VM_ENDMARK){
            next->size_status&= ~VM_PREVBUSY;
        }
    } 
    
    else{
    
        best->size_status |=VM_BUSY;



        struct b_header *nxt= block_n(best);
        if(nxt->size_status !=VM_ENDMARK){




            nxt->size_status|= VM_PREVBUSY;

        }
    }
 
    return (void *)((char *)best+sizeof(struct b_header));
}
 }
