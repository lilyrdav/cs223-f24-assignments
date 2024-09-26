/**
* The main driver program for A03 dynamic_songs.c.
*
* This code implements a program, danceable.c, that allows users to easily access the most danceable songs in songlist.csv.
* The program should load songs from a CSV file, storing the songs in a linked list. When the user asks for the most
* danceable song, your program should find the song with the highest danceability, show the user its information, 
* and then delete it from the linked list. The program includes helper functions for printing, removing nodes, clearing the list, 
* and inserting to the list. The program runs without memory leaks, handles the case where the list is empty, and cleans up
* memory if the list is non-empty when the user quits.
*
* @author: Lily Davoren
* @version: September 26, 2024
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

//Struct that holds the information for each node in the linked list.
typedef struct node {
  song data;
  struct node* next;
} node;

//Function that inserts a song into the linked list.
node* insert_front(song data, struct node* head) {
  struct node* n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("ERROR: Out of space!\n");
    exit(1);
  }
  n->data = data;
  n->next = head;
  return n;
};

//Function that deletes a node from the linked list.
void delete_node(node* list) {
  node* temp = list;
  list = list->next;
  free(temp);
}

//Function that clears the linked list.
void clear_list(node* list) {
  node* temp;
  while (list != NULL) {
    temp = list;
    list = list->next;
    free(temp);       
  }
}

//Function that prints out the song list.
void print_songs(node* list) {
  int index = 0;
  for (node* n = list; n != NULL; n = n->next) {
    printf("%d) %-30s artist: %-20s duration: %d:%-20d danceability: %-20.3f energy: %-20.3f tempo: %-20.3f valence: %.3f\n\n", 
      index, n->data.title, n->data.artist, n->data.duration/60000, (n->data.duration - n->data.duration/60000*60000)/1000, n->data.danceability, 
        n->data.energy, n->data.tempo, n->data.valence);
    index++;
  }
  printf("\n");
  printf("Dataset contains %d songs\n\n", index);
  printf("=============================================\n");
}

int main() {
  //Skip the first line of the file
  char secondLine[300];
  FILE *fp;
  fp = fopen("songlist.csv", "r");
  fgets(secondLine, 300, fp);

  //Read and store the rest of the file
  song* songs = (song*)malloc(5 * sizeof(song));
  for (int i = 0; i < 5; i++) {
    char line[300];
    char* elements;
    fgets(line, 300, fp);
    elements = strtok(line, ",");
    strcpy(songs[i].title, elements);
    elements = strtok(NULL, ",");
    strcpy(songs[i].artist, elements);
    elements = strtok(NULL, ",");
    songs[i].duration = atoi(elements);
    elements = strtok(NULL, ",");
    songs[i].danceability = atof(elements);
    elements = strtok(NULL, ",");
    songs[i].energy = atof(elements);
    elements = strtok(NULL, ",");
    songs[i].tempo = atof(elements);
    elements = strtok(NULL, ",");
    songs[i].valence = atof(elements);
    elements = strtok(NULL, ",");
  }
  fclose(fp);

  //Create a linked list of the songs
  node* n1 = insert_front(songs[4], NULL);
  n1 = insert_front(songs[3], n1);
  n1 = insert_front(songs[2], n1);
  n1 = insert_front(songs[1], n1);
  n1 = insert_front(songs[0], n1);

  int true = 1;
  while(true == 1) {
    print_songs(n1);

    char input;
    if (n1!= NULL) {
      printf("Press 'd' to show the most danceable song (any other key to quit): ");
      scanf(" %c", &input);
    }
    else{
      clear_list(n1);
      free(songs);
      exit(0);
    }

    if (input == 'd') {
      float max = 0;
      node* mostDanceable;

      for (node* n = n1; n != NULL; n = n->next) {
        if (n->data.danceability > max) {
          max = n->data.danceability;
          mostDanceable = n;
        }
      }

      printf("\n---------------------------------------- Most danceable ----------------------------------------\n");
      printf("%-30s artist: %-20s duration: %d:%-20d danceability: %-20.3f energy: %-20.3f tempo: %-20.3f valence: %.3f\n", 
        mostDanceable->data.title, mostDanceable->data.artist, mostDanceable->data.duration/60000, 
          (mostDanceable->data.duration - mostDanceable->data.duration/60000*60000)/1000, mostDanceable->data.danceability, 
            mostDanceable->data.energy, mostDanceable->data.tempo, mostDanceable->data.valence);
      printf("--------------------------------------------------------------------------------------------------\n\n");
      
      node* curr = n1;
      node* prev = NULL;
      while (curr != mostDanceable) {
        prev = curr;
        curr = curr->next;
      }
      if (prev == NULL) {
        n1 = curr->next;
      }
      else {
        prev->next = curr->next;
      }
      
      delete_node(curr);
    }
    else {
      clear_list(n1);
      free(songs);
      exit(0);
    }
  }

  free(songs);

  return 0;
}