3. Code description

Q1. fork() and exec()
1. “Why This Works" explanation: 
fork() is called twice in a non-nested manner, thus spawnning two children, first one calls execlp() with first two 
command line args, and the second child calls exevp with the third argv and the array represented by argv[3].
then the program uses waitpid twice with both pids, getting out of a status. 

2. Testing strategy and inputs: tested with two basic echo commands; then echo and one ls
3. Output analysis: both commands executing success, reporting exit status success.

Q2. Pipes
1. “Why This Works" explanation: fork() is chained 3 times. first child: routing output of ls to pipe1[1]; then in the 
second child, xargs is executed after routing input from pipe1[0] and output to pipe2[1]; lastly the cute command reads
from pipe2[0], printing to console. in the end parents reaps all 3 dead children and terminates
2. Testing strategy and inputs: tested with ./pipe a b, with a, b being arbitrary ints. 
3. Output analysis: the output is the equivalent of that of executing the target command to be implemented. 
