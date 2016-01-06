/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 4

   FindPath.c
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
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
   }

   // Reading from input file
   lineCount = 1;
   int check = 0;
   int firstLoop = 0;
   
   while(fgets(line, MAX_LEN, in) != NULL){
      int a;
      int b; 
      //get the order of the Graph, n, and set it.
      if(lineCount==1){
         n = atoi(line);
         G = newGraph(n);
            
      }
      
      // Read till the first dummy line, add an edge between each pair of vertices
      if(strcmp(line, dummyLine)!=0 && check==0 && lineCount!=1){
         token = strtok(line, " ");
         a = atoi(token);
         token = strtok(NULL, " ");
         b = atoi(token);
         
          
         addEdge(G, a, b);
          //fprintf(out, "%d\n", lineCount); 
      }
      if(strcmp(line, dummyLine)==0 && firstLoop==0){
         check = 1;
         printGraph(out, G);
         fprintf(out, "\n");
         firstLoop = 1;
         
      }
      // Read after the first dummy line
      if(strcmp(line, dummyLine)!=0 && check==1){
         
         token = strtok(line, " ");
         a = atoi(token);
         token = strtok(NULL, " ");
         b = atoi(token);
         
         BFS(G, a);
         
         if(getDist(G,b)!=INF){
            fprintf(out, "The distance from %d to %d is: %d\n", a, b, getDist(G, b) );
            fprintf(out, "The shortest %d-%d path is: ", a, b);
            getPath(L, G, b);
            printList(out, L);
            fprintf(out, "\n\n");
         } else{
            fprintf(out, "The distance from %d to %d is infinity\n", a, b);
            fprintf(out, "No %d-%d path exists\n\n", a, b);
         }
         clear(L); 
      }
      lineCount++;
          
   }
   freeList(&L); 
   freeGraph(&G); 
   return(0);
}
