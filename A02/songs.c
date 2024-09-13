/**
* The main driver program for A02 songs.c.
*
* This code implements a program, songs.c, that fills an array
* with three or more songs and then allows the user to edit the 
* data in the array.
*
* @author: Lily Davoren
* @version: September 12, 2024
*/

#include <stdio.h>
#include <string.h>

typedef struct song {
  int duration;
  float danceability;
  char artist[32];
  char title[32];
} song;

void songList(song songs[]) {
  printf("Welcome to Lily Davoren's Song List.\n\n");
  for (int i = 0; i < 3; i++) {
    printf("%d) %-20s artist: %-25s duration: %d:%-20d danceability: %f.2f\n\n", 
      i, songs[i].title, songs[i].artist, songs[i].duration/60, songs[i].duration - songs[i].duration/60*60, songs[i].danceability);
  }
  printf("=============================================\n\n");
}

int main() {
  song song1 = {205, 0.6, "The Bangles", "Walk Like an Egyptian"};
  song song2 = {252, 0.8, "Culture Club", "Karma Chameleon"};
  song song3 = {197, 0.5, "The Cardigans", "Lovefool"};

  song songs[] = {song1, song2, song3};

  songList(songs);

  printf("Enter a song id to edit [0, 1, 2]: ");
  int id;
  scanf("%d", &id);
  if (id >= 3){
    printf("Invalid song id.\n");
    printf("Enter a song id to edit [0, 1, 2]: ");
    scanf("%d", &id);
  }

  printf("Which attribute do you wish to edit? [duration, danceability, artist, title]: ");
  char attribute[32];
  scanf("%s", attribute);
  printf("Enter %s: ", attribute);
  if (strcmp(attribute, "duration") == 0) {
    int value;
    scanf("%d%*c", &value);
    songs[id].duration = value;
    songList(songs);
  }
  else if (strcmp(attribute, "danceability") == 0) {
    float value;
    scanf("%f%*c", &value);
    songs[id].danceability = value;
    songList(songs);
  }
  else if (strcmp(attribute, "artist") == 0) {
    char value[32];
    scanf(" %[^\n]%*c", value);
    strcpy(songs[id].artist, value);
    songList(songs);
  }
  else if (strcmp(attribute, "title") == 0) {
    char value[32];
    scanf(" %[^\n]%*c", value);
    strcpy(songs[id].title, value);
    songList(songs);
  }
  return 0;
}
