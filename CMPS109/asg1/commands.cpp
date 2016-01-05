// Author: Sean McGrath, ssmcgrat@ucsc.edu
// 
// $Id: commands.cpp,v 1.11 2014-06-11 13:49:31-07 - - $

#include "commands.h"
#include "debug.h"

#include <string>
#include <vector>

commands::commands(): map ({
   {"#"     , fn_htag  },
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
}){}


command_fn commands::at (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   command_map::const_iterator result = map.find (cmd);
   if (result == map.end()) {
      throw yshell_exn (cmd + ": no such function");
   }
   return result->second;
}

string cmdPrompt;

string commands::getPrompt(){
   return prompt; 
}

void commands::setPrompt(string p){
   prompt = p;

   
}

// This ignores a commands starting with '#'
void fn_htag (inode_state& state, const wordvec& words){
   // Just making checksource not complain about unused variables
   state.get_cwd();
   words.at(0);
   return;
}



void fn_cat (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // error chk, prints messgae if no file is specified.
   if(words.size()<2){
      throw yshell_exn("No file name specified");      
      return;
   }

   inode_ptr currDir = state.get_cwd();
   string pathname = words.at(1);
   wordvec path = split(pathname, "/");
   bool fromRoot = false;
   char firstChar = pathname.at(0);
   if(firstChar=='/') fromRoot = true;
   string filename;
   int pathSize = path.size();

   // if we are cating a file in the cwd
   if(pathSize==1){
      filename = path.at(0);
      bool b = currDir->findChild(filename);
      if(!b){
         throw yshell_exn(filename + ": No such file or directory"); 
         return;
      } else {
         inode_ptr file = currDir->getChild(filename);
         inode_t type = file->get_type();
         if(type==1){
            throw yshell_exn(filename+ ": Is a directory"); 
            return;
         }
         file->readFile();    
         return;  
      }
   }
  
   // If we are cating a file in another directory
   //
   if(fromRoot) currDir = state.get_root();
   bool childChk = false;
   string wantedChild;
   // checking if path is correct
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      } else{
         throw yshell_exn("The path "+words.at(1)+ ": does not exist"); 
         return;
      }
      childChk = false;
   }
   // If the path does exist, check if the file exists in currDir.
   // If not, print an error msg. If it is a directory,
   // print an error msg. If it is a file, cat it. 
   filename = path.at(pathSize-1);
   bool b = currDir->findChild(filename);
   if(b){
      inode_ptr file = currDir->getChild(filename);
      inode_t type = file->get_type();
      if(type==1){
         throw yshell_exn(words.at(1)+ ": is a directory"); 
         return;
      }
      file->readFile();
      return;
   } else{
      throw yshell_exn(words.at(1)+ ": No such file or directory"); 
   }
   
}

void fn_cd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // error check, prints message if too many arguments given.
   if(words.size()>2){
      throw yshell_exn(" Too many arguments given");
      return;
   }

   // Set cwd to root if no directory given
   if(words.size()<2){
      inode_ptr r = state.get_root();
      state.set_cwd(r);
      return;
   }
   
   inode_ptr ptr = state.get_cwd();
   string pathname = words.at(1);
   wordvec path = split(pathname, "/");
   bool fromRoot = false;
   char firstChar = pathname.at(0);
   if(firstChar=='/') fromRoot = true;
   
   //
   // The following three if statements are special cases, namely, if 
   // pathname is ".", "..", or "/".
   //
   if(pathname==".") return;
   if(pathname==".."){
      inode_ptr newcwd = ptr->getChild("..");
      state.set_cwd(newcwd);
      return; 
   }
   if(pathname == "/" ){
     inode_ptr r = state.get_root();
     state.set_cwd(r);
     return; 
   }   

   int pathSize = path.size();
   
   // if we are simply changing to directory that's a child of cwd. 
   if(pathSize==1){ 
      string str = path.at(0);
      bool b = ptr->findChild(str);
      if(b==true){
         inode_ptr newcwd = ptr->getChild(str);
         state.set_cwd(newcwd);
         return;
      }   
   }

   // otherwise
   inode_ptr currDir;
   if(fromRoot){
      currDir = state.get_root();
   } else {
      currDir = ptr;
   }
   bool childChk = false;    
   string wantedChild;
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      } else{
        throw yshell_exn(" "+words.at(1)+
                          ": No such file or directory"); 
        return;
      }
      childChk = false;
   }
   // if path is correct, check that the deepest directory exists, 
   //if so, set cwd to this directory.
   
   string str = path.at(pathSize-1);
   bool b = currDir->findChild(str);
   if(b){
      currDir = currDir->getChild(str);
      state.set_cwd(currDir);
   }else{
      throw yshell_exn(" "+words.at(1)+
                      ": No such file or directory"); 
   }
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   int length = words.size();   
   for (int i=1; i<length; ++i){
      cout << words.at(i) << " " ;
   }
   cout << endl;
   
}

void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
 
   // check if no status is given, in which case exit with status 0. 
   if (words.size() < 2) {
      throw ysh_exit_exn();
      exit_status_message();    
   } else{ // if a status is given...
      const string str = words.at(1);
      int length = str.size();

      // iterate through each character in str
      for(int i=0; i<length; ++i){
         char c = str[i];
         // if a non-decimal is found, exit_status set to 127 and break
         if(c != '0' && c!='1' && c!='2' && c!='3' && c!='4' && c!='5'
            && c!='6' && c!='7' && c!='8' && c!='9'){
            exit_status::set(127);
            break;
         }else{
         // set exit_status to inputted status            
            int exit = stoi(str);
            exit_status::set(exit);
         }     
      }
   throw ysh_exit_exn();
   exit_status_message();
   }
}

void fn_ls (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   inode_ptr ptr = state.get_cwd();
   string pathname;
   wordvec path;
   bool fromRoot = false;

   bool printcwd = false;  
   int size = words.size();
   if(size==1) printcwd = true; // if no pathname, ls cwd
   if(size>1){
      pathname = words.at(1);  // get the pathname
      path = split(pathname, "/");
      if(pathname == ".") printcwd = true;// if pathname = ".", ls cwd. 
   }
   
   // print description of files and/or directories of cwd.
   if(printcwd){
      ptr->printContents();      
      return;     
   }

   //
   // This is a special case, when we print out the contents of ".."
   if(pathname==".."){
      ptr = ptr->getChild("..");
      ptr->printContents();
      return;
   }
   // Special case where we print contents of root directory
   if(pathname=="/"){
      ptr = state.get_root();
      ptr->printContents();
      return;
   }


   // otherwise we print out the contents of the directory specified 
   // by the pathname given, provided the path and directory exist.

   int pathSize = path.size();
   char firstChar = pathname.at(0);
   if(firstChar=='/') fromRoot = true;
   if(fromRoot){
      ptr = state.get_root();
   }      
    
   bool childChk = false;
   string wantedChild;
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = ptr->findChild(wantedChild);
      if(childChk){
         ptr = ptr->getChild(wantedChild);
      } else {
         throw yshell_exn(words.at(1)+
                          ": No such file or directory"); 
         return;
      }
      childChk = false;
   } 

   // if path is correct, check the deepest directory exists, 
   // if so, print its contents, otherwise print an error msg.
   string str = path.at(pathSize-1);
   bool b = ptr->findChild(str);
   if(b){
      ptr = ptr->getChild(str);
      ptr->printContents();
   } else{
       throw yshell_exn(words.at(1)+
                          ": No such file or directory");
      
   }

}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   inode_ptr currDir = state.get_cwd();
   string pathname;
   wordvec path;
   bool fromRoot = false;

   bool printcwd = false;
   int size = words.size();
   if(size==1) printcwd = true;
   if(size>1){
      pathname = words.at(1);
      path = split(pathname, "/");
      if(pathname == ".") printcwd = true;
   }
   // print contents of cwd and the contents of all its subdirectories.
   if(printcwd){
      currDir->printRecursive(currDir);
      return;
   }

   //
   // This is a special case, when we print out the contents of ".."
   if(pathname==".."){
      currDir = currDir->getChild("..");
      currDir->printRecursive(currDir);
      return;
   }
   // Special case where we print the contents of root direcotry
   if(pathname=="/"){
      currDir = state.get_root();
      currDir->printRecursive(currDir);
      return;
   }

   // otherwise we print out the contents of the directory specified 
   // by the pathname give, provided the path and direcotry exist.

   int pathSize = path.size();
   char firstChar = pathname.at(0);
   if(firstChar=='/') fromRoot = true;
   if(fromRoot){
      currDir = state.get_root();
   }      
    
   bool childChk = false;
   string wantedChild;
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      } else {
        throw yshell_exn(words.at(1)+ ": No such file or directory"); 
         return;
      }
      childChk = false;
   } 

   // if path is correct, check the deepest directory exists, 
   // if so, print its contents, otherwise print an error msg.
   string str = path.at(pathSize-1);
   bool b = currDir->findChild(str);
   if(b){
      currDir = currDir->getChild(str);
      currDir->printRecursive(currDir);
   } else{
      throw yshell_exn(words.at(1)+ ": No such file or directory"); 
   }
}


void fn_make (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   // Make sure a file name is specified, if not, print an error msg.
   if(words.size()==1){
      throw yshell_exn("No file name given");
      return;
   }
   
   string pathname = words.at(1);
   inode_ptr currDir = state.get_cwd();
   wordvec path = split(pathname, "/");
   bool fromRoot = false;
   char firstChar = pathname.at(0);
   if(firstChar=='/') fromRoot = true;
   int pathSize = path.size();
   string filename;
   
   // If we are creating a file in the cwd.
   if(pathSize==1){
      string str = path.at(0);
      bool b = currDir->findChild(str);

      // If there is no file with given name in cwd
      if(!b){
         filename = path.at(0);
         currDir->mkFile(filename);
         if(words.size()>2){
            inode_ptr file = currDir->getChild(filename);
            int numWords = words.size();
            wordvec newdata;
            for(int i=2; i<numWords; ++i){
               newdata.emplace_back(words.at(i));
            }
            file->writeFile(newdata);      
         }
      } else{
         filename = path.at(0);
         inode_ptr file = currDir->getChild(filename);
         inode_t type = file->get_type();
         if(type==1){
            throw yshell_exn(words.at(1)+ 
                          ": already exists as a directory"); 
            return;
         }
         wordvec newdata;
         if(words.size()>2){ 
            int numWords = words.size();
            for(int i=2; i<numWords; ++i){
               newdata.emplace_back(words.at(i));
            }  
         }
         file->writeFile(newdata);
         return;
      }
   }

   // otherwise
   if(fromRoot) currDir = state.get_root();
   bool childChk = false;
   string wantedChild;
   // checking if path is correct
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      } else{
         throw yshell_exn("the path '" +words.at(0)+
                          "' does not exist");  
         return;
      }
      childChk = false;
   }
   // If path does exist, check if file exists in currDir, if so, 
   // overwrite it. If it does not exist, create a new file.
   // If it is a directory, print error msg.
   filename = path.at(pathSize-1);
   bool b = currDir->findChild(filename);
   if(b){
      inode_ptr file = currDir->getChild(filename);
      inode_t type = file->get_type();
      if(type==1){
         throw yshell_exn(words.at(1)+ 
                       ": already exists as a directory"); 
         return;
      }else{
         wordvec newdata;
         if(words.size()>2){
            int numWords = words.size();
            for(int i=2; i<numWords; ++i){
               newdata.emplace_back(words.at(i));
            }
         }
         file->writeFile(newdata);
         return;
      }
   } else{
      currDir->mkFile(filename);
      if(words.size()>2){
         inode_ptr file = currDir->getChild(filename);
         int numWords = words.size();
         wordvec newdata;
         for(int i=2; i<numWords; ++i){
            newdata.emplace_back(words.at(i));
         } 
         file->writeFile(newdata);
      }
   }

}

void fn_mkdir (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   // make sure a directory name is given, if not print error.
   if(words.size()==1){
      throw yshell_exn("No directory name given"); 
      return;
   }
   inode_ptr currDir = state.get_cwd();
   string pathname = words.at(1);
   wordvec path = split(pathname, "/");
   bool fromRoot = false;
   char firstChar = pathname.at(0);
   if(firstChar=='/') fromRoot = true;
   string dirName;
   int pathSize = path.size();
   
   // if we are simply creating a directory that's a child of cwd.
   if(pathSize==1){
      string str = path.at(0);
      bool b = currDir->findChild(str);
      if(!b){
         dirName = path.at(0);
         currDir->addDir(dirName, currDir);
         return;
      } else{
         throw yshell_exn(path.at(0)+ ": The file already exists"); 
         return;
      }
   }

   // otherwise
   if(fromRoot) currDir = state.get_root(); 
   bool childChk = false;
   string wantedChild;
   //checking if path is correct
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      } else{
         throw yshell_exn(path.at(pathSize-1)+ 
                          ": Path does not exist"); 
         return;
      }
      childChk = false;
   }
   // if path is correct, check that the deepest directory does
   //  not exist, if not, create a new directory.
   //  If it does exist, print an error msg.

   string str = path.at(pathSize-1);
   bool b = currDir->findChild(str);
   if(!b){
      currDir->addDir(str, currDir);
   } else {
      throw yshell_exn(path.at(pathSize-1)+ 
                          ": Path does not exist");  
   } 
   
}

void fn_prompt (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   string newPrompt;
   int l = words.size();
   for(int i=1; i<l; ++i){
      if(i > 1) newPrompt += " ";
      newPrompt += words.at(i);
      
   }
   state.setPrompt(newPrompt); 
}

void fn_pwd (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   wordvec names;
   string name;
   
   inode_ptr ptr = state.get_cwd();
   // add names of inodes to wordvec until we get to root.
   for(;;){ 
      name = ptr->name; 
      names.emplace_back(name);
      if(name=="/") break;
      ptr = ptr->getChild("..");  
   }
   
   int size = names.size();
   for(int i=size-1; i>=0; --i){
      if(i==size-1){
         cout << names.at(i) ;   
      }else if(i>0){
         cout << names.at(i) << "/" ;
      }else{
         cout << names.at(i);
      }
   }
   cout << endl;
}

void fn_rm (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words.size()<2){
      throw yshell_exn("No pathname specified"); 
      return;
   }
   
   inode_ptr currDir = state.get_cwd();
   string pathname = words.at(1);
   wordvec path = split(pathname, "/");
   bool fromRoot = false;
   if(pathname.at(0)=='/') fromRoot = true;
   string filename;
   int pathSize = path.size();

   // if we are removing a file from cwd
   if(pathSize==1){
      filename = path.at(0);
      bool b = currDir->findChild(filename);
      if(!b){
         throw yshell_exn(words.at(1)+
                         ": No such file or directory"); 
         return;
      }
      inode_ptr file = currDir->getChild(filename);
      inode_ptr parent = currDir->getChild("..");
      // check if file is a directory, if so,
      // check that its size is only 2.
      inode_t fileType = file->get_type();
      if(fileType==1){
         size_t fileSize = file->size();
         if(fileSize>2){
            throw yshell_exn(words.at(1)+
                    ": Directory is not empty, cannot remove"); 
            return;
         }
         parent->deleteFile(filename);
         return; 
      }
      parent->deleteFile(filename);
      return; 
   }

   // If we are removing a file from another directory
   //
   if(fromRoot) currDir = state.get_root();
   bool childChk = false;
   string wantedChild;
   // checking if path is correct
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      } else{
         throw yshell_exn(words.at(1)+ ": path does not exist"); 
         return;
      }
   }
   // If the path exists, check if the file exists in currDir.
   // If not, print an error msg. IF it is a directory, check that
   // it is empty. If not, print error msg. If file is empty 
   // directory or plain_file, remove it.
   filename = path.at(pathSize-1);
   bool b = currDir->findChild(filename);
   if(b){
      inode_ptr file = currDir->getChild(filename);
      inode_t fileType = file->get_type();
      if(fileType==1){
         size_t fileSize = file->size();
         if(fileSize>2){
            throw yshell_exn(words.at(1) +
                     ": Directory is not empty, cannot remove"); 
            return;
         }
         currDir->deleteFile(filename);
      } else{
         currDir->deleteFile(filename);
      }
   } else{
      throw yshell_exn(words.at(1)+ ": No such file or directory"); 
   }
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words.size()<2){
      throw yshell_exn("No pathname specified");
      return;
   }
   
   inode_ptr currDir = state.get_cwd();
   string pathname = words.at(1);
   wordvec path = split(pathname, "/");
   bool fromRoot = false;
   if(pathname.at(0)=='/') fromRoot = true;
   string filename;
   int pathSize = path.size();

   // if we are removing files beginning with cwd
   if(pathSize==1){
      filename = path.at(0);
      bool b = currDir->findChild(filename);
      if(!b){
         throw yshell_exn(words.at(1)+ ": No such file or directory"); 
         return;
      }
      inode_ptr file = currDir->getChild(filename);
      
      file->deleteFileRec(currDir, file, filename);
      return;
   }

   // If we are removing a file from another directory
   //
   if(fromRoot) currDir = state.get_root();
   bool childChk = false;
   string wantedChild;
   // checking if path is correct
   for(int i=0; i<pathSize-1; ++i){
      wantedChild = path.at(i);
      childChk = currDir->findChild(wantedChild);
      if(childChk){
         currDir = currDir->getChild(wantedChild);
      }else{
         throw yshell_exn(words.at(1)+ ": Path does not exist"); 
         return;
      }
   }
   // if the path exists, check if the file exists in currDir, if not
   // print an error msg.
   filename = path.at(pathSize-1);
   bool b = currDir->findChild(filename);
   if(b){
      inode_ptr file = currDir->getChild(filename);
      file->deleteFileRec(currDir, file, filename);
      
   }else{
      throw yshell_exn(words.at(1)+": No such file or directory"); 
   }
    
}

int exit_status_message() {
   int exit_status = exit_status::get();
   cout << execname() << ": exit(" << exit_status << ")" << endl;
   return exit_status;
}

