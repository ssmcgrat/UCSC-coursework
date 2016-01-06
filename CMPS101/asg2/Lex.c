/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS 101 Programming Assingment 2

   Lex.c
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "List.h"

#define MAX_LEN 160

int main(int argc, char* argv[]){

   int count=0;
   FILE *in, *out;
   char line[MAX_LEN];
   char array[MAX_LEN][80];
   char* token = NULL;

   for(int i=0; i<MAX_LEN; i++){
      strcpy(array[i], "");
   }

   List L = newList();
  
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

   int i = 0;

   // count lines of input file 
   while(fgets(line, MAX_LEN, in) != NULL){
      count++;
      token = strtok(line, " \n");
      while(token!=NULL){
         strcat(array[i], token);
         token = strtok(NULL, " \n");
         i++;
      }  
   }

   // Insertion Sort
   for(int i=0; i<count; i++){
      moveTo(L, 0);
      if(length(L)==0){
         append(L, 0);
      }else{
         while(1){
             if(strcmp(array[getElement(L)], array[i]) > 0){
                insertBefore(L, i);
                break;
             }else{
                moveNext(L);
                if(getIndex(L)==-1){
                   append(L, i);
                   break;
                }
             }
         }
      }
   }

   for(moveTo(L, 0); getIndex(L) != -1; moveNext(L)){
      fprintf(out, "%s\n", array[getElement(L)]);
   }


   // close files
   fclose(in);
   fclose(out);
   freeList(&L);
   return(0);
}





