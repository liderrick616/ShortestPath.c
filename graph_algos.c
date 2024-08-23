/*
 * Graph algorithms.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include <limits.h>
#include <stdlib.h>
#include "graph.h"
#include "minheap.h"
#include <stdio.h>

/*
 * A structure to keep record of the current running algorithm.
 */
typedef struct records
{
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
  int* distances;                     //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/
/*
 * Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex){
    MinHeap* heap = newHeap(graph->numVertices);
    for (int i = 0; i <= graph->numVertices-1; i++) {
        int priority;
        if (i == startVertex) {
            priority = 0;  // Priority 0 for the start vertex
        } else {
            priority = INT_MAX;  // INT_MAX for all other vertices
        }
        insert(heap, priority, i); // perform insert to assign priority
    }
    return heap;
}
/*
 * Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records* initRecords(Graph* graph, int startVertex){
    Records* records = (Records*)calloc(1, sizeof(Records));
    if (records == NULL) return NULL;
    records->numVertices = graph->numVertices;
    records->heap = initHeap(graph, startVertex);
    if (records->heap == NULL) {
        free(records);
        return NULL;
    }
    records->finished = (bool*)calloc(graph->numVertices, sizeof(bool));
    records->predecessors = (int*)calloc(graph->numVertices, sizeof(int));
    records->tree = (Edge*)calloc((graph->numEdges),sizeof(Edge));
    records->numTreeEdges = 0;
    for (int i = 0; i < graph->numVertices; i++) { // no -1
        records->predecessors[i] = -1;  //no predecessor
    }
    records->predecessors[startVertex] = -1;//
    records->distances = (int*) malloc(graph->numVertices * sizeof(int));
    for (int i = 0; i < graph->numVertices; i++) { // no -1
        records->distances[i] = (i == startVertex) ? 0 : INT_MAX;
        // initialize distance
    }
    if (records->finished == NULL || records->predecessors == NULL) {
        free(records->heap->indexMap);
        free(records->heap);
        free(records->finished);
        free(records->predecessors);
        free(records);
        return NULL;
    }
    return records;
}
/*
 * Returns true iff 'heap' is NULL or is empty.
 */
bool isEmpty(MinHeap* heap){
    return heap == NULL || heap->size == 0;
}
/*
 * Add a new edge to records at index ind.
 */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight){
    records->tree[ind].fromVertex = fromVertex;
    records->tree[ind].toVertex = toVertex;
    records->tree[ind].weight = weight;
}

/*
 * Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */

EdgeList* makePath(Edge* distTree, int vertex, int startVertex) {
    EdgeList* path = NULL;
    int currentVertex = vertex;
    // Traverse vertex to startVertex, makePath in reverse order
    while (currentVertex != startVertex) {
        int pred = distTree[currentVertex].fromVertex;
        if (pred == -1) {
            while (path != NULL) {
                EdgeList* temp = path;
                path = path->next;
                free(temp->edge); // free edge
                free(temp);
            }
            return NULL;
        }
        // find direct weight using accumulative weights from dijkstra
        int directWeight = distTree[currentVertex].weight - (pred != startVertex ? distTree[pred].weight : 0);
        // create edge
        Edge* newEdge = (Edge*)malloc(sizeof(Edge));
        if (newEdge == NULL) {
            while (path != NULL) {
                EdgeList* temp = path;
                path = path->next;
                free(temp->edge); // free edge
                free(temp);
            }
            return NULL;
        }
        newEdge->fromVertex = currentVertex;
        newEdge->toVertex = pred;
        newEdge->weight = directWeight;
        EdgeList* newNode = (EdgeList*)malloc(sizeof(EdgeList));
        if (newNode == NULL) {
            free(newEdge);
            while (path != NULL) {
                EdgeList* temp = path;
                path = path->next;
                free(temp->edge);//free edge
                free(temp);
            }
            return NULL;
        }
        // prepend edge to front of linked list
        newNode->edge = newEdge;
        newNode->next = path;
        path = newNode;
        currentVertex = pred;
    }
    // reverse list to match output
    EdgeList* reversedPath = NULL;
    while (path != NULL) {
        EdgeList* nextNode = path->next;
        // Prepend current node in to reverse list
        path->next = reversedPath;
        reversedPath = path;
        path = nextNode;
    }
    return reversedPath;
}
bool isInHeap(MinHeap* heap, int vertexId) {
    if (heap == NULL || vertexId < 0 || vertexId >= heap->capacity) {
        return false;  // check error
    }
    // return true if valid, else (-1) return false
    return heap->indexMap[vertexId] != -1;
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
void printRecords(Records* records)
{
    if (records == NULL)
        return;
    int numVertices = records->numVertices;
    printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

    printf("The PQ is:\n");
    printHeap(records->heap);

    printf("The finished array is:\n");
    for (int i = 0; i < numVertices; i++)
        printf("\t%d: %d\n", i, records->finished[i]);

    printf("The predecessors array is:\n");
    for (int i = 0; i < numVertices; i++)
        printf("\t%d: %d\n", i, records->predecessors[i]);

    printf("The TREE edges are:\n");
    for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

    printf("... done.\n");
}
// use this helper function to reduce redundancy in code
void freeRecords(Records* records) {
    if (!records) return;
    deleteHeap(records->heap);
    free(records->finished);
    free(records->predecessors);
    free(records->distances);
    free(records->tree);
    free(records);
}

Edge* getMSTprim(Graph* graph, int startVertex) {
    if (graph == NULL || startVertex < 0 || startVertex >= graph->numVertices) {
        return NULL;
    }
    Records *records = initRecords(graph, startVertex);
    if (records == NULL) {
        return NULL;
    }
    while (!isEmpty(records->heap)) {
        HeapNode minNode = extractMin(records->heap);
        int u = minNode.id;
        records->finished[u] = true; // mark vertex finished
        if (records->predecessors[u] != -1 && u != startVertex) {
            addTreeEdge(records, records->numTreeEdges, u, records->predecessors[u], minNode.priority);
            records->numTreeEdges++; // number of tree edges ++
        }
        for (EdgeList *adj_list = graph->vertices[u]->adjList; adj_list != NULL; adj_list = adj_list->next) {
            Edge *edge = adj_list->edge;
            int v = edge->toVertex;
            if (!records->finished[v] && edge->weight < records->distances[v]) {
                decreasePriority(records->heap, v, edge->weight);
                records->distances[v] = edge->weight;  // update min edge weight
                records->predecessors[v] = u;  // Update predecessor
            }
        }
    }
    Edge* tree = records->tree;
    records->tree = NULL;
    freeRecords(records);
    return tree;
}

Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex) {
    if (startVertex < 0 || startVertex >= graph->numVertices) {
        return NULL;
    }
    Records* records = initRecords(graph, startVertex);
    if (records == NULL) {
        return NULL;
    }
    // create and allocate disTree
    // used later in getshortestpath
    Edge* distTree = (Edge*)calloc(graph->numVertices, sizeof(Edge));
    if (distTree == NULL) {
        freeRecords(records);
        return NULL;
    }
    while (!isEmpty(records->heap)) {
        HeapNode minNode = extractMin(records->heap);
        int u = minNode.id;
        records->finished[u] = true;
        for (EdgeList* adjList = graph->vertices[u]->adjList; adjList != NULL; adjList = adjList->next) {
            Edge* edge = adjList->edge;
            int v = edge->toVertex;
            int weight = edge->weight;
            int distance = records->distances[u] + weight;
            if (!records->finished[v] && distance < records->distances[v]) {
                decreasePriority(records->heap, v, distance);
                records->distances[v] = distance;
                records->predecessors[v] = u;
                distTree[v].fromVertex = u;
                distTree[v].toVertex = v;
                distTree[v].weight = records->distances[v];
            }
        }
    }
    // to match output given sample_output.txt
    distTree[startVertex].fromVertex = startVertex;
    distTree[startVertex].toVertex = startVertex;
    distTree[startVertex].weight = 0;
    freeRecords(records);
    return distTree;
}
EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex) {
    if (!distTree) return NULL;
    distTree[startVertex].fromVertex = -1; // No predecessor
    distTree[startVertex].toVertex = startVertex;
    distTree[startVertex].weight = 0;
    EdgeList** paths = (EdgeList**)calloc(numVertices, sizeof(EdgeList*));
    if (!paths) {
        return NULL;
    }
    for (int i = 0; i < numVertices; i++) {
        if (i == startVertex) {
            paths[i] = NULL;
        } else if (distTree[i].fromVertex != -1) {
            paths[i] = makePath(distTree, i, startVertex);
            if (!paths[i] && distTree[i].fromVertex != -1) {
                for (int j = 0; j < i; j++) {
                    EdgeList* current = paths[j];
                    while (current) {
                        EdgeList* next = current->next;
                        free(current->edge);
                        free(current);
                        current = next;
                    }
                }
                free(paths);
                return NULL;
            }
        }
    }
    return paths;
}
/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/

