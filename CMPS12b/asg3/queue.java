// Authors: Sean McGrath, ssmcgrat@ucsc.edu,
//          Sterling Salvaterra ssalvate@ucsc.edu
//
// $Id: queue.java,v 1.1 2014-02-11 12:11:33-08 - - $

import java.util.Iterator;
import java.util.NoSuchElementException;

class queue <item_t> implements Iterable <item_t> {

   private class node {
      item_t item;
      node link;
   }
   private node head = null;
   private node tail = null;

   public boolean isempty () {
      return head==null;
      //throw new RuntimeException ("Replace this with working code");
   }

   public void insert (item_t newitem) {
      node n = new node();
      n.item = newitem;
      n.link = null;
      if(isempty()){
         head = n;
      }else{
         tail.link = n;
      }
      tail = n;
      //throw new RuntimeException ("Replace this with working code");
   }

   public Iterator <item_t> iterator () {
      return new itor ();
   }

   private class itor implements Iterator <item_t> {
      node next = head;
      public boolean hasNext () {
         return next != null;
      }
      public item_t next () {
         if (! hasNext ()) throw new NoSuchElementException ();
         item_t result = next.item;
         next = next.link;
         return result;
      }
      public void remove () {
         if(isempty()) throw new NoSuchElementException ();
         
         head = head.link;
         if(isempty()) tail = null;
// throw new UnsupportedOperationException ();
      }
   }

}
