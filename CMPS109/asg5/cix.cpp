// Author: Sean McGrath, ssmcgrat@ucsc.edu
//
// $Id: cix.cpp,v 1.2 2015-08-08 13:35:15-07 - - $

#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream log (cout);
struct cix_exit: public exception {};

unordered_map<string,cix_command> command_map {
   {"exit", CIX_EXIT},
   {"help", CIX_HELP},
   {"ls"  , CIX_LS  },
   {"get" , CIX_GET },
   {"put" , CIX_PUT },
   {"rm"  , CIX_RM  },
};

void cix_help() {
   static vector<string> help = {
      "exit         - Exit the program.  Equivalent to EOF.",
      "get filename - Copy remote file to local host.",
      "help         - Print help summary.",
      "ls           - List names of files on remote server.",
      "put filename - Copy local file to remote host.",
      "rm filename  - Remove file from remote server.",
   };
   for (const auto& line: help) cout << line << endl;
}

void cix_ls (client_socket& server) {
   cix_header header;
   header.command = CIX_LS;
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if (header.command != CIX_LSOUT) {
      log << "sent CIX_LS, server did not return CIX_LSOUT" << endl;
      log << "server returned " << header << endl;
   }else {
      char buffer[header.nbytes + 1];
      recv_packet (server, buffer, header.nbytes);
      log << "received " << header.nbytes << " bytes" << endl;
      buffer[header.nbytes] = '\0';
      cout << buffer;
   }
}

void cix_rm (client_socket& server, const string& filename) {
   cix_header header;
   header.command = CIX_RM;
   strcpy(header.filename, filename.c_str()); 
   log << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   log << "received header " << header << endl;
   if(header.command != CIX_RM) {
      log << "sent CIX_RM, server did not return CIX_RM" << endl;
      log << "server returned " << header << endl;
   }else {
      log << "received " << header.nbytes << " bytes" << endl; 
   }
}

void cix_put (client_socket& server, const string& filename) {
   cix_header header;
   header.command = CIX_PUT;

   // open the file from cd, operations performed in binary mode
   ifstream file (filename, ifstream::binary);
   
   // Check if the file opened successfully
   if(file.is_open()) {
      // get length of file
      file.seekg(0, file.end);
      int length = file.tellg();
      file.seekg(0, file.beg);

      char* buffer = new char[length];

      // read data into buffer
      file.read(buffer, length);
      file.close(); 
      buffer[length] = '\0';
      // send a packet with the name of the file
      header.nbytes = length;
      strcpy(header.filename, filename.c_str()); 
      send_packet (server, &header, sizeof header);
      log << "sending header " << header << endl;
      memset (header.filename, 0, FILENAME_SIZE);
      
      // send a packet with the contents of the file
      header.nbytes = length; 
      send_packet (server, buffer, length);
      log << "sent " << length << " bytes" << endl;
    
      // recieve an ACK or NAK from server 
      recv_packet (server, &header, sizeof header);
      log << "received header " << header << endl;
      if(header.command == CIX_NAK){
         log << "server returned " << header << endl;
      } else if (header.command == CIX_ACK) {
         log << "server returned " << header << endl;
      }   
   } else {
      log << "put failed: " << filename << ": " ;
      log << strerror (errno)  << endl;
   }   
}

void cix_get (client_socket& server, const string& filename) {
   cix_header header;
   header.command = CIX_GET;
   strcpy(header.filename, filename.c_str());   

   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   if(header.command != CIX_FILE) {
      log << "sent CIX_GET, server did not return CIX_FILE" << endl;
      log << "server returned " << header << endl;
   } else {
      log << "received " << header.nbytes << " bytes" << endl;
      char buffer[header.nbytes+1];
      recv_packet (server, buffer, header.nbytes);
      buffer[header.nbytes] = '\0';

      //open or create a new file
      ofstream file (filename, ifstream::binary);
      file.write(buffer, sizeof buffer);
      file.close();
   }
}

void usage() {
   cerr << "Usage: " << log.execname() << " [host] [port]" << endl;
   throw cix_exit(); 
}

vector<string> split (const string& line, const string& delimiters) {
   vector<string> words;
   size_t end = 0;
   for (;;){
      size_t start = line.find_first_not_of (delimiters, end);
      if(start == string::npos) break;
      end = line.find_first_of (delimiters, start);
      words.push_back (line.substr (start, end - start));
   }
   return words;
}

int main (int argc, char** argv) {
   log.execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   in_port_t port = 65535;
   string host = "localhost";
   if(args.size()==1) {
      port = get_cix_server_port (args, 0);
   } else{
      host = get_cix_server_host (args, 0);
      port = get_cix_server_port (args, 1);
   }
   log << to_string (hostinfo()) << endl;
   try {
      log << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      log << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cix_exit();
         vector<string> vs = split(line, " \t");
         log << "command " << line << endl;
         const auto& itor = command_map.find (vs.at(0));
         cix_command cmd = itor == command_map.end()
                         ? CIX_ERROR : itor->second; 
         switch (cmd) {
            case CIX_EXIT:
               throw cix_exit();
               break;
            case CIX_HELP:
               cix_help();
               break;
            case CIX_LS:
               cix_ls (server);
               break;
            case CIX_RM:
               cix_rm (server, vs.at(1));
               break;
            case CIX_PUT:
               cix_put (server, vs.at(1));
               break;
            case CIX_GET:
               cix_get (server, vs.at(1));
               break;
            default:
               log << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

