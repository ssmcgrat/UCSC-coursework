/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS 101 Programming Assignment 3
*/

import java.lang.*;
import java.io.*;
import java.util.Scanner;

class ListTest{
   public static void main(String[] args) throws IOException{

     List[] array = new List[3];
     for(int i=0; i<array.length; i++){
        array[i] = new List();
     }

     List List0 = array[0];
     double foo = 4.0;
     List0.append(foo);
     int bar = List0.length();
     System.out.println(List0 + "List0 has length" + bar);
      
      // Create our Object List.
      List L = new List();

      // print L has length zero
      int length = L.length();
      System.out.println("L has length " + length);

      // add two booleans to L, then print L   
      boolean b = true;
      L.append(b);
      b = false;
      L.prepend(b);
      String S = L.toString();
      System.out.println(S);

      // insert a double before the last element in L, then a double after the first element
      // print L
      double d = 23.4;
      L.moveTo(1);
      L.insertBefore(d);
      d = 56.7;
      L.moveTo(0);
      L.insertAfter(d);
      System.out.println(L);

      // show our move functions work
      L.moveTo(0);
      System.out.print("Cursor is at index: " + L.getIndex() + " ");
      L.moveNext();
      System.out.print(L.getIndex() + " ");
      L.movePrev();
      System.out.print(L.getIndex() + "\n");

      // show our delete functions work
      L.deleteBack();
      L.deleteFront();
      System.out.println(L);
      // add a String to the middle of L, then delete it.
      L.moveTo(0);
      String s = "ThisIsAString";
      L.insertAfter(s);
      System.out.println(L);
      L.moveNext();
      L.delete();
      System.out.println(L);
  
      //test getelement()
      L.moveTo(1);
      Object o = L.getElement();
      System.out.println("The object is " +o);

      // test of copy and delete
      List L2 = L.copy();
      System.out.println("Here is a copy of L: " + L2);
      L.clear();
      length = L.length();
      System.out.println("L now has length " + length);

   }
}
