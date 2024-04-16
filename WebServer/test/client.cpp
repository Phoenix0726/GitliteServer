#include "Client.h"


using std::ifstream;


int main()
{
    Client client;
    string filename = "../test/test.txt";
    ifstream fin(filename);
    client.send("file: test.txt");
    client.sendfile(fin);
    client.send(":end");
    fin.close();
    return 0;
}
