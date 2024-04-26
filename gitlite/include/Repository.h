#ifndef _REPOSITORY_H_
#define _REPOSITORY_H_

#include <string>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include <unordered_map>

using std::string;
using std::filesystem::current_path;
using std::unordered_set;
using std::unordered_map;
using std::vector;


class Blob;
class Commit;
class StageArea;


class Repository {
public:
    /*
    .gitlite
        |--objects
        |   |--commit and blob
        |--refs
        |   |--heads
        |       |--main
        |--HEAD
        |--stage
        |--USER
    */
    static const string CWD;
    static const string GITLITE_DIR;
    static const string OBJECTS_DIR;
    static const string REFS_DIR;
    static const string HEADS_DIR;
    static const string HEAD;
    static const string STAGE;
    static const string USER;

    static void init();     // gitlite init
    static void add(string fileName);   // gitlite add [file name]
    static void remove(string fileName);    // gitlite remove [file name]
    static void commit(string msg);     // gitlite commit [msg]
    static void log();      // gitlite log
    static void globalLog();    // gitlite global-log
    static void find(string message);   // gitlite find [message]
    static void status();       // gitlite status
    static void checkout(string file);      // gitlite checkout -- [file name]
    static void checkout(string commitId, string file);     // gitlite checkout [commit id] -- [file name]
    static void checkoutCommit(string commitId);        // gitlite checkout [commit id]
    static void checkoutBranch(string branchName);      // gitlite checkout [branch name]
    static void showBranch();      // gitlite branch
    static void createBranch(string branchName);        // gitlite branch [branch name]
    static void removeBranch(string branchName);        // gitlite rm-branch branchName
    static void reset(string commitId);     // gitlite reset [commit]
    static void merge(string branchName);   // gitlite merge [branch name]
    static void push();     // gitlite push
    static void clone();    // gitlite clone
    static void set(string username);   // gitlite set username [username]

    static void checkIfInitialized();
private:
    static Commit getCurCommit();
    static string getUsername();
    static void setCurBranch(string branchName);
    static void initCommit();
    static void addCommit(Commit commit);
    static Commit getHeadCommitByBranchName(string branchName);
    static Commit getCommitById(string id);
    static Blob getBlobById(string id);
    static void printCommit(Commit commit);
    static StageArea readStageArea();
    static void printBranches();
    static void printStagedFiles();
    static void printRemovedFiles();
    static void printModificationsNotStagedForCommit();
    static void printUntrackedFiles();
    static void printFileList(vector<string> fileList);
    static void changeCommitTo(string commitId);
    static Commit getCommonCommit(Commit a, Commit b);
    static unordered_set<string> getAllFiles(unordered_map<string, string> a, unordered_map<string, string> b, unordered_map<string, string> c);
    static string dealWithConflict(string file, string curBlobId, string givBlobId);
};

#endif
