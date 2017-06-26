#include "Downloader.h"
#include <string>

void Downloader::storeLinks(){
    string response, link;

    request.requestLinks(response);

    //Parse the links to the queue
    
    int index = 0;
    int numImages = 0;
    
    ImageLink temp;

    while((index = response.find("<link>", index)) != string::npos){
        ++numImages;
        
        link = "";

        //Fully read in the link
        for(; response[index] != "<"; ++index){
            link += response[index];
        }
        temp.link = link;
        temp.i = numImages;

        //Add link to the queue
        imQueue_.push_back(temp);
    }
}

void Downloader::startDownload(){
    for(int i = 0; i < num_threads_; ++i){
        workers_.push_back(thread(downloadImage));
    }
}

void Downloader::downloadImage(){
    HTTPRequest request(); //TODO make this global for all classes
    
    while(true){
        ImageLink imLink;
        {
            //Get the lock
            unique_lock<mutex> lock(queue_mutex_);
            
            //Exit if the queue is empty
            if(imQueue_.empty()){
                return;
            }

            imLink = imQueue_.front();
            
            //release the lock
        }
        stringstream ss;
        ss << imLink.i;

        request.getImageToFile(imLink.link, ("res/image" + ss.str()  + ".jpg").c_str());

        //DEBUG
        {
            unique_lock<mutex> lock(debug_mutex_);

            cout << "Downloaded image " + imLink.i << endl; 
        }
    }
}

Downloader::~Downloader(){
    for(int i = 0; i < workers_.size(); ++i){
        workers_[i].join();
    }
}
