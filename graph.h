#ifndef _GRAPH_H_
#define _GRAPH_H_

typedef enum {
	UNDIRECTED = 0, DIRECTED
} graph_type_e;

/* Adjacency list node*/
typedef struct adjlist_node{
	int vertex; /*Index of the vertex*/
	adjlist_node *next; /*Pointer to the next node*/
} adjlist_node;

/* Adjacency list */
typedef struct {
	int num_members; /*number of members in the list (for future use)*/
	adjlist_node *head; /*head of the adjacency linked list*/
} adjlist;

/* Graph structure. A graph is an array of adjacency lists.
 Size of array will be number of vertices in graph*/
typedef struct {
	graph_type_e type; /*Directed or undirected graph */
	int num_vertices; /*Number of vertices*/
	adjlist *adjListArr; /*Adjacency lists' array*/
} graph;

adjlist_node* createNode(int v);

/* Function to create a graph with n vertices; Creates both directed and undirected graphs*/
graph* createGraph(int n, graph_type_e type);

/*Destroys the graph*/
void destroyGraph(graph *G);

/* Adds an edge to a graph*/
void addEdge(graph *G, int src, int dest);

/* Function to print the adjacency list of graph*/
void displayGraph(graph *G);

/* Return 1 if src is incident of dest */
int isIncident(graph *graph, int src, int dest);

#endif
