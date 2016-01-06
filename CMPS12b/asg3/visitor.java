// Authors: Sean McGrath, ssmcgrat@ucsc.edu,
//          Sterling Salvaterra ssalvate@ucsc.edu
//
// $Id: visitor.java,v 1.1 2014-02-11 12:11:33-08 - - $

interface visitor <key_t, value_t> {
   public void visit (key_t key, value_t value);
}

