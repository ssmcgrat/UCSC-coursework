// Authors: Sean McGrath, ssmcgrat@ucsc.edu
//          Sterling Salvaterra, ssalvate@ucsc.edu
//
// $Id: treemap.java,v 1.27 2014-02-14 14:16:31-08 - - $

import static java.lang.System.*;

class treemap <key_t extends Comparable <key_t>, value_t> {

   private class node {
      key_t key;
      value_t value;
      node left;
      node right;
   }
   public node root;
   
   
   private void debug_dump_rec (node tree, int depth) {
      
      if(tree==null) return;
      debug_dump_rec(tree.left, depth+1);
      for(int i =0; i<depth; i++){
         out.printf("  ");
      }
      out.printf("%d" +" " + "%s" + " " +
           "%s%n", depth, tree.key, tree.value);
      debug_dump_rec(tree.right, depth+1);
      
   }

   private void do_visit_rec (visitor <key_t, value_t> visit_fn,
                              node tree) {
      if(tree == null) return;
      do_visit_rec (visit_fn, tree.left);
      visit_fn.visit (tree.key, tree.value);
      do_visit_rec (visit_fn, tree.right);

      //throw new UnsupportedOperationException ();
   }

   public value_t get (key_t key) {
      node curr = root;
      while(curr != null) {
         int cmp = curr.key.compareTo(key);
         if(cmp == 0) return curr.value;
         if(cmp < 0 ) curr = curr.right;
             else curr = curr.left;
      }
      return null;

      //throw new UnsupportedOperationException ();
   }

   public value_t put (key_t key, value_t value) {
      node curr = root;
      node parent = null; 
      //parent = curr;
      int cmp=1;
      while(curr != null) {
         cmp = curr.key.compareTo(key);
         if (cmp == 0) break;
         parent = curr;
         if (cmp < 0)  curr = curr.right;
                 else curr = curr.left;
      }
      if(cmp != 0) {      
         node tmp = new node();
         tmp.left = null;
         tmp.right = null;
         tmp.key = key;
         tmp.value = value;
         if(root==null){
            root = tmp;
            parent = root;
         }else if(cmp<0){
            parent.right = tmp;
         }else parent.left = tmp;
      }else{
         value_t oldValue = curr.value;
         curr.value = value;
         return oldValue;
      }
      
      return null;
      //throw new UnsupportedOperationException ();
   }

   public void debug_dump () {
      debug_dump_rec (root, 0);
   }

   public void do_visit (visitor <key_t, value_t> visit_fn) {
      do_visit_rec (visit_fn, root);
   }

}
