// Sean McGrath, ssmcgrat@ucsc.edu, Sterling Salvaterra, ssalvate@ucsc.edu
// $Id: bigint.c,v 1.13 2014-01-24 18:33:47-08 - - $

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"
#include "debug.h"

#define MIN_CAPACITY 16

bool sign_flip = false;

struct bigint {
   size_t capacity;
   size_t size;
   bool negative;
   char *digits;
};

static void trim_zeros (bigint *this) {
   while (this->size > 0) {
      size_t digitpos = this->size - 1;
      if (this->digits[digitpos] != 0) break;
      --this->size;
   }
}

bigint *new_bigint (size_t capacity) {
   bigint *this = malloc (sizeof (bigint));
   assert (this != NULL);
   this->capacity = capacity;
   this->size = 0;
   this->negative = false;
   this->digits = calloc (this->capacity, sizeof (char));
   assert (this->digits != NULL);
   DEBUGS ('b', show_bigint (this));
   return this;
}


bigint *new_string_bigint (char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   bigint *this = new_bigint (length > MIN_CAPACITY
                            ? length : MIN_CAPACITY);
   char *strdigit = &string[length - 1];
   if (*string == '_') {
      this->negative = true;
      ++string;
   }
   char *thisdigit = this->digits;
   while (strdigit >= string) {
      assert (isdigit (*strdigit));
      *thisdigit++ = *strdigit-- - '0';
   }
   this->size = thisdigit - this->digits;
   trim_zeros (this);
   DEBUGS ('b', show_bigint (this));
   return this;
}

static bigint *do_add (bigint *this, bigint *that) {
   bigint *result = new_bigint (this->capacity+1);
   //if(this->negative == that->negative){
     // result->negative = this->negative;
   //}
   int digit = 0;
   int carry = 0;
   for(unsigned int i=0; i<that->size; i++){
      digit = this->digits[i] + that->digits[i] + carry;
      result->digits[i] = digit % 10;
      carry = digit/10;
      result->size ++;
   }  
    
   //if one bigint is longer than the other
   if(this->size > that->size) {
      for(unsigned int i=that->size; i<this->size; i++){
         digit = this->digits[i] + carry;
         result->digits[i] = digit % 10;
         carry = digit/10;
         result->size ++;
      }
   }
   
   if(carry>0) {
      result->size ++; 
      
      result->digits[result->size-1] = carry;
      
   }
   return result;

//DEBUGS ('b', show_bigint (this));
   //DEBUGS ('b', show_bigint (that));
   //STUB (return NULL);
}

static bigint *do_sub (bigint *this, bigint *that) {
   bigint *result = new_bigint (this->capacity);
   result->size = this->size;
   int digit = 0;
   int borrow = 0;
   for(unsigned int i=0; i<that->size; i++){
      digit = this->digits[i] - that->digits[i] - borrow + 10;
      result->digits[i] = digit % 10;
      borrow = 1 - digit/10;
   }  
   if(this->size > that->size){
      for(unsigned int i=that->size; i<this->size; i++){
         digit = this->digits[i] - borrow + 10;
         result->digits[i] = digit % 10;
         borrow = 1 - digit/10;
      }
      trim_zeros(result);
   } 
   

   return result;
  // DEBUGS ('b', show_bigint (this));
  // DEBUGS ('b', show_bigint (that));
  // STUB (return NULL);
}
void free_bigint (bigint *this) {
   free (this->digits);
   free (this);
}

void print_bigint (bigint *this, FILE *file) {
   if(this->negative) fprintf(file, "-");
   for(char *byte = &this->digits[this->size-1];
       byte >= this->digits; --byte) {
      fprintf(file, "%d", *byte);
   }
   fprintf(file, "\n");  
 //DEBUGS ('b', show_bigint (this));
}

//compares the size of two bigints
int compare_bigint (bigint *this, bigint *that){
   int cmp = 0;
   int thissz = this->size;
   int thatsz = that->size;
   if(thissz>thatsz) cmp = 1;
   else if(thissz<thatsz) cmp = -1;
   else{
      for(int i=this->size-1; i>=0; i--){
         if(this->digits[i]>that->digits[i]){
            cmp = 1;
            break;
         }if(this->digits[i] < that->digits[i]){
             cmp = -1;
             break;
         }
       }
   } 

   return cmp;  
    
}

bigint *add_bigint (bigint *this, bigint *that) {
   int cmp = compare_bigint(this, that);
   bigint *sum;
      
   if(cmp < 0){
      if(this->negative == that->negative){
         sum = do_add(that, this);
      }else{
          sum = do_sub(that, this);
          if(that->negative) sign_flip = true;
      }
   }else if(this->negative == that->negative){
            sum = do_add(this, that);
         }else{
             sum = do_sub(this, that);
             if(this->negative) sign_flip = true;
         }
   if(this->negative == that->negative){
      sum->negative = this->negative;
   }else if(sign_flip) sum->negative = !sum->negative; 
   
    
   sign_flip = false;
   


//DEBUGS ('b', show_bigint (this));
   //DEBUGS ('b', show_bigint (that));
   //STUB (return NULL);
   return sum;
}

bigint *sub_bigint (bigint *this, bigint *that) {
   int cmp = compare_bigint(this, that);
   bigint *dif ;
   if(this->negative == that->negative){
      if(cmp >= 0) dif = do_sub(this, that);
      else{
         dif = do_sub(that, this);
         sign_flip = true;
      }
   }else{
      if(cmp>=0){
         dif = do_add(this,that);
         if(this->negative) sign_flip = true;
      }else{
         dif= do_add(that, this);
         if(this->negative) sign_flip = true;
      }
   }
   if(this->negative == that->negative){
      dif->negative=this->negative;
   }else{
      if(sign_flip) dif->negative = !dif->negative;
   }
   sign_flip = false;
   return dif;
   //DEBUGS ('b', show_bigint (this));
   //DEBUGS ('b', show_bigint (that));
   //STUB (return NULL);
   //return NULL;
}



bigint *mul_bigint (bigint *this, bigint *that) {
   bigint *product = new_bigint(this->capacity);
   if(this->size==0 || that->size==0){
      product->size = 1;
      product->digits[0] = 0;
      return product;
   }
   product->size = this->size + that->size;
   int digit = 0;
   
   for(unsigned int i=0; i<this->size; i++){
      int carry = 0;
      for(unsigned int j=0; j<that->size; j++){
         digit = product->digits[i+j] +
              (that->digits[j] * this->digits[i]) + carry;
         product->digits[i+j]= digit % 10 ;
         product->size ++;
         carry = digit/10;         
      }
      product->digits[i+that->size]=carry;
   }
   if(product->size > 1) trim_zeros(product);
   if(this->negative==that->negative){
      product->negative = false;
   }else product->negative = true;
   
   
//   DEBUGS ('b', show_bigint (this));
//   DEBUGS ('b', show_bigint (that));
//   STUB (return NULL);
   return product;
}

void show_bigint (bigint *this) {
   fprintf (stderr, "bigint@%p->{%lu,%lu,%c,%p->", this,
            this->capacity, this->size, this->negative ? '-' : '+',
            this->digits);
   for (char *byte = &this->digits[this->size - 1];
        byte >= this->digits; --byte) {
      fprintf (stderr, "%d", *byte);
   }
   fprintf (stderr, "}\n");
}

