#include "XHTMLwrier.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

XHTMLWriter::XHTMLWriter(const string& fileName, const string & title,
            const string& author) :
                fileName_(fileName), title_(title), author_(author){
    
    xhtmlFile_ = string("");
    contents_ = string("<?xml version="1.0" encoding="UTF-8"??>\n");

}

void XHTMLWriter::addMetaData(){
    
    
}

void XHTMLWriter::writeImage(const string& imageFileName){
    
}

void XHTMLWriter::init(){
    //Create the directory that will contain 
    system("mkdir " + fileName_);
}
