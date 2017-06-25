#ifndef _THREAD_POOL_
#define _THREAD_POOL_

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

class ThreadPool{
    public:
        ThreadPool();
        
        //Request file with all of the image links,
        //parse links, and add them to the queue
        void storeLinks();

        //Create all of the threads and start downloading
        void startDownload();
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
