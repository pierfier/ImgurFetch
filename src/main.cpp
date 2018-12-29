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

        //Argument for fetching images from imgur
        if(string(argv[i]) == "-g"){
            
            //Get the client key file name
            ifstream in(argv[i+1]);

            //Check if the file exists
            //Otherwise prompt for the file name at the commandline
            if(!in.good()){
                string keyFile;
                cout << "Enter key file name: ";
                cin >> keyFile;
                in = ifstream(keyFile.c_str());
            }
            
            in >> global::key;

            //Get the album id

            // Check that there is a next argument and that it is not another argument
            if(argc <= i + 2 || string(argv[i+2])[0] == '-'){
                cout << "Need to specify an album id: " << endl;
                cin >> global::id;
            }else{
                global::id = string(argv[i+2]);
            }

            //Get the starting number for the image names. 
            //If number not given start at 1 by default
            //Used if the same book is stored across different albums   
            int startImage = 1;
            string desFolder = string("res/");

            //DEBUG
            cout << "argc (number of arguments): " << argc << endl;
            cout << "index for the starting image: " << i + 3 << endl;

            //Check that there is a valid argument for the starting image
            if(argc > i + 3 && isdigit(argv[i + 3][0]) ){
                startImage = atoi(argv[i + 3]);
            }

            //Check for valid argument for the destination folder for the images
            if(argc > i + 4){
                desFolder = string(argv[i+4]);
            }

            //Object that handles the downloading of images from album with key "key"
            Downloader downloader(4, desFolder);
            
            //Method to actually get Images
            downloader.storeLinks(startImage);
            downloader.startDownload();

        }

        // "-c" flag parses the list of images, the book folder, the title of the book, and the author
        else if(string(argv[i]) == "-c"){
            //New Approach use a file to parse all of the information
            //Parse the file
            
            ifstream inConfig(string(argv[i+1]));

            vector<string> images;
            string bookDest;
            string imageSrc;
            string title;
            string author;
            int numImages = 0;

            //Check if more command line arguments were given
            if(argc < i + 4){
                string im = string("c");
                
                //Keep reading in the images
                while(im != ""){
                    cout << "Enter the number of the starting image file:" << endl;
                    cin >> numImages;
                }
                
                cout << "Enter the book folder: ";
                cin >> bookDest;
                
                cout << "Enter the title of the book: ";
                cin >> title;
                
                cout << "Enter the author of the book: ";
                cin >> author;

            }else{
                
                //Parse the commandline arguments into the variables
            }
            
            //Initialize compiler object
            epubCompiler eCompiler(bookDest, title, author);
            string file;

            for(int i = 0; i < numImages; ++i){
                stringstream ss;
                ss << (i + 1);

                file = "images/image" + ss.str();

                eCompiler.addImage(file, i);
            }

        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "\n\t-g <key file> <id>" << "\t" << "Grab the images from a certain imgur album" << endl << endl;
            cout  << "\t-c [num] <book destination> <title> <author>\t"; 
            cout << " Compiles all of the images (already downloaded) into an epub file based on the given number" << endl;
        }

    }
}
