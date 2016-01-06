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
   int* d;      // array of ints, ith element is discover time of vertex i
   int* f;      // array of ints, ith element is the finish time of vertex i
   int source;  // number of the last source node used to run DFS algorithm 
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
   G->f     = calloc(n, sizeof(int));
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
      free((*pG)->f);
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

// int getParent(Graph G, int u): returns the parent of the vertex u in the 
//                                DFS tree created by DFS(), or NIL if 
//                                either u has no parent or DFS() not yet been called
//                                Pre: 1<=u<=n=getOrder(G)
int getParent(Graph G, int u){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling getParent() on invalid vertex u\n");
      exit(1);
   }

   if(G->source==NIL) return NIL;
   int par = G->p[u];
   return(par);
}

//int getDiscover(Graph G, int u): Returns the discover time associated with vertex u
//                                  Pre: 1<=u<=n=getOrder(G)
int getDiscover(Graph G, int u){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling getDiscover() on invalid vertex u\n");
      exit(1);
   }
   if(G->source==NIL) return UNDEF;
   return(G->d[u]);

}

//int getFinish(Graph G, int u): Returns finish time associated with vertex u
int getFinish(Graph G, int u){
   if(u<1 || u>getOrder(G)){
      printf("Graph Error: calling getFinish() on invalid vertex u\n");
      exit(1);
   }
   if(G->source==NIL) return UNDEF;
   return(G->f[u]);
}

// Manipulation procedures ----------------------------------------------------------

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


//void DFS(Graph G, List S) : Performs DFS algorithm on G. 
//                            Pre: getLength(S)==n
//                                 S contains integers {1, 2, ..., n} n=getOrder(G)

int time;

void Visit(Graph G, int x, List S);

void DFS(Graph G, List S){
   if(length(S)!=G->order){
      printf("Graph Error: calling DFS() on invalid Graph/List pairing\n");
      exit(1);
   }
   G->source = 1;
   time = 0;
   
   for(int i=1; i<=G->order; i++){
      G->color[i] = 'w';
      G->p[i] = NIL;
      
   }
  
   List SC = copyList(S);
   moveTo(SC, 0);
   for(int i=1; i<=G->order; i++){
      int e = getElement(SC);
      
      if(G->color[e]=='w'){
         Visit(G, e, S);    
      }
      moveNext(SC);
   }
 
   freeList(&SC); 
}

//void Visit(Graph G, int x): recursive visit called by DFS()
void Visit(Graph G, int x, List S){
   G->color[x] = 'g';
   time++;
   G->d[x] = time;
  
   moveTo(G->nbors[x], 0);
   while(getIndex(G->nbors[x])!=-1){
      int y = getElement(G->nbors[x]);
      if(G->color[y]=='w'){
         G->p[y] = x;
         
         Visit(G, y, S);
      }
      moveNext(G->nbors[x]);
   }
   G->color[x] = 'b';
   prepend(S, x);
   deleteBack(S);
   time++;
   G->f[x] = time;
   

}



// Other operations -----------------------------------------------------------------

//Graph transpose(Graph G): returns a reference to a new graph object representing the 
//                          transpose of G
Graph transpose(Graph G){
   Graph T = newGraph(G->order);
   for(int i=1; i<=G->order; i++){
      if(length(G->nbors[i])>0){
         moveTo(G->nbors[i], 0);
         while(getIndex(G->nbors[i])!=-1){
            int temp = getElement(G->nbors[i]);
            append(T->nbors[temp],i);
            moveNext(G->nbors[i]);
         }
      }
   }

   return T;
}

//Graph copyGraph(Graph G): returns a reference to a new graph that is a copy of G
Graph copyGraph(Graph G){
   Graph C = newGraph(G->order);

   //copy adj list
   for(int i=1; i<=G->order; i++){
      if(length(G->nbors[i])>0){
         moveTo(G->nbors[i], 0);
         while(getIndex(G->nbors[i])!=-1){
            int temp = getElement(G->nbors[i]);
            append(C->nbors[i],temp);
            moveNext(G->nbors[i]);
         }
      }
      C->color[i] = G->color[i];
      C->p[i] = G->p[i];
      C->d[i] = G->d[i];
      C->f[i] = G->f[i];
   }
   C->size = G->size;

   return C;   
}

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


















 
