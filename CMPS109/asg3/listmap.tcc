// Author: Sean McGrath, ssmcgrat@ucsc.edu
//
// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);

   node* prev = anchor();
   node* curr = prev->next;
   node* tmp;
    
   iterator found = find(pair.first);
    
   if(found == curr) {
      curr->value.second = pair.second;
      return iterator(curr);
   } 
   while ( curr != &anchor_ && less (curr->value.first, pair.first)) {
      curr = curr->next;
      prev = prev->next;
      if (found == curr ) {
         curr->value.second = pair.second;
         return iterator(curr);
      }
   }
      
   

   tmp = new node(curr, prev, pair);
   curr->prev = tmp;
   prev->next = tmp;

   return iterator(tmp);
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) const {
   TRACE ('l', that);

   node* curr = anchor_.next;
   node* tmp;
   while(curr != &anchor_){
      if(curr->value.first == that) { 
         tmp = curr;
         return iterator(tmp);         
      }
      curr = curr->next;
   }
  
    
   return iterator();
}


//
// listmap::print_pairs()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::print_pairs() const{
   node* curr = anchor_.next;
   node* tmp;
   while(curr != &anchor_){
      cout << curr->value.first << " = " << curr->value.second << endl;
      tmp = curr;
      curr = curr->next;
   }
   return iterator(tmp);
}

//
// listmap::print_keys(const mapped_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::print_keys(const mapped_type& that) const{
   node* curr = anchor_.next;
   while(curr != &anchor_){
      if(curr->value.second == that){
         cout << curr->value.first << " = " ;
         cout << curr->value.second << endl;
      }
      curr = curr->next;
   }
   return iterator();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);
   node* pos; 
   pos = position.where;
   pos->prev->next = pos->next;
   pos->next->prev = pos->prev;
   delete pos;
   return iterator();
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

//
// bool listmap::iterator::is_null() 
//
template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::iterator::is_null() const{
   bool b = false;
   if(this->where == nullptr)  b = true; 
   return b;
}
