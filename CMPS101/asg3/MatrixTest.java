/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS 101 Programming Assingment 3
*/

class MatrixTest{
   public static void main(String[] args){
      int i, j, n=10000;
      Matrix A = new Matrix(n);
      Matrix B = new Matrix(n);

      //checking that getNNZ() and getSize() work properly. 
      int NNZ = A.getNNZ();
      int size = A.getSize();

      System.out.println("The size of A is: " + size);
      System.out.println("Number of NNZs in A is: " + NNZ);

      //changing Entries in A
      A.changeEntry(1,1,1);
      A.changeEntry(1,2,2);
      A.changeEntry(1,3,3);
      A.changeEntry(1,6,6);
      A.changeEntry(1,5,5);
      A.changeEntry(3,6,1);
      A.changeEntry(3,2,3);
      A.changeEntry(3,4,4);
      A.changeEntry(3,10,10);
      A.changeEntry(1,3,7.5);

      //changing Entries in B
      B.changeEntry(1,2,2);
      B.changeEntry(1,3,3);
      B.changeEntry(1,4,4);
      B.changeEntry(1,9,9);
      B.changeEntry(2,3,3);
      B.changeEntry(3,1,1);
      B.changeEntry(3,4,4);
      B.changeEntry(7,3,3);
      B.changeEntry(7,4,4);

      //Making sure A.NNZ is incremented correctly
      NNZ = A.getNNZ();
      System.out.println("Number of NNZs in A is: " + NNZ);
      System.out.println("\n\n");

      //Testing Matrix's toString() method
      System.out.println("Matrix A:");
      System.out.println(A);
      System.out.println("Matrix B:");
      System.out.println(B);
      
      //Testing our add() function
      Matrix C = A.add(B);
      System.out.println("Matrix C = A + B:");
      System.out.println(C);

      C = A.add(A);
      System.out.println("A+A =");
      System.out.println(C);


      //Testing our sub() function
      C = A.sub(B);
      System.out.println("Matrix C = A-B:");
      System.out.println(C);

      //a new Matrix D
      Matrix D = new Matrix(100);
      D.changeEntry(1,1,2);
      D.changeEntry(1,3,1);
      D.changeEntry(2,3,3);
      D.changeEntry(3,1,1);
      D.changeEntry(3,2,4);
      D.changeEntry(3,3,5);

      System.out.println("D =");
      System.out.println(D);
      Matrix E = new Matrix(100);

      // Testing transpose()
      E = D.transpose();
      System.out.println("D transpose =");
      System.out.println(E);


      // Testing scalarMult
      Matrix scalarMultA = A.scalarMult(5);
      System.out.println(scalarMultA);



      Matrix P1 = new Matrix(3);
      Matrix P2 = new Matrix(3);

      P1.changeEntry(1,1,1);
      P1.changeEntry(1,2,2);
      P1.changeEntry(2,1,2);
      P1.changeEntry(2,2,1);
      P1.changeEntry(2,3,1);
      P1.changeEntry(3,3,2);

      P2.changeEntry(1,1,3);
      P2.changeEntry(2,2,1);
      P2.changeEntry(2,3,1);
      P2.changeEntry(3,1,1);
      P2.changeEntry(3,2,2);

      //testing mult()
      Matrix product = P1.mult(P2);
      System.out.println("The product of P1 and P2 =");
      System.out.println(product);

      //testing copy() and equals()
      Matrix copyA = A.copy();
      boolean copyAflag = copyA.equals(A);
      System.out.println("copyA is equal to A is: " + copyAflag);
      boolean BequalAflag = A.equals(B);
      System.out.println("A is equal to B is: " + BequalAflag);

      //testing makeZero() and equals()
      A.makeZero();
      A.changeEntry(231,13,45.6);
      System.out.println(A);
      copyAflag = copyA.equals(A);
      System.out.println(copyAflag);


      D = A.add(A);
      System.out.println(D);



   }
}
