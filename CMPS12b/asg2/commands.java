// Sean McGrath, ssmcgrat@ucsc.edu,  Sterling Salvaterra, ssalvate@ucsc.edu
// $Id: commands.java,v 1.1 2014-01-30 18:25:49-08 - - $

import java.util.HashMap;
import static java.lang.System.*;

class commands {

   private static void command_00 (String[] words) {
      // Executing a comment does nothing.

   }

   private static void command_bp (String[] words) {
     jroff.bp = true;
      
     
      
   }

   private static void command_br (String[] words) {
     //remove all elements in wordqueue
     linkedqueue.plCount ++;    
     String[] a = new String [2];
     a[1] = "0";
     command_sp (a);     

   }

   private static void command_cc (String[] words) {
      try{
         jroff.cc = words[1];
      }catch(ArrayIndexOutOfBoundsException e){
         jroff.cc = ".";
      }
       
      
   }

   private static void command_in (String[] words) {
      linkedqueue.plCount ++;
      try{      
         jroff.ind = Integer.parseInt(words[1]); 
      }catch(ArrayIndexOutOfBoundsException e){   
         jroff.ind = 0;         
      }
      String[] a = new String [2];
      a[1] = "0";
      command_sp (a); 
        
   }

   private static void command_ll (String[] words) {
      try{      
         int tmp = Integer.parseInt(words[1]);
         jroff.lineLength = tmp;
      }catch(ArrayIndexOutOfBoundsException e){
         e.getMessage();
      }
      
   }

   private static void command_mt (String[] words) {
      try{
         int tmp = Integer.parseInt(words[1]);
         jroff.mt = tmp;
      }catch(ArrayIndexOutOfBoundsException e){
         e.getMessage();
      }
      
   }

   private static void command_pl (String[] words) {
      try{
         int tmp = Integer.parseInt(words[1]);
         jroff.pl =tmp;
      }catch(ArrayIndexOutOfBoundsException e){
         e.getMessage();
      }
      
   }

   private static void command_po (String[] words) {
      try{
         int tmp = Integer.parseInt(words[1]);
         jroff.po = tmp;
      }catch(ArrayIndexOutOfBoundsException e){
         e.getMessage();
      }
      
   }
   
   

   private static void command_sp (String[] words) {
      int n;
      try{
         n = Integer.parseInt(words[1]);
         
      }catch(ArrayIndexOutOfBoundsException e){
         n = 1;
         
      }
      linkedqueue.plCount += n;
      
    
      for(int i =0; i<=n; i++){
         linkedqueue.document.append("\n");
          
      }
      
      linkedqueue.currLength = 0;
      
           
     
   }

   private static void STUB (String[] words) {
      out.printf ("%s: STUB: %s",
                  auxlib.PROGNAME, auxlib.caller());
      for (String word: words) out.printf (" %s", word);
      out.printf ("%n");
   }

   public static void do_command (String[] words) {
      switch (words[0].substring(1,words[0].length())) {
         case "\\\"": command_00 (words); break;
         case "bp"  : command_bp (words); break;
         case "br"  : command_br (words); break;
         case "cc"  : command_cc (words); break;
         case "in"  : command_in (words); break;
         case "ll"  : command_ll (words); break;
         case "mt"  : command_mt (words); break;
         case "pl"  : command_pl (words); break;
         case "po"  : command_po (words); break;
         case "sp"  : command_sp (words); break;
         default     : throw new IllegalArgumentException (words[0]);
      }
   }

}

