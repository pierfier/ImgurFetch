#include "epubCompiler.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

epubCompiler::epubCompiler(const string& bookFolder_, const string & title,
            const string& author) :
                bookFolder_(bookFolder_), title_(title), author_(author){
    
    createMimeType();
    createMETAINF();
    createXHTML();
    createContentOPF();
    createTOC();    
}

//Creates mimetype file with a single line
//If it does not exist already
void epubCompiler::createMimeType(){
    ofstream out(string(bookFolder_ + "/mimetype").c_str(), ofstream::out);
    
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
    ofstream out(string(bookFolder_ + "/META-INF/container.xml").c_str(), ofstream::out);

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
    ofstream out(string(bookFolder_ + "/OEBPS/main.html").c_str(), ofstream::out);
    
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
    out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"" << endl;
    out << "\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">" << endl;
    out << "<html xmlns=\"http://www.w3.org/1999/xhtml\">" << endl;
    out << "<head>" << endl;
    out << "<title>Book</title>" << endl;
    out << "</head>" << endl;
    out << "<body>" << endl;
    out << "<div>" << endl;
    out.close();
}

void epubCompiler::finishXHTML(){
    ofstream out(string(bookFolder_ + "/OEBPS/main.html").c_str(), ofstream::app);
    
    out << "</div>" << endl;
    out << "</body>" << endl;
    out << "</html>" << endl;
    
    out.close();
}

//Write the image tag to the manifest of Content OPF
//and to the xhml file
//Use the fileName given to place into 
void epubCompiler::addImage(const string& fileName, int num){
    ofstream outM(string(bookFolder_ + "/OEBPS/content.opf").c_str(), ofstream::app);
    ofstream out(string(bookFolder_ + "/OEBPS/main.html").c_str(), ofstream::app);
    
    //Add image to the xhtml file
    out << "<img src=\"";
    out << fileName;
    out << "\" alt=\"--\"/>" << endl;

    //Add image to the manifest
    outM << "<item id=\"img"; //use the name of the file name minus the extension
    outM << "\" href=\"" << fileName;
    outM << "\" media-type=\"image/jpeg\" />" << endl;
    
    outM.close();
    out.close();
}

//This method adds to Content.opf up until
//the middle of the manifest where images are being referenced
void epubCompiler::createContentOPF(){

    ofstream out(string(bookFolder_ + "/OEBPS/content.opf").c_str(), ofstream::out); 

    //Check file created
    if(!out.is_open()){
        cerr << "Content Manifest not created" << endl;
        exit(1);
    }

    //Write all of the data
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    out << "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookID\" version=\"2.0\" >" << endl;
    out << "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">" << endl;
    out << "<dc:title>" << title_ << "</dc:title>" << endl;
    out << "<dc:creator opf:role=\"aut\">Yoda47</dc:creator>" << endl;
    out << "<dc:language>en-US</dc:language>" << endl; 
    out << "<dc:rights>Imgur</dc:rights>" << endl;
    out << "<dc:publisher> Imgur website</dc:publisher>" << endl;
    out << "<dc:identifier id=\"BookID\" opf:scheme=\"UUID\">qwertystorm1234567890</dc:identifier>" << endl;
    out << "</metadata>" << endl;
    out << "<manifest>" << endl;
    out << "<item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />" << endl;
    out << "<item id=\"book\" href=\"main.html\" media-type=\"application/xhtml+xml\" />" << endl;
    out.close();
}

//Creates the table of contents file
void epubCompiler::createTOC(){
    ofstream out(string(bookFolder_ + "/OEBPS/toc.ncx").c_str());
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

//Finish the content.opf file
void epubCompiler::finishContentOPF(){
     ofstream out(string(bookFolder_ + "/OEBPS/content.opf").c_str(), ofstream::app); 

    //Check file created
    if(!out.is_open()){
        cerr << "Unable to reopen content.opf" << endl;
        exit(1);
    }
    
    out << "</manifest>" << endl;
    out << "<spine toc=\"ncx\">" << endl;
    out << "    <itemref idref=\"book\" />" << endl;
    out << "</spine>" << endl;
    out << "</package>" << endl;
    out.close();
}

//Finalizes all of the necessary files
epubCompiler::~epubCompiler(){
    finishXHTML();
    finishContentOPF();
}
