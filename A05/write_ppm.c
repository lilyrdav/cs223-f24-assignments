/**
* The main driver program for A05 write_ppm.c.
*
* This program writes PPM files. The write_ppm() function takes a filename and an array of struct ppm_pixels as input
* and prints the contents of the newly written file.
*
* @author: Lily Davoren
* @version: October 3, 2024
*/

#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

//Write in a PPM file in binary format
void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    //Open the file
    FILE* fp = fopen(filename, "wb");
    
    //Check if the file is NULL
    if (fp == NULL) {
        return;
    }
    
    //Write the header
    fprintf(fp, "P6\n");
    fprintf(fp, "Created by Lily <3\n");
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "255\n");
    
    //Write the pixels
    fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);
    fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
