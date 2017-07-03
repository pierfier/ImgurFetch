#include "epubCompiler.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

epubCompiler::epubCompiler(const string& fileName, const string & title,
            const string& author) :
                fileName_(fileName), title_(title), author_(author){
}

//Creates mimetype file with a single line
//If it does not exist already
void epubCompiler::createMimeType(){
    ofstream out(string(fileName_ + "mimetype").c_str());
    
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
    cout << fileName_;
    ofstream out(string(fileName_ + "/META-INF/container.xml").c_str());

    //Check file created
    if(!out.is_open()){
        cerr << "conatiner.xml not created" << endl;
        exit(1);
    }

    out << "<?xml version=\"1.0\"?>" << endl;
    out << "<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">" << endl;
    out << "<rootfiles>" << endl;
    out << "<rootfile full-path=\"OEBPS/content.opf\" media-type=\"application/oebps-package+xml\"/>" << endl;
    out << "</rootfiles>" << endl;
    out << "</container>" << endl;
    out.close();
}

void epubCompiler::startXHTML(){
    ofstream out();


    out.close();
}

//Write the image tag to the manifest of Content OPF
//and to the xhml file
void epubCompiler::addImage(const string& fileName){
    
}

//This method adds to Content.opf up until
//the middle of the manifest where images are being referenced
void epubCompiler::startContentOPF(){
    //TODO make these directory: OEBPS

    ofstream out(string(fileName_ + "/OEBPS/Content.opf").c_str()); //TODO need to finish this statement

    //Check file created
    if(!out.is_open()){
        cerr << "Content Manifest not created" << endl;
        exit(1);
    }

    //Write all of the data
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"??>" << endl;
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
    out << "<item id=\"titlepage\" href=\"title_page.xhtml\" media-type=\"application/xhtml+xml\" />" << endl;
    out << "<item id=\"book\" href=\"book.xhtml\" media-type=\"application/xhtml+xml\" />" << endl;
    out << "</metadata>" << endl;
    out << "<manifest>" << endl;

    out.close();
}

void epubCompiler::finishContentOPF(){
     ofstream out(string(fileName_ + "").c_str(), ofstream::app); //TODO need to finish this statement

    //Check file created
    if(!out.is_open()){
        cerr << "Unable to reopen content.opf" << endl;
        exit(1);
    }
    
    out << "</manifest>" << endl;
    out << "<spine toc=\"ncx\">" << endl;
    out << "<itemref idref=\"titlepage\" />" << endl;
    out << "<itemref idref=\"book\" />" << endl;
    out << "</spine>" << endl;
    out << "</package>" << endl;
    out.close();
}
