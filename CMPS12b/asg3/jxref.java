// Authors: Sean McGrath, ssmcgrat@ucsc.edu,
//          Sterling Salvaterra ssalvate@ucsc.edu
// $Id: jxref.java,v 1.53 2014-02-14 15:30:45-08 - - $

import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class jxref {
   static final String STDIN_NAME = "-";
   static boolean fopt = false;
   static boolean printtree = false;
   static boolean df = false;
   static class printer implements visitor <String, queue <Integer>> {
      public void visit (String key, queue <Integer> value) {
         out.printf ("%s ", key);
         for (int linenr: value) out.printf (" %d", linenr);
         out.printf ("%n");
      }
   }

    static void xref_file (String filename, Scanner scan) {
      treemap <String, queue <Integer>> map =
            new treemap <String, queue <Integer>> ();
      //queue <Integer> newQ = new queue <Integer>();
      for (int linenr = 1; scan.hasNextLine (); ++linenr) {
         for (String word: scan.nextLine().split ("\\W+")) {
            if (word.matches ("^\\d*$")) continue;
            if(fopt) word = word.toLowerCase();
            queue <Integer> intQ = map.get(word);
            if(intQ==null) {
               intQ = new queue <Integer>();
               intQ.insert(linenr);
               map.put(word, intQ);
            }else{
               intQ.insert(linenr);
            }
                      
           
             
         }
      }
      if(printtree) map.debug_dump();
      if(df==false){
         visitor <String, queue <Integer>> print_fn = new printer ();
         map.do_visit (print_fn);
      }
   }
   public static void getopt(String args){
      while(args.length()>1){
         switch(args){
            case "-f": 
               fopt=true;
               break;
            case "-d":
               printtree=true;
               df = true;
               break;
            case "-df":
               fopt=true;
               printtree=true;
               df = true;
               break;
            case "-fd":
               fopt=true;
               printtree=true;
               df = true;
               break; 
            default:
               err.printf("Option not found");
               exit(1);
         }break;
      }
   }
   public static void main (String[] args) {
      String headertop = "";
      String headerbottom = "";
      headertop += "\n";
      for(int i=0; i<64; i++) headertop +=":";
      headertop += "\n";
      for(int i=0; i<64; i++) headerbottom +=":";
      headerbottom += "\n\n";
      boolean optused = false;
      if (args.length == 0) {
         xref_file (STDIN_NAME, new Scanner (in));
      }else {
         
         for (int argi = 0; argi < args.length; ++argi) {
            optused = false;
            String filename = args[argi];
            if(filename.startsWith("-") && argi==0){
               getopt(filename);
               optused = true;
            }
            if (filename.equals (STDIN_NAME)) {
               xref_file (STDIN_NAME, new Scanner (in));
            }else {
               try {
                  Scanner scan = new Scanner (new File (filename));
                  out.print(headertop+ filename +"\n"+ headerbottom);
                  xref_file (filename, scan);
                  scan.close ();
               }catch (IOException error) {
                  if(optused==false){
                     auxlib.warn (error.getMessage ());
                     exit(1);
                  }
               }
            }
         }
      }
      auxlib.exit ();
   }

}

