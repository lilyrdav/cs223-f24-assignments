/**
* The main driver program for A11 tree.c.
*
* This program implements a binary search tree. 
*
* @author: Lily Davoren
* @version: November 29, 2024
*/

#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>

struct tree_node* find(const char* name, struct tree_node* root)
{
  if (root == NULL) {
    return NULL;
  }
  if (strcmp(name, root->data.name) < 0) {
    return find(name, root->left);
  }
  else if (strcmp(name, root->data.name) > 0) {
    return find(name, root->right);
  }
  return root;
}

struct tree_node* insert(const char* name, struct tree_node* root)
{
  if (root == NULL) {
    root = (struct tree_node*)malloc(sizeof(struct tree_node));
    if (root == NULL) {
      perror("Out of space!!!");
      return NULL;
    }
    else {
      strncpy(root->data.name, name, sizeof(root->data.name) - 1);
      root->data.name[sizeof(root->data.name) - 1] = '\0';
      root->left = root->right = NULL;
    }
  }
  else if (strcmp(name, root->data.name) < 0) {
    root->left = insert(name, root->left);
  }
  else if (strcmp(name, root->data.name) > 0) {
    root->right = insert(name, root->right);
  }
  return root;
}

void clear(struct tree_node* root)
{
  if (root == NULL) {
    return;
  }

  clear(root->left);
  clear(root->right);

  free(root);
}

void printUtil(struct tree_node* root, int space) {
  if (root == NULL) return;

  int count = 2;
  space += count;
  
  for (int i = 0; i < space; i++) {
    printf(" ");
  }
  if (root->left != NULL) {
    printf("l:%s\n", root->left->data.name);
  }

  for (int i = 0; i < space; i++) {
    printf(" ");
  }
  if (root->right != NULL) {
    printf("r:%s\n", root->right->data.name);
  }
  printUtil(root->left, 0);
  printUtil(root->right, 0);
}

void print(struct tree_node* root)
{
  printf("%s\n", root->data.name);
  printUtil(root, 0);
  printf("\n");
}

void printSorted(struct tree_node* root)
{
  if (root == NULL) {
    printf("\n");
    return;
  }

  printSorted(root->left);
  printf("%s ", root->data.name);
  printSorted(root->right);
}