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
#include "epubCompiler.h"

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
        if(string(argv[i]) == "-g"){
            
            //Get the client key
            ifstream in(argv[i+1]);

            //Check if the file exists
            if(!in.good()){
                string keyFile;
                cout << "Enter key file name: ";
                cin >> keyFile;
                in = ifstream(keyFile.c_str());
            }

            in >> global::key;
            
            //Get the album id
            ifstream idIn(argv[i+2]);

            //Check if the file exists
            if(!idIn.good()){
                string idFile;
                cout << "Enter album id: ";
                cin >> idFile;
                idIn = ifstream(idFile.c_str());
            }
            
            idIn >> global::id;

            //Get the starting number for the image names. 
            //If number not given start at 1 by default
            //Used if the same book is stored across different albums   
            int startImage = 1;
            
            //DEBUG
            cout << "argc (number of arguments): " << argc << endl;
            cout << "index for the starting image: " << i + 3 << endl;

            if(argc > i + 3){

                //Check if the next argument is a number
                if(isdigit(argv[i + 3][0])){
                    startImage = atoi(argv[i + 3]);
                }
            }

            //Object that handles the downloading of images from album with key "key"
            Downloader downloader(4);
            
            //Method to actually get Images
            downloader.storeLinks(startImage);
            downloader.startDownload();

        }

        // "-c" flag parses the list of images(relative path from main ), the book folder, the title of the book, and the author
        
        else if(string(argv[i]) == "-c"){
            vector<string> images;
            string bookDest;
            string imageSrc;
            string title;
            string author;
            
            //Check if more command line arguments were given
            if(argc < i + 4){
                
                cout << "Enter the book folder: ";
                cin >> bookDest;
                
                cout << "Enter the title of the book: ";
                cin >> title;
                
                cout << "Enter the author of the book: ";
                cin >> author;
                
                string im = string("c");
                
                //Keep reading in the images
                for(;im != "";){
                    cout << "Enter image file" << endl;
                    cin >> im;

                    if(im != string("")){
                        images.push_back(im);
                    }
                }

            }else{
                
                //Parse the commandline arguments into the variables
                bookDest = string(argv[i + 1]);
                title = string(argv[i + 2]);
                author = string(argv[i + 3]);
                
                //Grab image file names until there are no more
                for(int j = i + 4; j < argv; ++j){
                    images.push_back(string(argv[j]));
                }
            }
            
            //Initialize compiler object
            epubCompiler eCompiler(bookDest, title, author);
            
            for(int i = 0; i < images.size(); ++i){
                eCompiler.addImage(images[i]);
            }

        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "\n\t-g <key file> <id file>" << "\t" << "Grab the images from a certain imgur album" << endl << endl;
            cout  << "\t-c" << "\t" <<  "[num] <book destination> <title> <author>"; //TODO will need to change this
            cout << " Compiles all of the images (already downloaded) into an epub file based on the given number" << endl;
        }

    }
}
