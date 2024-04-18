#include "Commit.h"
#include "Utils.h"
#include "sha1.h"
#include "Repository.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>

using std::ifstream;
using std::ofstream;


Commit::Commit() {
    date = getTime();
    message = "initial commit";
    id = generateId();
    file = generateObjectFile();
}

Commit::Commit(string message, vector<string> parents, unordered_map<string, string> blobs) {
    this->date = getTime();
    this->message = message;
    this->parents = parents;
    this->blobs = blobs;
    this->id = generateId();
    this->file = generateObjectFile();
}

string Commit::generateId() {
    return SHA1::sha1(date, message, toString(parents), toString(blobs));
}

string Commit::generateObjectFile() {
    return join(Repository::OBJECTS_DIR, id);
}

string Commit::getId() {
    return id;
}

string Commit::getFile() {
    return file;
}

unordered_map<string, string> Commit::getBlobs() {
    return blobs;
}

vector<string> Commit::getParents() {
    return parents;
}

string Commit::getMessage() {
    return message;
}

string Commit::getDate() {
    return date;
}

void Commit::save() {
    string dir = getParentFile(file);
    if (access(dir.c_str(), 0) == -1) {
        mkdir(dir.c_str(), S_IRWXU);
    }

    ofstream fout(file);

    fout << "COMMIT\n";

    fout << message + '\n';
    fout << id + '\n';

    fout << parents.size() << '\n';
    for (auto it : parents) {
        fout << it + '\n';
    }

    fout << blobs.size() << '\n';
    for (auto it : blobs) {
        fout << it.first + '\n' + it.second + '\n';
    }

    fout << date;
    fout << file + '\n';
    fout.close();
}

Commit Commit::readObject(string commitFile) {
    Commit commit;
    ifstream fin(commitFile);
    
    string line;
    getline(fin, line);
    if (line != "COMMIT") {
        throw "It's not a Commit";
    }

    getline(fin, commit.message);
    getline(fin, commit.id);
    
    
    getline(fin, line);
    int psize = atoi(line.c_str());
    for (int i = 0; i < psize; i++) {
        getline(fin, line);
        commit.parents.push_back(line);
    }

    getline(fin, line);
    int bsize = atoi(line.c_str());
    for (int i = 0; i < bsize; i++) {
        getline(fin, line);
        string line2;
        getline(fin, line2);
        commit.blobs.insert({line, line2});
    }

    getline(fin, commit.date);
    getline(fin, commit.file);
    fin.close();

    return commit;
}

string Commit::getTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    return dt;
}