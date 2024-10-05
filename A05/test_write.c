/**
* The main driver program for A05 test_write.c.
*
* This program contains a short test that calls the write_ppm() function in write_ppm.c on an array of pixels 
* read using read_ppm(), calls read_ppm() on the new PPM file created using write_ppm() and prints the contents of feep-raw.ppm
*
* @author: Lily Davoren
* @version: October 3, 2024
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: %s <width> <height>\n", argv[0]);
    exit(0);
  }

  char* width = argv[1];
  char* height = argv[2];

  int w = atoi(width);
  int h = atoi(height);

  // Read in the PPM file
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  // Write the PPM file
  write_ppm("test.ppm", pixels, w, h);
  // Read in the new PPM file
  struct ppm_pixel* pixels2 = read_ppm("test.ppm", &w, &h);

  printf("Testing file test.ppm: %d %d\n", w, h);
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      printf("(%d, %d, %d)  ", pixels2[i * w + j].red, pixels2[i * w + j].green, pixels2[i * w + j].blue);
    }
    printf("\n");
  }

  free(pixels2);
  free(pixels);
  return 0;
}
