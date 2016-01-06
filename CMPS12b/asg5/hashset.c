// Sean McGrath, ssmcgrat@ucsc.edu 
// Sterling Salvaterra, ssalvate@ucsc.edu
// $Id: hashset.c,v 1.8 2014-03-05 19:24:07-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %d, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

void free_hashset (hashset *this) {
   
   free(this);
   
   DEBUGF ('h', "free (%p)\n", this);
}

void double_hashset(hashset *orig){
   hashset *newhashset = malloc (sizeof (struct hashset));
   assert(newhashset != NULL);
   newhashset->load = orig->load;
   newhashset->size = orig->size * 2 + 1;
   size_t sizeof_chains = newhashset->size * sizeof(hashnode *);
   newhashset->chains = malloc(sizeof_chains);
   assert(newhashset->chains != NULL);
   memset(newhashset->chains, 0, sizeof_chains);   

  //new_hashset->chains = NULL; 
   for(unsigned int i=0; i<orig->size; i++){
      hashnode *head = orig->chains[i];
      while(head != NULL){
         hashnode *temp = head;
          
         size_t hashed = strhash (temp->word);
         size_t remainder = hashed % newhashset->size;
         hashnode *newhead = newhashset->chains[remainder];
         newhashset->chains[remainder] = temp;
         head = head ->link;
         temp->link = newhead;
      }
   }
   free(orig->chains); 
   
   memcpy(orig, newhashset, sizeof (struct hashset));
   
   //*orig = *new_hashset;
   //free(new_hashset); 
   
   
    
}

void print_stats(hashset *this){
   printf("%10lu words in the hash set\n", this->load);
   printf("%10lu size of the hash array\n", this->size);
  
   for(int i=1; i<15; i++){
      int count = 0;
      int count2 = 0;
      for(unsigned int j=0; j<this->size; j++){
         hashnode *h = this->chains[j];
         while(h != NULL){
            count ++;
            h = h->link;
            
         }
        if(count==i) count2 ++;
        count = 0; 
      }
      if(count2 > 0){
         printf("%10d chains of size %d\n", count2, i); 
      }
    }
}

void dump_hashset(hashset *this){
   for(unsigned int i=0; i<this->size; i++){
       hashnode *head = this->chains[i];
       
       if(head != NULL){
          size_t hashcode = strhash(head->word);
          printf("array[%10u] = %12lu \"%s\"\n",
                  i, hashcode, head->word);
          head= head->link;
          while(head != NULL){
             size_t hc = strhash(head->word);
             char *space = " ";
             printf("%18s= %12lu \"%s\'\n", space, hc, head->word);
             head = head->link;
          } 
       }
   }
}

void print_size(hashset *this){
   printf("%lu\n", this->size);
}

void print_load(hashset *this){
   printf("load = %lu\n", this->load);
}
void print_word(hashset *this){
   hashnode *h = this->chains[21];
  // while(h != NULL){
      printf("%s\n", h->word);
    //  h = h->link;
  // }
}

void put_hashset (hashset *this, const char *item) {
   
   if(has_hashset (this, item) == true){
      DEBUGF('q', "Already in hashset\n");
      return;
   }
   size_t hashed = strhash (item);
   size_t remainder = hashed % this->size;
   
   hashnode *newnode = malloc (sizeof (struct hashnode)); 
   char *newword = strdup(item);
   newnode->word = newword;
   newnode->link = NULL;
   hashnode *head = this->chains[remainder];
   if(head==NULL){
      this->chains[remainder] = newnode;
      this->load ++;
      return;
   }
   hashnode *temp;
  
   while(head != NULL){
      temp = head;
      head = head->link;
   }
   temp->link = newnode;
   this->load ++;
   
   if(this->load * 2 > this->size){
      double_hashset(this);
   }
   
//print_size(this);
//print_load(this);

remainder = hashed % this->size;
//printf("remainder = %lu\n", remainder);
if(remainder==21) print_word(this);
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
  
return;
}

bool has_hashset (hashset *this, const char *item) {
   size_t hashed = strhash (item);
   size_t hash_index = hashed % this->size;
   hashnode *head = this->chains[hash_index];
   while(head != NULL){
      int cmp = strcmp(item, head->word);
      if(cmp==0) return true;
      head = head->link;
   }
   //STUBPRINTF ("hashset=%p, item=%s\n", this, item);
   return false;
}

