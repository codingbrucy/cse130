/* Compile using cc shmp2.c -o shmp2 -lrt -lpthread */

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

int main() {
	const char* zone = "/memzone1";
	int md = shm_open(zone, O_RDWR, S_IRWXU); //create a shared memory object
	
	int *t = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FILE, md, 0); //map the object to the memory
	if(t == MAP_FAILED) {
		printf("mmap failure: %s\n", strerror(errno));
		exit(1);
	}

	// Access the semaphore
	sem_t* s = sem_open("/sema2", O_EXCL); //exclusive access

	// Edit the first 4 bytes of the memory mapping - incremement the integer to 10 million
	int i;
	for(i=0; i<100000000;i++) {
	    sem_wait(s);	
		t[0] = t[0] + 1;
		sem_post(s);
	}
	int final = t[0];
	printf("Final: %d\n", final);
	
	// Unmap the object
	sem_close(s);
	munmap(t, 4096);
	close(md); //close the file descriptor
	
	return 0;
}
