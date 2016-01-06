// Sean McGrath, ssmcgrat@ucsc.edu 

// $Id: jfmt.java,v 1.2 2014-01-14 11:33:20-08 - - $
//
// Starter code for the jfmt utility.  This program is similar
// to the example code jcat.java, which iterates over all of its
// input files, except that this program shows how to use
// String.split to extract non-whitespace sequences of characters
// from each line.
//

import java.io.*; 
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import static java.lang.System.*;

class jfmt {
   // Static variables keeping the general status of the program.
   public static final String JAR_NAME = get_jarname();
   public static final int EXIT_SUCCESS = 0;
   public static final int EXIT_FAILURE = 1;
   public static int exit_status = EXIT_SUCCESS;
   public static int globalVariable = 65; //this is width of line, initially set to 65.

   // A basename is the final component of a pathname.
   // If a java program is run from a jar, the classpath is the
   // pathname of the jar.
   static String get_jarname() {
      String jarpath = getProperty ("java.class.path");
      int lastslash = jarpath.lastIndexOf ('/');
      if (lastslash < 0) return jarpath;
      return jarpath.substring (lastslash + 1);
   }


   // Formats a single file.
   static void format (Scanner infile) {
      List<String> words = new LinkedList<String> (); 	
      // Read each line from the opened file, one after the other.
      // Stop the loop at end of file.
      for (int linenr = 1; infile.hasNextLine(); ++linenr) {
         String line = infile.nextLine();
         

         
         

         // Split the line into words around white space and iterate
         // over the words.
         for (String word: line.split ("\\s+")) {

            // Skip an empty word if such is found.
            if (word.length() == 0) continue;

            // Append the word to the end of the linked list.
            words.add (word);

         }
         
	}
         //Variable for keeping track of number of characters in a line.
         int counter = 0; 
 
         //Loops through the array 'words' until each word is printed out in a paragraph.
         //Separate if statements control when to start a new line. 
         for (String word: words) {
                counter += word.length();
                if(counter == globalVariable){
                        out.print(word + "\n" );
                        counter = 0;

                }
                else if(counter < globalVariable){
                        out.print(word + " ");
                        counter += 1;

                }
                else if(counter > globalVariable){
                        out.print("\n" + word + " " );                
                        counter = word.length() + 1; 
                }
				
         } 
        words.clear(); 
   }

   
      
 

   // Main function scans arguments and opens/closes files.
   public static void main (String[] args) {
      
      if (args.length == 0) {
         // There are no filenames given on the command line.
         
         format (new Scanner (in));
      }else {
	 //Set argix to 0 to deal with first argument given in command line. 
         int argix = 0;
         String filename = args[argix];
         if(filename.length() != 0){ 
                //Deals with a file begining with '-', extracts an in from file name.  
                if(filename.charAt(0)== '-'){
                        //Sets globalVariable to the integer value of the first 'file'.
                        globalVariable = Integer.parseInt(filename)* -1;
                        argix = 1;
                }
         }
         // Iterate over each filename given on the command line.
         while( argix < args.length) {
            filename = args[argix];

            if (filename.equals ("-")) {
               // Treat a filename of "-" to mean System.in.
           
               format (new Scanner (in));
            }else {
               // Open the file and read it, or error out.
               try {
                  Scanner infile = new Scanner (new File (filename));
                  out.printf ("\n");
                  format (infile);
                  infile.close();
               }catch (IOException error) {
                  exit_status = EXIT_FAILURE;
                  err.printf ("%s: %s%n", JAR_NAME,
                              error.getMessage());
               }
            }
	   ++argix;	
         }
      }
      exit (exit_status);
   }

}
