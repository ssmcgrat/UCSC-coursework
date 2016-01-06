/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 4

   Graph.c
   Implementation file for Graph ADT
*/

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include"List.h"

// structs --------------------------------------------------------------------------

// private GraphObj type
typedef struct GraphObj{
   int order;   // number of vertices in Graph
   int size;    // number of edges in Graph
   List* nbors; // array of Lists, ith element contains neighbors of vertex i
   char* color; // array of chars, ith element is the color(w, g, b) of vertex i
   int* p;      // array of ints, ith element is the parent of vertex i
   int* d;      // array of ints, ith element is distance from (most recent) source to vertex
                // i
   int source; // number of the last source node used to run BFS algorithm 
} GraphObj;

// Graph type
typedef GraphObj* Graph;

// Constructors-Destructors----------------------------------------------------------

// Graph newGraph(int n): returns a Graph pointing to a newly created GraphObj 
//                        representing a graph having n vertices and no edges.
Graph newGraph(int n){
   Graph G;
   G = malloc(sizeof(GraphObj));
   G->order = n;
   G->size = 0;
   G->source = NIL;
   G->nbors = calloc(n, sizeof(List));
   G->color = calloc(n, sizeof(char));
   G->p     = calloc(n, sizeof(int));
   G->d     = calloc(n, sizeof(int));

   for(int i=1; i<=G->order; i++){
      G->nbors[i]=newList();        //note this gives the 8 bytes mem error
   }
   return(G);
}

// void freeGraph(Graph* pG): frees all dynamic memory associated with the Graph *pG
//                            then sets the handle *pG to NULL.
void freeGraph(Graph* pG){
   if(pG!=NULL && *pG!=NULL){
      for(int i=1; i<=(*pG)->order; i++){
         List pL = (*pG)->nbors[i];
         freeList(&pL);
      }
      free((*pG)->nbors);
      free((*pG)->color);
      free((*pG)->p);
      free((*pG)->d);
      free(*pG);

      *pG = NULL;
   }
}



// Access functions ------------------------------------------------------------------

// int getOrder(Graph G): returns the order field of G
int getOrder(Graph G){
   return(G->order);
}

// int getSize(Graph G): returns the size of the Graph G
int getSize(Graph G){
   return(G->size);
}
// int getSource(Graph G): returns the source vertex most recenty used in function
//                         BFS(), or NIL if BFS() has not yet been called. 
int getSource(Graph G){
   if(G->source==NIL) return NIL;
   int s = G->source;
   return(s);
}
// int getParent(Graph G, int u): returns the parent of the vertex u in the 
//                                Bread-First tree created by BFS(), or NIL if 
//                                either u has no parent or BFS() not yet been called
//                                pre: 1<=u<=getOrder(G)
int getParent(Graph G, int u){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling getParent() on invalid vertex u\n");
      exit(1);
   }

   if(G->source==NIL) return NIL;
   int par = G->p[u];
   return(par);
}

// int getDist(Graph G, int u): returns the distance from the most recent BFS source 
//                              to vertex u, or INF if either u is not reachable from
//                              source or if BFS() has not yet been called.
//                              pre: 1<=u<=getOrder(G) 
int getDist(Graph G, int u){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling getDist() on invalid vertex u\n");
      exit(1);
   }

   int dist = G->d[u];
   return(dist);
}
// void getPath(List L, Graph G, int u): appends to the List L the vertices of a 
//                                       shortest path in G from source to u, or 
//                                       appends to L the value NIL if no such 
//                                       path exists. pre: getSource(G)!=NIL
//                                       pre: 1<=u<=getOrder(G)
void getPath(List L, Graph G, int u){
   if(getSource(G)==NIL){
      printf("Graph Error: calling getPath() before running BFS\n");
      exit(1);
   }
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling getPath() on invalid vertex u\n");
      exit(1);
   }
   if(G->d[u]==INF){
      append(L, INF);
      
   }else{
   
      append(L, u);
      int parent = G->p[u];
      while(parent!=NIL){
         append(L, parent);
         parent = G->p[parent];
      }
      if(parent!=NIL){
         clear(L);
         append(L, INF);
      }

   }

}
// Manipulation procedures ----------------------------------------------------------

// void makeNull(Graph G): deletes all edges of G, restoring it to its original state.
void makeNull(Graph G){
   for(int i=1; i<= G->order;  i++){
      List L = G->nbors[i];
      if(length(L)>0){
         clear(L);
      }
   }
   G->size = 0;
   G->source = NIL;
   for(int i=0; i<=G->order; i++){
      G->color[i] = 'w';
      G->p[i] = NIL;
      G->d[i] = NIL;
   }
   
}
// void addEdge(Graph G, int u, int v): inserts a new edge joining u to v, i.e. u is
//                                      added to the adjacency List of v, and v to the
//                                      adjacency List of u.
//                                      pre: 1<={u,v}<=getOrder(G)
void addEdge(Graph G, int u, int v){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling addEdge() on invalid u value\n");
      exit(1);
   } 
   if(v<1 || v>getOrder(G)){
      printf("Graph Error: calling addEdge() on invalid v value\n");
      exit(1);
   }

   addArc(G, u, v);
   addArc(G, v, u);
   G->size--;
}

// void addArc(Graph G, int u, int v): inserts a new directed edge from u to v, i.e. v
//                                     is added to the adjacency List of u. 
//                                     pre: 1<={u,v}<=getOrder(G)
void addArc(Graph G, int u, int v){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling addEdge() on invalid u value\n");
      exit(1);
   } 
   if(v<1 || v>getOrder(G)){
      printf("Graph Error: calling addEdge() on invalid v value\n");
      exit(1);
   }

   List L = G->nbors[u];
   moveTo(L, 0);
   if(length(L)==0){
      append(L, v);
   }else{
      while(1){
        if(getElement(L) > v){
           insertBefore(L, v);
           break;
        }else{
           moveNext(L);
           if(getIndex(L)==-1){
              append(L, v);
              break;
           }
        }
      }
   }
   G->size++;
}
// void BFS(Graph G, int s): runs the BFS algorithm on the Graph G with source s, 
//                           setting the color, distance, parent, and source fields of
//                           G accordingly. 
void BFS(Graph G, int s){
   
   G->source = s;
    
   for(int i=0; i<=G->order; i++){
      G->color[i] = 'w';
      G->p[i] = NIL;
      G->d[i] = NIL;
   }

   //Initialize nodes in G that are less than s
   for(int i=0; i<s; i++){
      G->color[i] = 'w';
      G->d[i] = INF;
      G->p[i] = NIL;  
   }
   //Initialize nodes in G that are greater than s
   for(int i=(s+1); i<=G->order; i++){
      G->color[i] = 'w';
      G->d[i] = INF;
      G->p[i] = NIL;
   }
  
   

   List Q = newList();
   append(Q, s);
  
   List L = newList();
    
   while(!isEmpty(Q)){
      int u = front(Q);
      deleteFront(Q); 
      L = copyList(G->nbors[u]);
      
      while(!isEmpty(L)){
         int v = front(L);
         deleteFront(L);  
         if(G->color[v] == 'w'){
            G->color[v] = 'g';
            G->d[v] = G->d[u]+1;
            G->p[v] = u;
            append(Q, v);
         } 
         
      }
      G->color[u] = 'b';     
   }
   freeList(&Q);
   
   freeList(&L);
}


// Other operations -----------------------------------------------------------------

// void printGraph(FILE* out, Graph G): prints the adjacency list representation of G
//                                      to the file pointed to by out.
void printGraph(FILE* out, Graph G){
   int order = G->order;
   for(int i =1; i<=order; i++){
      List L = G->nbors[i];
      fprintf(out, "%d: ",i);
      printList(out, L);
      fprintf(out, "\n"); 
   }

}


















 
