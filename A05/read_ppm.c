/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE* fp = fopen(filename, "rb");

  if (fp == NULL) {
    return NULL;
  }

  char magic_number[3];
  fgets(magic_number, 3, fp);
  if (strcmp(magic_number, "P6") != 0) {
    fclose(fp);
    return NULL;
  }

  char c;
  while ((c = fgetc(fp)) == '#') {
    while ((c = fgetc(fp)) != '\n');
  }
  ungetc(c, fp);

  char whitespaces[100];
  fgets(whitespaces, 100, fp);

  char width_height[100];
  fgets(width_height, 100, fp);

  char max_color[4];
  fgets(max_color, 4, fp);

  char whitespace[2];
  fgets(whitespace, 2, fp);

  char raster[1000];
  fgets(raster, 1000, fp);

  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * (*w * *h));
  fread(pixels, sizeof(struct ppm_pixel), *w * *h, fp);
  fclose(fp);
  
  return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
