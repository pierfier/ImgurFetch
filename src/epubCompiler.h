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
        
        // Entry point for starting the build of the epub
        void compileImages(const string& rootImageSrc);

        //Starts the creation of the files
        void createMimeType();
        void createMETAINF();
        void createCoverHTML(string cover_image_file);
        
        void startTOC();
        void startContentOPF();
        void startXHTML(string & chapter);
        
        void finishXHTMLStrings();
        void finishContentOPFString();
        
        void transferImageToBookDir(string image_path);

        void addChapter(const string & imgDir, string & chapter);
        void writeAllFiles();

        // Returns passed string to lowercase
        string strToLower(string str){    
            string temp = string();    
                   
            for(int i = 0; i < str.length(); ++i){    
                temp += tolower(str[i]);    
            }    
                       
            return temp;    
        }
        
        // Compare image file names so that they are ordered by numbers
        bool static compFileName(string i, string j){
            i = i.substr(0, i.find("."));
            j = j.substr(0, j.find("."));
            
            // Prefix is a number
            long int num1;
            long int num2;
            num1 = strtol(i.c_str(), NULL, 10);
            num2 = strtol(j.c_str(), NULL, 10);
 
            if(num1 == 0 || num2 == 0){
                // Contains characters
                return i < j;
            }else{
                // Number comparisons
                return num1 < num2;
            }
        }

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
