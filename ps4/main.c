#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "matrix.h"

Matrix* loadMatrix(char* fName) {
   FILE* fd = fopen(fName,"r");
   if (fd==NULL) return NULL;
   Matrix* m = readMatrix(fd);
   fclose(fd);
   return m;
}

int main(int argc,char* argv[]) {
   if (argc < 4) {
      printf("usage: mult <file1> <file2> <#workers>\n");
      exit(1);
   }

   /* load the two matrices and obtain the number of worker processes */
   Matrix* a = loadMatrix(argv[1]);
   Matrix* b = loadMatrix(argv[2]);
   if (a==NULL || b==NULL) {
      printf("Wrong filenames. Couldn't load matrices [%s,%s]\n",argv[1],argv[2]);
      exit(2);
   }
   int nbW = atoi(argv[3]);

   // Creating a memory object 
   const char* zone = "/matzone1";
   int md = shm_open(zone,O_RDWR|O_CREAT,S_IRWXU);

   //TODO: 
      //1. Create a shared mapping that is large enough to store a matrix of size a->r x b->c. Use the sizeMatrix() function to get the size of the matrix.
      size_t sz = sizeMatrix(a->r, b->c);
      if (ftruncate(md, sz) == -1) {
         perror("ftruncate");
         exit(1);
      }
      //2. Make use of the makeMatrixMap() function to map a matrix of size a->r x b->c to the shared memory region.
      void* region = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
      if (region == MAP_FAILED) { perror("mmap"); shm_unlink(zone); exit(5); }
      Matrix* res = makeMatrixMap(region, a->r, b->c);
      if (res == NULL) { fprintf(stderr, "makeMatrixMap failed\n"); munmap(region, sz); shm_unlink(zone); exit(6); }
      //3. Create a semaphore to synchronize the parent and worker (child) processes.
      const char* semName = "/matsem1";
      sem_t* sem = sem_open(semName, O_CREAT | O_EXCL, 0644, 0);
      if (sem == SEM_FAILED) { perror("sem_open"); munmap(region, sz); shm_unlink(zone); exit(7); }
      //4. Use the parMultMatrix() function to multiply matrices a and b using nbW processes and store the result in the shared memory region. 
      parMultMatrix(nbW, sem, a, b, res);
      //5. Wait for the worker processes to finish. You cannot use the wait() system call here. Hint: use the semaphore.
      for (int i = 0; i < nbW; i++) {
        sem_wait(sem);
      }
      //6. Print the resultant matrix using the printMatrix() function.  
      printMatrix(res);
      //7. Clean up all resources. 
      sem_close(sem);
      sem_unlink(semName);
      munmap(region, sz);
      close(md);
      shm_unlink(zone);
      freeMatrix(a);
      freeMatrix(b);
   
   return 0;
}
