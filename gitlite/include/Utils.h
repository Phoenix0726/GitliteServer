#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

vector<string> readContents(string file);

void writeContents(string file, string line);

void writeContents(string file, vector<string> content);

string join(string dir, string file);

string getParentFile(string file);

void Exit(string msg);

string toString(vector<string> lines);

string toString(unordered_map<string, string> umap);

string readContentsAsString(string file);

vector<string> plainFilenamesIn(string dir);

bool startsWith(const string& str, const string& head);

string getFileName(string file);

#endif