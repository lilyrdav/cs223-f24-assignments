/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* fp = fopen(filename, "wb");
    
    if (fp == NULL) {
        return;
    }
    
    fprintf(fp, "P6\n");
    fprintf(fp, "Created by Lily <3\n");
    fprintf(fp, "%d %d\n", w, h);
    fprintf(fp, "255\n");
    
    fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);
    fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
