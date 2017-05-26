#include<iostream>
#include"HTTPRequest.h"

using namespace std;

//This method returns the content length of the http response.
//The bio should have read up to \n\n 
int HttpRequest::getContentLength(const string& header){
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
void HttpRequest::getHeader(BIO * bio, string& header){
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
void HttpRequest::requestResponse(BIO * bio, string id, string& album, string& response){
     //Connection has been established
    //Searching can now be done
    
    string request;
    
    //Write the GET header
    request = "GET /3/album/" + album + "/images.xml";

    request += " HttP/1.1";

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
