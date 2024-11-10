/**
* The main driver program for A09 thread_mandelbrot.c.
*
* This program outputs a PPM image of the mandelbrot set using MULTITHREADING.
*
* @author: Lily Davoren
* @version: November 9, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"

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
  struct ppm_pixel* palette;
  int size;
};

void generate_palette(struct ppm_pixel* palette, int maxIterations) {
  srand(time(0));
  for (int k = 0; k < maxIterations; k++) {
    palette[k].red = rand() % 256;
    palette[k].green = rand() % 256;
    palette[k].blue = rand() % 256;
  }
}

void* compute_mandelbrot(void* user_data) {
  struct params* data = (struct params*)user_data;
  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->col_start; j < data->col_end; j++) {
      float xfrac = (float)(j - data->col_start) / (data->col_end - data->col_start);
      float yfrac = (float)(i - data->row_start) / (data->row_end - data->row_start);
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

      struct ppm_pixel color = {0, 0, 0};
      if (iter < data->maxIterations) {
        color = data->palette[iter];
      }

      data->pixels[i * data->size + j] = color;
    }
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 2000;
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

  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * (size * size));
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);

  double timer;
  struct timeval tstart, tend;

  generate_palette(palette, maxIterations);

  gettimeofday(&tstart, NULL);

  pthread_t thread[4];

  float xmid = (xmin + xmax) / 2.0;
  float ymid = (ymin + ymax) / 2.0;

  struct params data[4] = {
    {0, size / 2, 0, size / 2, xmin, xmid, ymin, ymid, maxIterations, pixels, palette, size},
    {0, size / 2, size / 2, size, xmid, xmax, ymin, ymid, maxIterations, pixels, palette, size},
    {size / 2, size, 0, size / 2, xmin, xmid, ymid, ymax, maxIterations, pixels, palette, size},
    {size / 2, size, size / 2, size, xmid, xmax, ymid, ymax, maxIterations, pixels, palette, size}
  };

  for (int i = 0; i < 4; i++) {
    pthread_create(&thread[i], NULL, compute_mandelbrot, (void*)&data[i]);
    printf("Thread %lld) sub-image block: cols (%d, %d) to rows (%d, %d)\n", (long long)thread[i], data[i].col_start, data[i].col_end, data[i].row_start, data[i].col_end);
  }

  for (int i = 0; i < 4; i++) {
    pthread_join(thread[i], NULL);
    printf("Thread %lld) finished\n", (long long)thread[i]);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: mandelbrot-%d-%lld.ppm\n", size, (long long)time(0));

  char filename[100];
  sprintf(filename, "mandelbrot-%d-%lld.ppm", size, (long long)time(0));
  write_ppm(filename, pixels, size, size);

  free(pixels);
  free(palette);

  return 0;
}