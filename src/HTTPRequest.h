#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include<iostream>
#include<string>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

using namespace std;

class HttpRequest{

    // Member functions
    int getContentLength(const string& header);
    void getHeader(BIO* bio, string& header);
    void requestResponse(BIO * bio, string id, string& album, string& response);

};

#endif
