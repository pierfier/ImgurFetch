#ifndef _DOWNLOADER_
#define _DOWNLOADER_

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "HTTPRequest.h"

using namespace std;

//Link that contains the image to be downloaded
struct ImageLink{
    string link;
    int i;
};

class Downloader{
    public:
        Downloader() : num_threads_(1) {}
        Downloader(int num_thread) : num_threads_(num_thread){}

        //Request file with all of the image links,
        //parse links, and add them to the queue
        void storeLinks();

        //Create all of the threads and start downloading
        void startDownload();
        
        //Wait for all of the threads to join
        ~Downloader();
    private:
        //Methods
        
        //Grab next image from queue and 
        void downloadImage();

        //Variables
        vector<thread> workers_;
        int num_threads_;
        deque<ImageLink> imQueue_;
        mutex queue_mutex_;

        //Used to get the response with all of the image links
        HTTPRequest request_;

        //DEBUGging purposes, mainly to print to the console
        mutex debug_mutex_;
        
        //TODO hopefully will not need this to work
        //condition_variable queue_ready_;

};

#endif
