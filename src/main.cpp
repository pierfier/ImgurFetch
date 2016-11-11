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


using namespace std;

//Loop through the response to get all of the links and download
//Each image
void getImages(const string& response, BIO * bio, const string& id){
    
    //Find the links for the images and then download them
    string link;
    
    //Buffer for reading in data
    char cur;

    //Index to indicate progress along the album xml structure
    int i = 0;
    
    //Stores the number of images that were downloaded
    int numIm = 0;

    //Loop through the xml and download the images from each link
    while( (i = response.find("link", i)) != string::npos /*&& numIm < 10*/){ //TODO here is the condition that limits number of images downloaded
        ++numIm;

        i = response.find("com", i);
        i += 4;

        link = "";
        for(;response[i] != '<'; ++i){
            link += response[i];
        }

        //DEBUG
        cout << link << endl;

        //Setup request to grab image
        
        string request = "GET /" + link;
        
        request += " HTTP/1.1";

        request += "\r\nHost: i.imgur.com\r\n";
        
        request += "Authorization: Client-ID " + id + "\r\n";
        
        request += "Connection: keep-alive\r\n\r\n";

        //Send request
        if ( BIO_write(bio, request.c_str(), request.size()) <= 0) {
            cout << "Write Failed" << endl;
            exit(1);
        }

        BIO_flush(bio);

        stringstream ss;
        ss << numIm;

        //Read in the response
        //And then write to image file
        ofstream out(("image" + ss.str()  + ".jpg").c_str(), ofstream::binary);
         
        string header;
        
        getHeader(bio, header);

        int length = getContentLength(header);
 
        //DEBUG
        //cout << header << endl;
        //cout << length << endl;
        //exit(1);


        //Write image to file
        for(int i = 0; i < length; ++i){
            BIO_read(bio, &cur, 1);
            out.write(&cur, 1);
        }
    }
}

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
