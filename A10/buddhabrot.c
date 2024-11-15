/**
* The main driver program for A10 buddhabrot.c.
*
* This program outputs a PPM image of the buddhabrot set using MULTITHREADING.
*
* @author: Lily Davoren
* @version: November 14, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <math.h>
#include <getopt.h>

struct params {
  int row_start;
  int row_end;
  int col_start;
  int col_end;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  struct ppm_pixel* pixels;
  int size;
};

pthread_barrier_t mybarrier;
pthread_mutex_t mutex;
int global_max_count = 0;

void* compute_mandelbrot(void* user_data) {
  struct params* data = (struct params*)user_data;

  int rows = data->row_end - data->row_start;
  int cols = data->col_end - data->col_start;
  int** membership = malloc(rows * sizeof(int*));
  for (int q = 0; q < rows; q++) {
    membership[q] = calloc(cols, sizeof(int));
  }

  // Determine mandelbrot set membership
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      float xfrac = (float)j / cols;
      float yfrac = (float)i / rows;
      float x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      float y0 = data->ymin + yfrac * (data->ymax - data->ymin);

      float x = 0;
      float y = 0;
      int iter = 0;

      while (iter < data->maxIterations && x * x + y * y < 4) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iter++;
      }

      if (iter < data->maxIterations) {
        membership[i][j] = 0;
      }
      else {
        membership[i][j] = 1;
      }
    }
  }

  int** count = malloc(rows * sizeof(int*));
  for (int t = 0; t < rows; t++) {
    count[t] = calloc(cols, sizeof(int));
  }

  int local_max_count = 0;
  //Compute visited counts
  for (int k = 0; k < rows; k++) {
    for (int l = 0; l < cols; l++) {
      if (membership[k][l] == 1) {
        continue;
      }
      
      float xfrac = (float)l / cols;
      float yfrac = (float)k / rows;
      float x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      float y0 = data->ymin + yfrac * (data->ymax - data->ymin);

      float x = 0;
      float y = 0;

      while (x * x + y * y < 4) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;

        int yrow = (int)round(rows * (y - data->ymin) / (data->ymax - data->ymin));
        int xcol = (int)round(cols * (x - data->xmin) / (data->xmax - data->xmin));

        if (yrow < 0 || yrow >= rows){
          continue;
        }
        if (xcol < 0 || xcol >= cols) {
          continue;
        }
        
        pthread_mutex_lock(&mutex);
        count[yrow][xcol]++;
        if (count[yrow][xcol] > local_max_count) {
          local_max_count = count[yrow][xcol];
        }
        pthread_mutex_unlock(&mutex);
      }
    }
  }
  printf("Local max count: %d\n", local_max_count);

  if (local_max_count > global_max_count) {
    global_max_count = local_max_count;
  }
  printf("Max Count: %d\n", global_max_count);

  pthread_barrier_wait(&mybarrier);
  
  //Compute colors
  float gamma = 0.681;
  double factor = 1.0 / gamma;
  for (int o = 0; o < rows; o++) {
    for (int p = 0; p < cols; p++) {
      double value = 0;

      if (count[o][p] > 0) {
        value = log((double)count[o][p]) / log((double)global_max_count);
        value = pow(value, factor);
      }

      struct ppm_pixel color = {
        (unsigned char)(value * 255),
        (unsigned char)(value * 255),
        (unsigned char)(value * 255)
      };

      data->pixels[(data->row_start + o) * data->size + (data->col_start + p)] = color;
    }
  }

  for (int i = 0; i < rows; i++) {
    free(membership[i]);
    free(count[i]);
  }
  free(membership);
  free(count);

  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }

  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f, %.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f, %.4f]\n", ymin, ymax);

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * size * size);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  pthread_t thread[4];
  pthread_barrier_init(&mybarrier, NULL, 4);

  float xmid = (xmin + xmax) / 2.0;
  float ymid = (ymin + ymax) / 2.0;

  struct params data[4] = {
    {0, size / 2, 0, size / 2, xmin, xmid, ymin, ymid, maxIterations, pixels, size},
    {0, size / 2, size / 2, size, xmid, xmax, ymin, ymid, maxIterations, pixels, size},
    {size / 2, size, 0, size / 2, xmin, xmid, ymid, ymax, maxIterations, pixels, size},
    {size / 2, size, size / 2, size, xmid, xmax, ymid, ymax, maxIterations, pixels, size}
  };

  for (int i = 0; i < 4; i++) {
    pthread_create(&thread[i], NULL, compute_mandelbrot, (void*)&data[i]);
    printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n",
           i, data[i].col_start, data[i].col_end, data[i].row_start, data[i].row_end);
  }

  for (int i = 0; i < 4; i++) {
    pthread_join(thread[i], NULL);
    printf("Thread %d) finished\n", i);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);

  char filename[100];
  sprintf(filename, "buddhabrot-%d-%lld.ppm", size, (long long)time(0));
  write_ppm(filename, pixels, size, size);

  free(pixels);
  pthread_barrier_destroy(&mybarrier);

  return 0;
}