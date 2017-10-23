#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include "stack.h"
#include "queue.h"
#include "graph.h"

void metrics(graph *G, int size, double* betweenness, double* closeness);
void eigenvectorMetric(graph *G, int size, double *eigenvector);
void executionTime(clock_t t, double *clockTime);
int randomInt(int min_num, int max_num);
void appMetrics(graph *G, int size, double *approBetweenness,
		double *approCloseness, double sampleSize);
void parallelTime(double timer, double *clockTime);
void formatData();

int main() {
	FILE *fp, *ftime;
	fp = fopen("Network.txt","r");
	int n;
	fscanf(fp, "%d", &n);
	graph *undirGraph = createGraph(n, UNDIRECTED);
	int v1, v2;
	while (fscanf(fp, "%d %d", &v1, &v2) == 2) {
		addEdge(undirGraph, v1, v2);
	}
	fclose(fp);

	double *betweenness = new double[n];
	double *closeness = new double[n];
	double *degree = new double[n];
	double *eigenvector = new double[n];

	for (int i = 0; i < n; i++) {
		betweenness[i] = 0;
		closeness[i] = 0;
		degree[i] = 0;
		eigenvector[i] = 0;
	}

	ftime = fopen("computingTime.txt","a");

	double clockTime[3];
	double timer = omp_get_wtime();
	metrics(undirGraph, n, betweenness, closeness);
	timer = omp_get_wtime() - timer;
	parallelTime(timer, clockTime);
	fprintf(ftime, "metrics() took %.0fh %.0fmin %.0fs to execute \n",
			clockTime[0], clockTime[1], clockTime[2]);

	clock_t t = clock();
	for (int i = 0; i < n; i++)
		degree[i] = undirGraph->adjListArr[i].num_members;
	executionTime(t, clockTime);
	fprintf(ftime, "degree() took %.0fh %.0fmin %.0fs to execute \n",
			clockTime[0], clockTime[1], clockTime[2]);

	t = clock();
	eigenvectorMetric(undirGraph, n, eigenvector);
	executionTime(t, clockTime);
	fprintf(ftime, "eigenvector() took %.0fh %.0fmin %.0fs to execute \n",
			clockTime[0], clockTime[1], clockTime[2]);

	destroyGraph(undirGraph);
	fclose(ftime);

	fp = fopen("NetworkResults.txt","w");
	fprintf(fp,"vertex, betweenness, closeness, degree, eigenvector\n");
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%d, %f, %f, %.0f, %f\n", i, betweenness[i], closeness[i], degree[i], eigenvector[i]);
	}
	
	fclose(fp);

	return 0;

}

void metrics(graph *G, int size, double *betweenness, double *closeness) {
#pragma omp parallel for
	for (int s = 0; s < size; s++) {
		queue Q;
		stack S;
		int *dist = new int[size];
		double *sigma = new double[size];
		QueueInit(&Q, size);
		StackInit(&S, size);
		graph *pred = createGraph(size, UNDIRECTED);
		for (int t = 0; t < size; t++)
			dist[t] = INT_MAX;
		for (int i = 0; i < size; i++)
			sigma[i] = 0;
		dist[s] = 0;
		sigma[s] = 1;
		QueueEnter(&Q, s);
		while (!QueueIsEmpty(&Q)) {
			int v = QueueDelete(&Q);
			StackPush(&S, v);
			adjlist_node *adjListPtr = G->adjListArr[v].head;
			while (adjListPtr) {
				int w = adjListPtr->vertex;
				adjListPtr = adjListPtr->next;			
				if (dist[w] == INT_MAX) {
					dist[w] = dist[v] + 1;
					QueueEnter(&Q, w);
				}
				if (dist[w] == dist[v] + 1) {
					sigma[w] += sigma[v];
					addEdge(pred, v, w);
				}
			}
		}
		for (int i = 0; i < size; i++)
			if (dist[i] > 0)
				closeness[s] += dist[i];
		closeness[s] = (size - 1.0) / closeness[s];
		double *delta = new double[size];
		for (int i = 0; i < size; i++)
			delta[i] = 0;
		while (!StackIsEmpty(&S)) {
			int w = StackPop(&S);
			if (w != s) {
				adjlist_node *adjListPtr = pred->adjListArr[w].head;
				while (adjListPtr) {
					int v = adjListPtr->vertex;
					adjListPtr = adjListPtr->next;
					if (v != s)
						delta[v] += (sigma[v] / sigma[w]) * (1 + delta[w]);
				}
				betweenness[w] += delta[w] / 2.0;
			}
		}
		destroyGraph(pred);
		StackDestroy(&S);
		QueueDestroy(&Q);
	}
}

void eigenvectorMetric(graph *G, int size, double *eigenvector) {
	double sum = size;
	double *aux = new double[size];
	for (int i = 0; i < size; i++)
		aux[i] = 1;
	for (int it = 0; it < 100; it++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++)
				eigenvector[i] += aux[j] * isIncident(G, i, j);
			eigenvector[i] /= sum;
		}
		sum = 0;
		for (int i = 0; i < size; i++) {
			aux[i] = eigenvector[i];
			sum += aux[i];
		}
	}
}

void appMetrics(graph *G, int size, double approBetweenness[],
		double approCloseness[], double sampleSize) {
	queue Q;
	QueueInit(&Q, size);
	stack S;
	StackInit(&S, size);
	int *dist = new int[size], *marked = new int[size];
	for (int t = 0; t < size; t++)
		marked[t] = 0;
	double *sigma = new double[size];
	int s = randomInt(0, size);
	int k = 0;
	while (k < size * sampleSize) {
		while (marked[s] == 1)
			s = randomInt(0, size);
		marked[s] = 1;
		k++;
		graph *pred = createGraph(size, DIRECTED);
		for (int t = 0; t < size; t++)
			dist[t] = INT_MAX;
		for (int i = 0; i < size; i++)
			sigma[i] = 0;
		dist[s] = 0;
		sigma[s] = 1;
		QueueEnter(&Q, s);
		while (!QueueIsEmpty(&Q)) {
			int v = QueueDelete(&Q);
			StackPush(&S, v);
			for (int w = 0; w < size; w++)
				if (isIncident(G, v, w) == 1) {
					if (dist[w] == INT_MAX) {
						dist[w] = dist[v] + 1;
						QueueEnter(&Q, w);
					}
					if (dist[w] == dist[v] + 1) {
						sigma[w] += sigma[v];
						addEdge(pred, v, w);
					}
				};
		}
		for (int i = 0; i < size; i++)
			approCloseness[i] += dist[i] * size;

		double *delta = new double[size];
		for (int i = 0; i < size; i++)
			delta[i] = 0;		
		while (!StackIsEmpty(&S)) {
			int w = StackPop(&S);
			if (w != s) {
				for (int v = 0; v < size; v++)
					if (v != s)
						if (isIncident(pred, v, w) == 1)
							delta[v] += (sigma[v] / sigma[w]) * (1 + delta[w]);
				approBetweenness[w] += delta[w] / 2.0;
			}
		}
		destroyGraph(pred);
	}
	for (int i = 0; i < size; i++)
		approCloseness[i] = 1.0 / (approCloseness[i] / (k * size - k));
	for (int i = 0; i < size; i++)
		approBetweenness[i] = size * approBetweenness[i] / k;
	StackDestroy(&S);
	QueueDestroy(&Q);
}


void executionTime(clock_t t, double clockTime[]) {
	t = clock() - t;
	double s = ((double) t) / CLOCKS_PER_SEC;
	double m = s / 60.0;
	double h = m / 60.0;
	clockTime[0] = floor(h);
	clockTime[1] = floor(m) - clockTime[0] * 60;
	clockTime[2] = floor(s) - clockTime[1] * 60 - clockTime[0] * 3600;
}

void parallelTime(double timer, double clockTime[]) {
	double s = timer;
	double m = s / 60.0;
	double h = m / 60.0;
	clockTime[0] = floor(h);
	clockTime[1] = floor(m) - clockTime[0] * 60;
	clockTime[2] = floor(s) - clockTime[1] * 60 - clockTime[0] * 3600;
}

//generate a random int [min,max)
int randomInt(int min, int max) {
	srand(time(NULL));
	return (rand() % (max - min)) + min;
}
