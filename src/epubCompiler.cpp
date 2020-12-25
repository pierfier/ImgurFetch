#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "epubCompiler.h"

using namespace std;

epubCompiler::epubCompiler(const string& bookFolder_, const string & title,
            const string& author) :
                bookFolder_(bookFolder_), title_(title), author_(author){
    
    // Create the bookfolder directory if it does not exist
    DIR* tempDir = opendir(bookFolder_.c_str());
    
    if(tempDir){
        closedir(tempDir);
    }else{
        mkdir(bookFolder_.c_str(), 0777);
    }
    
    // Create the Meta-Inf directory
    tempDir = opendir((bookFolder_ + "/META-INF").c_str());
    if(tempDir){
        closedir(tempDir);
    }else{
        mkdir((bookFolder_ + "/META-INF").c_str(), 0777);
    }

    //Create the OEBPS directory
    tempDir = opendir((bookFolder_ + "/OEBPS").c_str());
    if(tempDir){
        closedir(tempDir);
    }else{
        mkdir((bookFolder_ + "/OEBPS").c_str(), 0777);
    }

    //Create the images subdirectory
    tempDir = opendir((bookFolder_ + "/OEBPS/images").c_str());
    if(tempDir){
        closedir(tempDir);
    }else{
        mkdir((bookFolder_ + "/OEBPS/images").c_str(), 0777);
    }

    // Create the files needed for the epub
    createMimeType();
    createMETAINF();
    createContentOPF();
    createTOC();
}

//Creates mimetype file with a single line
//If it does not exist already
//This does not need a running string file because it is literally one line that is needed
void epubCompiler::createMimeType(){
    ofstream out((bookFolder_ + "/mimetype").c_str(), ofstream::out);
    
    //Check to make sure that the file is created
    if(!out.is_open()){
        cerr << "mimetype file not created" << endl;
        exit(1);
    }

    out << "application/epub+zip";
    out.close();
}

//Creates the META-INF Folder and its contents
void epubCompiler::createMETAINF(){
    ofstream out((bookFolder_ + "/META-INF/container.xml").c_str(), ofstream::out);

    //Check file created
    if(!out.is_open()){
        cerr << "conatiner.xml not created" << endl;
        exit(1);
    }
    
    out << "<?xml version=\"1.0\" ?>" << endl;
    out << "<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">" << endl;
    out << "    <rootfiles>" << endl;
    out << "        <rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/>" << endl;
    out << "    </rootfiles>" << endl;
    out << "</container>" << endl;

    out.close();
}

//Adds the header information to any xhtml file
//Commonly used for the chapter file, but also with creating the cover file 
void epubCompiler::createXHTML(string & xhtml_file){

    // Add to single xhtml
    xhtml_file += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    xhtml_file += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n";
    xhtml_file += "<head>\n";
    xhtml_file += "<title>Book</title>\n";
    xhtml_file += "</head>\n<body>\n<div>\n";
}

//Create the cover html file with the cover image
void epubCompiler::createCoverHTML(string cover_image_file){
    // Start up the header information
    string cover_html = string();
    createXHTML(cover_html);

    cover_html += "<img src=\"";
    cover_html += cover_image_file;
    cover_html += "images/over.png\" alt=\"Title page\"/>\n";
    cover_html += "</div>\n";
    cover_html += "</body>\n";
    cover_html += "</html>\n";

    // Save contents to cover file
    ofstream out((bookFolder_ + "cover.html").c_str(), ofstream::out);
    out << cover_html;
    out.close();
}

//Finish all of the chapter xhtml files
void epubCompiler::finishXHTMLs(){
    // Add finishing tags to all of the chapter strings
    for(int i = 0; i < chapter_xhtml_.size(); ++i){
        chapter_xhtml_[i] += "</div>\n";
        chapter_xhtml_[i] += "</body>\n";
        chapter_xhtml_[i] += "</html>\n";
    
        
        // Write all of the chapters to files
        ofstream out((bookFolder_ + "/OEBPS/chapter" + to_string(i+1) + ".html").c_str(), ofstream::out);
        out << chapter_xhtml_[i];

        out.close();
    }

}

// Either try to find images in this folder, or search for individual chapter subfolders
// There should only be two levels so I think continuously checking is not necessary
void epubCompiler::compile(const string& rootImageSrc){
    
    DIR * dir;
    DIR * subdir;
    struct dirent * ent;

    if(dir = opendir(rootImageSrc.c_str())){

        // Initialize the first chapter element
        string ch1 = string("");
        createXHTML(ch1);
        chapter_xhtml_.push_back(ch1);

        bool is_first_chapter = true;
        
        // Read each directory item in dir (root directory which is the function's argument)
        while((ent = readdir(dir)) != NULL){
            
            //Check that the directory is not the current or previous
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
                continue;
            }

            //DEBUG
            cout << "Initial entry name: " << ent->d_name << endl;

            if((subdir = opendir(ent->d_name)) != NULL){
                if(is_first_chapter){
                    is_first_chapter = false;
                }else{
                    string new_chap = string("");
                    createXHTML(new_chap);
                    chapter_xhtml_.push_back(new_chap);
                }
                
                //DEBUG
                cout << "\n-- Entered Forbidden Zone ---\n";
                cout << "Entry name is" << ent->d_name << "\n\n";

                // Read in the directory and start adding the images into the current chapter this chapter
                addChapter(ent->d_name, chapter_xhtml_[chapter_xhtml_.size() - 1]);

            }else{
                // Entry in the root image directory is a file
                // DEBUG
                cout << "Starting the root directory\n";
                cout << "Current entry is " << ent->d_name;

                // Check if its a cover image
                if(strToLower(ent->d_name).find(string("cover")) != std::string::npos){
                    //DEBUG
                    cout << "Found cover image\n";
                    cout << "Cover name: " << ent->d_name;
                    createCoverHTML(string(ent->d_name)); //TODO should I include the folder here??
                    
                }else{
                    //TODO 
                    //addChapter(, chapter_xhtml_[chapter_xhtml_.size() - 1]);
                }              
            }
        }

        //DEBUG
        cout << "End of loop";

    }else{
        cout << "Directory " << rootImageSrc << " does not exist\n";
    }
    

}

//Folder directory must only contain image files
//Write image to the respective chapter html string
//Add the image source to the manifest
void epubCompiler::addChapter(const string& imgDir, string& chapter){
    
    DIR * dir;
    DIR * subdir;
    struct dirent * ent;

    if(dir = opendir(imgDir.c_str())){
        
        //Read in each entry of the root directory
        while((ent = readdir(dir)) != NULL){
            if(strToLower(ent->d_name).find(string("cover")) == std::string::npos){
            
                //Add the image to the main body of the html
                chapter += "<img src=\"";
                chapter += ent->d_name;
                chapter += "\" alt=\"--\"/>\n";

                //Get the file type from the name
                size_t ext_start = string(ent->d_name).find(".");
                string extension = string(ent->d_name).substr(ext_start, string::npos);

                //Add image source to the manifest of (content.opf)
                content_man_ += "<item id=\"img\" href=\""; 
                content_man_ += ent->d_name;
                content_man_ += "\" media-type=\"image/" + extension + "\"/>\n";
            }
        }

    }else{
        cout << "Error reading directory " << dir << endl;
    }
}   

//This method adds to Content.opf string up until
//the middle of the manifest where images are being referenced
void epubCompiler::createContentOPF(){

    //Generate a random UUID
    string charset = string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    string uuid;
    
    srand(time(NULL));
    //Grab random characters to construct a uuid
    for(int i = 0; i < 26; ++i){
        uuid += charset[rand() % (charset.length())];
    }

    //Write all of the data
    content_man_ += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    content_man_ += "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookID\" version=\"2.0\" >\n";
    content_man_ += "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">\n";
    content_man_ += "<dc:title>"; 
    content_man_ += title_;
    content_man_ += "</dc:title>\n";
    content_man_ += "<dc:creator opf:role=\"aut\">Yoda47</dc:creator>\n";
    content_man_ += "<dc:language>en-US</dc:language>\n"; 
    content_man_ += "<dc:rights>Imgur</dc:rights>\n";
    content_man_ += "<dc:publisher> Imgur website</dc:publisher>\n";
    content_man_ += "<dc:identifier id=\"BookID\" opf:scheme=\"UUID\">";
    content_man_ += uuid;
    content_man_ += "</dc:identifier>\n";
    content_man_ += "</metadata>\n";
    content_man_ += "<manifest>\n";
    content_man_ += "<item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />\n";

    //Add all of the chapter files
    for(int i = 0; i < chapter_xhtml_.size(); ++i){
        string file_name = string("chapter" + to_string(i+1) + ".html");
        content_man_ += "<item id=\"book\" href=\"";
        content_man_ += file_name; 
        content_man_ += "\" media-type=\"application/xhtml+xml\" />\n";
    }    
}

//Finish the content.opf file
//TODO very possible that the spine needs to be fixed if there are multiple chapters
void epubCompiler::finishContentOPF(){
    content_man_ += "</manifest>\n";
    content_man_ += "<spine toc=\"ncx\">\n";
    content_man_ += "    <itemref idref=\"book\" />\n";
    content_man_ += "</spine>\n";
    content_man_ += "</package>\n";
}

//Creates the table of contents file
// TODO change to include each chapter file
void epubCompiler::createTOC(){
    toc_ += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    toc_ += "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">\n";
    toc_ += "<head>\n";
    toc_ += "<meta name=\"dtb:uid\" content=\"qwertystorm1234567890\"/>\n";
    toc_ += "<meta name=\"dtb:depth\" content=\"1\"/>\n";
    toc_ += "<meta name=\"dtb:totalPageCount\" content=\"0\"/>\n";
    toc_ += "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>\n";
    toc_ += "</head>\n";
    toc_ += "<docTitle>\n";
    toc_ += "<text>eBook</text>\n";
    toc_ += "</docTitle>\n";
    toc_ += "<navMap>\n";
    toc_ += "<navPoint id=\"chapter01\" playOrder=\"1\">\n";
    toc_ += "<navLabel>\n";
    toc_ += "<text>Chapter 1</text>\n"; // TODO include the chapter html files
    toc_ += "</navLabel>\n";
    toc_ += "<content src=\"main.html\"/>\n";
    toc_ += "</navPoint>\n";
    toc_ += "</navMap>\n";
    toc_ += "</ncx>\n";
}

//Finalizes all of the necessary files
epubCompiler::~epubCompiler(){
    finishXHTMLs();
    finishContentOPF();
}
