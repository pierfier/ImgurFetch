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

//Generates the epubfile
//This is used to assume that all of the images are downloaded
void compileEPUB(int length){
    string title = "";

    cout << "Enter the name for the pdf: ";

    cin >> title;
    //Create Comic Title name

    ofstream out(string(title + ".tex").c_str(), fstream::app);
    
    for(int i = 1; i < length; ++i){
        out << "\\includegraphics[scale=.2]{image" << i << ".jpg}" << endl;   
    }
    
    //End the tex document
    out << "\\end{document}" << endl;

    //TODO need to learn the actual methods for starting up processes
    system(string("pdflatex  " + title + ".tex").c_str());
}


int main(int argc, char *argv[]){
    global::host = "api.imgur.com";
    //Check for certain arguments
    for(int i = 0; i < argc; ++i){
        if(string(argv[i]) == "-c"){
            
            //The argument for compiling is the number of images to
            compileEPUB(atoi(argv[i+1]));
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
            downloader->getImages();

        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "-g" << "\t" << "Grab the images from a certain imgur album" << endl;
            cout  << "-c" << "\t" <<  "[num] Compiles all of the images (already downloaded) into an epub file based on the given num" << endl;
        }

    }
}
