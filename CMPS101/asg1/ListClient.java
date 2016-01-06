// Name: Sean McGrath
// User ID: ssmcgrat
// Assignment: CMPS101 Programming Assignment 1
//
// a test client for the List ADT


import java.lang.*;


public class ListClient{
   public static void main(String[] args){
      List A = new List();
      List B = new List();

      for(int i=1; i<=20; i++){
         A.append(i);
         B.prepend(i);
      }
      System.out.println(A);
      System.out.println(B);   

      for(A.moveTo(0); A.getIndex()>=0; A.moveNext()){
         System.out.print(A.getElement() + " ");
      }
      System.out.println();
      for(B.moveTo(B.length()-1); B.getIndex()>=0; B.movePrev()){
         System.out.print(B.getElement() + " ");
      }
      System.out.println();

      List C = A.copy();
      System.out.println(A.equals(B));
      System.out.println(B.equals(C));
      System.out.println(C.equals(A));

      A.moveTo(5);
      A.insertBefore(-1);
      A.moveTo(15);
      A.insertAfter(-2);
      A.moveTo(10);
      A.delete();
      System.out.println(A);
      A.clear();
      System.out.println(A.length());


   }
}
