/* Introducing semaphores to protect the critical section */
/* Compile using cc shmp1.c -o shmp1 -lrt -lpthread */

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
	int md = shm_open(zone, O_RDWR|O_CREAT, S_IRWXU); //create a shared memory object
	ftruncate(md, 4096); //size of the object is exactly 1 page

	int *t = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, md, 0); //map the object to the memory
	if(t == MAP_FAILED) {
		printf("mmap failure: %s\n", strerror(errno));
		exit(1);
	}

	memset(t,0,4096); //initialize the contents of the page to 0

	// Create a semaphore
	sem_t* s = sem_open("/sema2", O_CREAT, 0666, 1); //read write permissions are given; initialized to high

	// Edit the first 4 bytes of the memory mapping - incremement the integer to 1 billion
	int i;
	for(i=0; i<1000000000;i++) {
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

	shm_unlink(zone);
	sem_unlink("/sema2");

	return 0;
}
