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

void epubCompiler::startContentOPF(){
    ofsream out();

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
    
}
