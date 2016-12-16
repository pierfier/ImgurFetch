#include "Downloader.h"
#include <string>

using namespace std;

Downloader::~Downloader(){
    delete request_;
}

//Uses a single HTTPRequest object to download the images
void Downloader::getImages(){
    string linksXML;
    request_->requestLinks(linksXML);
    request_->getImages(linksXML);
}

Downloader::Downloader(const string& key, const string & id, const string & host) : 
                                           key_(key), id_(id), host_(host) {
    request_ = new HTTPRequest(key, id, host);
}
