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

void readLine(int sz, char buf[sz]) {
	char ch = getchar();
	char *p = buf;
	while(ch!=EOF && ch!='\n' && sz-- > 0) {
		*p++ = ch;
		ch = getchar();
	}
	*p= 0; //null terminate the string
}

int main() {
	const char* zone = "/memzone1"; //name of the memory mapping
	int md = shm_open(zone, O_RDWR|O_CREAT,S_IRWXU); //create a shared memory object. return a fd for zone where we can read/write or create one
    ftruncate(md, 4096); //specify the size of the memory mapping - must be a multiple of the page size for the system	
	
	void* ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FILE, md,0); //map the zone to virtual address space
	if(ptr==MAP_FAILED) {
		printf("mmap failure: %s\n", strerror(errno));
		exit(1);
	}

	sem_t* s = sem_open("/sema1", O_CREAT, S_IRWXU,0); //create a semaphore named sema1; 0 means the flag is down, 1 is up, 2 is higher
	memset(ptr,0,4096);
	char* buf = ptr;
	int svalue1 = 0, svalue2 = 0;
	while(1) {
		char line[256];
		readLine(255, line);
		strcpy(buf, line);
		sem_post(s); //raises the flag to indicate its added something to memory map
	}

	sem_close(s);
	munmap(ptr, 4096);
	close(md);

	shm_unlink(zone); //zone is no longer accessible
	sem_unlink("/sema1");
	
	return 0;
}
