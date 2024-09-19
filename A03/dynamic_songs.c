/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    printf("%d) %-29s artist: %-20s duration: %d:%-17d danceability: %-20.3f energy: %-20.3f tempo: %-20.3f valence: %.3f\n\n", 
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

  //Read and store the second line of the file to determine the name of each category
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