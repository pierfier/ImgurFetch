#include "Downloader.h"

void Downloader::~Downloader(){
    delete request_;
}

void Downloader::Downloader(const string& key, const string & id) : 
                                           key_(key), id_(id) {
    request_ = new HTTPRequest(key, id);
}
