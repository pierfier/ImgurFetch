#include <iostream>
#include <string>
#include <vector>
#include "HTTPRequest.h"
#include "main.h"

using namespace std;


string HTTPRequest::conImageRequest(const string& link){
    string request = "GET " + link.substr(link.find_last_of("/"), string::npos);

    request += " HTTP/1.1\r\n";
    
    request += "Host: i.imgur.com\r\n";
      
    request += "Connection: keep-alive\r\n\r\n";
    
    return request;
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

string HTTPRequest::conLinksRequest(){
    string request;

    //Write the GET header
    request = "GET /3/album/" + global::id + "/images.xml";

    request += " HTTP/1.1\r\n";

    request += "Host: api.imgur.com\r\n";

    request += "Authorization: Client-ID " + global::key + "\r\n";

    request += "Connection: alive\r\n\r\n";

    return request;
}

vector<string> HTTPRequest::parseLinks(const string& response){
    vector<string> links;
    string tempLink;
    int index = 0;

    //Find each link tag and 
    while((index = response.find("<link>", index)) != string::npos){
        index += 6;

        tempLink = ""; 

        //Fully read in the link
        for(; response[index] != '<'; ++index){
            tempLink += response[index];
        }
        
        links.push_back(tempLink);
    }
}
