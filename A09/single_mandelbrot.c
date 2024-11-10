/**
* The main driver program for A09 single_mandelbrot.c.
*
* This program outputs a PPM image of the mandelbrot set.
*
* @author: Lily Davoren
* @version: November 8, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

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

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * (size*size));
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);

  srand(time(0));
  for (int k = 0; k < maxIterations; k++) {
    palette[k].red = rand() % 256;
    palette[k].green = rand() % 256;
    palette[k].blue = rand() % 256;
  }

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      float xfrac = (float)j / size;
      float yfrac = (float)i / size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      
      while (iter < maxIterations && x*x + y*y < 2*2) {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iter++;
      }
      struct ppm_pixel color = {0, 0, 0}; 
      if (iter < maxIterations) {
        color = palette[iter];
      }

      pixels[i * size + j].red = color.red;
      pixels[i * size + j].green = color.green;
      pixels[i * size + j].blue = color.blue;
    }
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;

  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: mandelbrot-%d-%lld.ppm\n", size, (long long)time(0));

  char filename[100];
  sprintf(filename, "mandelbrot-%d-%lld.ppm", size, (long long)time(0));
  write_ppm(filename, pixels, size, size);

  free(pixels);
  free(palette);
}