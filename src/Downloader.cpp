#include "Downloader.h"
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include "HTTPRequest.h"
#include "SSL_Handler.h"

using namespace std;

//Store the links of each image and the number that keeps the images in order
void Downloader::storeLinks(int startNumImage){
    SSL_Handler ssl_sock;   
    vector<string> links;
    int index = 0;
    int numImages = startNumImage;
    
    //Parse the links to the queue
    links = ssl_sock.requestLinks();
    
    ImageLink temp;
    
    //Loop through each element and append a name into a queue
    vector<string>::iterator it;
    for(it = links.begin(); it != links.end(); ++it){
        temp.link = *it;
        temp.i = numImages;

        imQueue_.push_back(temp);
        ++numImages;
    }
}

void Downloader::startDownload(){
    for(int i = 0; i < num_threads_; ++i){
        workers_.push_back(thread(Worker(*this)));
    }
}

void Worker::operator()(){
    SSL_Handler ssl_sock;

    while(true){
        ImageLink imLink;
        string extension;

        {
            //Get the lock
            unique_lock<mutex> l(downloader_.queue_mutex_);
                            
            //Exit if the queue is empty
            if(downloader_.imQueue_.empty()){
                return;
            }

            imLink = downloader_.imQueue_.front();
            downloader_.imQueue_.pop_front();
            //release the lock
        }
        
        stringstream ss;
        ss << imLink.i;
        
        //Grab the extension of the link
        extension = imLink.link.substr(imLink.link.find_last_of("."), string::npos);
        
        ssl_sock.getImageToFile(imLink.link, (downloader_.getDownloadDir() + ss.str() + extension).c_str());

        //DEBUG
        {
            unique_lock<mutex> l(downloader_.debug_mutex_);

            cout << "Downloaded image " << imLink.i << " with " << imLink.link << endl; 
        }
    }
}

Downloader::~Downloader(){
    for(int i = 0; i < workers_.size(); ++i){
        workers_[i].join();
    }
}
