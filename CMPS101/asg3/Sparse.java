/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS 101 Programming Assignment 3
*/

import java.lang.*;
import java.io.*;
import java.util.Scanner;

public class Sparse{
   public static void main(String args[]) throws IOException{
      Scanner in = null;
      PrintWriter out = null;
      String inline = null;
      String[] token = null;
      int tl;
      int n = 0;	//size of our two matrices;
      int a = 0;	//nonzero entries in A
      int b = 0;        //nonzero entries in B
      int lineCount;    //keeps track of which line of input file we are on
      
      if( args.length != 2){
         System.err.println("Two arguments must be passed to Sparse.java");
         System.exit(1);
      }

      in = new Scanner(new File(args[0]));
      out = new PrintWriter(new FileWriter(args[1]));

      // Read first line of file
      inline = in.nextLine()+" ";	//add extra space so split works right
      token = inline.split("\\s+");     //split line around white space
      tl = token.length;

      n = Integer.parseInt(token[0]);
      a = Integer.parseInt(token[1]);
      b = Integer.parseInt(token[2]);
     
      Matrix A = new Matrix(n);
      Matrix B = new Matrix(n);

      lineCount = 2;
      int row = 0;
      int col = 0;
      double value = 0; 
      while(in.hasNextLine()){
         inline = in.nextLine()+" ";     //add extra space so split works right
         token = inline.split("\\s+");   //split line around white space 
         if(3<=lineCount && lineCount<=(2+a)){
            row = Integer.parseInt(token[0]);
            col = Integer.parseInt(token[1]);
            value = Double.parseDouble(token[2]);
            A.changeEntry(row,col,value);         
         }
         if(lineCount>(3+a)){
            row = Integer.parseInt(token[0]);
            col = Integer.parseInt(token[1]);
            value = Double.parseDouble(token[2]);
            B.changeEntry(row,col,value);
         }
         lineCount++;

      }
      out.println("A has " + A.getNNZ() + " non-zero entries:");
      out.println(A);
      out.println("B has " + B.getNNZ() + " non-zero entries:");
      out.println(B);
      out.println(); 

      //Scalar multiply A by 1.5
      double scalar = 1.5;
      Matrix C = A.scalarMult(1.5);
      out.println("(1.5)*A =");
      out.println(C);
      out.println();

      //Add A and B
      C = A.add(B);
      out.println("A+B =");
      out.println(C);
      out.println();   
       
      //Add A to itself
      C = A.add(A);
      out.println("A+A =");
      out.println(C);
      out.println();

      //B-A
      C = B.sub(A);
      out.println("B-A =");
      out.println(C);
      out.println();

      //A-A
      C = A.sub(A);
      out.println("A-A =");
      out.println(C);

      //Transpose A
      C = A.transpose();
      out.println("Transpose(A) =");
      out.println(C);
      out.println();

      //A*B
      C = A.mult(B);
      out.println("A*B =");
      out.println(C);
      out.println();

      //B*B
      C = B.mult(B);
      out.println("B*B =");
      out.println(C);
      
      in.close();
      out.close();

   }
}
