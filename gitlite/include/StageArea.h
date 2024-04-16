#ifndef _STAGEAREA_H_
#define _STAGEAREA_H_

#include <unordered_map>
#include <unordered_set>
#include <string>

using std::unordered_map;
using std::unordered_set;
using std::string;


class StageArea {
    unordered_map<string, string> added;
    unordered_set<string> removed;
public:
    unordered_map<string, string> getAdded();
    unordered_set<string> getRemoved();
    void save();
    bool add(string file);
    bool remove(string file);
    bool isEmpty();
    void clear();
    static StageArea readObject(string stageFile);
private:
    bool isNewBlob(Blob blob, string path);
};

#endif