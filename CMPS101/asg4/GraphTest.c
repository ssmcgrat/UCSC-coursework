/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 4

   This is a test client which tests our Graph ADT in isolation.
*/

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include"List.h"

int main(int argc, char* argv[]){
   int n = 6;
   Graph G = newGraph(n);
   List L = newList();
   printf("Here is our Graph G when first initialized\n");
   printGraph(stdout, G);
   printf("\n\n");



   addArc(G, 1, 2);
   addArc(G, 2, 1);
   addEdge(G, 2, 4);
//   makeNull(G);
   BFS(G, 2);
   freeList(&L);
   freeGraph(&G);





/**
   addArc(G, 1, 2);
   addArc(G, 2, 1);
   addEdge(G, 2, 4);
   addEdge(G, 1, 3);
   addEdge(G, 2, 5);
   addEdge(G, 2, 6);
   addEdge(G, 3, 4);
   addEdge(G, 4, 5);
   addEdge(G, 5, 6);

   printf("Here is G after assigning edges\n");
   printGraph(stdout, G);
   printf("\n\n");

   int order = getOrder(G);
   int size = getSize(G);
   printf("Size of G: %d\nOrder of G: %d\n", size, order);
   int source = getSource(G);
   printf("most recent source node is: %d\n", source);
   printf("Now let's run BFS(G, 1)\n\n");
     
 
   BFS(G, 1);
   printGraph(stdout, G);
   int par1 = getParent(G, 1);
   printf("parent of 1: %d\n", par1);
   printf("Distance from 1 to 6 is: %d\n", getDist(G, 6));
   getPath(L, G, 6);
   printf("Shortest 1-6 path is: ");
   printList(stdout, L);   
   printf("\n\n");

   printf("now let's run BFS(G, 4)\n");
   BFS(G, 4);
   printf("distance from 4 to 6 is: %d\n", getDist(G, 6));
  

   int dist5 = getDist(G, 5);
   printf("distance to node 5 from 1 is: %d\n", dist5);
   int par5 = getParent(G, 5);
   printf("parent of node 5 is: %d\n", par5);
   printf("parent of node 4 is: %d\n", getParent(G, 4));
   source = getSource(G);
   printf("the most recent source node is: %d\n", source);

   getPath(L, G, 5);
   printf("The shortest 1-5 path is: ");
   printList(stdout, L);
   printf("\n");
   clear(L);
   getPath(L, G, 6);
   printf("the shortest 1-6 path is: ");
   printList(stdout, L);
   printf("\n");

   printf("Now we make G null again\n");
   makeNull(G);

   printGraph(stdout, G);  
   printf("Size of G: %d\nOrder of G: %d\n", getOrder(G), getSize(G)); 

   printf("Here is our new G\n");
   addEdge(G, 3, 4);
   addEdge(G, 4, 2);
   printGraph(stdout, G);
   BFS(G, 3);
   clear(L);
   getPath(L, G, 2);
   
   printf("the shortest 3-2 path is: ");
   printList(stdout, L);
   printf("\n");
   clear(L);
   getPath(L, G, 1);
   printf("the shoutest 3-1 path is: ");
   printList(stdout, L);
   printf("\n");


   

*/

   //freeGraph(&G);
   return(0);
}
