#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define READEND 0
#define WRITEEND 1

int main(int argc,char* argv[]) {  
   if (argc < 3) {
      printf("Usage: pipes a b, where a and b are integers. \n");
      exit(1);
   }
   
   /* TODO: Implement the command: "ls /dev | xargs | cut -d ' ' -f<a>-<b>" 
      	    where <a> and <b> are integers that specify the range and are taken as input from the command line. */


   return 0;
}


