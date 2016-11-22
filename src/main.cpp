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
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

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
    
    //Check for certain arguments
    for(int i = 0; i < argc; ++i){
        if(string(argv[i]) == "-c"){
            
            //The argument for compiling is the number of images to
            compileEPUB(atoi(argv[i+1]));
        }else if(string(argv[i]) == "-g"){
            
            bio = init(ctx);
            
            string album = "";
            string response = "";
     		
            //Get the Client ID
            ifstream in("ID.txt");

            string id;

            in >> id;
            //Prompt for album ID
            cout << "Enter the album ID: ";

            cin >> album;
            
            HTTPRequest * request = new HTTPRequest(key, ID);

            //Request response
            requestResponse(bio, id, album, response);

            //------------------------ Code to Compile images into a pdf ---------------
  
            getImages(response, bio, id);        
            
            
        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "-g" << "\t" << "Grab the images from a certain imgur album" << endl;
            cout  << "-c" << "\t" <<  "[num] Compiles all of the images (already downloaded) into an epub file based on the given num" << endl;
        }

    }
}
