/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 2   

   List.c
   Implementation file for List ADT
*/

#include<stdio.h>
#include<stdlib.h>
#include "List.h"

// structs -------------------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj{
   int data;
   struct NodeObj* next;
   struct NodeObj* prev;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private ListObj type
typedef struct ListObj{
   Node front;
   Node back;
   int length; 
   Node cursor;
   int index;
} ListObj;


// Constructors-Destructors --------------------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes next, prev, and data fields.
// Private.
Node newNode(int data){
   Node N = malloc(sizeof(NodeObj));
   N->data = data;
   N->next = NULL;
   N->prev = NULL;
   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
// Private.
void freeNode(Node* pN){
   if(pN!=NULL && *pN!=NULL){
      free(*pN);
      *pN = NULL;
   }
}

// newList()
// Returns reference to new empty List object.
List newList(void){
   List L;
   L = malloc(sizeof(ListObj));
   L->front = L->back = L->cursor = NULL;
   L->index = -1;
   L->length = 0;
   return(L);
}

// freeList()
// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL){
   if(pL!=NULL && *pL!=NULL){
      while(length(*pL) > 0 ){
         deleteFront(*pL);
      }
      free(*pL);
      
      *pL = NULL;
   }
}


// Access functions ------------------------------------------------------------------------

// length(List L)
// Returns the length of L
int length(List L){
   if(L==NULL){
      printf("List Error: calling length() on NULL List reference\n");
      exit(1);
   }
   return L->length;
}



// getIndex(List L)
// Returns the index of the cursor element in L, or returns -1 if the cursor element is 
// undefinded.
int getIndex(List L){
   if(L==NULL){
      printf("List Error: calling getIndex() on NULL List reference\n");
      exit(1); 
   }
   return L->index;
}


// front(List L)
// Returns front element in L. Pre: length()>0
int front(List L){
   if(L==NULL){
      printf("List Error: calling front() on NULL list reference\n");
      exit(1);
   }
   if(L->length==0){ 
      printf("List Error: front() called on emptpy List");
      exit(1);
   }
   return L->front->data;
}

// back(List L)
// Return back element in L. Pre: length()>0
int back(List L){
   if(L==NULL){
      printf("List Error: calling back() on NULL list reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: back() called on empty List");
      exit(1);
   }
   return L->back->data;
}

// getElement(List L)
// Returns cursor element in L. Pre: length()>0, getIndex()>=0
int getElement(List L){
   if(L==NULL){
      printf("List Error: calling getElement() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: getElement() called on empty List\n");
      exit(1);
   }
   if(getIndex(L)<0){
      printf("List Error: getElement() called on undefined cursor");
      exit(1);
   }
   return L->cursor->data;
}


// equals(List A, List B)
// Returns (1) if A and B are the same integer sequence, returns (0) otherwise. The cursor
// is ignored in both Lists.
int equals(List A, List B){
   int flag = 1;
   Node N = NULL;
   Node M = NULL;

   if(A==NULL){
      printf("List Error: calling equals() on NULL List reference, List A\n");
      exit(1);
   }
   if(B==NULL){
      printf("List Error: calling equals() on NULL List reference, List B\n");
      exit(1);
   }
   N = A->front;
   M = B->front;
   if(A->length != B->length) {return 0; }
   while(flag && N!=NULL){
      flag = (N->data==M->data);
      N = N->next;
      M = M->next;
   }
   return flag;
}

// Manipulation procedures ------------------------------------------------------------------

// clear(List L)
// Re-sets L to the empty state
void clear(List L){
   if(L==NULL){
      printf("List Error: calling clear() on NULL List reference\n");
      exit(1); 
   }
   List* old = &L;
   L->front = NULL;
   L->back = NULL;
   L->cursor = NULL;
   L->length = 0;
   freeList(old);
}


// moveTo(List L, int i)
// If 0<=i<=length()-1, moves the cursor to the element at index i, otherwise cursor becomes 
// undefined
void moveTo(List L, int i){
   Node curr = NULL;

   if(L==NULL){
      printf("List Error: calling moveTo() on NULL List reference\n");
      exit(1);
   }
   if(0<=i && i<=(L->length-1) ){
      curr = L->front;
      for(int j=0; j<=i; j++){
         if(j==i){
            L->cursor = curr;
            L->index = j;
         }else {
            curr = curr->next;
         }
      }      
   }else {
      L->cursor = NULL;
      L->index = -1;
   }
}

// movePrev(List L)
// If 0<getIndex()<=length-1, moves the cursor one step towared the front of the list. If
// getIndex==0, cursor becomes undefined. If getIndex()==-1, cursor remains undefined. This
// operation is equivalent to moveTo(getIndex()-1).
void movePrev(List L){
   if(L==NULL){
      printf("List Error: calling movePrev() on NULL List reference\n");
      exit(1);
   }
   if(0<getIndex(L) && getIndex(L)<= (L->length-1) ){
      L->cursor = L->cursor->prev;
      L->index--;
   }
   else if(getIndex(L)==0){
      L->cursor = NULL;
      L->index = -1;
   }
   else if(getIndex(L)<=0){
      L->cursor = NULL;
   }
}


// moveNext(List L)
// If 0<=getIndex()<length-1, moves the cursor one step toward the back of the List. If 
// getIndex()==length-1, cursor becomes undefined. If getIndex()==-1, cursor remains undefined.
// This operation is equivalent to moveTo(getIndex()+1).
void moveNext(List L){
   if(L==NULL){
      printf("List Error: calling moveNext() on NULL List reference\n");
      exit(1);
   }
   if(0<=getIndex(L) && getIndex(L)< (L->length - 1) ){
      L->cursor = L->cursor->next;
      L->index++;
   }
   else if(getIndex(L)==(L->length-1) ){
      L->cursor = NULL;
      L->index = -1;
   }   
   else if(getIndex(L)==-1){
      L->cursor = NULL;
   }
}

// prepend(List L, int data)
// Places new data element at the front of L
void prepend(List L, int data){
   Node N = newNode(data);   

   if(L==NULL){
      printf("List Error: calling prepend() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0) { L->front = L->back = N; }
   else { N->next = L->front; L->front->prev = N; L->front = N; }
   L->length++;
   if(L->cursor==L->front){ L->index++; }
   
}


// append(List L, int data)
// Places new data element at the end of L
void append(List L, int data){
   Node N = newNode(data);

   if(L==NULL){
      printf("List Error: calling append() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){ L->front = L->back = N; }
   else{ L->back->next = N; N->prev = L->back; L->back = N; }
   L->length++;   
}


// insertBefore(List L, int data)
// Inserts new element before cursor element in L. Pre: length>0, getIndex()>=0
void insertBefore(List L, int data){
   Node N = NULL;
   Node P = NULL;
   
   if(L==NULL){
      printf("List Error: calling insertBefore() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: insertBefore() called on an empty List\n");
      exit(1);
   }
   int index = L->index;
   if(index<0){
      printf("List Error: insertBefore() called on an undefined List cursor\n");
      exit(1); 
   }
   N = newNode(data);

   if(L->cursor==L->front){
      N->next = L->front;
      L->front->prev = N;
      L->front = N;
   }else {
      P = L->cursor->prev;
      P->next = N;
      N->prev = P;
      L->cursor->prev = N;
      N->next = L->cursor;
   }
   L->length++;

   if(L->cursor!=NULL) { L->index++; }
}

// insertAfter(List L, int data)
// Inserts new data element after cursor element in L. Pre: length()>0, getIndex()>=0
void insertAfter(List L, int data){
   Node newN = NULL;
   Node N = NULL;

   if(L==NULL){
      printf("List Error: calling insertAfter() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: calling insertAfter() on an empty List\n");
      exit(1);
   }
   int index = getIndex(L);
   if(index<0){
      printf("List Error: calling insertAfter() on an undefined List cursor\n");
      exit(1);
   }

   newN = newNode(data);
   if(L->cursor==L->back){
      L->cursor->next = newN;
      newN->prev = L->cursor;
      L->back = newN;
   }
   else{
      N = L->cursor->next;
      N->prev = newN;
      newN->next = N;
      L->cursor->next = newN;
      newN->prev = L->cursor;
   }
   L->length++;

} 

// deleteFront(List L)
// Deletes data element at the front of L. Pre: length>0
void deleteFront(List L){
   Node N = NULL;

   if(L==NULL){
      printf("List Error: calling deleteFront() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: Calling deleteFront on an empty List\n");
      exit(1);
   }
   N = L->front;
   if(L->length>1) {
      L->front = L->front->next;
      L->front->prev = NULL;
   }
   else { L->front = L->back = NULL; }
   L->length--;
   if(L->cursor!=NULL) { L->index--; }
   freeNode(&N);
   
}

// deleteBack(List L)
// Deletes data element at the back of L. Pre: length>0
void deleteBack(List L){
   Node N = NULL;

   if(L==NULL){
      printf("List Error: calling deleteBack() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: calling deleteFront() on an empty List\n");
      exit(1);
   }
   N = L->back;
   if(L->length>1) {
      L->back = L->back->prev;
      L->back->next = NULL;
   }
   else { L->back = L->front = NULL; }
   L->length--;
   freeNode(&N);
} 

// delete(List L)
// Delests cursor element in this List. Cursor is undefined after operation. 
// Pre: length()>0, getIndex()>=0
void delete(List L){
   Node C = NULL;
   Node P = NULL;
   Node N = NULL;

   if(L==NULL){
      printf("List Error: calling delete() on NULL List reference\n");
      exit(1);
   }
   if(L->length==0){
      printf("List Error: calling delete() on an empty List\n");
      exit(1);
   }
   int index = getIndex(L);
   if(index<0){
      printf("List Error: callinge delete() on an undefined List Cursor\n");
      exit(1);
   }
   C = L->cursor;
   P = L->cursor->prev;
   N = L->cursor->next;
   P->next = N;
   N->prev = P;
   L->cursor = NULL;
   L->index = -1;
   L->length--;
   freeNode(&C);
}


// Other operations -------------------------------------------------------------------------

// printList(FILE* out, List L)
// Prints L to the file pointed to by out, formatted as a space-separated string. 
void printList(FILE* out, List L){
    Node N = NULL;

	if(L==NULL){
      printf("List Error: calling printList() on NULL List reference\n");
      exit(1);
    }
 
   for(N=L->front; N!=NULL; N=N->next){
      fprintf(out, "%d ", N->data );
   }
}

// copyList(List L)
// Returns a new List representing the same integer sequence as L. The cursor in the new list
// is undefined, regardless of the state of the cursor in L. L is unchanged.
List copyList(List L){
   if(L==NULL){
      printf("List Error: calling copyList() on undefined List reference\n");
      exit(1);
   }
   List newL = newList();
   Node N = newL->front;

   while(N!=NULL){
      append(L, N->data);
      N = N->next;
   }   
   return L;
}
