#include<fstream>
#include<iostream>
#include<string>

using namespace std;

class XHTMLWriter{
    public:     
        XHTMLWriter(const string& fileName, const string& title, const string& author);
        
        //Writes in all of the necessary
        void addMetaData();

        //This method only writes in the necessary data for the image
        //to read in the xhtml. It appends the tag to the end of the 
        //file
        void writeImage(const string& fileName);

        void compileEPUB();

        ~XHTMLWriter();
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
