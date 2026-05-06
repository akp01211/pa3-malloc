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
void vmfree(void *ptr) {
    // TODO
}
