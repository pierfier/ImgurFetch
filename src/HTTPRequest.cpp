#include<iostream>
#include"HTTPRequest.h"

using namespace std;

//This method returns the content length of the http response.
//The bio should have read up to \n\n 
int HttPRequest::getContentLength(const string& header){
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
void HttPRequest::getHeader(string& header){
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
    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL * ssl;

    //Loading the certificates
    if(!SSL_CTX_load_verify_locations(ctx, /*"UbuntuOne-Go_Daddy_CA.pem",*/ NULL, "/etc/ssl/certs/")){
        cerr << "Loading the certs file failed" << endl;
        exit(1);
    }
    

    //Setup the BIO
    bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
 
    //Check ssl pointer
    if(!ssl){
        cerr << "Cannot find ssl pointer" << endl;
        exit(1);
    }

    //Attempts a connection
    BIO_set_conn_hostname(bio, (host + ":" + PORT + "asdfas").c_str());

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
}

//Clean SSL context
void HTTPRequest::~HTTPRequest(){
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}

/* This method returns the response from a field search
 *
*/
void HTTPRequest::requestResponse(string& response){
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
