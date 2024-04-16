#include "Repository.hpp"

#include <iostream>
#include <cstring>

using std::cout;
using std::endl;


void validateArgsNum(int argc, int n) {
    if (argc != n) {
        std::cout << "Incorrect operands." << std::endl;
        exit(1);
    }
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "Please enter a command." << std::endl;
        return 1;
    }
    string op = argv[1];
    if (op == "init") {
        validateArgsNum(argc, 2);
        Repository::init();
    } else if (op == "add") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string file = argv[2];
        Repository::add(file);
    } else if (op == "rm") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string file = argv[2];
        Repository::remove(file);
    } else if (op == "commit") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string message = argv[2];
        Repository::commit(message);
    } else if (op == "log") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 2);
        Repository::log();
    } else if (op == "global-log") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 2);
        Repository::globalLog();
    }else if (op == "find") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string message = argv[2];
        Repository::find(message);
    } else if (op == "status") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 2);
        Repository::status();
    } else if (op == "checkout") {
        Repository::checkIfInitialized();
        switch (argc) {
            case 4: {
                if (!strcmp(argv[2], "--")) {
                    cout << "Incorrect operands." << endl;
                    exit(1);
                }
                string fileName = argv[3];
                Repository::checkout(fileName);
                break;
            }
            case 5: {
                if (!strcmp(argv[3], "--")) {
                    cout << "Incorrect operands." << endl;
                    exit(1);
                }
                string commitId = argv[2];
                string fileName = argv[4];
                Repository::checkout(commitId, fileName);
                break;
            }
            case 3: {
                string branchName = argv[2];
                Repository::checkoutBranch(branchName);
                break;
            }
            default: {
                cout << "Incorrect operands." << endl;
                exit(1);
            }
        }
    } else if (op == "branch") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string branchName = argv[2];
        Repository::createBranch(branchName);
    } else if (op == "rm-branch") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string branchName = argv[2];
        Repository::removeBranch(branchName);
    } else if (op == "reset") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string commitId = argv[2];
        Repository::reset(commitId);
    } else if (op == "merge") {
        Repository::checkIfInitialized();
        validateArgsNum(argc, 3);
        string branchName = argv[2];
        Repository::merge(branchName);
    } else {
        cout << "No command with that name exists." << endl;
    }
    return 0;
}