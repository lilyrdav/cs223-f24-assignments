/**
* The main driver program for A01.
*
* This program implements a math game.
*
* @author: Lily Davoren
* @version: September 4, 2024
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("Welcome to Math Game!\n");
  printf("How many rounds do you want to play? ");

  int rounds;
  scanf("%d", &rounds);

  int correct = 0;
  for (int i = 0; i < rounds; i++) {
    int a = (rand() % 9) + 1;
    int b = (rand() % 9) + 1;
    printf("%d + %d = ? ", a, b);
    int answer;
    scanf("%d", &answer);
    if (answer == a + b){
      printf("Correct!\n\n");
      correct++;
    }
    else {
      printf("Incorrect :(\n\n");
    }
  }
  printf("You answered %d/%d correctly.", correct, rounds);
  return 0;
}