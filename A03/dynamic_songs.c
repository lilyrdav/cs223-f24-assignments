/**
* The main driver program for A03 dynamic_songs.c.
*
* This code implements a program, dynamic_songs.c, that reads songs from a text file into an array. The program has the 
* following features:
*   The ability to open a CSV file (songlist.csv) and read its contents into an array created with malloc.
*   The ability to read in additional information: tempo, valence, and energy.
*   The ability to run with memory leaks or valgrind errors
*   The ability to set the contents of our dynamic array of songs, based on the file
*   The ability to print out the list of songs
*
* @author: Lily Davoren
* @version: September 18, 2024
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Struct that holds the information for each song.
typedef struct song {
  char title[128];
  char artist[128];
  int duration;
  float danceability;
  float energy;
  float tempo;
  float valence;
} song;

//Function that prints out the song list.
void songList(song songs[], int numSongs) {
  printf("Welcome to Lily Davoren's Song List.\n\n");
  for (int i = 0; i < 10; i++) {
    printf("%d) %-30s artist: %-20s duration: %d:%-20d danceability: %-20.3f energy: %-20.3f tempo: %-20.3f valence: %.3f\n\n", 
      i, songs[i].title, songs[i].artist, songs[i].duration/60000, (songs[i].duration - songs[i].duration/60000*60000)/1000, songs[i].danceability, 
        songs[i].energy, songs[i].tempo, songs[i].valence);
  }
  for (int i = 10; i < numSongs; i++) {
    printf("%d) %-29s artist: %-20s duration: %d:%-20d danceability: %-20.3f energy: %-20.3f tempo: %-20.3f valence: %.3f\n\n", 
      i, songs[i].title, songs[i].artist, songs[i].duration/60000, (songs[i].duration - songs[i].duration/60000*60000)/1000, songs[i].danceability, 
        songs[i].energy, songs[i].tempo, songs[i].valence);
  }
  printf("=============================================\n\n");
}

int main() {
  //Read and store the first line of the file to determine the number of songs
  char firstLine[300];
  char* firstElements;
  FILE *fp;
  fp = fopen("songlist.csv", "r");
  fgets(firstLine, 300, fp);
  firstElements = strtok(firstLine, ",");
  int songlistLength = atoi(&firstElements[0]);

  //Skip the second line of the file
  char secondLine[300];
  fgets(secondLine, 300, fp);

  //Read and store the rest of the file
  song* songlist = (song*)malloc(songlistLength * sizeof(song));
  for (int i = 0; i < songlistLength; i++) {
    char line[300];
    char* elements;
    fgets(line, 300, fp);
    elements = strtok(line, ",");
    strcpy(songlist[i].title, elements);
    elements = strtok(NULL, ",");
    strcpy(songlist[i].artist, elements);
    elements = strtok(NULL, ",");
    songlist[i].duration = atoi(elements);
    elements = strtok(NULL, ",");
    songlist[i].danceability = atof(elements);
    elements = strtok(NULL, ",");
    songlist[i].energy = atof(elements);
    elements = strtok(NULL, ",");
    songlist[i].tempo = atof(elements);
    elements = strtok(NULL, ",");
    songlist[i].valence = atof(elements);
    elements = strtok(NULL, ",");
  }
  fclose(fp);

  songList(songlist, songlistLength);

  free(songlist);

  return 0;
}