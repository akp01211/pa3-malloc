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

static struct block_header *block_n(struct block_header *b){
    return(struct block_header *)((char *)b+BLKSZ(b));
}
static void footer_w(struct block_header *b){
    struct block_footer *f = (struct block_footer *)((char*)b + BLKSZ(b)-sizeof(struct block_footer));


    f->size = BLKSZ(b);


}
 
static struct block_header *prev_block(struct block_header *b) {


    struct block_footer *pf = (struct block_footer *)((char *)b - sizeof(struct block_footer));



    return (struct block_header *)((char *)b -pf->size);
}

void vmfree(void *ptr){


     if(ptr==NULL){


        return;


    }
 
    struct block_header *bl = (struct block_header *)((char *)ptr - sizeof(struct block_header));
 


    if(!(bl->size_status& VM_BUSY)){


        return;


    }
 
    bl->size_status&= ~VM_BUSY;



 
    // Right first
    struct block_header *nxt=block_n(bl);


    if (nxt->size_status!= VM_ENDMARK&& !(nxt->size_status & VM_BUSY)){


        bl->size_status =(BLKSZ(bl) +BLKSZ(nxt)) |(bl->size_status & VM_PREVBUSY);



        struct block_header *after =block_n(bl);
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
        struct block_header *pr= prev_block(bl);
        pr->size_status= (BLKSZ(pr) +BLKSZ(bl)) |(pr->size_status & VM_PREVBUSY);
        
        
        footer_w(pr);


    }
}
