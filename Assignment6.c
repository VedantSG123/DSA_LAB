#include <stdio.h>
#include <stdlib.h>

struct Node{
	int data;
	struct Node *next;
};
typedef struct Node Node_t;

Node_t *createNode(int data){
	Node_t *temp = (Node_t*)malloc(sizeof(Node_t));
	temp->data = data;
	temp->next = NULL;
    return temp;
}

//Adjacency list 
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

//Queue (for BFS)
struct Queue{
	Node_t *front;
	Node_t *rear;
};
typedef struct Queue Queue_t;

Queue_t *initiateQueue(){
	Queue_t *temp = (Queue_t*)malloc(sizeof(Queue_t));
	temp->front = NULL;
	temp->rear = NULL;
	return temp;
}


void enQueue(Queue_t *q,int val){
	Node_t *new = createNode(val);
	if (q->front == NULL) {
        q->front = new;
        q->rear = new;
        new->next = new; 
    } else {
        new->next = q->front; 
        q->rear->next = new; 
        q->rear = new;
    }
}

int* deQueue(Queue_t* q) {
    if (q->front == NULL)
        return NULL;

    Node_t* temp = q->front;
    int* pointer = (int*)malloc(sizeof(int));
    *pointer = temp->data;

    if (q->front == q->rear) { 
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front = q->front->next;
        q->rear->next = q->front;
    }

    free(temp);
    return pointer;
}

int main(int argc, char *argv[]) {

    /*-------------------Read From File boiler plate-------------------*/
    if (argc > 2) {
    printf("NOT VALID. ENTER ONLY ONE FILE NAME");
    exit(1);
    }

    if (argc < 2) {
    printf("NOT VALID. PLEASE ENTER THE FILE NAME");
    exit(1);
    }

    FILE *fp_graph;
    FILE *fp_w;
    fp_graph = fopen(argv[1], "r");

    if (!fp_graph) {
    printf("FILE NOT FOUND");
    exit(2);
    }
    fp_w = fopen("sd.txt", "w");

    int vertices, edges;
    fscanf(fp_graph, "%d %d", &vertices, &edges);
    int visited[vertices];
    int distance[vertices];
    for(int i=0; i<vertices; ++i){
        visited[i] = 0;
        distance[i] = -1;
    }
    Node_t **adjacency = createTable(vertices);
    int x,y;
    while(fscanf(fp_graph, "%d %d\n", &x, &y) != EOF){
        Node_t *new_y = createNode(y);
        insert_start(&adjacency[x], new_y);
        Node_t *new_x = createNode(x);
        insert_start(&adjacency[y], new_x);
    }

   //BFS
    Queue_t *q = initiateQueue();
    visited[0] = 1;
    distance[0] = 0;
    enQueue(q, 0);
    while(q->front != NULL){
        int *removed = deQueue(q);
        int u = *removed;
        Node_t *travel = adjacency[u];
        while(travel){
            if(visited[travel->data] == 0){
                visited[travel->data] = 1;
                distance[travel->data] = distance[u] + 1;
                enQueue(q, travel->data);
            }
            travel = travel->next;
        }
    }

    for(int i=0; i<vertices; ++i){
        fprintf(fp_w,"%d\n", distance[i]);
    }

    fclose(fp_graph);
    fclose(fp_w);
    return 0;
}
