#include "Blob.h"
#include "sha1.h"
#include "Utils.h"
#include "Repository.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>

using std::ifstream;
using std::ofstream;


Blob::Blob(string srcFile) {
    this->srcFile = srcFile;
    this->content = readContents(srcFile);
    this->id = SHA1::sha1(srcFile, toString(content));
    this->blobFile = join(Repository::OBJECTS_DIR, id);
}

Blob::Blob(string id, string srcFile, string blobFile, vector<string> content) {
    this->id = id;
    this->srcFile = srcFile;
    this->blobFile = blobFile;
    this->content = content;
}

string Blob::getId() {
    return id;
}

string Blob::getFile() {
    return blobFile;
}

void Blob::save() {
    string dir = getParentFile(blobFile);
    if (access(dir.c_str(), 0) == -1) {
        mkdir(dir.c_str(), S_IRWXU);
    }
    
    ofstream fout(blobFile);
    fout << "BLOB\n";
    fout << id + '\n';
    fout << getRelativePath(Repository::CWD, srcFile) + '\n';
    fout << getRelativePath(Repository::CWD, blobFile) + '\n';
    for (auto line : content) {
        fout << line << '\n';
    }
    fout.close();
}

string Blob::getContentAsString() {
    string text = "";
    for (auto line : content) {
        text += line + '\n';
    }
    return text;
}

void Blob::writeContentToSource() {
    writeContents(srcFile, content);
}

Blob Blob::readObject(string blobFile) {
    ifstream fin(blobFile);

    string line;
    getline(fin, line);
    if (line != "BLOB") {
        throw "It's not a Blob";
    }

    string id;
    getline(fin, id);

    string srcFile;
    getline(fin, srcFile);
    srcFile = join(Repository::CWD, srcFile);

    getline(fin, line);
    
    vector<string> content;
    while (getline(fin, line)) {
        content.push_back(line);
    }

    return Blob(id, srcFile, blobFile, content);
}