#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/* Function to create an adjacency list node*/
adjlist_node* createNode(int v) {
	adjlist_node *newNode;
	newNode = (adjlist_node*) malloc(sizeof(adjlist_node));
	if (!newNode) {
		printf("Unable to allocate memory for node.\n");
		exit(1);
	}
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph* createGraph(int n, graph_type_e type) {
	graph *G = (graph*) malloc(sizeof(graph));
	if (!G) {
		printf("Unable to allocate memory for graph.\n");
		exit(1);
	}
	G->num_vertices = n;
	G->type = type;

	/* Create an array of adjacency lists*/
	G->adjListArr = (adjlist*) malloc(n * sizeof(adjlist));
	if (!G->adjListArr) {
		printf("Unable to allocate memory for adjacency list array.\n");
		exit(1);
	}

	for (int i = 0; i < n; i++) {
		G->adjListArr[i].head = NULL;
		G->adjListArr[i].num_members = 0;
	}
	return G;
}

/*Destroys the graph*/
void destroyGraph(graph *graph) {
	if (graph) {
		if (graph->adjListArr) {
			/*Free up the nodes*/
			for (int v = 0; v < graph->num_vertices; v++) {
				adjlist_node *adjListPtr = graph->adjListArr[v].head;
				while (adjListPtr) {
					adjlist_node *tmp = adjListPtr;
					adjListPtr = adjListPtr->next;
					free(tmp);
				}
			}
			/*Free the adjacency list array*/
			free(graph->adjListArr);
		}
		/*Free the graph*/
		free(graph);
	}
}

/* Adds an edge to a graph*/
void addEdge(graph *graph, int src, int dest) {
	/* Add an edge from src to dst in the adjacency list*/
	adjlist_node *newNode = createNode(dest);
	newNode->next = graph->adjListArr[src].head;
	graph->adjListArr[src].head = newNode;
	graph->adjListArr[src].num_members++;

	if (graph->type == UNDIRECTED) {
		/* Add an edge from dest to src also*/
		newNode = createNode(src);
		newNode->next = graph->adjListArr[dest].head;
		graph->adjListArr[dest].head = newNode;
		graph->adjListArr[dest].num_members++;
	}
}

/* Function to print the adjacency list of graph*/
void displayGraph(graph *graph) {
	for (int i = 0; i < graph->num_vertices; i++) {
		adjlist_node *adjListPtr = graph->adjListArr[i].head;
		printf("%d: ", i);
		while (adjListPtr) {
			printf("%d->", adjListPtr->vertex);
			adjListPtr = adjListPtr->next;
		}
		printf("NULL\n");
	}
}

/* Return 1 if src is incident of dest */
int isIncident(graph *graph, int src, int dest) {
	if (graph->type == UNDIRECTED)
		if (graph->adjListArr[src].num_members
				> graph->adjListArr[dest].num_members) {
			int aux = src;
			src = dest;
			dest = aux;
		}
	adjlist_node *adjList = graph->adjListArr[src].head;
	while (adjList) {
		if (adjList->vertex == dest)
			return 1;
		adjList = adjList->next;
	}
	return 0;
}
