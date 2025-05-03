/* Compile as cc shmp1.c -o shmp1 -lrt 
 * lrt links the real-time library that has functions for shared memory and semaphores */

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
	//creating a pretend file 
	const char* zone = "/memzone1";		
	int md = shm_open(zone, O_RDWR|O_CREAT, S_IRWXU); //creates a "file" in read/write mode. file has read/write/execute permissions
	ftruncate(md, 4096); //size of the file is exactly 1 page

	//creating a shared mapping
	int *t = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, md, 0); //map the object to the memory
	if(t == MAP_FAILED) {
		printf("mmap failure: %s\n", strerror(errno));
		exit(1);
	}
    
	//initializing the file with 0s
	memset(t,0,4096); 

	//update the first 4 bytes of the mmap - increment the integer to 1 billion
	for(int i=0; i<1000000000;i++) 
		t[0] = t[0] + 1;
	int final = t[0];
	
	printf("Final: %d\n", final);

	//unmapping the object
	munmap(t, 4096);
	close(md); //close the file descriptor

	//destroying the pretend file
	shm_unlink(zone);

	return 0;
}
