#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
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
    createXHTML();
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

//Adds the header files to each xhtml chapter file
void epubCompiler::createXHTML(){

    

    // Add to single xhtml
    main_xhtml_ += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    main_xhtml_ += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n";
    main_xhtml_ += "<head>\n";
    main_xhtml_ += "<title>Book</title>\n";
    main_xhtml_ += "</head>\n<body>\n<div>\n";
}

//Finish all of the chapter xhtml files
void epubCompiler::finishXHTML(){
    // Add finishing tags to all of the chapter strings
    for(int i = 0; i < chapter_xhtml_.size(); ++i){
        chapter_xhtml_[i] += "</div>" << endl;
        chapter_xhtml_[i] += "</body>" << endl;
        chapter_xhtml_[i] += "</html>" << endl;
    
        
        // Write all of the chapters to files
        ofstream out((bookFolder_ + "/OEBPS/chapter" + itoa(i+1) + ".html").c_str(), ofstream::out);
        out.write(chapter_xhtml_[i]);

        out.close();
    }

    
    


}

//Write image to the respective chapter html string
//Add the image source to the manifest
void epubCompiler::addImage(const string& imgDir, string& chapter){
    
    //Add the image to the main body of the html
    chapter += "<img src=\"" + imgDir + "\" alt=\"--\"/>\n";

    //Get the file type from the name
    size_t ext_start = imgDir.find(".");
    string extension = imgDir.substr(ext_start, string::npos);

    //Add image source to the manifest of (content.opf)
    content_man_ += "<item id=\"img\" href=\"" + imgDir;
    content_man_ += "\" media-type=\"image/" + extension + "\"/>\n";
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
    content_man_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    content_man_ << "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookID\" version=\"2.0\" >" << endl;
    content_man_ << "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">" << endl;
    content_man_ << "<dc:title>" << title_ << "</dc:title>" << endl;
    content_man_ << "<dc:creator opf:role=\"aut\">Yoda47</dc:creator>" << endl;
    content_man_ << "<dc:language>en-US</dc:language>" << endl; 
    content_man_ << "<dc:rights>Imgur</dc:rights>" << endl;
    content_man_ << "<dc:publisher> Imgur website</dc:publisher>" << endl;
    content_man_ << "<dc:identifier id=\"BookID\" opf:scheme=\"UUID\">";
    content_man_ << uuid << "</dc:identifier>" << endl;
    content_man_ << "</metadata>" << endl;
    content_man_ << "<manifest>" << endl;
    content_man_ << "<item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />" << endl;
    content_man_ << "<item id=\"book\" href=\"main.html\" media-type=\"application/xhtml+xml\" />" << endl;
}

//Finish the content.opf file
//TODO very possible that the spine needs to be fixed if there are multiple chapters
void epubCompiler::finishContentOPF(){
    content_man_ << "</manifest>" << endl;
    content_man_ << "<spine toc=\"ncx\">" << endl;
    content_man_ << "    <itemref idref=\"book\" />" << endl;
    content_man_ << "</spine>" << endl;
    content_man_ << "</package>" << endl;
}

//Creates the table of contents file
void epubCompiler::createTOC(){
    toc_ << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    toc_ << "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">" << endl;
    toc_ << "<head>" << endl;
    toc_ << "<meta name=\"dtb:uid\" content=\"qwertystorm1234567890\"/>" << endl;
    toc_ << "<meta name=\"dtb:depth\" content=\"1\"/>" << endl;
    toc_ << "<meta name=\"dtb:totalPageCount\" content=\"0\"/>" << endl;
    toc_ << "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>" << endl;
    toc_ << "</head>" << endl;
    toc_ << "<docTitle>" << endl;
    toc_ << "<text>eBook</text>" << endl;
    toc_ << "</docTitle>" << endl;
    toc_ << "<navMap>" << endl;
    toc_ << "<navPoint id=\"chapter01\" playOrder=\"1\">" << endl;
    toc_ << "<navLabel>" << endl;
    toc_ << "<text>Chapter 1</text>" << endl;
    toc_ << "</navLabel>" << endl;
    toc_ << "<content src=\"main.html\"/>" << endl;
    toc_ << "</navPoint>" << endl;
    toc_ << "</navMap>" << endl;
    toc_ << "</ncx>" << endl;
    out.close();
}

//Finalizes all of the necessary files
epubCompiler::~epubCompiler(){
    finishXHTML();
    finishContentOPF();
}
