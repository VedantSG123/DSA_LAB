#include <stdio.h>
#include <stdlib.h>

// Graph Implementation
struct Node {
  int vertex;
  int weight;
  struct Node *next;
};
typedef struct Node Node_t;

Node_t *createNode(int vertex, int distance) {
  Node_t *temp = (Node_t *)malloc(sizeof(Node_t));
  temp->vertex = vertex;
  temp->weight = distance;
  temp->next = NULL;
  return temp;
}

// Adjacency list
Node_t **createTable(int size) {
  Node_t **table = (Node_t **)malloc(size * sizeof(Node_t *));
  for (int i = 0; i < size; ++i) {
    table[i] = NULL;
  }
  return table;
}

void insert_start(Node_t **head, Node_t *new) {
  new->next = *head;
  *head = new;
}

// Priority Queue Implementation
struct HeapNode {
  int vert;
  int dist;
};
typedef struct HeapNode HNode_t;

HNode_t *createHeapNode(int vertex, int distance) {
  HNode_t *temp = (HNode_t *)malloc(sizeof(HNode_t));
  temp->vert = vertex;
  temp->dist = distance;
  return temp;
}

struct MinHeap {
  unsigned capacity;
  unsigned length;
  HNode_t **array;
};
typedef struct MinHeap Pqueue;

Pqueue *createMinHeap(unsigned capacity) {
  Pqueue *temp = (Pqueue *)malloc(sizeof(Pqueue));
  temp->capacity = capacity;
  temp->length = 0;
  temp->array = (HNode_t **)malloc(capacity * sizeof(HNode_t *));
  return temp;
}

int isPqueueEmpty(Pqueue *pq) { return pq->length == 0; }

int isPqueueFull(Pqueue *pq) { return pq->length == pq->capacity; }

void swapHNode(HNode_t **a, HNode_t **b) {
  HNode_t *t = *a;
  *a = *b;
  *b = t;
}

void heapify(Pqueue *pq, int i) {
  int smallest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < pq->length &&
      (pq->array[left]->dist < pq->array[smallest]->dist ||
       (pq->array[left]->dist == pq->array[smallest]->dist &&
        pq->array[left]->vert < pq->array[smallest]->vert))) {
    smallest = left;
  }

  if (right < pq->length &&
      (pq->array[right]->dist < pq->array[smallest]->dist ||
       (pq->array[right]->dist == pq->array[smallest]->dist &&
        pq->array[right]->vert < pq->array[smallest]->vert))) {
    smallest = right;
  }

  if (smallest != i) {
    swapHNode(&pq->array[smallest], &pq->array[i]);
    heapify(pq, smallest);
  }
}

void EnPqueue(Pqueue *pq, HNode_t *new) {
  if (isPqueueFull(pq)) {
    printf("P-Queue Full");
    return;
  }
  ++pq->length;
  int i = pq->length - 1;

  while (i && (new->dist < pq->array[(i - 1) / 2]->dist ||
               (new->dist == pq->array[(i - 1) / 2]->dist &&new->vert <
                                 pq->array[(i - 1) / 2]->vert))) {
    pq->array[i] = pq->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  pq->array[i] = new;
}

HNode_t *DePqueue(Pqueue *pq) {
  if (isPqueueEmpty(pq)) {
    printf("Dequeueing an empty P-Queue");
    return NULL;
  }
  HNode_t *temp = pq->array[0];
  pq->array[0] = pq->array[pq->length - 1];
  --pq->length;
  heapify(pq, 0);
  return temp;
}

// Utility
int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

int main(int argc, char *argv[]) {
  /*-------------------Read From File boiler plate-------------------*/
  if (argc > 3) {
    printf("NOT VALID. PLEASE ENTER VALID ARGUMENTS");
    exit(1);
  }

  if (argc < 3) {
    printf("NOT VALID. PLEASE ENTER ALL THE ARGUMENTS");
    exit(1);
  }

  FILE *fp_graph;
  FILE *fp_w;
  fp_graph = fopen(argv[1], "r");

  if (!fp_graph) {
    printf("FILE NOT FOUND");
    exit(2);
  }
  fp_w = fopen("dijkstra.txt", "w");
  int source = atoi(argv[2]);

  int x, y, d;
  int maximum = -1;
  while (fscanf(fp_graph, "%d %d %d\n", &x, &y, &d) != EOF) {
    maximum = max(maximum, x);
    maximum = max(maximum, y);
  }
  rewind(fp_graph);

  Node_t **adjacency = createTable(maximum + 1);
  while (fscanf(fp_graph, "%d %d %d\n", &x, &y, &d) != EOF) {
    Node_t *temp = createNode(y, d);
    insert_start(&adjacency[x], temp);
  }

  int distance[maximum + 1];
  for (int i = 0; i <= maximum; ++i) {
    distance[i] = -1;
  }

  Pqueue *pq = createMinHeap(maximum + 1);

  // Dijktra's algorithm
  distance[source] = 0;
  HNode_t *initial = createHeapNode(source, 0);
  EnPqueue(pq, initial);

  while (!isPqueueEmpty(pq)) {
    HNode_t *current = DePqueue(pq);
    Node_t *travel = adjacency[current->vert];
    while (travel) {
      int adjacentVertex = travel->vertex;
      int edgeWeight = travel->weight;
      int newDistance = edgeWeight + distance[current->vert];
      if (distance[adjacentVertex] == -1 ||
          newDistance < distance[adjacentVertex]) {
        distance[adjacentVertex] = newDistance;
        EnPqueue(pq, createHeapNode(adjacentVertex, newDistance));
      }
      travel = travel->next;
    }
  }

  for (int i = 0; i <= maximum; ++i) {
    fprintf(fp_w, "%d %d\n", i, distance[i]);
  }

  fclose(fp_graph);
  fclose(fp_w);

  return 0;
}
