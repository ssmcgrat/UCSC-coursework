// Author: Sean McGrath, ssmcgrat@ucsc.edu
//
//
// $Id: inode.cpp,v 1.12 2014-07-03 13:29:57-07 - - $

#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

#include "debug.h"
#include "inode.h"

int inode::next_inode_nr {1};

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case PLAIN_INODE:
           contents = make_shared<plain_file>();
           break;
      case DIR_INODE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

plain_file_ptr plain_file_ptr_of (file_base_ptr ptr) {
   plain_file_ptr pfptr = dynamic_pointer_cast<plain_file> (ptr);
   if (pfptr == nullptr) throw invalid_argument ("plain_file_ptr_of");
   return pfptr;
}

directory_ptr directory_ptr_of (file_base_ptr ptr) {
   directory_ptr dirptr = dynamic_pointer_cast<directory> (ptr);
   if (dirptr == nullptr) throw invalid_argument ("directory_ptr_of");
   return dirptr;
}


size_t plain_file::size() const {
   
   //DEBUGF ('i', "size = " << filesize);
   return filesize;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   string str;
   int size = data.size();
   for(int i=0; i<size; ++i){
      str += data.at(i);
      str += " ";
   }
   cout << str << endl;
   return data;
}

void inode:: readFile() const {
   auto file = plain_file_ptr_of(contents);
   wordvec readData = file->readfile();
   return ;
}

void inode::writeFile (const wordvec& words){
   auto file = plain_file_ptr_of(contents);
   file->writefile(words);
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   // find the size of the file
   int s = words.size();
   int size = 0;
   for(int i=0; i<s; ++i){
      if(i>0) ++size;
      string str = words.at(i);
      size += str.size();
   }
   filesize = size;
   data = words;
}

size_t directory::size() const {
   //size_t size {0}; 
   //DEBUGF ('i', "size = " << size);
   //size_t size = dirSize;   
   return dirSize;
}

void inode::deleteFile(const string& filename) {
   auto dir = directory_ptr_of(contents);
   dir->remove(filename);
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);

   dirents.erase(filename);
   --dirSize;
}

void inode::deleteFileRec(inode_ptr parent, 
                          inode_ptr curr, const string& filename){
    auto dir = directory_ptr_of(contents);
    dir->removeRecursive(parent, curr, filename);  
    return;
}


void directory::removeRecursive(inode_ptr parent, inode_ptr curr,
                                const string& filename){
   string n;
   inode_ptr inode;
   for(auto elem : dirents){
      n = elem.first;
      inode_t type = elem.second->get_type(); 
      if(type==0) curr->deleteFile(n);
      else{
        if(n=="." || n==".."){}
        else{
           inode = curr->getChild(n); 
           inode->deleteFileRec(curr, inode, n);   
        } 
      }
   }
   
   parent->deleteFile(filename);
   return; 
}

// called on by fn_make to create a new file in the 
// specified directory.
void inode::mkFile(const string& filename){
   auto directory = directory_ptr_of(contents);
   directory->mkfile(filename);
}

void directory::mkfile(const string& filename){
   inode_ptr i = make_shared<inode>(PLAIN_INODE);
   i->name = filename;
   dirents.insert (pair<string, inode_ptr>(filename, i) );
   ++dirSize;
   
}

// called on by fn_mkdir to create a new directory.
// castes contents as a directory_ptr, then calls directory::mkdir(). 
void inode::addDir(string str, inode_ptr parent){
   auto dir = directory_ptr_of(contents);
   dir->mkdir(str, parent);
}

// creates a new inode_ptr and adds this to the map dirents
// might need to return inode&...
void directory::mkdir (const string& dirname, inode_ptr parent){
   inode_ptr i = make_shared<inode>(DIR_INODE);   
   i->name = dirname; 
   dirents.insert (pair<string, inode_ptr>(dirname, i) );
   ++dirSize;
   i->insertIntoDir(parent, i); 
  
    
}

inode_state::inode_state() {
   root = make_shared<inode>(DIR_INODE);   
   cwd  = root;
   // create "." and ".." in root.
   

   //auto r = directory_ptr_of(root->contents);
   root->insertIntoDir(root, root);
   root->name = "/";
   
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt << "\"");
}

// called on to see if the string child is in the inode's map.
bool inode::findChild(string child){
   auto dir = directory_ptr_of(contents);
   bool b = dir->findChild(child);
   return b;
}

//returns a pointer to a child of the inode
inode_ptr inode::getChild(string child){
   auto dir = directory_ptr_of(contents);
   auto ptr = dir->getChild(child);
   return ptr;
} 

// finds if a string is in the directory's map.
bool directory::findChild(string child){
   for(auto elem : dirents){
      string str = elem.first;
      if(str==child) return true;
   }
   return false;
}

// returns a pointer to a child of the directory
inode_ptr directory::getChild(string child){
   inode_ptr foundChild;
   for(auto elem : dirents){
      if(elem.first==child) foundChild = elem.second;
   }
   return foundChild;
}

void inode::insertIntoDir(inode_ptr parent, inode_ptr curr){
   auto dir = directory_ptr_of(contents);
   dir->insertIntoDir(parent, curr);
   
   
}

// Creates the '.' and '..' entries in root, which are root.
// only to be called once by inode_state().
void directory::insertIntoDir(inode_ptr parent, inode_ptr curr){
   string dot = ".";
   string dotdot = "..";
   dirents.insert (pair<string, inode_ptr>(dot, curr) );
   dirents.insert (pair<string, inode_ptr>(dotdot, parent) );
   dirSize += 2; 
   

}

// returns the type of an inode
// 0 represents a plain file, 1 a directory.
inode_t inode::get_type(){
   return type;
}

size_t inode::size(){
   size_t size;
   inode_t t = get_type();   
   if(t==1){
      auto dir = directory_ptr_of(contents);
      size = dir->size();
   } else{
      auto file = plain_file_ptr_of(contents);
      size = file->size();
   }
   return size;
}


// Called by fn_ls to print the contents of an inode. 
// Calls directory::printDirents()
void inode::printContents(){
   auto dir = directory_ptr_of(contents);
   string inodeName;
   if(name=="/") {
      cout << name; 
      dir->printDirents();
   }else{
      wordvec names;
      names.emplace_back(name);
      inode_ptr ptr = this->getChild("..");
      for(;;){
        inodeName = ptr->name;
        names.emplace_back(inodeName);
        if(inodeName=="/") break;
        ptr = ptr->getChild(".."); 
      }
      int size = names.size();
      for(int i=size-1; i>=0; --i){
         if(i==size-1){
            cout << names.at(i) ;
         } else if(i>0){
            cout << names.at(i) << "/" ;
         } else{
            cout << names.at(i);
         }
      }
      dir->printDirents();
   }
}

void inode::printRecursive(inode_ptr ptr){
   auto dir = directory_ptr_of(ptr->contents);
   inode_ptr recptr = dir->printRecursive(ptr);
   if(recptr==ptr) return;
   printRecursive(recptr);
}

inode_ptr directory::printRecursive(inode_ptr ptr){
   //cout << "one directory" << endl;
   bool rec = false;
   inode_ptr recptr;
   for(auto elem : dirents){
     string n = elem.first;
     inode_t type= elem.second->get_type();
     if(type==1){
        if(n=="." || n==".."){}
        else { 
           rec = true;
           recptr = elem.second;
           
        }
     }
   }
   //cout << ptr->name << endl;
   ptr->printContents();
   if(rec)  return recptr;
   else{ return ptr; }
   
}

void directory::printDirents(){
   cout  << ":" << endl;
   string str;
   int inode_nr;
   size_t size;
   for(auto elem : dirents){
     str = elem.first;
     inode_t type = elem.second->get_type();
     if(type==1){
        if(str=="." || str==".."); 
        else{ str += "/";}
     }
     inode_nr = elem.second->get_inode_nr(); 
     size = elem.second->size(); 
     cout << setw(6) << setfill(' ') << inode_nr << "  ";
     cout << setw(6) << setfill(' ') << size << "  " << str << endl;
   }   
}

// Returns a pointer to the cwd
inode_ptr inode_state::get_cwd(){
   auto ptr = cwd;
   return ptr;
}

// Set cwd to the specified inode_ptr
void inode_state::set_cwd(inode_ptr newcwd){
   cwd = newcwd;
}

// Return a pointer to the root inode
inode_ptr inode_state::get_root(){
   auto ptr = root;
   return ptr;
}

int inode_state::get_cwd_nr(){
   return cwd->get_inode_nr();
}

// Sets the prompt, called upon by fn_prompt
void inode_state::setPrompt(string p){
   prompt = p;
}

// Public function that returns the value of prompt
string inode_state::getPrompt(){
   return prompt;
}


ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

