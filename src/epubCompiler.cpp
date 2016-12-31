#include "epubCompiler.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

epubCompiler::epubCompiler(const string& fileName, const string & title,
            const string& author) :
                fileName_(fileName), title_(title), author_(author){
    
    contents_ = string("<?xml version=\"1.0\" encoding=\"UTF-8\"??>\n");
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
void createMETAINF(){
    ofstream out(string(fileName_ + "META-INF/container.xml").c_str());

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

void epubCompiler::writeImage(const string& imageFileName){
    
}

void epubCompiler::init(){
    
}
