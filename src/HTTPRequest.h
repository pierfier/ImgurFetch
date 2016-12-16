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
        HTTPRequest(const string& key, const string& id, const string& host);
		
        //grabs the content length from the header
        int getContentLength(const string& header);

        //Reads in response header and writes it to variable header
        //Only to be used after a request has been sent
        void getHeader(string& header);

        //Uses the response to grab links and download the images
        void getImages(const string& response);

        //Send a request to grab the links for the images
        void requestLinks(string& response);
        
        //Sets up all of the SSL environment variables
        void init();

        //Destructor
        ~HTTPRequest();
    private:
    	string key_;
        string host_;
    	string ID_;
        SSL_CTX * ctx_;
        string port_;
        BIO * bio_;
};

#endif
