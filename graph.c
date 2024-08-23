/*
 * Our graph implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge)
{
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList* head)
{
  while (head != NULL)
  {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex* vertex)
{
  if (vertex == NULL)
  {
    printf("NULL");
  }
  else
  {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph* graph)
{
  if (graph == NULL)
  {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++)
  {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/

Edge* newEdge(int fromVertex, int toVertex, int weight)
{
    Edge* e = (Edge*)calloc(1, sizeof(Edge));
    if (e == NULL) {
        return NULL;
    }
    e->fromVertex = fromVertex;
    e->toVertex = toVertex;
    e->weight = weight;
    return e;
}

EdgeList* newEdgeList(Edge* edge, EdgeList* next)
{
    EdgeList* el = (EdgeList*)calloc(1, sizeof(EdgeList));
    if (el == NULL) {
        return NULL;
    }
    el->edge = edge;
    el->next = next;
    return el;
}

Vertex* newVertex(int id, void* value, EdgeList* adjList)
{
    Vertex* v = (Vertex*)calloc(1, sizeof(Vertex));
    if (v == NULL) {
        return NULL;
    }
    v->id = id;
    v->value = value;
    v->adjList = adjList;
    return v;
}
Graph* newGraph(int numVertices) {
    if (numVertices <= 0) {
        return NULL;
    }
    Graph* g = (Graph*)calloc(1, sizeof(Graph));
    if (g == NULL) {
        return NULL;
    }
    g->numVertices = numVertices;
    g->vertices = (Vertex**)calloc(numVertices, sizeof(Vertex*));
    if (g->vertices == NULL) {
        free(g);
        return NULL;
    }
    for (int i = 0; i < numVertices; i++) {
        g->vertices[i] = newVertex(i, NULL, NULL);
        if (g->vertices[i] == NULL) {
            // Free already allocated vertices and the graph itself
            for (int j = 0; j < i; j++) {
                free(g->vertices[j]);
            }
            free(g->vertices);
            free(g);
            return NULL;
        }
    }
    return g;
}
void deleteEdgeList(EdgeList* head)
{
    EdgeList* current = head;
    while (current != NULL) {
        EdgeList* next = current->next;
        free(current->edge);
        free(current);
        current = next;
    }
}
void deleteVertex(Vertex* vertex)
{
    if (vertex != NULL) {
        deleteEdgeList(vertex->adjList);
        free(vertex->value);
        free(vertex);
    }
}
void deleteGraph(Graph* graph) {
    if (!graph) return;
    if (graph->vertices) {
        for (int i = 0; i < graph->numVertices; i++) {
            if (graph->vertices[i]) {
                deleteVertex(graph->vertices[i]);
            }
        }
        free(graph->vertices);
    }
    free(graph);
}

