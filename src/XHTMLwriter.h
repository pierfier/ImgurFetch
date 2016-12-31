#include<fstream>
#include<iostream>

using namespace std;

class XHTMLWriter{
    
    XHTMLWriter();
    
    //Writes in all of the necessary
    addMetaData();

    //This method only writes in the necessary data for the image
    //to read in the xhtml. It appends the tag to the end of the 
    //file
    writeImage();

    ~XHTMLWriter();
};
