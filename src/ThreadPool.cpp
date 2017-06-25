#include "ThreadPool.h"

void ThreadPool::downloadImage(){
    HTTPRequest request(key, id, host); //TODO make this global for all classes
    
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
