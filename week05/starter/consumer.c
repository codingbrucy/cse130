/* Consumer for the producer-consumer problem with bounded buffer */

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
     * 1. Open the existing shared memory segment created by the producer in O_RDWR mode.
     * 2. Map the shared memory object into the consumer's address space. 
     * 3. Open the existing semaphores created by the producer.
     * 4. Consume/Print 10 values produced by the producer. Ensure proper synchronization.
     * 5. Clean up resources. */


    return 0;
}
