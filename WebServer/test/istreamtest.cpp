#include <iostream>
#include <fstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::istream;
using std::string;


void echo(istream& fin) {
    string buf;
    while (getline(fin, buf)) {
        cout << buf << endl;
    }
}

int main()
{
    ifstream fin("test.txt");
    echo(fin);
    return 0;
}
