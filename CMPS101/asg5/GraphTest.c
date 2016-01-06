/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 4
*/

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include"List.h"

int main(int argc, char* argv[]){
   int order = 8;	//the number of nodes in our Graph G
   Graph G = newGraph(order);
   List S = newList();

   //insert numbers into List S
   for(int i=1; i<=8; i++){
      append(S, i);
   }
   
   printf("Here is our Graph G when first initialized\n");
   printGraph(stdout, G);
   printf("Here is our List S\n");
   printList(stdout, S);
   printf("\n\n");


   //calling addArc() to assign edges to our Graph G
   addArc(G, 1, 2);
   addArc(G, 2, 3);
   addArc(G, 3, 4);
   addArc(G, 4, 3);
   addArc(G, 4, 8);
   addArc(G, 8, 8);
   addArc(G, 7, 8);
   addArc(G, 3, 7);
   addArc(G, 7, 6);
   addArc(G, 6, 7);
   addArc(G, 2, 6);
   addArc(G, 5, 6);
   addArc(G, 5, 1);
   addArc(G, 2, 5); 

   printf("Here is the adjacency list representation of G after adding our directed edges\n");
   printGraph(stdout, G);
   printf("\n");
   
   //Test that transpose() works properly
   Graph T = transpose(G);
   printf("Here is the transpose of G:\n");
   printGraph(stdout, T);

   printf("The order of G is: %d\n", getOrder(G));
   printf("The size of G is: %d\n", getSize(G));
   printf("The parent of node 2 is: %d\n", getParent(G, 2));
   printf("The discover time of node 2 is: %d\n", getDiscover(G, 2));
   printf("The finish tim of node 2 is: %d\n\n\n", getFinish(G, 2));

   DFS(G,S);

   printf("Here is our List S after running DFS():\n");
   printList(stdout, S);
   printf("\n");   
   //Test that discover time, finish time, and parent fields correct after call to DFS()
   for(int i = 1; i<=order; i++){
      printf("Node %d: discovery time = %d ; finish time = %d ; parent = %d\n", 
               i, getDiscover(G, i), getFinish(G, i), getParent(G, i));
   }

   //Test that copyGraph() works properly
   Graph C = copyGraph(G);
   printf("Here is the copy of G:\n");
   printGraph(stdout, C);
   for(int i = 1; i<=order; i++){
      printf("Node %d: discovery time = %d ; finish time = %d ; parent = %d\n", 
               i, getDiscover(C, i), getFinish(C, i), getParent(C, i));
   }

   freeList(&S);
   freeGraph(&G);
   freeGraph(&T);
   freeGraph(&C);
   return(0);
}
