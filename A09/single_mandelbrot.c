#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

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
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * (size*size));
  struct ppm_pixel* palette = malloc(sizeof(struct ppm_pixel) * maxIterations);

  srand(time(0));
  for (int k = 0; k < maxIterations; k++) {
    palette[k].red = rand() % 255;
    palette[k].green = rand() % 255;
    palette[k].blue = rand() % 255;
  }

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      float xfrac = (float)i / size;
      float yfrac = (float)j / size;
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

  write_ppm("mandelbrot-output.ppm", pixels, size, size);

  free(pixels);
  free(palette);


  // generate pallet

  // compute image
}
