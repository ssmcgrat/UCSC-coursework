// Sean McGrath, ssmcgrat.ucsc.edu, Sterling Salvaterra, ssalvate@ucsc.edu
// $Id: linkedqueue.java,v 1.2 2014-01-30 10:17:34-08 - - $

import java.util.NoSuchElementException;

class linkedqueue <item_t> {

   private class node{
      item_t item;
      node link;
   }

   //
   // INVARIANT:  front == null && rear == null
   //          || front != null && rear != null
   // In the latter case, following links from the node pointed 
   // at by front will lead to the node pointed at by rear.
   //
   private node front = null;
   private node rear = null;

   public boolean empty (){
      return front == null;
   }

   public void insert (item_t any) {
      node tmp = new node();
      tmp.item = any;
      tmp.link = null;
      if(empty() ) {
         front = tmp;
      }else{
         rear.link = tmp;
      } 
      rear = tmp; 
      
   }

   public item_t remove (){
      if (empty ()) throw new NoSuchElementException ();
      else{
         item_t temp = front.item;
         front = front.link;
         if(empty()) {rear = null;} 
         return temp;   
      }

   }
   public static StringBuffer document = new StringBuffer();
   public static int plCount;
   public static void printTopPage(){
      document.append('\f');
      String numberLine = "";
      for(int i=0; i<(jroff.mt)/2; i++){
         document.append('\n');
      }
      for(int i=0; i<74; i++){
         numberLine += " ";
      }
      document.append(numberLine + jroff.pgNum);
      for(int i=0; i<((jroff.mt)/2)-1; i++){
         document.append('\n');
      }
      jroff.pgNum ++;
      jroff.bp = false;
      jroff.indbr = false;
      plCount = jroff.mt;
   }


   public static boolean sp = false; //for sp command
   public static int currLength = 0;
   private static String lastWord = "";
   
   
   public static void printparagraph(linkedqueue<String> wordqueue){
      StringBuffer lineToPrint = new StringBuffer();
      
      String space = " ";
      String dspace = "  ";
      String s;
      String pgOffset="";
      String ind="";
      boolean top = false;
       
      

      if(jroff.bp==true){
         printTopPage(); 
         top = true;
      }
      //Checks if the current word is the start of a new sentence,
      //important for formatting numbers of spaces b4 word.
      boolean startSentence = false;

      //Adds appropriate amount of space chars to pgOffset + ind. 
      for(int i=0; i<jroff.po; i++){
         pgOffset += " ";
      }
      for(int i=0; i<jroff.ind; i++){
         ind += " ";
      }
      
      
      
      while(wordqueue.front != null){
         boolean startpara = false;
         
         
         if(currLength ==0){
            currLength += jroff.ind;
            document.append(pgOffset +ind);
            startpara = true;//for blank lines, no double pgOffset
          } 
          
         String word = wordqueue.remove();
         //if word is greater than line length
         if(word.length()>jroff.lineLength){
            System.err.printf(".: warning: can't break line\n");
         }
                  
         currLength += word.length();
         s = space;
         
         if(lastWord.endsWith(".")||lastWord.endsWith("?") ||
             lastWord.endsWith("!")||lastWord.endsWith(":")
             ||lastWord.endsWith(";")){            
            s = dspace;
            
         }
         currLength += s.length(); 
         if(currLength <= jroff.lineLength){
           
            if(currLength-word.length()== 0){
               document.append(word);
              
            }else{
               if(startpara==false){
                  document.append(s + word);
                  if(currLength > jroff.lineLength){
                     break;
                  }
               }else document.append(word);
            }
                     
         }
         //For blank lines 
         if(word.equals("")){
            if(lastWord.equals("") != true){
               document.append("\n");
               plCount++;
            }
            currLength=jroff.lineLength+1;
            startpara = true;
         }
 
          if(currLength>jroff.lineLength){
            if(startpara==false ){
            document.append("\n"+pgOffset + ind);
            }else  document.append('\n');
            
            plCount++;
            if(plCount>=jroff.pl){
               printTopPage();
               document.append(pgOffset + ind);
            }
            currLength = 0;
            document.append(word );
            currLength = word.length()+ jroff.ind;
         }
         lastWord = word;
        
      }
     
   }

}
