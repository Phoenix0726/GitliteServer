#ifndef _BLOB_H_
#define _BLOB_H_

#include <string>
#include <vector>

using std::string;
using std::vector;


class Blob {
    string id;
    string srcFile;
    string blobFile;
    vector<string> content;
public:
    Blob(string srcFile);
    Blob(string id, string srcFile, string blobFile, vector<string> content);
    string getId();
    string getFile();
    void save();
    string getContentAsString();
    void writeContentToSource();
    static Blob readObject(string blobFile);
};

#endif