#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "SSL_Handler.h"
#include "main.h"

using namespace std;

//Default Constructor
SSL_Handler::SSL_Handler(){
    port_ = string("443");        
    init();
}

/* @param response is how the resulting xml is saved
 * This method returns the response from a field search
*/
vector<string> SSL_Handler::requestLinks(){
    //Connection has been established
    //Searching can now be done
    
    string request = http_request_.conLinksRequest();

    if ( BIO_write(bio_, request.c_str(), request.size()) <= 0) {
        cout << "Album request failed" << endl;
        exit(1);
    }
    
    BIO_flush(bio_);
    
    //Read in the response
    char cur;
    int length;
 
    string header, response;

    readInHeader(header);

    length = http_request_.parseContentLength(header);

    //Read in the xml
    for(int i = 0; i < length; ++i){
        BIO_read(bio_, &cur, 1);
        response += cur;
    }

    // Parse the links into a list and pass them up to the Downloader object
    return http_request_.parseLinks(response);
}

//Download the individual image file and write it to
//local image file
void SSL_Handler::getImageToFile(const string& link, 
                                 const string& fileName){
    char cur;

    //Construct the request to grab an individual image    
    string request = http_request_.conImageRequest(link);
    
    //Send request
    if ( BIO_write(bio_, request.c_str(), request.size()) <= 0) {
        cout << "Unable to get image at link: " << link << endl;
        exit(1);
    }

    BIO_flush(bio_);


    string header;
    readInHeader(header);

    int length = http_request_.parseContentLength(header);
 
    //Read in the response
    //And then write to image file
    ofstream out(fileName.c_str(), std::ofstream::binary);
    
    //Write image to file
    for(int i = 0; i < length; ++i){
        BIO_read(bio_, &cur, 1);
        out.write(&cur, 1);
    }

    out.close();
}

//Gets the response header
void SSL_Handler::readInHeader(string& header){

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
void SSL_Handler::init(){
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
SSL_Handler::~SSL_Handler(){
    BIO_free_all(bio_);
    SSL_CTX_free(ctx_);
}
