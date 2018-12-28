#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <string>
#include <vector>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "HTTPRequest.h"

using namespace std;

class HTTPRequest {
	public:
        //Constructor
        HTTPRequest() {};

        //Header information for an individual image
        string conImageRequest(const string& link);

        //grabs the content length from the header
        int parseContentLength(const string& header); //Primarily to get the size of the image

        //Send a request to grab the links for the images
        string conLinksRequest();
 
        //Used to parse the links from the response
        vector<string> parseLinks(const string& response);
};

#endif
