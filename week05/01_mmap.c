#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    //get the size of the file to be mapped
    struct stat fs;
    stat("sample.c", &fs);
    int sz = fs.st_size;
    int pg = (sz / 4096) + 1;   //number of pages needed 

    //create the mapping
    int md = open("sample.c", O_RDWR);
    char* ptr = mmap(NULL, pg * 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE, md, 0);    //creates a private mapping and give read/write protection
    if(ptr == MAP_FAILED) {
        printf("mmap failure: %s\n", strerror(errno));
        exit(1);
    }

    //read the contents of the file char-by-char by accessing memory locations
    for(int i=0; i<sz;i++)
        printf("%c", ptr[i]);
    printf("Done!\n");
    munmap(ptr, 4096);
}