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
        if(string(argv[i]) == "-c"){
            int numImages;
            string bookDest;
            string imageSrc;
            string title;
            string author;

            //Check if more command line arguments were given
            if(argc < i + 4){
                cout << "Enter number of images in book: ";
                cin >> numImages;
                
                cout << "Enter the book folder: ";
                cin >> bookDest;
                
                cout << "Enter the title of the book: ";
                cin >> title;
                
                cout << "Enter the author of the book: ";
                cin >> author;

            }else{
                numImages = atoi(argv[i+1]);
                bookDest = string(argv[i+2]);
                title = string(argv[i+3]);
                author = string(argv[i+4]);               
            }
            
            epubCompiler eCompiler(bookDest, title, author);
            string file;

            for(int i = 0; i < numImages; ++i){
                stringstream ss;
                ss << (i + 1);

                file = bookDest + "/OEBPS/images/" + "image" + ss.str() + ".jpg";

                eCompiler.addImage(file);
            }

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
            Downloader downloader(4);
            
            //Method to actually get Images
            downloader.storeLinks();
            downloader.startDownload();

        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "-g <key file> <id file>" << "\t" << "Grab the images from a certain imgur album" << endl;
            cout  << "-c" << "\t" <<  "[num] <book destination> <title> <author>"; 
            cout << "Compiles all of the images (already downloaded) into an epub file based on the given num" << endl;
        }

    }
}
