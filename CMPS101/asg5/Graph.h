//-----------------------------------------------------------------------------------
// Name: Sean McGrath
// ID: ssmcgrat
// Assignment: CMPS101 Programming Assignment 5 
//
// Graph.h
// Header file for Graph ADT
//----------------------------------------------------------------------------------

#ifndef _GRAPH_H_INCLUDE_
#define _GRAPH_H_INCLUDE_
#define INF -2
#define NIL 0
#define UNDEF -1
#include "List.h"

// Exported type-------------------------------------------------------------------
typedef struct GraphObj* Graph; 

// Constructors-Deconstructors------------------------------------------------------

Graph newGraph(int n);

void freeGraph(Graph* pG);

// Access functions----------------------------------------------------------------

int getOrder(Graph G);

int getSize(Graph G);

int getParent(Graph G, int u); // Pre: 1<=u<=n=getOrder(G)*/ 

int getDiscover(Graph G, int u); // Pre: 1<=u<=n=getOrder(G)*/ 

int getFinish(Graph G, int u); // Pre: 1<=u<=n=getOrder(G)*/ 

// Manipulation procedures----------------------------------------------------------

void makeNull(Graph G);

void addEdge(Graph G, int u, int v); // pre: 1<=u<=n, 1<=v<=n /*

void addArc(Graph G, int u, int v);  // pre: 1<=u<=n, 1<=v<=n /*

void DFS(Graph G, List S);

// Other operations ----------------------------------------------------------------

Graph transpose(Graph G);

Graph copyGraph(Graph G);

void printGraph(FILE* out, Graph G);

#endif
