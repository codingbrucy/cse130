/* Producer for the producer-consumer problem with bounded buffer */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#define BUFFER_SIZE 5

typedef struct {
    int buffer[BUFFER_SIZE];		// The buffer itself
    int in;  					    // Index for the next write
    int out; 					    // Index for the next read
} SharedBuffer;

int main() {
    /* TODO: 
        * 1. Create or open the shared memory segment in O_RDWR mode.
        * 2. Set the size of the shared object to the size of the struct (extrapolated to a page size)
        * 3. Map the shared memory object into the producer's address space. 
        * 4. Initialize buffer indices in and out.
        * 5. Create and initialize the semaphores.
        * 6. Produce 10 values. Ensure proper synchronization.
        * 7. Clean up resources. Unlink semaphores and shared memory */
       
    return 0;
}
