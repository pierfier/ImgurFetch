#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class epubCompiler{
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
        void createImagesFolder();
        void createContentFile();
        void createXHTMLContents();
        
        void compileEPUB();

        ~epubCompiler();
    private:
        void init();

        //
        string xhtmlFile_;
        string contents_;

        //Attributes to the ebook
        string fileName_;
        string title_;
        string author_;
};
