// Sean McGrath, ssmcgrat@ucsc.edu 
// Sterling Salvaterra, ssalvate@ucsc.edu
// $Id: spellchk.c,v 1.7 2014-03-05 19:24:07-08 - - $

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

void print_error (const char *object, const char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   Exit_Status = 2;
}

FILE *open_infile (const char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

char* lowerconvert(char *this){
   int length = strlen(this);
   //char *result[length];
   for(int i=0; i<length; i++){
      this[i] = tolower(this[i]);
   }
   this[length] = '\0';
   return this;
}

void spellcheck (const char *filename, hashset *hashset) {
   //FILE *yyin = open_infile(filename);
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   //char *yytext = NULL;
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      bool found = has_hashset(hashset, yytext);
      char *orig = strdup(yytext);
      if(!found){
         char *lowered = lowerconvert(yytext);
         found = has_hashset(hashset, lowered);
         if(!found){
           printf("%s: %2d: %s\n", filename, yylineno, orig);
           Exit_Status = 1;
         }
      } 
      DEBUGF ('k', "line %d, yytext = \"%s\"\n", yylineno, yytext);
      //STUBPRINTF ("%s: %d: %s\n", filename, yylineno, yytext);
      free(orig);
   }
}

void load_dictionary (const char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   char buf[1040];
   FILE *file = open_infile(dictionary_name);
   //FILE *file = fopen(dictionary_name, "r");
   //iterate through d8ctionary_name and put words into hashset.
   int numwords = 0;
   while(fgets(buf, 1040, file) != NULL){
      
      char *newln = strchr(buf, '\n');
      if(newln != NULL) *newln = '\0';
      numwords ++;
      DEBUGF ('u', "get loop %d (%s)\n", numwords, buf);
      put_hashset(hashset, buf);      
       
   } 
      
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
   //STUBPRINTF ("Open dictionary, load it, close it\n");
  
}
/*
void print_stats(hashset *this){
   printf("%10lu words in the hash set\n", this->load);
   printf("%10lu size of the hash array\n", this->size);
} */

int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   //void *tofree = hashset;
   yy_flex_debug = false;
   int xopt_count = 0;
   bool dont_check = false;
   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': xopt_count ++;
                   dont_check = true; 
                  //STUBPRINTF ("-x\n");
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);
   
   if(xopt_count==1) print_stats(hashset); 
   if(xopt_count==2) dump_hashset(hashset);
    // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      if(!dont_check) spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            if(!dont_check) spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            if(!dont_check) spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   
   yylex_destroy ();
   free_hashset(hashset); 
   return Exit_Status;
}

