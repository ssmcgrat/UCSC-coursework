/**
   Name: Sean McGrath
   ID: ssmcgrat
   Assignment: CMPS 101 Programming Assingment 3
*/

class Matrix{

   private class Entry{
      // Fields
      int column;
      double value;
      
      // returns true if two Entries are equal
      public boolean equals(Object x){
         Entry E = (Entry) x;
         boolean flag = true;
         if(this.column != E.column) flag = false;
         if(this.value != E.value) flag = false;
         return flag;
      }

      // prints out an Entry
      public String toString(){
         String s = "(" + this.column + ", " + this.value + ")";
         return s;
      } 
   }


   //Fields
   private int NNZ;
   private int size;
   private List[] A;

   // Constructor
   //Matrix(int n): Makes a new n x n zero Matrix. pre: n>=1
   Matrix(int n){
      A = new List[n+1];
      NNZ = 0;
      size = n*n;

      for(int i=1; i<A.length; i++){
         A[i] = new List();
      }
   }


   // Access functions
   //int getSize(): Returns n, the number of rows and columns of this Matrix
   int getSize(){
      return this.size;
      
   }


   //int getNNZ() : Returns the number of non-zero entries in this Matrix
   int getNNZ(){
      return this.NNZ;
   }
   //public boolean equals(Object x) : overrides Object's equals() method
   public boolean equals(Object x){
      Matrix X = (Matrix) x;
      boolean flag = true;
      if(this.getSize() != X.getSize()) return false;
      for(int i=1; i<this.A.length; i++){
         List LT = this.A[i];
         List LX = X.A[i];
         flag = LT.equals(LX);
         if(!flag) return false;
      }
      return flag;
   }


   // Manipulation procedures
   //void makeZero() : sets this Matrix to the zero state
   void makeZero(){
      for(int i=1; i<this.A.length; i++){
         List L = this.A[i];
         L.clear();
      }
      this.NNZ = 0;
   }

   //Matrix copy() : returns a new Matrix having the same entries as this Matrix
   Matrix copy(){
      int n = this.A.length-1;
      Matrix newM = new Matrix(n);
      newM.NNZ = this.NNZ;
      List l;
      for(int i=1; i<this.A.length; i++){
         l = this.A[i];
         if(l.length()>0){
            l.moveTo(0);
            int k = 1;
            while(k<=l.length()){
               Entry e = (Entry) l.getElement();
               double d = e.value;
               int r = e.column;
               newM.changeEntry(i,r,d);
               l.moveNext();
               k++;
            }
         }
      }
     
      return newM; 
   }
   //void changeEntry(int i, int j, double x) // changes ith row, jth column of this 
					      // Matrix to x. Pre: 1<=i<=getSize(),
                                            // 1<=j<=getSize()
   void changeEntry(int i, int j, double x){
      Entry E = new Entry();
      E.value = x;
      E.column = j;
      boolean replaced = false;

      List list = this.A[i];
      if(list.length()==0){
         list.append(E);
         this.NNZ++;
      }else{
         list.moveTo(0);
         int k = 1; 
         Entry old = (Entry) list.getElement();
         while(k<= list.length() && old.column <= E.column){
            list.moveNext();
            if(list.getIndex()!=-1){
               old = (Entry) list.getElement();
               if(old.column==E.column){
                  old.value = x;
                  replaced = true;
                  break;
               }
            }
            k++;
         }

         if(list.getIndex()==-1){
            list.append(E);
            this.NNZ++;
         } else if(!replaced){
            list.insertBefore(E);
            this.NNZ++;
         }
      }     
   }


   //Matrix scalarMult(double x) : returns a new Matrix that is the scalar product of this 
   //                              Matrix with x
   Matrix scalarMult(double x){
      Matrix newM = this.copy();
      List l;
      for(int i=1; i<newM.A.length; i++){
         l = newM.A[i];
         if(l.length()>0){
            l.moveTo(0);
            int k =1;
            while(k<=l.length()){
               Entry e = (Entry) l.getElement();
               double value = e.value;
               e.value = value * x;
               l.moveNext();
               k++;
            }
         }
      }
      return newM;
   }


   //Matrix add(Matrix M) : retruns a new Matrix that is the sum of this Matrix with M
   //                       pre: getSize()==M.getSize()
   Matrix add(Matrix M){   
      int size = M.A.length-1;      
      Matrix sum = new Matrix(size);
      List thislist;
      List Mlist;
      Entry newEntry = new Entry();

      if(this.equals(M)){
         sum = this.scalarMult(2);
         return sum;
      }

      for(int i=1; i<M.A.length; i++){
         thislist = this.A[i];
         Mlist    = M.A[i];

         if(thislist.length()>0 && Mlist.length()>0){
            thislist.moveTo(0);
            Mlist.moveTo(0);
            int TL = 1;
            int ML = 1;
            while(TL<=thislist.length() && ML<=Mlist.length()){
               Entry entryT = (Entry) thislist.getElement();
               Entry entryM = (Entry) Mlist.getElement();
               int colT = entryT.column;
               int colM = entryM.column;
               if(colT<colM){
                  sum.changeEntry(i, colT, entryT.value);
                  thislist.moveNext();
                  TL++;    
               }
               if(colT==colM){
                  double v = entryT.value + entryM.value;
                  if(v != 0.0){
                     sum.changeEntry(i, colT, v);
                  }
                  thislist.moveNext();
                  Mlist.moveNext();
                  TL++;
                  ML++;
               }
               if(colT>colM){
                  sum.changeEntry(i, colM, entryM.value);
                  Mlist.moveNext();
                  ML++;
               }
               
            }
            if(TL<=thislist.length()){
               while(TL<=thislist.length()){
                  Entry e = (Entry) thislist.getElement();
                  sum.changeEntry(i, e.column, e.value);
                  thislist.moveNext();
                  TL++;
               }
            }
            if(ML<=Mlist.length()){
               while(ML<=Mlist.length()){
                  Entry e = (Entry) Mlist.getElement();
                  sum.changeEntry(i, e.column, e.value);
                  Mlist.moveNext();
                  ML++;
               }
            }
            
         } else if(thislist.length()>0){
            thislist.moveTo(0);
            int y = 1;
            while(y<=thislist.length()){
               Entry e = (Entry) thislist.getElement();
               sum.changeEntry(i, e.column, e.value);
               thislist.moveNext();
               y++;
            }

         } else if(Mlist.length()>0){
            Mlist.moveTo(0);
            int z = 1;
            while(z<=Mlist.length()){
               Entry e = (Entry) Mlist.getElement();
               sum.changeEntry(i, e.column, e.value);
                Mlist.moveNext();
                z++;
            }

         }

      }      
      return sum;
   }


   //Matrix sub(Matrix M) : returns a new Matrix that is the difference of this Matrix 
   //                       with M. pre: getSize()==M.getSize()
   Matrix sub(Matrix M){
      Matrix dif = new Matrix(M.A.length-1);
      List thislist;
      List Mlist;
      Entry newEntry = new Entry();

      if(this.equals(M)){
         dif.makeZero();
         return dif;
      }

      for(int i=1; i<M.A.length; i++){
         thislist = this.A[i];
         Mlist    = M.A[i];

         if(thislist.length()>0 && Mlist.length()>0){
            thislist.moveTo(0);
            Mlist.moveTo(0);
            int TL = 1;
            int ML = 1;
            while(TL<=thislist.length() && ML<=Mlist.length()){
               Entry entryT = (Entry) thislist.getElement();
               Entry entryM = (Entry) Mlist.getElement();
               int colT = entryT.column;
               int colM = entryM.column;
               if(colT<colM){
                  dif.changeEntry(i, colT, entryT.value);
                  thislist.moveNext();
                  TL++;    
               }
               if(colT==colM){
                  double v = entryT.value - entryM.value;
                  if(v != 0.0){
                     dif.changeEntry(i, colT, v);
                  }
                  thislist.moveNext();
                  Mlist.moveNext();
                  TL++;
                  ML++;
               }
               if(colT>colM){
                  dif.changeEntry(i, colM, -(entryM.value));
                  Mlist.moveNext();
                  ML++;
               }
               
            }
            if(TL<=thislist.length()){
               while(TL<=thislist.length()){
                  Entry e = (Entry) thislist.getElement();
                  dif.changeEntry(i, e.column, e.value);
                  thislist.moveNext();
                  TL++;
               }
            }
            if(ML<=Mlist.length()){
               while(ML<=Mlist.length()){
                  Entry e = (Entry) Mlist.getElement();
                  dif.changeEntry(i, e.column, -(e.value));
                  Mlist.moveNext();
                  ML++;
               }
            }
            
         } else if(thislist.length()>0){
            thislist.moveTo(0);
            int y = 1;
            while(y<=thislist.length()){
               Entry e = (Entry) thislist.getElement();
               dif.changeEntry(i, e.column, e.value);
               thislist.moveNext();
               y++;
            }

         } else if(Mlist.length()>0){
            Mlist.moveTo(0);
            int z = 1;
            while(z<=Mlist.length()){
               Entry e = (Entry) Mlist.getElement();
               dif.changeEntry(i, e.column, -(e.value));
                Mlist.moveNext();
                z++;
            }

         }

      } 
      return dif;
   }
   // Matrix transpose() : returns a new Matrix that is the transpose of this Matrix
   Matrix transpose(){
      Matrix T = new Matrix(this.A.length-1);
      List L;
      for(int i=1; i<this.A.length; i++){
         L = this.A[i];
         if(L.length()>0){
            L.moveTo(0);
            int j = 1;
            while(j<=L.length()){
               Entry e = (Entry) L.getElement();
               T.changeEntry(e.column, i, e.value);
               L.moveNext();
               j++;
            } 
            
         }
      }
      return T;
   }

   // Returns the dot product of two vectors (Lists)
   private static double dot(List P, List Q){
      double dotprod=0;
      
      if(P.length()>0 && Q.length()>0){
         P.moveTo(0);
         Q.moveTo(0);
         int PL = 1;
         int QL = 1;
         while(PL<=P.length() && QL<=Q.length()){
            Entry entryP = (Entry) P.getElement();
            Entry entryQ = (Entry) Q.getElement();
            if(entryP.column < entryQ.column){
               P.moveNext();
               PL++;
            }
            if(entryP.column==entryQ.column){
               dotprod += (entryP.value * entryQ.value);

               P.moveNext();
               Q.moveNext();
               PL++;
               QL++;
            }
            if(entryP.column > entryQ.column){
               Q.moveNext();
               QL++;
            }
         }
      } 
     
      return dotprod;
   }



   // Matrix mult(Matrix M) : returns a new Matrix that is the product of this Matrix 
   //                         with M. pre: getSize()==M.getSize()
   Matrix mult(Matrix M){
      Matrix product = new Matrix(M.A.length-1);
      Matrix transpose = M.transpose();
      double dotp;
      for(int i=1; i<this.A.length; i++){
         dotp =0;
         List L1 = this.A[i];
         if(L1.length()>0){
            for(int j=1; j<transpose.A.length; j++){
               List L2 = transpose.A[j];
               if(L2.length()>0){
                  dotp = dot(L1, L2);
               }
               if(dotp != 0.0){
                  product.changeEntry(i, j, dotp);
               }
            }  
         }
      }
      
      return product;
   }


   //Other functions --------------------------------------------------------------------

   //public String toString(): overrides Object's toString() method
   public String toString(){
      String s = "";
      List l;
      for(int row=1; row<this.A.length; row++){
         l = this.A[row];
         if(l.length()>0){
            s += row + ": " + this.A[row] + "\n";
         }
      }
      return s;
   } 
}
