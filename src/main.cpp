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

int main(int argc, char *argv[]){
    global::host = "api.imgur.com";
    
    //Check for certain arguments
    for(int i = 0; i < argc; ++i){
        if(string(argv[i]) == "-c"){
            
            //The argument for compiling is the number of images to
            //TODO figure out how to use the object
        }else if(string(argv[i]) == "-g"){
            
            //Get the Client ID
            ifstream in(argv[i+1]);

            //Check if the file exists
            if(!in.good()){
                cout << "No file found. There must be a key.txt in directory res/" << endl;
                exit(1);
            }

            in >> global::key;
            
            //This is the application's id
            ifstream idIn(argv[i+2]);

            idIn >> global::id;

            //Object that handles the downloading of images from album with key "key"
            Downloader * downloader = new Downloader();
            
            //Method to actually get Images
            downloader->storeLinks();
            downloader->startDownload();

        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "-g" << "\t" << "Grab the images from a certain imgur album" << endl;
            cout  << "-c" << "\t" <<  "[num] Compiles all of the images (already downloaded) into an epub file based on the given num" << endl;
        }

    }
}
