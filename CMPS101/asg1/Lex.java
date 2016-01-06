/**
   Name: Sean McGrath
   User ID: ssmcgrat
   Assignment: CMPS101 Programming Assignment 1
*/

import java.lang.*;
import java.io.*;
import java.util.Scanner;

class Lex{
   public static void main(String[] args) throws IOException{
      if( args.length != 2){
         System.err.println("Two arguments must be passed to Lex.java");
         return;
      }
    
      PrintWriter out = null; 
      int lineCount = 0;

      // Count number of lines in file
      try{ 
      lineCount = countLines(args[0]);
      }catch(FileNotFoundException e){
         System.err.println(e);
      }

      // Creates a string array from input file, each element in array is a line from file     
      String[] array = new String[lineCount];
      array = readFile(args[0], lineCount);
      

      // This is our second array for testing purposes
      String[] a = new String[lineCount];

      // Creates our integer list
      List L = InsertionSort(array);
      out = new PrintWriter(new FileWriter(args[1]));

      // Writing lines to output file
      for(int i=0; i<lineCount; i++){
         L.moveTo(i);
         int d = L.getElement();
         String line = array[d];
         out.println(line);
      }
      out.close();
   }

   // Method that sorts a String array and places the sorted array indeces into a List
   // returns the List
   //
   public static List InsertionSort(String[] a){
      List L = new List();
      L.append(0);
      L.moveTo(0);

      int i;
      int j;

      for(i=1; i<a.length; i++){
         L.moveTo(0);
         String temp = a[i];
         
         int elem  = L.getElement();
         j = i; 
         while( j>0 && temp.compareTo(a[elem])>0){
            L.moveNext();
            if(L.getIndex()!=-1){
               elem = L.getElement();
            }
            j--;
         }

         if(L.getIndex()==-1){
            L.append(i);
         } else{
            L.insertBefore(i);
            
         }

         
     }
      return L;
   }







   // Method that counts the number of lines in a file and returns that number.
   public static int countLines(String inFile) throws FileNotFoundException{
      int lineCount = 0;
      Scanner input = new Scanner(new FileReader(inFile));
      while(input.hasNextLine()){
         lineCount++;
         input.nextLine();
      }
      return lineCount;
   }

   // Method that reads lines from a file into an array of length "length" and returns array
   public static String[] readFile(String inFile, int length){
      String[] array = new String[length];
      try{
         FileInputStream fis = new FileInputStream(inFile);
         BufferedReader reader = new BufferedReader(new InputStreamReader(fis));

         String line = reader.readLine();
         for(int i=0; line!=null; i++){
            array[i] = line;
            line = reader.readLine();
         }
      }catch (FileNotFoundException e) {System.err.println(e);}
      catch (IOException ex){System.err.println(ex);}

      return array;      
   }

}
