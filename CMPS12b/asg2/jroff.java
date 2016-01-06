// Sean McGrath, ssmcgrat@ucsc.edu, Sterling Salvaterra, ssalvate@ucsc.edu
// $Id: jroff.java,v 1.2 2014-01-30 10:18:39-08 - - $

import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class jroff{
   static final String STDIN_NAME = "-";
   public static int lineLength = 65;
   public static int mt = 6;
   public static int pl = 60;
   public static int ind = 0;
   public static int po = 10;
   public static String cc = ".";
   public static boolean bp = true;
   public static int pgNum = 1;
   public static boolean indbr = false;
   public static linkedqueue<String> wordqueue = new
      linkedqueue<String> ();   
   static void scanfile (String filename, Scanner infile) {

      
      for (int linenr = 1; infile.hasNextLine(); ++linenr) {
         String line = infile.nextLine();
          
         String[] words = line.split ("\\s+");
         if (words.length > 0 && words[0].startsWith (cc)) {
            try {
               commands.do_command (words);
            }catch (IllegalArgumentException error) {
               auxlib.warn (filename, linenr, words[0],
                            "invalid command");
            }
         }else {
            for (String word: words){//add word to end of the queue. 
               wordqueue.insert(word);
            }
             
         }
        linkedqueue.printparagraph(wordqueue);
      }
      System.out.print(linkedqueue.document + "\n" + "\n");
   }
   public static void main (String[] args) {
      if (args.length == 0) {
         scanfile (STDIN_NAME, new Scanner (in));
      }else {
         for (String filename : args) {
            if (filename.equals (STDIN_NAME)) {
               scanfile (STDIN_NAME, new Scanner (in));
            }else {
               try {
                  Scanner scan = new Scanner (new File (filename));
                  scanfile (filename, scan);
                  scan.close();
               }catch (IOException error) {
                  auxlib.warn (error.getMessage());
               }
            }
         }
      }
   }

}


