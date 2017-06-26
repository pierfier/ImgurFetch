<<<<<<< HEAD
#include <iostream>
#include <fstream>
#include <sstream>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "HTTPRequest.h"
#include "main.h"

using namespace std;

//Default Constructor
HTTPRequest::HTTPRequest(){
    port_ = string("443");        
    init();
}

//Initial Values Constructor
HTTPRequest::HTTPRequest(){
    
    port_ = string("443");    
    init();    
}

/* @param response is how the resulting xml is saved
 * This method returns the response from a field search
*/
void HTTPRequest::requestLinks(string& response){
    //Connection has been established
    //Searching can now be done
    
    string request;
    
    //Write the GET header
    request = "GET /3/album/" + global::key + "/images.xml";

    request += " HTTP/1.1\r\n";

    request += "Host: api.imgur.com\r\n";

    request += "Authorization: Client-ID " + global::id + "\r\n";

    request += "Connection: alive\r\n\r\n";

    //DEBUG
    cout << request;
    if ( BIO_write(bio_, request.c_str(), request.size()) <= 0) {
        cout << "Album request failed" << endl;
        exit(1);
    }
    
    BIO_flush(bio_);
    
    //Read in the response
    char cur;
    int length;
 
    string header;

    readInHeader(header);

    length = parseContentLength(header);

    //Read in the xml
    for(int i = 0; i < length; ++i){
        BIO_read(bio_, &cur, 1);
        response += cur;
    }
    
    //cout << response << endl;
}


//This method returns the content length of the http response.
//The bio_ should have read up to \n\n 
int HTTPRequest::parseContentLength(const string& header){

    int i = header.find("Content-Length: ");
    i+= 16;
    
    string num;
    
    while(header[i] != '\n'){
        num += header[i];
        ++i;
    }

    return atoi(num.c_str());
}

//Download the individual image file and write it to
//local image file
void HTTPRequest::getImageToFile(const string& link, 
                                 const string& fileName){
    string request;

    //Setup request to grab image    
    string request = "GET " + link.substr(link.find_last_of("/"), string::npos);
       
    request += " HTTP/1.1\r\n";
    
    request += "Host: i.imgur.com\r\n";
      
    request += "Authorization: Client-ID " + global::id + "\r\n";
      
    request += "Connection: keep-alive\r\n\r\n";

    //DEBUG
    cout << request << endl;

    //Send request
    if ( BIO_write(bio_, request.c_str(), request.size()) <= 0) {
        cout << "Unable to get image at link: " << link << endl;
        exit(1);
    }

    BIO_flush(bio_);

    stringstream ss;
    ss << numIm;

    string header;
    
    readInHeader(header);

    int length = parseContentLength(header);
 
    //DEBUG
    //cout << header << endl;
    //cout << length << endl;
    //exit(1);

    //Read in the response
    //And then write to image file
    ofstream out(fileName.c_str(), std::ofstream::binary);
    
    //Write image to file
    for(int i = 0; i < length; ++i){
        BIO_read(bio_, &cur, 1);
        out.write(&cur, 1);
    }
}

//Gets the response header
void HTTPRequest::readInHeader(string& header){

    char cur;
    while (BIO_read(bio_, &cur, 1) > 0 ){
        header += cur;

        if(cur == '\r'){
            BIO_read(bio_, &cur, 1);
            header += cur;
            
            if(cur == '\n'){
                BIO_read(bio_, &cur, 1);
                header += cur;
                
                if(cur == '\r'){
                    BIO_read(bio_, &cur, 1);
                    break;
                }
            }
        }
    }
}

//Initialize the SSl environment
void HTTPRequest::init(){
    //Initialize SSL
    ERR_load_crypto_strings();
    ERR_load_SSL_strings();
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();    
    OpenSSL_add_all_algorithms();

    //Setting up SSL pointers
    ctx_ = SSL_CTX_new(SSLv23_client_method());
    SSL * ssl;

    //Loading the certificates
    if(!SSL_CTX_load_verify_locations(ctx_, /*"UbuntuOne-Go_Daddy_CA.pem",*/ NULL, "/etc/ssl/certs/")){
        cerr << "Loading the certs file failed" << endl;
        exit(1);
    }
    

    //Setup the BIO
    bio_ = BIO_new_ssl_connect(ctx_);
    BIO_get_ssl(bio_, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
 
    //Check ssl pointer
    if(!ssl){
        cerr << "Cannot find ssl pointer" << endl;
        exit(1);
    }

    //Attempts a connection
    BIO_set_conn_hostname(bio_, (global::host + ":" + port_).c_str());

    //Check connection
    if(BIO_do_connect(bio_) <= 0){
        cerr << "Connection Failed" << endl;
        BIO_free_all(bio_);
        SSL_CTX_free(ctx_);
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
    if(BIO_do_handshake(bio_) <= 0){
        cerr << "Cannot establish SSL connection" << endl;
        exit(1);
    }
}

//Clean SSL context
HTTPRequest::~HTTPRequest(){
    BIO_free_all(bio_);
    SSL_CTX_free(ctx_);
}
