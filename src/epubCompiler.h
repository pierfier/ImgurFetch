#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class epubCompiler{

    //Abstract Function: creates content of an epub file and then places any existing images in res/ and embeds them into the epub file in the order of their
    //numbers
    
    public:     
        epubCompiler(const string& bookFolder, const string& title, const string& author);
        
        //Looks into the folder to find the cover and content images
        void addImages(const string& imgDir);
        
        ~epubCompiler();
    private:
        //Starts the creation of the files
        void createMimeType();
        void createMETAINF();
        void createXHTML();
        void finishXHTML();
        void createTOC();
        void createContentOPF();
        void finishContentOPF();

        //Attributes to the ebook
        string bookFolder_;
        string title_;
        string author_;
        
        //ofstreams for the main html and manifest file
        ofstream outMan;
        ofstream outMain;
};
