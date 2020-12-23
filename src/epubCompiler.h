#include<fstream>
#include<iostream>
#include<string>
#include<vector>

using namespace std;

class epubCompiler{

    //Abstract Function: creates content of an epub file and then places any existing images in res/ and embeds them into the epub file in the order of their
    //numbers
    
    public:     
        epubCompiler(const string& bookFolder, const string& title, const string& author);
        
        // Add the image information to the chapter xhmtl and the manifest
        void addImage(const string& imgDir, string& chapter_html);
        
        // Entry point for starting the build of the epub
        void compile(const string& rootImageSrc);

        ~epubCompiler();
    private:
        //Starts the creation of the files
        void createMimeType();
        void createMETAINF();
        void createXHTML(string & chapter);
        void finishXHTMLs();
        void createTOC();
        void createContentOPF();
        void finishContentOPF();

        void addChapter(const string & imgDir, string & chapter);

        //Local folder used to compose the book
        string bookFolder_;
        
        //Attributes to the ebook
        string title_;
        string author_;
        
        //Strings to hold all of the continuously modified files
        
        string toc_; //Stores the table of contents (toc.ncx)
        string content_man_; //Stores the information for the manifest (content.opf)
        vector<string> chapter_xhtml_; //Stores the image information for each chapter (later separated into main.html files)
};
