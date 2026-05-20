#include "vm.h"
#include "vmlib.h"

/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmalloc().
 * Otherwise, or if vmfree(ptr) has already been called before,
 * undefined behavior occurs.
 *
 * If ptr is NULL, no operation is performed.
 */

static struct b_header *block_n(struct b_header *b){
    return(struct b_header *)((char *)b+BLKSZ(b));
}
static void footer_w(struct b_header *b){
    struct footer_w *f = (struct footer_w *)((char*)b + BLKSZ(b)-sizeof(struct footer_w));


    f->size = BLKSZ(b);


}
 
static struct b_header *prev_block(struct b_header *b) {


    struct footer_w *pf = (struct footer_w *)((char *)b - sizeof(struct footer_w));



    return (struct b_header *)((char *)b -pf->size);
}

void vmfree(void *ptr){


     if(ptr==NULL){


        return;


    }
 
    struct b_header *bl = (struct b_header *)((char *)ptr - sizeof(struct b_header));
 


    if(!(bl->size_status& VM_BUSY)){


        return;


    }
 
    bl->size_status&= ~VM_BUSY;



 
    // Right first
    struct b_header *nxt=block_n(bl);


    if (nxt->size_status!= VM_ENDMARK&& !(nxt->size_status & VM_BUSY)){


        bl->size_status =(BLKSZ(bl) +BLKSZ(nxt)) |(bl->size_status & VM_PREVBUSY);



        struct b_header *after =block_n(bl);
        if (after->size_status != VM_ENDMARK){


            after->size_status &=~VM_PREVBUSY;
        }
    } 
    
    else if(nxt->size_status !=VM_ENDMARK){


        nxt->size_status&= ~VM_PREVBUSY;


    }
 
    footer_w(bl);


 
    // Left
    if(!(bl->size_status& VM_PREVBUSY)){
        struct b_header *pr= prev_block(bl);
        pr->size_status= (BLKSZ(pr) +BLKSZ(bl)) |(pr->size_status & VM_PREVBUSY);
        
        
        footer_w(pr);


    }
}
