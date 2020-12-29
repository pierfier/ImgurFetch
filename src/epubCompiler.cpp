#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
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

    // Write the headers of the content.opf file
    startContentOPF();
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
void epubCompiler::startXHTML(string & xhtml_file){
    // Add to single xhtml
    xhtml_file += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    xhtml_file += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n";
    xhtml_file += "<head>\n";
    xhtml_file += "<title>";
    xhtml_file += title_;
    xhtml_file += "</title>\n";
    xhtml_file += "</head>\n<body>\n<div>\n";
}

//Finish all of the chapter xhtml files
void epubCompiler::finishXHTMLStrings(){
    // Add finishing tags to all of the chapter strings
    for(int i = 0; i < chapter_xhtml_.size(); ++i){
        chapter_xhtml_[i] += "</div>\n";
        chapter_xhtml_[i] += "</body>\n";
        chapter_xhtml_[i] += "</html>\n";   
    }
}

// Either try to find images in this folder, or search for individual chapter subfolders
// There should only be two levels so I think continuously checking is not necessary
void epubCompiler::compileImages(const string& rootImageSrc){
    
    DIR * dir;
    DIR * subdir;
    struct dirent * ent;

    if(dir = opendir(rootImageSrc.c_str())){

        // Initialize the first chapter element
        string ch1 = string("");
        startXHTML(ch1);
        chapter_xhtml_.push_back(ch1);

        bool is_first_chapter = true;
        
        // Check if root directory has other directories
        bool has_subdirs = false;
        while((ent = readdir(dir)) != NULL){
            //Skip the current and upstream directories
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
                continue;
            }

            //There is a subdirectory
            if((subdir = opendir(ent->d_name)) != NULL){
                has_subdirs = true;
                closedir(subdir);
            }

            // Check if there is a cover image
            if(string(strToLower(ent->d_name)).find(string("cover")) != std::string::npos){
                cout << "Found cover image\n";
                cout << "Cover name: " << ent->d_name << "\n";
    
                // Get image extension
                size_t ext_start = string(ent->d_name).find(".");
                string extension = string(ent->d_name).substr(ext_start + 1, string::npos);
                if(extension.compare("jpg") == 0){
                    extension = string("jpeg");
                }

                // Reference cover image information
                content_man_ += "<item id=\"cover-image\" ";
                content_man_ += "href=\"images/";
                content_man_ += ent->d_name;
                content_man_ += "\" media-type=\"image/";
                content_man_ += extension + "\"/>\n";
            }
        }
        closedir(dir);
        
        if(has_subdirs){
            // Loop through each subdirectory and create new chapter content files  
            dir = opendir(rootImageSrc.c_str());
            while((ent = readdir(dir)) != NULL){
                
                //Skip the current and upstream directories
                if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
                    continue;
                }
                
                // Add this subdirectory files to a new chapter contents string
                string new_chap = string("");
                startXHTML(new_chap);
                chapter_xhtml_.push_back(new_chap);
                addChapter(rootImageSrc + "/" + string(ent->d_name), chapter_xhtml_[chapter_xhtml_.size() - 1]);
            }
            closedir(dir);
        }else{
            // Add all images in root directory into single chapter html    
            addChapter(rootImageSrc, chapter_xhtml_[0]);
        }
        
        // Add all chapter file references to manifest
        for(int i = 0; i < chapter_xhtml_.size(); ++i){
            string file_name = string("chapter" + to_string(i+1) + ".html");
            content_man_ += "<item id=\"book\" href=\"";
            content_man_ += file_name; 
            content_man_ += "\" media-type=\"application/xhtml+xml\" />\n";
        }

    }else{
        cout << "Directory " << rootImageSrc << " does not exist\n";
    }
}

//Folder directory must only contain image files
//Write image to the respective chapter html string
//Add the image source to the manifest
void epubCompiler::addChapter(const string& imgDir, string& chapter){
    
    DIR * dir;
    struct dirent * ent;

    if((dir = opendir(imgDir.c_str())) != NULL){
        vector<string> img_files;
        while((ent = readdir(dir)) != NULL){
            //Skip the current and upstream directories
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
                continue;
            }

            img_files.push_back(ent->d_name);
        }
        //Sort the file names
        sort(img_files.begin(), img_files.end(), compFileName);
        
        //Reset directory pointer
        closedir(dir);

        //Read in each entry of the root directory
        for(vector<string>::iterator it = img_files.begin(); it != img_files.end(); ++it){
            //Skip the current and upstream directories
            if(strcmp((*it).c_str(), ".") == 0 || strcmp((*it).c_str(), "..") == 0){
                continue;
            }

            if(strToLower(*it).find(string("cover")) == std::string::npos){
            
                //Add the image to the main body of the html
                chapter += "<img src=\"";
                chapter += "images/";
                chapter += *it;
                chapter += "\" alt=\"--\"/>\n";

                //Get the file type from the name
                size_t ext_start = string(*it).find(".");
                string extension = string(*it).substr(ext_start + 1, string::npos);
                if(extension.compare("jpg") == 0){
                    extension = string("jpeg");
                }

                //Add image source to the manifest of (content.opf)
                content_man_ += "<item id=\"img";
                content_man_ += string(*it).substr(0, ext_start);
                content_man_ += "\" href=\"images/"; 
                content_man_ += *it;
                content_man_ += "\" media-type=\"image/" + extension + "\"/>\n";
            }
        }
    }else{
        cout << "Error reading directory " << dir << endl;
    }
}   

//
void epubCompiler::generateUUID(){
    //Generate a random UUID
    string charset = string("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    
    srand(time(NULL));
    //Grab random characters to construct a uuid
    for(int i = 0; i < 26; ++i){
        uuid_ += charset[rand() % (charset.length())];
    }
}

//This method adds to Content.opf string up until
//the middle of the manifest where images are being referenced
void epubCompiler::startContentOPF(){

    //Write all of the data
    content_man_ += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    content_man_ += "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"BookID\" version=\"2.0\" >\n";
    content_man_ += "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">\n";
    content_man_ += "<dc:title>"; 
    content_man_ += title_;
    content_man_ += "</dc:title>\n";
    content_man_ += "<dc:creator opf:role=\"aut\">";
    content_man_ += author_;
    content_man_ += "</dc:creator>\n";
    content_man_ += "<dc:language>en-US</dc:language>\n"; 
    content_man_ += "<dc:rights>Imgur</dc:rights>\n";
    content_man_ += "<dc:publisher> Imgur website</dc:publisher>\n";
    content_man_ += "<dc:identifier id=\"BookID\" opf:scheme=\"UUID\">";
    content_man_ += "urn:uuid:";
    content_man_ += uuid_;
    content_man_ += "</dc:identifier>\n";
    content_man_ += "<meta name=\"cover\" content=\"cover-image\"/>";
    content_man_ += "</metadata>\n";
    content_man_ += "<manifest>\n";
    content_man_ += "<item id=\"ncx\" href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" />\n";
}

//Finish the content.opf file
//TODO very possible that the spine needs to be fixed if there are multiple chapters
void epubCompiler::finishContentOPFString(){
    content_man_ += "</manifest>\n";
    content_man_ += "<spine toc=\"ncx\">\n";
    content_man_ += "<itemref idref=\"book\" />\n";
    

    content_man_ += "</spine>\n";
    content_man_ += "</package>\n";
}

// Creates the table of contents file
// This should be called after 
void epubCompiler::createTOC(){
    toc_ += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    toc_ += "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\">\n";
    toc_ += "<head>\n";
    toc_ += "<meta name=\"urn:uuid:\" content=\"";
    toc_ += uuid_;
    toc_ += "\"/>\n";
    toc_ += "<meta name=\"dtb:depth\" content=\"1\"/>\n";
    toc_ += "<meta name=\"dtb:totalPageCount\" content=\"0\"/>\n";
    toc_ += "<meta name=\"dtb:maxPageNumber\" content=\"0\"/>\n";
    toc_ += "</head>\n";
    toc_ += "<docTitle>\n";
    toc_ += "<text>eBook</text>\n";
    toc_ += "</docTitle>\n";
    toc_ += "<navMap>\n";
    
    // Add all of the chapter references
    for(int i = 0; i < chapter_xhtml_.size(); ++i){
        toc_ += "<navPoint id=\"chapter";
        toc_ += to_string(i+1);        
        toc_ += "\" playOrder=\"";
        toc_ += to_string(i+1);
        toc_ += "\">\n";
        toc_ += "<navLabel>\n";
        toc_ += "<text>Chapter ";
        toc_ += to_string(i+1); 
        toc_ += "</text>\n";
        toc_ += "</navLabel>\n";
        toc_ += "<content src=\"chapter";
        toc_ += to_string(i+1);      
        toc_ += ".html\"/>\n";
        toc_ += "</navPoint>\n";
    }
    
    toc_ += "</navMap>\n";
    toc_ += "</ncx>\n";
}

// Copies the image to the images folder of the book
// Takes the full path, including file name as the argument
void epubCompiler::transferImagesToBookDir(string images_path){
    
    string cp_cmd = string(); 
    cp_cmd += "cp -r " + images_path + "/* " + bookFolder_ + "/OEBPS/images/";

    FILE * fp;
    if((fp = popen(cp_cmd.c_str(), "r")) == NULL){
        cout << "ERROR: Image files could not be copied over!" << endl;
    }
    pclose(fp);
}

// Write all of the content strings to the respective files
void epubCompiler::writeAllFiles(){
    // Write all of the chapter xhmtl files
    for(int i =0; i < chapter_xhtml_.size(); ++i){    
        ofstream out((bookFolder_ + "/OEBPS/chapter" + to_string(i+1) + ".html").c_str(), ofstream::out);
        out << chapter_xhtml_[i];

        out.close();
    }
    
    //-- Write to content.opf
    finishContentOPFString();

    ofstream outOPF(bookFolder_ + "/OEBPS/content.opf", ofstream::out);
    outOPF << content_man_;
    outOPF.close();
    
    //-- Write to toc.ncx
    ofstream outTOC(bookFolder_ + "/OEBPS/toc.ncx", ofstream::out);
    outTOC << toc_;
    outTOC.close();

}

// Zip up the epub
void epubCompiler::zipEpub(){
    FILE * fp;
 
    char cwd[500];
    getcwd(cwd, sizeof(cwd));
    
    // Construct full command to zip and return finished epub
    string cmd = string();
    
    cmd += "cd " + bookFolder_;
    cmd += "; zip -0Xqv \'";
    cmd += title_ + ".epub\' mimetype;";
    cmd += "zip -Xr9Dqv \'";
    cmd += title_ + ".epub\' *; ";
    cmd += "cd " + string(cwd);

    if((fp = popen(cmd.c_str(), "w")) == NULL){
        cout << "Zipping failed!" << endl;
    }
    pclose(fp);
}
