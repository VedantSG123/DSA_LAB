#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 25000

struct Node {
  char *data;
  struct Node *next;
};

typedef struct Node Node_t;

Node_t *createNode(const char *data) {
  Node_t *node = (Node_t *)malloc(sizeof(Node_t));
  node->data = strdup(data);
  node->next = NULL;
  return node;
}

void insert_start(Node_t **head, Node_t *new) {
  new->next = *head;
  *head = new;
}

Node_t **createTable(int size) {
  Node_t **table = (Node_t **)malloc(size * sizeof(Node_t *));
  for (int i = 0; i < size; ++i) {
    table[i] = NULL;
  }
  return table;
}

int hash(char word[], int m) {
  int length = strlen(word);
  int sum = 0;
  for (int i = 0; i < length; ++i) {
    sum += word[i];
  }

  return sum % m;
}

int compare(const void *a, const void *b) { return (*(char *)a - *(char *)b); }

void sortString(char *str) {
  size_t len = strlen(str);
  qsort(str, len, sizeof(char), compare);
}

int main(int argc, char *argv[]) {

  /*-------------------Read From File boiler plate-------------------*/
  if (argc > 4) {
    printf("NOT VALID. ENTER THE THREE REQUIRED PARAMETERS");
    exit(1);
  }

  if (argc < 4) {
    printf("NOT VALID. PLEASE ENTER ALL THE PARAMETERS");
    exit(1);
  }

  FILE *fp_words;
  FILE *fp_query;
  FILE *fp_w;
  fp_words = fopen(argv[1], "r");
  int length = atoi(argv[2]);
  fp_query = fopen(argv[3], "r");

  if (!fp_words || !fp_query) {
    printf("FILE NOT FOUND");
    exit(2);
  }
  fp_w = fopen("anagrams.txt", "w");

  if (length <= 0) {
    printf("Enter a valid length of hashmap");
    exit(2);
  }

  char word[512];
  Node_t **record = createTable(length);

  while (fscanf(fp_words, "%s\n", word) != EOF) {
    int hash_value = hash(word, length);
    Node_t *new_node = createNode(word);
    insert_start(&record[hash_value], new_node);
  }

  /*for (int i = 0; i < length; ++i) {
    Node_t *temp = record[i];
    while (temp) {
      printf("%s ", temp->data);
      temp = temp->next;
    }
    printf("\n");
  }*/

  char query[512];
  char copy[512];
  while (fscanf(fp_query, "%s\n", query) != EOF) {
    int hash_value = hash(query, length);
    strcpy(copy, query);
    sortString(copy);
    Node_t *travel = record[hash_value];
    int first = 0;
    while (travel != NULL) {
      char word[512];
      strcpy(word, travel->data);
      sortString(word);
      int compare = strcmp(word, copy);
      if (!compare) {
        first++;
        if (first == 1) {
          fprintf(fp_w, "%s", travel->data);
        } else {
          fprintf(fp_w, " %s", travel->data);
        }
      }
      travel = travel->next;
    }
    fprintf(fp_w, "\n");
  }

  fclose(fp_words);
  fclose(fp_query);
  fclose(fp_w);
  return 0;
}
