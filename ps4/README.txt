2. Code description

1. “Why This Works" explanation: 
in main.c, we calculate the size of a matrix and size the 
memory region accordingly. Then we map the virtual memory region 
to `region`, and call make MatrixMap on the region we have a shared 
memory. Now all child process can work on this matrix. 
Then we oepn a semaphore '/matsem1' the sema is posted by workers nbW times, 
and that's why we can call sem_wait nbW times (in a for loop).