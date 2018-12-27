#ifndef SSLHANDLER_HPP
#define SSLHANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "HTTPRequest.h"

using namespace std;

class SSL_Handler{
	public:
        //Constructor
        SSL_Handler();

        //Send a request to grab the links for the images
        vector<string> requestLinks();
        
        
        //Grab an individual image with the link and write it to the given file
        void getImageToFile(const string& link, const string& fileName);

        //Destructor
        ~SSL_Handler();
    private:
        //Methods

        //grabs the content length from the header
        int parseContentLength(const string& header); //Primarily to get the size of the image
        
        //Sets up all of the SSL environment variables
        void init();
        
        //Reads in response header and writes it to the header variable
        //Only to be used after a request has been sent
        void readInHeader(string& header);
        
        //Variables
        HTTPRequest http_request_;
        SSL_CTX * ctx_;
        string port_;
        BIO * bio_;
};

#endif
