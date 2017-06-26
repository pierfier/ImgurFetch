#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include<iostream>
#include<string>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

using namespace std;

class HTTPRequest{
	public:
        //Constructor
        HTTPRequest();
        HTTPRequest();
        

        //Send a request to grab the links for the images
        void requestLinks(string& response);
        
        
        //Grab an individual image with the link and write it to the given file
        void getImageToFile(const string& link, const string& fileName);

        //Destructor
        ~HTTPRequest();
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
        SSL_CTX * ctx_;
        string port_;
        BIO * bio_;
};

#endif
