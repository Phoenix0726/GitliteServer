#include "StageArea.h"
#include "Repository.h"
#include "Commit.h"
#include "Blob.h"

#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

using std::ofstream;
using std::ifstream;


unordered_map<string, string> StageArea::getAdded() {
    return added;
}

unordered_set<string> StageArea::getRemoved() {
    return removed;
}

void StageArea::save() {
    ofstream fout(Repository::STAGE);
    fout << added.size() << '\n';
    for (auto it : added) {
        fout << it.first << '\n' << it.second << '\n';
    }
    fout << removed.size() << '\n';
    for (auto it : removed) {
        fout << it << '\n';
    }
    fout.close();
}

bool StageArea::add(string file) {
    Blob blob(file);
    if (added.count(file)) {    // the added set already had the file
        if (added[file] == blob.getId()) {  // the file had not modify
            return false;
        }
    }
    if (isNewBlob(blob, file)) {
        added.insert({file, blob.getId()});
    }
    removed.erase(file);

    blob.save();
    save();
    return true;
}

bool StageArea::remove(string file) {
    if (added.count(file)) {    // if added set had this file
        added.erase(file);
        save();
        return true;
    }
    Commit curCommit = Repository::getCurCommit();
    if (curCommit.getBlobs().count(file)) {
        if (access(file.c_str(), 0) != -1) {
            std::remove(file.c_str());
        }
        removed.insert(file);
        save();
        return true;
    }
    return false;
}

bool StageArea::isEmpty() {
    return added.empty() && removed.empty();
}

void StageArea::clear() {
    added.clear();
    removed.clear();
    save();
}

StageArea StageArea::readObject(string stageFile) {
    StageArea stageArea;
    ifstream fin(stageFile);
    string line;
    getline(fin, line);
    int asize = atoi(line.c_str());
    for (int i = 0; i < asize; i++) {
        getline(fin, line);
        string line2;
        getline(fin, line2);
        stageArea.added.insert({line, line2});
    }
    getline(fin, line);
    int rsize = atoi(line.c_str());
    for (int i = 0; i < rsize; i++) {
        getline(fin, line);
        stageArea.removed.insert(line);
    }
    fin.close();
    return stageArea;
}

bool StageArea::isNewBlob(Blob blob, string path) {
    Commit curCommit = Repository::getCurCommit();
    if (curCommit.getBlobs().count(path)) {
        if (curCommit.getBlobs()[path] == blob.getId()) {
            return false;
        }
    }
    return true;
}