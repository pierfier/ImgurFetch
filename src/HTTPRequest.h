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
        HTTPRequest(const string& key, const string& id);
		
        int getContentLength(const string& header);
        void getHeader(string& header);
        void requestResponse(string& response);
        void init();

        //Destructor
        ~HTTPRequest();
    private:
    	string key_;
    	string ID_;
        SSL_CTX ctx_;
        string port_;
};

#endif
