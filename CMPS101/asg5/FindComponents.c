/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS 101 Programming Assignment 5

   FindComponents.c
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Graph.h"
#include"List.h"

#define MAX_LEN 160

int main(int argc, char* argv[]){

   FILE *in, *out;
   int n;
   char line[MAX_LEN];
   char array[MAX_LEN][80];
   char* token = NULL;
   int lineCount;
   Graph G;
   char* dummyLine = "0 0\n";
   List L = newList();

   for(int i=0; i<MAX_LEN; i++){
      strcpy(array[i], "");
   }

   // check command line for correct number of arguments
   if(argc != 3){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

   // open files for reading and writing
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");
   if(in==NULL){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }
   if(out==NULL){
      printf("Unable to open file %s for reading\n", argv[2]);
      exit(1);
   }

   // Reading from input file
   lineCount = 1;
   //int check = 0;
   //int firstLoop = 0;
   
   while(fgets(line, MAX_LEN, in) != NULL){
      int a;
      int b;
      // get the order of the Graph, n, and set it.
      if(lineCount==1){
         n = atoi(line);
         G = newGraph(n);
         for(int i=1; i<=n; i++) append(L, i);
      }

      // Read untill dummy line, add a directed edge between each pair of vertices
      if(strcmp(line, dummyLine)!=0  && lineCount>1){
         
         token = strtok(line, " ");
         a = atoi(token);
         
         token = strtok(NULL, " ");
         
         b = atoi(token);
         addArc(G, a, b);
      }

      if(strcmp(line, dummyLine)==0 ){
         fprintf(out, "Adjacency list representation of G:\n");  
         printGraph(out, G);
         fprintf(out, "\n");
         
      }
      
      lineCount++;      
   }

    DFS(G, L);
     
   // calculate transpose of G
   Graph T = transpose(G);
   // Run DFS() on transpose of G
   DFS(T, L);
   
  // Finding connected components
  
//   for(int i=1; i<=getOrder(G); i++){
//      printf("Node %d: discovery time = %d ; finish time = %d ; parent = %d \n",
//                i, getDiscover(T, i), getFinish(T, i), getParent(T, i));
//   } 

   clear(L);

   //Count number of connected components
   int connComp = 0;
   for(int i=1; i<=getOrder(G); i++){
      if(getParent(T, i)==0) connComp++;
   }

   // Print number of connected components
   fprintf(out, "G contains %d strongly connected components\n", connComp);
     
   // Find connected components
   int compCount = 0;
   for(int i=1; i<=getOrder(G); i++){
      if(getParent(T, i)==0){
         compCount++;
         clear(L);
         append(L, i);
         int parent = i;
         int noParent = 0;
         while(noParent==0){
            noParent = 1;
            for(int j=1; j<=getOrder(G); j++){
               if(getParent(T, j)==parent){
                  append(L, j);
                  parent = j;
                  noParent = 0;
               }
            }       
         }
         fprintf(out, "Component %d: ", compCount); 
         printList(out, L);
         fprintf(out, "\n");
      }
   }

   fclose(in);
   fclose(out);
   freeGraph(&G);
   freeGraph(&T);
   freeList(&L);
   return(0);
}


