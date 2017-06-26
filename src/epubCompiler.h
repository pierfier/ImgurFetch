#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class epubCompiler{

    //Abstract Function: creates content of an epub file and then places any existing images in res/ and embeds them into the epub file in the order of their
    //numbers
    
    public:     
        epubCompiler(const string& fileName, const string& title, const string& author);
        
        //Writes in all of the necessary
        void addMetaData();

        //This method only writes in the necessary data for the image
        //to read in the xhtml. It appends the tag to the end of the 
        //file
        void addImage(const string& fileName);
        
        //Depending on what is written in all of these,
        //they could all be under one method
        void createMimeType();
        void createMETAINF();
        void createOEBPS();
        
        void compileEPUB();

        ~epubCompiler();
    private:
        //Starts the creation of the files
        void init();
        void startContentOPF();
        void finishContentOPF();

        //Attributes to the ebook
        string fileName_;
        string title_;
        string author_;
};
