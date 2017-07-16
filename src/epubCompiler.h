#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class epubCompiler{

    //Abstract Function: creates content of an epub file and then places any existing images in res/ and embeds them into the epub file in the order of their
    //numbers
    
    public:     
        epubCompiler(const string& bookFolder, const string& title, const string& author);
        
        //This method only writes in the necessary data for the image
        //to read in the xhtml. It appends the tag to the end of the 
        //file
        void addImage(const string& fileName);
        
        ~epubCompiler();
    private:
        //Starts the creation of the files
        void createMimeType();
        void createMETAINF();
        void createXHTML();
        void finishXHTML();
        void createContentOPF();
        void finishContentOPF();

        //Attributes to the ebook
        string bookFolder_;
        string title_;
        string author_;
};
