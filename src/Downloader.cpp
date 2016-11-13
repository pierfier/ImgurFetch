#include "Downloader.h"

//Loop through the response to get all of the links and download
//Each image
void Downloader::getImages(const string& response, BIO * bio, const string& id){
    
    //Find the links for the images and then download them
    string link;
    
    //Buffer for reading in data
    char cur;

    //Index to indicate progress along the album xml structure
    int i = 0;
    
    //Stores the number of images that were downloaded
    int numIm = 0;

    //Loop through the xml and download the images from each link
    while( (i = response.find("link", i)) != string::npos /*&& numIm < 10*/){ //TODO here is the condition that limits number of images downloaded
        ++numIm;

        i = response.find("com", i);
        i += 4;

        link = "";
        for(;response[i] != '<'; ++i){
            link += response[i];
        }

        //DEBUG
        cout << link << endl;

        //Setup request to grab image
        
        string request = "GET /" + link;
        
        request += " HTTP/1.1";

        request += "\r\nHost: i.imgur.com\r\n";
        
        request += "Authorization: Client-ID " + id + "\r\n";
        
        request += "Connection: keep-alive\r\n\r\n";

        //Send request
        if ( BIO_write(bio, request.c_str(), request.size()) <= 0) {
            cout << "Write Failed" << endl;
            exit(1);
        }

        BIO_flush(bio);

        stringstream ss;
        ss << numIm;

        //Read in the response
        //And then write to image file
        ofstream out(("image" + ss.str()  + ".jpg").c_str(), ofstream::binary);
         
        string header;
        
        getHeader(bio, header);

        int length = getContentLength(header);
 
        //DEBUG
        //cout << header << endl;
        //cout << length << endl;
        //exit(1);


        //Write image to file
        for(int i = 0; i < length; ++i){
            BIO_read(bio, &cur, 1);
            out.write(&cur, 1);
        }
    }
}
