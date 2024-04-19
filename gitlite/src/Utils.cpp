#include "Utils.h"

#include <iostream>
#include <fstream>
#include <numeric>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>

using std::ifstream;
using std::ofstream;


vector<string> readContents(string file) {
    vector<string> content;
    ifstream fin(file);
    string line;
    while (getline(fin, line)) {
        content.push_back(line);
    }
    fin.close();
    return content;
}

void writeContents(string file, string line) {
    ofstream fout(file);
    fout << line;
    fout.close();
}

void writeContents(string file, vector<string> content) {
    ofstream fout(file);
    for (auto line : content) {
        fout << line + '\n';
    }
    fout.close();
}

string join(string dir, string file) {
    return dir + '/' + file;
}

string getParentFile(string file) {
    for (int i = file.size() - 1; i >= 0; i--) {
        if (file[i] == '/') {
            return file.substr(0, i);
        }
    }
    return "";
}

void Exit(string msg) {
    std::cout << msg << std::endl;
    exit(1);
}

string toString(vector<string> lines) {
    string text = "";
    return accumulate(lines.begin(), lines.end(), text);
}

string toString(unordered_map<string, string> umap) {
    string text = "";
    for (auto it : umap) {
        text += it.first + it.second;
    }
    return text;
}

string readContentsAsString(string file) {
    return toString(readContents(file));
}

vector<string> plainFilenamesIn(string dir) {
    vector<string> files;
    DIR* pDir;
    struct dirent* ptr;
    if (!(pDir = opendir(dir.c_str()))) {
        std::cout << "Folder doesn't exist!" << std::endl;
        exit(1);
    }
    while (ptr = readdir(pDir)) {
        if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name, "..")) {
            files.push_back(ptr->d_name);
        }
    }
    closedir(pDir);
    return files;
}

bool isDir(string dir) {
    struct stat buffer;
    return (stat (dir.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool startsWith(const string& str, const string& head) {
    return str.compare(0, head.size(), head) == 0;
}

string getFileName(string file) {
    string name = "";
    for (int i = file.size() - 1; i >= 0; i--) {
        if (file[i] == '/') {
            break;
        }
        name = file[i] + name;
    }
    return name;
}

string getRelativePath(string dir, string path) {   // 获取文件在 dir 下的相对位置
    int n = dir.size();
    if (path.size() < n) {
        std::cout << path << " is not a file in dir " << dir << std::endl;
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        if (dir[i] != path[i]) {
            std::cout << path << " is not a file in dir " << dir << std::endl;
            exit(1);
        }
    }
    return path.substr(n);
}

void mkdirOfPath(string path) {
    string fpath = getParentFile(path);
    if (fpath == "") return;
    if (getParentFile(fpath) != "") mkdirOfPath(fpath);
    if (access(fpath.c_str(), 0) == -1) {
        mkdir(fpath.c_str(), S_IRWXU);
    }
}