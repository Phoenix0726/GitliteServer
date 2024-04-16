#ifndef _SHA1_H_
#define _SHA1_H_

#include <string>

using std::string;

namespace SHA1 {
    string sha1(string s1, string s2);
    string sha1(string s1, string s2, string s3, string s4);
};

#endif