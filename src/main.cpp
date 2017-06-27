#include <iostream>
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include "Downloader.h"
#include "main.h"

using namespace std;

//Initializing values in namespace
namespace global{
    string host = "";
    string key = "";
    string id = "";
}

int main(int argc, char *argv[]){
    global::host = "api.imgur.com";
    
    //Check for certain arguments
    for(int i = 0; i < argc; ++i){
        if(string(argv[i]) == "-c"){
            
            //The argument for compiling is the number of images to
            //TODO figure out how to use the object
        }else if(string(argv[i]) == "-g"){
            
            //Get the client key
            ifstream in(argv[i+1]);

            //Check if the file exists
            if(!in.good()){
                cout << "Must specify a key file" << endl;
                exit(1);
            }

            in >> global::key;
            
            //Get the album id
            ifstream idIn(argv[i+2]);

            //Check if the file exists
            if(!idIn.good()){
                cerr << "Must specify an Album ID" << endl;
                exit(1);
            }

            idIn >> global::id;

            //Object that handles the downloading of images from album with key "key"
            Downloader downloader(1);
            
            //Method to actually get Images
            downloader.storeLinks();
            downloader.startDownload();

        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "-g <key file> <id file>" << "\t" << "Grab the images from a certain imgur album" << endl;
            cout  << "-c" << "\t" <<  "[num] Compiles all of the images (already downloaded) into an epub file based on the given num" << endl;
        }

    }
}
