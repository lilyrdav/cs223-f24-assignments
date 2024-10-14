/**
* The main driver program for A05 read_ppm.c.
*
* This program reads PPM files stored in binary format. The read_ppm() function should take a filename as input
* and return a 2D array of struct pixel.
*
* @author: Lily Davoren
* @version: October 3, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  // Open the file
  FILE* fp = fopen(filename, "rb");

  // Check if the file is NULL
  if (fp == NULL) {
    return NULL;
  }

  // Read the header
  char magic_number[3];
  fgets(magic_number, 3, fp);
  if (strcmp(magic_number, "P6") != 0) {
    fclose(fp);
    return NULL;
  }

  // Skip comments
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

  // Read the pixels
  struct ppm_pixel* pixels = malloc(sizeof(struct ppm_pixel) * (*w * *h));
  fread(pixels, sizeof(struct ppm_pixel), *w * *h, fp);
  fclose(fp);
  
  return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}