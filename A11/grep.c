/**
* The main driver program for A11 grep.c.
*
* This program implements a simplified version of the bash command grep. Grep searches a list of files for a given keyword or expression. 
*
* @author: Lily Davoren
* @version: November 28, 2024
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

struct thread_data {
  int start_index;
  int end_index;
  char** list;
  char* keyword;
  int id;
  int line_count;
};

char** execute_find(const char* command, int* file_count) {
  FILE* file;
  char buffer[1024];
  char** files = malloc(100 * sizeof(char*));
  *file_count = 0;

  file = popen(command, "r");
  if (file == NULL) {
    perror("Error executing find command");
    return NULL;
  }

  while (fgets(buffer, sizeof(buffer), file)) {
    buffer[strcspn(buffer, "\n")] = 0;
    files[*file_count] = malloc(strlen(buffer) + 1);
    strcpy(files[*file_count], buffer);
    (*file_count)++;
    if (*file_count >= 100) {
      break;
    }
  }

  fclose(file);
  return files;
}

void* search_files(void* userdata) {
  struct thread_data* data = (struct thread_data*)userdata;
  for (int i = data->start_index; i < data->end_index; i++) {
    FILE* file = fopen(data->list[i], "r");
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
      if (strstr(buffer, data->keyword)) {
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        pthread_mutex_lock(&mutex);
        data->line_count++;
        printf("%d) %s: %s", data->id, data->list[i], buffer);
        pthread_mutex_unlock(&mutex);
      }
    }
    fclose(file);
  }
  return (void*)NULL;
}

int main(int argc, char** argv) {
  const int num_threads = atoi(argv[1]);

  if (argc != 4) {
    fprintf(stderr, "usage: ./grep <NumThreads> <Keyword> <Files>");
    return 1;
  }

  char* files_input = argv[3];
  int file_count = 0;
  char** files = NULL;

  files = execute_find(files_input, &file_count);
  if (files == NULL) {
    return 1;
  }

  printf("Searching %d files for keyword: %s\n", file_count, argv[2]);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
  struct thread_data* data = (struct thread_data*)malloc(sizeof(struct thread_data) * num_threads);
  int subsize = file_count / num_threads;
  int remainder = file_count % num_threads;
  for (int j = 0; j < num_threads; j++) {
    data[j].list = files;
    data[j].keyword = argv[2];
    data[j].id = j;
    data[j].line_count = 0;
    data[j].start_index = subsize*j;
    if ((remainder != 0) && (j == num_threads - 1)) {
      data[j].end_index = subsize*(j+1) + remainder;
    }
    else {
      data[j].end_index = subsize*(j+1);
    }
    pthread_create(&threads[j], NULL, search_files, (void*)&data[j]);
    printf("Thread %d searching %d files (%d to %d)\n", data[j].id, data[j].end_index - data[j].start_index, data[j].start_index + 1, 
        data[j].end_index);
  }

  for (int l = 0; l < num_threads; l++) {
    pthread_join(threads[l], NULL);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Elapsed time is %g seconds\n", timer);
  
  for (int n = 0; n < num_threads; n++) {
    printf("Thread %d found %d lines containing keyword: %s\n", data[n].id, data[n].line_count, argv[2]);
  }

  for (int m = 0; m < file_count; m++) {
    free(files[m]);
  }
  free(files);
  free(threads);
  free(data);

  return 0;
}