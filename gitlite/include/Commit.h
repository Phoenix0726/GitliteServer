#ifndef _COMMIT_H_
#define _COMMIT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>

using std::string;
using std::vector;
using std::unordered_map;


class Commit {
    string message;
    string id;
    vector<string> parents;
    unordered_map<string, string> blobs;    // first: filename, second: blobId
    string date;
    string file;
public:
    Commit();
    Commit(string message, vector<string> parents, unordered_map<string, string> blobs);
    string getId();
    string getFile();
    unordered_map<string, string> getBlobs();
    vector<string> getParents();
    string getMessage();
    string getDate();
    void save();
    static Commit readObject(string commitFile);
private:
    string generateId();
    string generateObjectFile();
    string getTime();
};

#endif