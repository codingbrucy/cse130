// pipes.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define READEND 0
#define WRITEEND 1

int main(int argc, char *argv[])
{
   if (argc < 3)
   {
      printf("Usage: pipes a b, where a and b are integers. \n");
      exit(1);
   }

   /* TODO: Implement the command: "ls /dev | xargs | cut -d ' ' -f<a>-<b>"
             where <a> and <b> are integers that specify the range and are taken as input from the command line. */
   // Parse field range
   int a = atoi(argv[1]);
   int b = atoi(argv[2]);
   if (a <= 0 || b <= 0 || a > b)
   {
      fprintf(stderr, "Error: <a> and <b> must be positive with a <= b\n");
      return 1;
   }
   // Build the "-f<a>-<b>" argument
   char field_arg[32];
   snprintf(field_arg, sizeof(field_arg), "%d-%d", a, b);

   // Create the two pipes
   int pipe1[2], pipe2[2];
   if (pipe(pipe1) < 0 || pipe(pipe2) < 0)
   {
      perror("pipe");
      return 1;
   }

   pid_t pid;

   // Stage 1: ls /dev
   pid = fork();
   if (pid < 0)
   {
      perror("fork");
      return 1;
   }
   if (pid == 0)
   {
      // Child 1: stdout -> pipe1 write end
      dup2(pipe1[1], STDOUT_FILENO);
      close(pipe1[0]);
      close(pipe1[1]);
      close(pipe2[0]);
      close(pipe2[1]);
      execlp("ls", "ls", "/dev", (char *)NULL);
      perror("execlp ls");
      exit(1);
   }

   // Stage 2: xargs
   pid = fork();
   if (pid < 0)
   {
      perror("fork");
      return 1;
   }
   if (pid == 0)
   {
      // Child 2: stdin <- pipe1 read end, stdout -> pipe2 write end
      dup2(pipe1[0], STDIN_FILENO);
      dup2(pipe2[1], STDOUT_FILENO);
      close(pipe1[0]);
      close(pipe1[1]);
      close(pipe2[0]);
      close(pipe2[1]);
      execlp("xargs", "xargs", (char *)NULL);
      perror("execlp xargs");
      exit(1);
   }

   pid = fork();
   if (pid < 0)
   {
      perror("fork");
      return 1;
   }
   if (pid == 0)
   {
      // Child 3: stdin <- pipe2 read end
      dup2(pipe2[0], STDIN_FILENO);
      close(pipe1[0]);
      close(pipe1[1]);
      close(pipe2[0]);
      close(pipe2[1]);
      execlp("cut", "cut", "-d", " ", "-f", field_arg, (char *)NULL);
      perror("execlp cut");
      exit(1);
   }

   // Parent: close all pipe fds, then wait for children
   close(pipe1[0]);
   close(pipe1[1]);
   close(pipe2[0]);
   close(pipe2[1]);

   for (int i = 0; i < 3; ++i)
   {
      wait(NULL);
   }

   return 0;
}
