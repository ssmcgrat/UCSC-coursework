// Author: Sean McGrath, ssmcgrat@ucsc.edu
// $Id: main.cpp,v 1.8 2015-04-28 19:23:13-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

const string cin_name = "-";

class keypair_quit: public exception{};
void do_quit (str_str_map&, const char){
   throw keypair_quit();
}


// compares two paramaters of the same type passed in lexicographically 
// and prints the results. 
//
template <typename Type, class Less=xless<Type>>
void compare(const Type &left, const Type &right){
   Less less;
   bool is_less = less (left, right);
   bool is_equal = !less (left, right) && !less(right, left);
   bool is_greater = not is_less and not is_equal;
   cout << left << " < " << right << " = " << is_less << endl;
   cout << left << " == " << right << " = " << is_equal << endl;
   cout << left << " > " << right << " = " << is_greater << endl;
   cout << endl; 
}

// trim the leading and trailing whitespace of the string str
//
void trim(string& str){
   string trim = "";
   size_t first = str.find_first_not_of(" ");
   size_t last = str.find_last_not_of(" ");
   for(size_t i=first; i<=last; ++i){
      trim += str.at(i);
   }

   str = trim;

     
}

// Display contents of a file
void catfile (istream& infile, const string& filename, 
                                      str_str_map& map){
   static string colons (32, ':');
   cout << colons << endl << filename << endl << colons << endl;
   int count = 0;
   size_t equal;
   size_t first_not_white;
   size_t last_not_white;
   const char comment = '#';
   string key;
   string value;
     
 
   for(;;){
      // Print out the current line of the file
      string line; 
      getline (infile, line);
      ++count;
      if (infile.eof()) break;
      cout << filename << ": " << count << ": "<< line << endl;
  
      // Find first and last non-whitespace characters
      first_not_white = line.find_first_not_of(" ");
      last_not_white  = line.find_last_not_of(" ");

      // If no non-whitespace characters, do nothing
      if (first_not_white==string::npos)   continue; 

      // Check if the line is a comment, if so do nothing.
      if(line.at(first_not_white)==comment)  continue; 

      // find first occurence of '='.
      equal = line.find_first_of("=", 0); 

      // If there is an '=' in the line
      if(equal != string::npos){
         // If no key specified
         if(equal == first_not_white){
            // If no value specified
            if(equal == last_not_white){
               //cout << "print all key and value pairs" << endl;
               map.print_pairs();
            }else{
            // There is a value
               value = line.substr((equal+1), last_not_white);
               trim(value);
               //cout << "print all keys with value of: " << value << endl;
               map.print_keys(value);
            }
         } else{
            // There is a key
            key = line.substr(0, equal);
            trim(key);  
            if(equal == last_not_white){
               //cout << "delete the key '" << key << "' and its value" << endl;
               auto it = map.find(key);
               if(!it.is_null()) map.erase(it);
            }else{
               value = line.substr(equal+1, last_not_white-1);
               trim(value);
               //cout << "set the key '" << key << "' and its value '";
               //cout << value << "' in the map" << endl;
               
               str_str_pair str_pair (key, value);  
               //cout << "Before insert: " << str_pair << endl;
              
               auto it = map.insert(str_pair);
               cout << it->first << " = " << it->second << endl;
            }
         }
      }else{
         key = line.substr(first_not_white, last_not_white+1);
         trim(key);
         //cout << "print the key '" << key << "' and its value" << endl;
         auto it = map.find(key);
         
         if(it.is_null()) cout << key << ": key not found" << endl;
         else { 
            cout << it->first << " = " << it->second << endl;
         } 
          
      }       
      
      
      
      
   }
}

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            traceflags::setflags (optarg);
            break;
         default:
            complain() << "-" << (char) optopt << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   int status = 0;
   sys_info::set_execname (argv[0]);
   scan_options (argc, argv);
   
   str_str_map test;
    
   string progname = argv[0];
   bool cinchk = true;
    
    
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      cinchk = false;
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      const auto& filename = *argp;
      if(filename==cin_name){
         catfile(cin, cin_name, test);
         continue;
      }
      ifstream infile (filename); 
      if(infile.fail()){
         status = 1;
         cerr << progname << ": " << filename << ": "
              << "could not open for reading" << endl;
      }else {
         catfile (infile, filename, test);
         infile.close();
      }
      test.insert (pair);
   }

   if(cinchk) catfile(cin, cin_name, test);
   
   str_str_map::iterator itor = test.begin();
   
   if(!itor.is_null()){ 
      while(itor != test.end()){
         str_str_map::iterator old = itor; 
         ++itor;
         test.erase (old);
      } 

   }
      //str_str_map::iterator itor = test.begin();
      //test.erase (itor);
      
   return status;
}

