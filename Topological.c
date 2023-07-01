#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 100

// Structure to represent a graph
struct Graph {
    int numVertices;
    int** adjacencyMatrix;
};

// Stack structure
struct Stack {
    int top;
    int array[MAX_VERTICES];
};

// Function to create an empty stack
struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    return stack;
}

// Function to check if the stack is empty
int isEmpty(struct Stack* stack) {
    return (stack->top == -1);
}

// Function to push an element onto the stack
void push(struct Stack* stack, int item) {
    stack->array[++stack->top] = item;
}

// Function to pop an element from the stack
int pop(struct Stack* stack) {
    return stack->array[stack->top--];
}

// Function to create a graph with a given number of vertices
struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->adjacencyMatrix = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        graph->adjacencyMatrix[i] = (int*)calloc(numVertices, sizeof(int));
    }
    return graph;
}

// Function to add a directed edge to the graph
void addEdge(struct Graph* graph, int src, int dest) {
    if (src >= 0 && src < graph->numVertices && dest >= 0 && dest < graph->numVertices) {
        graph->adjacencyMatrix[src][dest] = 1;
    }
}

// Depth-First Search function for topological sort
void DFS(struct Graph* graph, int vertex, int* visited, struct Stack* stack) {
    visited[vertex] = 1;

    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->adjacencyMatrix[vertex][i] == 1 && visited[i] == 0) {
            DFS(graph, i, visited, stack);
        }
    }

    push(stack, vertex);
}

// Function to perform topological sort using Depth-First Search
void topologicalSort(struct Graph* graph) {
    int* visited = (int*)calloc(graph->numVertices, sizeof(int));
    struct Stack* stack = createStack();

    for (int i = 0; i < graph->numVertices; i++) {
        if (visited[i] == 0) {
            DFS(graph, i, visited, stack);
        }
    }

    printf("Topological Sort: ");
    while (!isEmpty(stack)) {
        int vertex = pop(stack);
        printf("%d ", vertex);
    }
    printf("\n");

    free(visited);
    free(stack);
}

// Function to free the memory occupied by the graph
void freeGraph(struct Graph** graph) {
    for (int i = 0; i < (*graph)->numVertices; i++) {
        free((*graph)->adjacencyMatrix[i]);
    }
    free((*graph)->adjacencyMatrix);
    free(*graph);
    *graph = NULL;
}

int main() {
    struct Graph* graph = createGraph(6);

    addEdge(graph, 5, 2);
    addEdge(graph, 5, 0);
    addEdge(graph, 4, 0);
    addEdge(graph, 4, 1);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 1);

    topologicalSort(graph);

    freeGraph(&graph);

    return 0;
}
