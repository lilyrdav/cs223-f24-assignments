/**
* The main driver program for A08 warmup.c.
*
* This program creates a child process using the fork() system call.
*
* @author: Lily Davoren
* @version: October 28, 2024
*/

#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid;
  printf("%d] A0\n", getpid());
  fflush(stdout);

  pid = fork();
  if (pid == 0) {
    printf("%d] B1\n", getpid());
    fflush(stdout);
  }
  else {
    printf("%d] B0\n", getpid());
    fflush(stdout);
    pid = fork();
    if (pid == 0) {
      printf("%d] C1\n", getpid());
      fflush(stdout);
    }
    else {
      printf("%d] C0\n", getpid());
      fflush(stdout);
    }
  }
  printf("%d] Bye\n", getpid());
  fflush(stdout);

  return 0;
}
