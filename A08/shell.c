/**
* The main driver program for A08 shell.c.
*
* This program implements a simple shell. The program prints a prompt showing the current working directory 
* and looks distinct from lab machine prompts. It uses the readline() function to get user input and the 
* add_history() function to save user history. The program quits if the users types "exit". When the user 
* gives a command, it is split into a command and arguments and then a command is forked to execute it. The 
* terminal waits for the command to finish. If the command terminates with an error, the error is reported.
*
* @author: Lily Davoren
* @version: October 29, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <signal.h>
#define ANSI_COLOR_PURE_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"

void segfault_handler(int signal) {
  fprintf(stderr, "Error: Segmentation fault (signal %d)\n", signal);
  exit(EXIT_FAILURE);
}

int main() {
  signal(SIGSEGV, segfault_handler);
  
  char* banner = "LILY'S SHELL";

  char input[256];
  snprintf(input, sizeof(input), "echo \"%s\" | ./dir/usr/bin/figlet-figlet -d ./dir/usr/share/figlet", banner);

  system(input);

  char user[256];
  getlogin_r(user, 255);
  char host[256];
  gethostname(host, 255);
  char cwd[256];
  getcwd(cwd, 255);

  pid_t pid;
  static char *line = (char *)NULL;

  while(1) {
    //Print the prompt
    char prompt[1000];
    sprintf(prompt, ANSI_COLOR_PURE_MAGENTA "%s@%s:/%s$ " ANSI_COLOR_RESET, user, host, cwd);
    line = readline(prompt); 
    if (line == NULL) {
      free(line);
      break;
    }
    add_history(line);

    //Separate the command and arguments
    char* command = strtok(line, " ");
    if (command == NULL) {
      free(line);
      continue;
    }

    // Initialize the arguments array
    char* arguments[256];
    int i = 0;
    arguments[i++] = command;
    char* arg;
    while ((arg = strtok(NULL, " ")) != NULL) {
      arguments[i++] = arg;
    }
    arguments[i] = NULL;
    
    if (strcmp(command, "exit") == 0) {
      free(line);
      break;
    }
  
    //Fork a child process
    pid = fork();
    if (pid == 0) {
      execvp(command, arguments);
      perror("execvp failed"); 
      exit(EXIT_FAILURE); 
    } else {
      int status;
      wait(&status);

      if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
        fprintf(stderr, ANSI_COLOR_RED "Segmentation fault occurred: " ANSI_COLOR_RESET);
        printf("THAT MEMORY IS NOT YOURS! FIX YOUR CODE!\n");
      }
    }

    free(line);
  }
  return 0;
}