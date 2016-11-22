#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include"HTTPRequest.h"
#include<string>

class Downloader{
    public:
        Downloader(const string & key_, const string& id_);
        ~Downloader();
    private:
        string key_;
        string id_;
};
#endif
