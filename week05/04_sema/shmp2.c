/* Compile using cc shmp2.c -o shmp2 -lrt -lpthread */

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main() {
	const char* zone = "/memzone1";
	int md = shm_open(zone, O_RDWR, S_IRWXU);

	void* ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FILE, md, 0);
	if(ptr==MAP_FAILED) {
		printf("mmap failure: %s\n", strerror(errno));
		exit(1);
	}
	char* buf = ptr;
	sem_t* s = sem_open("/sema1", O_EXCL); //exclusive access to sema1
	while(1) {
		sem_wait(s);
		printf("got [%s]\n", buf);
	}
	
	sem_close(s);
	munmap(ptr, 4096);
	close(md);

	return 0;
}
