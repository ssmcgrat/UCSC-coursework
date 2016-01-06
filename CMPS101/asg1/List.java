/**
   Name: Sean McGrath
   User ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 1
*/

class List{

   private class Node{
      // Fields
      int data;
      Node next;
      Node prev;
      // Constructor
      Node(int data){
         this.data = data;
         next = null;
         prev = null;
      }	
      //toString: overides the object's toString method
      public String toString(){
         return String.valueOf(data); 
      }
   }

   // Fields
   private Node front;
   private Node back;
   private int length;
   private Node cursor;
   private int index = -1;
 

   // Constructor
   List(){
      front = back = cursor = null;
      length = 0;
   }

   // Access functions ------------------------------------------------------------- 

   // length():  returns the length of this list
   int length(){ return length;}

   /**
     int getIndex(): returns the index of the cursor element in this list, or
     returns -1 if the cursor element is undefined.
   */
   int getIndex(){
      return this.index;
   }



   // int front():   returns front element in this list. Pre: length()>0
   int front(){
      if(this.length()==0){
         throw new RuntimeException("List Error: front() called on empty List");
      }
      return front.data;
   }

   // int back(): Returns back element in this List. Pre: length()>0
   int back(){
      if(this.length()==0){
         throw new RuntimeException("List Error: back() called on empty List");
      }
      return back.data;
   }

   /**
      int getElement():   Returns cursor element in this list.
                          Pre: length()>0, getIndex()>=0
   */
   int getElement(){
      if(this.length()==0){
         throw new RuntimeException("List Error:  getElement() called on empty List");
      }
      if(this.getIndex()>=0){
         return cursor.data;
      }else {
         throw new RuntimeException("List Error: getElement()called on undefined cursor");
      }
   }



   /**
      boolean equals(List L): Returns true if this List and L are the same integer 
                              sequence. The cursor is ignored in both lists.
   */
   boolean equals(List L){
      boolean flag = true;
      Node N = this.front;
      Node M = L.front;

      if( this.length==L.length ){
         while(flag && N!=null){
            flag = (N.data==M.data);
            N = N.next;
            M = M.next;
         }
         return flag;
      }else{
         return false;
      }
   }


   // Manipulation procedures -------------------------------------------------------

   // void clear(): Re-sets this list to the empty state
   void clear(){
      this.front = null;
      this.back  = null;
      this.cursor = null;
      this.length = 0;
   } 

   /**
      void moveTo(int i):  If 0<=i<=length()-1, moves the cursor to the element 
                           at index i, otherwise the cursor becomes undefined.
   */
   void moveTo(int i){
      if(0<=i && i<= (this.length()-1) ){
         Node curr = this.front;
         for(int j=0; j<=i; j++){
            if(j==i){
               this.cursor = curr;
               this.index = j;
            }else {
               curr = curr.next;
            }
         }
      }else{
         this.cursor = null;
         this.index = -1;
      }
   }


   /**
      void movePrev():  If 0<getIndex()<=length()-1, moves teh cursor one step towared the
                        front of the list. If getIndex()==0, cursor becomes undefined.
                        If getIndex()==-1, cursor remains undefined. This operation is 
                        equivalent to moveTo(getIndex()-1).   
   */
   void movePrev(){
      if(0<this.getIndex() && this.getIndex()<= (this.length()-1) ){
         cursor = cursor.prev;
         index--;
      }
      else if(this.getIndex()==0){
         cursor = null;
         index = -1;
      }
      else if(this.getIndex()<=0){
         cursor = null;
      }
   }




   /**
      void moveNext():  If 0<=getIndex()<lenth()-1, moves the cursor one step toward the back
                        of the list. If getIndex()==lenth()-1, cursor becomes undefined. If 
                        getIndex()==-1, cursor remains undefined. This operation is 
                        equivalent to moveTo(getIndex()+1).
   */
   void moveNext(){
      if(0<=this.getIndex() && this.getIndex()< (this.length()-1) ){
         cursor = cursor.next;
         index++;
      }
      else if(this.getIndex()==(this.length()-1) ){
         cursor = null;
         index = -1;
      }
      else if(this.getIndex()==-1){
         cursor = null;
      }
   }

   // void prepend(int data):  Inserts new element before front element in this List.
   void prepend(int data){ 
      Node node = new Node(data);
      if(this.length()==0) {
         front = back = node;
      } else {
         node.next = front;
         front.prev = node;
         front = node;
      }  
      length++;
      if(cursor==front){
         index++;
      }
   }


   // void append(int data):   Inserts new element after back element in this List.
   void append(int data){
      Node node = new Node(data);
      if(this.length()==0) {
         front = back = node;
      } else {
         back.next = node;
         node.prev = back;
         back = node;
      }
      length++;
   }

   /**
      void insertBefore(int data):   Inserts new element before cursor element in this List.
                                     Pre: length()>0, getIndex()>=0
   */
   void insertBefore(int data){
      if(this.length()==0){
         throw new RuntimeException("List Error: insertBefore() called on empty List");
      }
      Node node = new Node(data);
      if(cursor==front){
         node.next = front;
         front.prev = node;
         front = node;
         
      }else {
         Node prevNode = cursor.prev;
         prevNode.next = node;
         node.prev = prevNode;
         cursor.prev = node;
         node.next = cursor;
      }
      length++;

      if(this.cursor!=null){
         index++;
      }
   }


   /**
      void insertAfter(int data):    Inserts new element after cursor element in this List.
                                     Pre: length()>0, getIndex()>=0
   */
   void insertAfter(int data){
      if(this.length()==0){
         throw new RuntimeException("List Error: insertAfter() called on empty List");
      }
      Node node = new Node(data);
      if(cursor==back){
         cursor.next = node;
         node.prev = cursor;
         back = node;
      } else {
         Node nextNode = cursor.next;
         nextNode.prev = node;
         node.next = nextNode;
         cursor.next = node;
         node.prev = cursor;
      }
      length++;
      
   }

   // void deleteFront():   Deletes the front element in this List. Pre: length()>0
   void deleteFront(){
      if(this.length()==0){
         throw new RuntimeException("Queue Error: deleteFront() called on empty List");
      }
      if(this.length>1) {
         front = front.next;
         front.prev = null;
      } else { front = back = null; } 
      length --;
      if(this.cursor!=null){
         index--;
      }
   }

   // void deleteBack():    Deletes the back element in this List. Pre: length()>0
   void deleteBack(){
      if(this.length()==0){
         throw new RuntimeException("Queue Error: deleteBack() called on empty List");
      }
      if(this.length>1) {
         back = back.prev;
         back.next = null;
      }
      else { front = back = null; }
      length--; 
   }

   // void delete():   Deletes cursor element in this List. Cursor is undefined after 
   //                  operation. Pre: length()>0, getIndex()>=0
   void delete(){
      Node prev = cursor.prev;
      Node next = cursor.next;
      prev.next = next;
      next.prev = prev;
      cursor = null;
      index = -1;
      length--;
   }




   // Other methods ----------------------------------------------------------------------- 

   /**
      public String toString():   Overides Object's toString method. Creates a string 
                                  consisting of a space separated sequence of integers
                                  with front on the left and back on the right. The 
                                  cursor is ignored.
   */
   public String toString(){
      String str = "";
      for(Node N=front; N!=null; N=N.next){
         str += N.toString() + " ";
      }
      return str;
   }

   /**
      List copy():   Returns a new list representing the same integer sequence as this list.
                     The cursor in the new list is undefined, regardless of the state of 
                     the cursor in this List. This List is unchanged. 
   */
   List copy(){
      List L = new List();
      Node N = this.front;

      while(N!=null){
         L.append(N.data);
         N = N.next;
      }
      return L;
   }







}
