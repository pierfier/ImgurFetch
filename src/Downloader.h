#ifndef DOWNLOADER_H
#define DOWNLOADER_H
#include"HTTPRequest.h"
#include <string>
#include <vector>
#include <thread>

using namespace std;

class Downloader{
    public:
        Downloader(const string & key_, const string& id_, const string& host, int num);
        void getImages();
        ~Downloader();
    private:
        string key_;
        string host_;
        string id_;
        int numThreads_;
        vector<thread> workers;
        HTTPRequest * request_;
};
#endif
