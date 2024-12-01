/**
* The main driver program for A11 dependency.c.
*
* A dependency is a general software term for any software required by another. This program stores files in a binary search tree
* and list their file dependencies, as determined by #include statements. For example, the file "Animal.h" depends on "Locomotion.h".
* This programs uses N threads to build a binary search tree of a given set of files. After building the tree, the program gives 
* the user a prompt where they can list the processed files in alphabetical order and then query the dependencies of the file by 
* giving the filename.
*
* @author: Lily Davoren
* @version: December 1, 2024
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include "tree.h"

struct thread_data {
  int start_index;
  int end_index;
  char** files;
  int id;
  struct tree_node** root;
  pthread_mutex_t* mutex;
};

struct file_dependencies {
  char name[100];
  char dependencies[100][100];
  int dep_count;
};

struct file_dependencies dependency_map[100];
int global_dep_count = 0;

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

void* construct_tree(void* userdata) {
  struct thread_data* data = (struct thread_data*)userdata;
  for (int i = data->start_index; i < data->end_index; i++) {
    if (!data->files[i]) continue;

    pthread_mutex_lock(data->mutex);
    *data->root = insert(data->files[i], *data->root);
    pthread_mutex_unlock(data->mutex);

    FILE* file = fopen(data->files[i], "r");
    if (!file) {
      fprintf(stderr, "Error opening %s\n", data->files[i]);
      continue;
    }

    pthread_mutex_lock(data->mutex);
    strncpy(dependency_map[global_dep_count].name, data->files[i], sizeof(dependency_map[global_dep_count].name) - 1);
    dependency_map[global_dep_count].name[sizeof(dependency_map[global_dep_count].name) - 1] = '\0';
    dependency_map[global_dep_count].dep_count = 0;
    pthread_mutex_unlock(data->mutex);

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
      char* include = strstr(buffer, "#include");
      if (include) {
        include += strlen("#include");
        while (*include == ' ' || *include == '"' || *include == '<') {
          include++;
        }
        char* end_include = include;
        while (*end_include && *end_include != '"' && *end_include != '>') {
          end_include++;
        }
        *end_include = '\0';

        pthread_mutex_lock(data->mutex);
        strncpy(dependency_map[global_dep_count].dependencies[dependency_map[global_dep_count].dep_count], include, 
          sizeof(dependency_map[global_dep_count].dependencies[0]) - 1);
        dependency_map[global_dep_count].dependencies[dependency_map[global_dep_count].dep_count]
          [sizeof(dependency_map[global_dep_count].dependencies[0]) - 1] = '\0';
        dependency_map[global_dep_count].dep_count++;
        pthread_mutex_unlock(data->mutex);
      }
    }
    fclose(file);

    pthread_mutex_lock(data->mutex);
    global_dep_count ++;
    pthread_mutex_unlock(data->mutex);
  }
  return NULL;
}

int main(int argc, char** argv) {
  const int num_threads = atoi(argv[1]);

  if (argc != 3) {
    fprintf(stderr, "usage: ./grep <NumThreads> <Files>");
    return 1;
  }

  char* files_input = argv[2];
  int file_count = 0;
  char** files = NULL;

  files = execute_find(files_input, &file_count);
  if (files == NULL) {
    return 1;
  }

  printf("Processing %d files\n", file_count);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
  struct thread_data* data = (struct thread_data*)malloc(sizeof(struct thread_data) * num_threads);
  struct tree_node* root = NULL;
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  int subsize = file_count / num_threads;
  int remainder = file_count % num_threads;
  for (int j = 0; j < num_threads; j++) {
    data[j].files = files;
    data[j].root = &root;
    data[j].mutex = &mutex;
    data[j].id = j;
    data[j].start_index = subsize*j;
    if ((remainder != 0) && (j == num_threads - 1)) {
      data[j].end_index = subsize*(j+1) + remainder;
    }
    else {
      data[j].end_index = subsize*(j+1);
    }

    pthread_create(&threads[j], NULL, construct_tree, (void*)&data[j]);
    printf("Thread %d processing %d files (files %d to %d)\n", data[j].id, data[j].end_index - data[j].start_index, data[j].start_index + 1, 
        data[j].end_index);
  }

  for (int l = 0; l < num_threads; l++) {
    pthread_join(threads[l], NULL);
  }

  pthread_mutex_destroy(&mutex);

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Elapsed time is %g seconds\n", timer);

  char command[256];
  while (1) {
    printf("$ ");
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    if (strcmp(command, "quit") == 0) {
      break;
    }

    if (strcmp(command, "list") == 0) {
      printSorted(root);
    }
    else {
      struct tree_node* node = find(command, root);
      if (node == NULL) {
        printf("%s not found\n", command);
        continue;
      }

      for (int i = 0; i < global_dep_count; i++) {
        if (strcmp(dependency_map[i].name, command) == 0) {
          printf("%s has the following dependencies\n", command);
          for (int j = 0; j < dependency_map[i].dep_count; j++) {
            printf("  %s\n", dependency_map[i].dependencies[j]);
          }
          break;
        }
      }
    }
  }


  for (int m = 0; m < file_count; m++) {
    free(files[m]);
  }
  free(files);
  free(threads);
  free(data);

  clear(root);
  return 0;
}