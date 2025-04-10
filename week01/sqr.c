/* Prints squares of all numbers between 1 to n */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Correct usage is ./sqr <integer>\n");
        return -1;
    }

    int n = atoi(argv[1]);

    for(int i=1; i<=n; i++)
        printf("Square of %d: %d\n", i, i*i);

    return 0;
}