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

string PORT("443");

//This method returns the content length of the http response.
//The bio should have read up to \n\n 
int getContentLength(const string& header){
    int i = header.find("Content-Length: ");
    i+= 16;
    
    string num;
    
    while(header[i] != '\n'){
        num += header[i];
        ++i;
    }

    return atoi(num.c_str());
}

//Gets the response header
void getHeader(BIO * bio, string& header){
    char cur;
    while (BIO_read(bio, &cur, 1) > 0 ){
        header += cur;

        if(cur == '\r'){
            BIO_read(bio, &cur, 1);
            header += cur;
            
            if(cur == '\n'){
                BIO_read(bio, &cur, 1);
                header += cur;
                
                if(cur == '\r'){
                    BIO_read(bio, &cur, 1);
                    break;
                }
            }
        }
    }
}

/* This method returns the response from a field search
 *
*/
void requestResponse(BIO * bio, string id, string& album, string& response){
     //Connection has been established
    //Searching can now be done
    
    string request;
    
    //Write the GET header
    request = "GET /3/album/" + album + "/images.xml";

    request += " HTTP/1.1";

    request += "\r\nHost: api.imgur.com\r\n";
    
    request += "Authorization: Client-ID " + id + "\r\n";

    request += "Connection: alive\r\n\r\n";

    if ( BIO_write(bio, request.c_str(), request.size()) <= 0) {
        cout << "Write Failed" << endl;
        exit(1);
    }
    BIO_flush(bio);
    
    //Read in the response

    char cur;
    int length;
 
    string header;

    getHeader(bio, header);

    length = getContentLength(header);

    //Read in the xml
    for(int i = 0; i < length; ++i){
        BIO_read(bio, &cur, 1);
        response += cur;
    }

    //cout << response << endl;
}

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

//Initialize the SSl environment
BIO* init(SSL_CTX *& ctx){
    //Initialize SSL
    ERR_load_crypto_strings();
    ERR_load_SSL_strings();
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();    
    OpenSSL_add_all_algorithms();

    //Setting up SSL pointers
    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL * ssl;

    //Loading the certificates
    if(!SSL_CTX_load_verify_locations(ctx, /*"UbuntuOne-Go_Daddy_CA.pem",*/ NULL, "/etc/ssl/certs/")){
        cerr << "Loading the certs file failed" << endl;
        exit(1);
    }
    

    //Setup the BIO
    BIO * bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
 
    //Check ssl pointer
    if(!ssl){
        cerr << "Cannot find ssl pointer" << endl;
        exit(1);
    }

    //Attempts a connection
    BIO_set_conn_hostname(bio, ("imgur.com:" + PORT + "asdfas").c_str());

    //Check connection
    if(BIO_do_connect(bio) <= 0){
        cerr << "Connection Failed" << endl;
        BIO_free_all(bio);
        SSL_CTX_free(ctx);
        exit(1);
    }

    //Check certificate verification
    if(SSL_get_verify_result(ssl) != X509_V_OK){
        cout << "Connection not secured. Do you want to continue[Y/n]?" << endl;
        char ans;
        cin >> ans;
    
        if(ans == 'n'){
            exit(1);
        }
    }
    
    //Handshake
    if(BIO_do_handshake(bio) <= 0){
        cerr << "Cannot establish SSL connection" << endl;
        exit(1);
    }
    return bio;
}

int main(int argc, char *argv[]){
    
    //Check for certain arguments
    for(int i = 0; i < argc; ++i){
        if(string(argv[i]) == "-c"){
            
            //The argument for compiling is the number of images to
            compileEPUB(atoi(argv[i+1]));
        }else if(string(argv[i]) == "-g"){
            SSL_CTX * ctx;
            BIO * bio;
            
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
            
            //Clean SSL context
            BIO_free_all(bio);
            SSL_CTX_free(ctx);
        }else if(string(argv[i]) == "--help"){
            cout << "Usage: " << endl;
            cout  << "-g" << "\t" << "Grab the images from a certain imgur album" << endl;
            cout  << "-c" << "\t" <<  "[num] Compiles all of the images (already downloaded) into an epub file based on the given num" << endl;
        }

    }
}
