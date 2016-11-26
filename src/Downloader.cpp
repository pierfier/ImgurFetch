#include "Downloader.h"
#include <string>

using namespace std;

Downloader::~Downloader(){
    delete request_;
}

//Uses a single HTTPRequest object to download the images
void Downloader::getImages(){
    string linksXML;
    request->requestLinks(linksXML);
    request->getImages(linksXML);
}

Downloader::Downloader(const string& key, const string & id) : 
                                           key_(key), id_(id) {
    request_ = new HTTPRequest(key, id);
}
