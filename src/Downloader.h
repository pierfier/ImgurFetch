#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include"HTTPRequest.h"
#include<string>

class Downloader{
    public:
        Downloader(const string & key_, const string& id_);
        void getImages();
        ~Downloader();
    private:
        string key_;
        string id_;
        HTTPRequest * request_;
};
#endif
