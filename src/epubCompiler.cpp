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
        mkdir(bookFolder_.c_str(), 0666);
    }
    
    // Create the Meta-Inf directory
    tempDir = opendir((bookFolder_ + "/META-INF").c_str());
    if(tempDir){
        closedir(tempDir);
    }else{
        mkdir((bookFolder_ + "/META-INF").c_str(), 0666);
    }

    //Create the OEBPS directory
    tempDir = opendir((bookFolder_ + "/OEBPS").c_str());
    if(tempDir){
        closedir(tempDir);
    }else{
        mkdir((bookFolder_ + "/OEBPS").c_str(), 0666);
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

//Adds the header files to the xhtml
void epubCompiler::createXHTML(){
    ofstream outMain((bookFolder_ + "/OEBPS/main.html").c_str(), ofstream::out);
    
    outMain << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
    outMain << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"" << endl;
    outMain << "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">" << endl;
    outMain << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << endl;
    outMain << "<head>" << endl;
    outMain << "<title>Book</title>" << endl;
    outMain << "</head>" << endl;
    outMain << "<body>" << endl;
    outMain << "<div>" << endl;
}

void epubCompiler::finishXHTML(){
    ofstream out((bookFolder_ + "/OEBPS/main.html").c_str(), ofstream::app);
    
    out << "</div>" << endl;
    out << "</body>" << endl;
    out << "</html>" << endl;
    out.close();
}

//Write the image tag to the manifest of Content OPF
//and to the xhml file
//Use the fileName given to place into 
void epubCompiler::addImages(const string& imgDir){
    //TODO need to badly fix this function so that it reads in the 
    //image directory and add the corresponding images

    //Add image to the xhtml file
    outMain << "<img src=\"";
    outMain << imgDir;
    outMain << "\" alt=\"--\"/>" << endl;

    //Add image to the content.opf (manifest)
    outMan << "<item id=\"img"; //use the name of the file name minus the extension
    outMan << "\" href=\"" << imgDir;
    outMan << "\" media-type=\"image/jpeg\" />" << endl;
    
    outMan.close();
    outMain.close();
}

//This method adds to Content.opf up until
//the middle of the manifest where images are being referenced
void epubCompiler::createContentOPF(){

    ofstream outMan((bookFolder_ + "/OEBPS/content.opf").c_str(), ofstream::out); 

    //Check file created
    if(!outMan.is_open()){
        cerr << "Content Manifest not created" << endl;
        exit(1);
    }
    
    //Generate a random UUID
    string charset = string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    string uuid;
    
    srand(time(NULL));
    //Grab random characters to construct a uuid
    for(int i = 0; i < 26; ++i){
        uuid += charset[rand() % (charset.length())];
    }

    //Write all of the data
    outMan << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    outMan << "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookID\" version=\"2.0\" >" << endl;
    outMan << "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">" << endl;
    outMan << "<dc:title>" << title_ << "</dc:title>" << endl;
    outMan << "<dc:creator opf:role=\"aut\">Yoda47</dc:creator>" << endl;
    outMan << "<dc:language>en-US</dc:language>" << endl; 
    outMan << "<dc:rights>Imgur</dc:rights>" << endl;
    outMan << "<dc:publisher> Imgur website</dc:publisher>" << endl;
    outMan << "<dc:identifier id=\"BookID\" opf:scheme=\"UUID\">";
    outMan << uuid << "</dc:identifier>" << endl;
    outMan << "</metadata>" << endl;
    outMan << "<manifest>" << endl;
    outMan << "<item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />" << endl;
    outMan << "<item id=\"book\" href=\"main.html\" media-type=\"application/xhtml+xml\" />" << endl;
}

//Finish the content.opf file
void epubCompiler::finishContentOPF(){
    outMan << "</manifest>" << endl;
    outMan << "<spine toc=\"ncx\">" << endl;
    outMan << "    <itemref idref=\"book\" />" << endl;
    outMan << "</spine>" << endl;
    outMan << "</package>" << endl;
    outMan.close();
}

//Creates the table of contents file
void epubCompiler::createTOC(){
    ofstream out((bookFolder_ + "/OEBPS/toc.ncx").c_str());
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    out << "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">" << endl;
    out << "<head>" << endl;
    out << "<meta name=\"dtb:uid\" content=\"qwertystorm1234567890\"/>" << endl;
    out << "<meta name=\"dtb:depth\" content=\"1\"/>" << endl;
    out << "<meta name=\"dtb:totalPageCount\" content=\"0\"/>" << endl;
    out << "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>" << endl;
    out << "</head>" << endl;
    out << "<docTitle>" << endl;
    out << "<text>eBook</text>" << endl;
    out << "</docTitle>" << endl;
    out << "<navMap>" << endl;
    out << "<navPoint id=\"chapter01\" playOrder=\"1\">" << endl;
    out << "<navLabel>" << endl;
    out << "<text>Chapter 1</text>" << endl;
    out << "</navLabel>" << endl;
    out << "<content src=\"main.html\"/>" << endl;
    out << "</navPoint>" << endl;
    out << "</navMap>" << endl;
    out << "</ncx>" << endl;
    out.close();
}

//Finalizes all of the necessary files
epubCompiler::~epubCompiler(){
    finishXHTML();
    finishContentOPF();
}
