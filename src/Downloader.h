#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include"HTTPRequest.h"
#include<string>

class Downloader{
    public:
        void getImages();
        Downloader();
        ~Downloader();
    private:
        string key_;
        string id_;
        BIO * bio_;
};
#endif
