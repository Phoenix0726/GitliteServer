#include "Repository.h"
#include "Utils.h"
#include "Blob.h"
#include "Commit.h"
#include "StageArea.h"

#include "../../WebServer/include/Client.h"

#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>

using std::cout;
using std::endl;
using std::queue;
using std::ifstream;
using std::ofstream;


const string Repository::CWD = current_path();
const string Repository::GITLITE_DIR = join(Repository::CWD, ".gitlite");
const string Repository::OBJECTS_DIR = join(Repository::GITLITE_DIR, "objects");
const string Repository::REFS_DIR = join(Repository::GITLITE_DIR, "refs");
const string Repository::HEADS_DIR = join(Repository::REFS_DIR, "heads");
const string Repository::HEAD = join(Repository::GITLITE_DIR, "HEAD");
const string Repository::STAGE = join(Repository::GITLITE_DIR, "stage");
const string Repository::USER = join(Repository::GITLITE_DIR, "user");

// gitlite init
void Repository::init() {
    if (access(GITLITE_DIR.c_str(), 0) != -1) {
        Exit("A Gitlet version-control system already exists in the current directory.");
    }
    mkdir(GITLITE_DIR.c_str(), S_IRWXU);
    mkdir(OBJECTS_DIR.c_str(), S_IRWXU);
    mkdir(REFS_DIR.c_str(), S_IRWXU);
    mkdir(HEADS_DIR.c_str(), S_IRWXU);

    setCurBranch("main");
    initCommit();
}

// gitlite add [file name]
void Repository::add(string fileName) {
    string file = join(CWD, fileName);
    if (access(file.c_str(), 0) == -1) {
        Exit("File does not exist.");
    }
    StageArea stageArea = readStageArea();
    if (stageArea.add(file)) {
        stageArea.save();
    }
}

// gitlite remove [file name]
void Repository::remove(string fileName) {
    string file = join(CWD, fileName);
    StageArea stageArea = StageArea::readObject(STAGE);
    if (!stageArea.remove(file)) {
        Exit("No reason to remove the file.");
    }
}


// gitlite commit [msg]
void Repository::commit(string msg) {
    if (msg == "") {
        Exit("Please enter a commit message.");
    }

    StageArea stageArea = StageArea::readObject(STAGE);
    if (stageArea.isEmpty()) {
        Exit("No changes added to the commit.");
    }

    Commit curCommit = getCurCommit();

    unordered_map<string, string> blobs = curCommit.getBlobs();
    unordered_map<string, string> added = stageArea.getAdded();
    unordered_set<string> removed = stageArea.getRemoved();

    blobs.insert(added.begin(), added.end());
    for (string file : removed) {
        blobs.erase(file);
    }

    vector<string> parents;
    parents.push_back(curCommit.getId());
    Commit commit(msg, parents, blobs);
    commit.save();
    
    addCommit(commit);
    
    stageArea.clear();
}

// gitlite log
void Repository::log() {
    Commit commit = getCurCommit();
    while (!commit.getParents().empty()) {
        printCommit(commit);
        vector<string> parents = commit.getParents();
        commit = getCommitById(parents[0]);
    }
    printCommit(commit);
}


// gitlite global-log
void Repository::globalLog() {
    vector<string> commitList = plainFilenamesIn(OBJECTS_DIR);
    for (string commitId : commitList) {
        try {
            Commit commit = getCommitById(commitId);
            printCommit(commit);
        } catch (...) {
        }
    }
}


// gitlite find [message]
void Repository::find(string message) {
    vector<string> commitList = plainFilenamesIn(OBJECTS_DIR);
    bool founded = false;
    for (string commitId : commitList) {
        try {
            Commit commit = getCommitById(commitId);
            if (message == commit.getMessage()) {
                cout << commitId << endl;
                founded = true;
            }
        } catch (...) {
            continue;
        }
    }
    if (!founded) {
        cout << "Found no commit with that message." << endl;
    }
}

// gitlite status
void Repository::status() {
    printBranches();
    printStagedFiles();
    printRemovedFiles();
    printModificationsNotStagedForCommit();
    printUntrackedFiles();
}

// gitlite checkout -- [file name]
void Repository::checkout(string file) {
    Commit curCommit = getCurCommit();
    checkout(curCommit.getId(), file);
}

// gitlite checkout [commit id] -- [file name]
void Repository::checkout(string commitId, string fileName) {
    Commit commit = getCommitById(commitId);

    unordered_map<string, string> blobs = commit.getBlobs();
    string file = join(CWD, fileName);
    if (blobs.count(file)) {
        string blobId = blobs[file];
        Blob blob = getBlobById(blobId);
        blob.writeContentToSource();
    } else {
        Exit("File does not exist in that commit.");
    }
}

// gitlite checkout [commit id]
void Repository::checkoutCommit(string commitId) {
    Commit commit = getCommitById(commitId);
    unordered_map<string, string> blobs = commit.getBlobs();
    for (auto it : blobs) {
        string blobId = it.second;
        Blob blob = getBlobById(blobId);
        blob.writeContentToSource();
    }
}

// gitlite checkout [branch name]
void Repository::checkoutBranch(string branchName) {
    string curBranch = readContentsAsString(HEAD);
    if (branchName == curBranch) {
        Exit("No need to checkout the current branch.");
    }
    vector<string> branches = plainFilenamesIn(HEADS_DIR);
    if (std::count(branches.begin(), branches.end(), branchName) == 0) {
        Exit("No such branch exists.");
    }

    Commit newCommit = getHeadCommitByBranchName(branchName);
    string newCommitId = newCommit.getId();
    changeCommitTo(newCommitId);

    setCurBranch(branchName);
}

// gitlite branch
void Repository::showBranch() {
    vector<string> branchList = plainFilenamesIn(HEADS_DIR);
    string curBranch = readContentsAsString(HEAD);
    cout << "*" << curBranch << endl;
    for (string branch : branchList) {
        if (branch != curBranch) {
            cout << branch << endl;
        }
    }
}

// gitlite branch [branch name]
void Repository::createBranch(string branchName) {
    if (branchName.size() >= 40) {
        Exit("Branch name length can't be greater than or equal to 48.");
    }
    vector<string> branchList = plainFilenamesIn(HEADS_DIR);
    if (std::count(branchList.begin(), branchList.end(), branchName)) {
        Exit("A branch with that name already exists.");
    }
    string branchFile = join(HEADS_DIR, branchName);
    Commit curCommit = getCurCommit();
    writeContents(branchFile, curCommit.getId());
}

// gitlite rm-branch branchName
void Repository::removeBranch(string branchName) {
    string curBranch = readContentsAsString(HEAD);
    if (branchName == curBranch) {
        Exit("Cannot remove the current branch.");
    }
    vector<string> branches =  plainFilenamesIn(HEADS_DIR);
    if (std::count(branches.begin(), branches.end(), branchName) == 0) {
        Exit("A branch with that name does not exist.");
    }
    string branchFile = join(HEADS_DIR, branchName);
    std::remove(branchFile.c_str());
}

// gitlite reset [commit]
void Repository::reset(string commitId) {
    changeCommitTo(commitId);
    string curBranch = readContentsAsString(HEAD);
    string branchFile = join(HEADS_DIR, curBranch);
    writeContents(branchFile, commitId);
}

// gitlite merge [branch name]
void Repository::merge(string branchName) {
    StageArea stageArea = readStageArea();
    if (!stageArea.isEmpty()) {
        Exit("You have uncommited changes.");
    }

    vector<string> branches = plainFilenamesIn(HEADS_DIR);
    if (std::count(branches.begin(), branches.end(), branchName) == 0) {
        Exit("A branch with that name does not exist.");
    }

    string curBranch = readContentsAsString(HEAD);
    if (branchName == curBranch) {
        Exit("Cannot merge a branch with itself.");
    }

    Commit curCommit = getCurCommit();
    Commit givCommit = getHeadCommitByBranchName(branchName);
    Commit comCommit = getCommonCommit(curCommit, givCommit);

    if (comCommit.getId() == givCommit.getId()) {
        Exit("Given branch is an ancestor of the current branch.");
    }
    if (comCommit.getId() == curCommit.getId()) {
        cout << "Current branch fast-forwarded." << endl;
        checkoutBranch(branchName);
        exit(0);
    }

    string message = "Merged " + branchName + " into " + curBranch + ".";
    vector<string> parents;
    parents.push_back(curCommit.getId());
    parents.push_back(givCommit.getId());
    unordered_map<string, string> newBlobs = curCommit.getBlobs();

    unordered_map<string, string> curBlobs = curCommit.getBlobs();
    unordered_map<string, string> givBlobs = givCommit.getBlobs();
    unordered_map<string, string> comBlobs = comCommit.getBlobs();

    for (auto it : givBlobs) {
        string file = it.first;
        if (!curBlobs.count(file) && !comBlobs.count(file)) {
            string untrackedFile = file;
            if (access(untrackedFile.c_str(), 0) != -1) {
                Exit("There is an untracked file in the way; delete it, or add and commit it first.");
            }
        }
    }

    bool haveConflict = false;
    unordered_set<string> allFiles = getAllFiles(curBlobs, givBlobs, comBlobs);
    for (string file : allFiles) {
        if (curBlobs.count(file) && givBlobs.count(file) && comBlobs.count(file)) {
            string curBlobId = curBlobs[file];
            string givBlobId = givBlobs[file];
            string comBlobId = comBlobs[file];
            // case 1
            if (curBlobId == comBlobId && givBlobId != comBlobId) {
                Blob blob = getBlobById(givBlobId);
                blob.writeContentToSource();
                newBlobs.insert({file, givBlobId});
                stageArea.add(file);
            }
            // case 8
            if (curBlobId != comBlobId && givBlobId != comBlobId && curBlobId != givBlobId) {
                string newBlobId = dealWithConflict(file, curBlobId, givBlobId);
                newBlobs.insert({file, newBlobId});
                haveConflict = true;
            }
        }
        // case 5
        if (!curBlobs.count(file) && givBlobs.count(file) && !comBlobs.count(file)) {
            string givBlobId = givBlobs[file];
            Blob blob = getBlobById(givBlobId);
            blob.writeContentToSource();
            newBlobs.insert({file, givBlobId});
            stageArea.add(file);
        }
        if (curBlobs.count(file) && !givBlobs.count(file) && comBlobs.count(file)) {
            string curBlobId = curBlobs[file];
            string comBlobId = comBlobs[file];
            // case 6
            if (curBlobId == comBlobId) {
                std::remove(file.c_str());
                newBlobs.erase(file);
            } else {    // case 8
                string newBlobId = dealWithConflict(file, curBlobId, "");
                newBlobs.insert({file, newBlobId});
                haveConflict = true;
            }
        }
        if (curBlobs.count(file) && givBlobs.count(file) && !comBlobs.count(file)) {
            string curBlobId = curBlobs[file];
            string givBlobId = givBlobs[file];
            // case 8
            if (curBlobId != givBlobId) {
                string newBlobId = dealWithConflict(file, curBlobId, givBlobId);
                newBlobs.insert({file, newBlobId});
                haveConflict = true;
            }
        }
        if (!curBlobs.count(file) && givBlobs.count(file) & comBlobs.count(file)) {
            string givBlobId = givBlobs[file];
            string comBlobId = comBlobs[file];
            // case 8
            if (givBlobId != comBlobId) {
                string newBlobId = dealWithConflict(file, "", givBlobId);
                newBlobs.insert({file, newBlobId});
                haveConflict = true;
            }
        }
    }
    
    Commit newCommit(message, parents, newBlobs);
    newCommit.save();
    addCommit(newCommit);
    stageArea.clear();

    if (haveConflict) {
        cout << "Encountered a merge conflict." << endl;
    }
}

// gitlite push
void Repository::push() {
    string username = getUsername();
    if (username == "") {
        printf("please set ther username first!");
        return;
    }

    Client client;

    client.send("username: " + username);

    queue<string> fileq;
    fileq.push(GITLITE_DIR);
    while (!fileq.empty()) {
        string file = fileq.front();
        fileq.pop();
        if (isDir(file)) {  // 如果是文件夹，继续展开
            vector<string> files = plainFilenamesIn(file);
            for (auto it : files) {
                fileq.push(join(file, it));
            }
        } else {    // 发送文件
            client.sendfile(getRelativePath(CWD, file));
        }
    }
}

// gitlite clone
void Repository::clone() {
    string username = getUsername();
    if (username == "") {
        printf("please set ther username first!");
        return;
    }

    Client client;

    client.send("username: " + username);
    client.receive();

    // 根据 .gitlite 读出当前 commit 源文件
    Commit curCommit = getCurCommit();
    unordered_map<string, string> blobs = curCommit.getBlobs();
    for (auto it : blobs) {
        string blobId = it.second;
        Blob blob = getBlobById(blobId);
        blob.writeContentToSource();
    }
}

// gitlite set username [username]
void Repository::set(string username) {
    ofstream fout(USER);
    fout << username;
    fout.close();
}


/* private function */

string Repository::getUsername() {
    ifstream fin(USER);
    if (fin) {
        string username;
        fin >> username;
        fin.close();
        return username;
    }
    return "";
}

void Repository::setCurBranch(string branchName) {
    writeContents(HEAD, branchName);
}

Commit Repository::getCurCommit() {
    string curBranch = readContentsAsString(HEAD);
    return getHeadCommitByBranchName(curBranch);
}

void Repository::checkIfInitialized() {
    if (access(GITLITE_DIR.c_str(), 0) == -1) {
        Exit("Not in an initialized Gitlet directory.");
    }
}

Commit Repository::getHeadCommitByBranchName(string branchName) {
    string branchHeadFile = join(HEADS_DIR, branchName);
    string branchHeadCommitId = readContentsAsString(branchHeadFile);
    string branchHeadCommitFile = join(OBJECTS_DIR, branchHeadCommitId);
    Commit branchHeadCommit = Commit::readObject(branchHeadCommitFile);
    return branchHeadCommit;
}

StageArea Repository::readStageArea() {
    if (access(STAGE.c_str(), 0) == -1) {
        return StageArea();
    }
    return StageArea::readObject(STAGE);
}

void Repository::addCommit(Commit commit) {
    string curBranch = readContentsAsString(HEAD);
    string curBranchHeadFile = join(HEADS_DIR, curBranch);
    writeContents(curBranchHeadFile, commit.getId());
}

void Repository::initCommit() {
    Commit commit;
    commit.save();
    addCommit(commit);
}

Commit Repository::getCommitById(string id) {
    vector<string> commitList = plainFilenamesIn(OBJECTS_DIR);
    for (string commitId : commitList) {
        if (startsWith(commitId, id)) {
            id = commitId;
            break;
        }
    }
    string commitFile = join(OBJECTS_DIR, id);
    if (access(commitFile.c_str(), 0) == -1) {
        Exit("No commit with that id exists.");
    }
    Commit commit = Commit::readObject(commitFile);
    return commit;
}

Blob Repository::getBlobById(string id) {
    string blobFile = join(OBJECTS_DIR, id);
    if (access(blobFile.c_str(), 0) == -1) {
        exit(1);
    }
    Blob blob = Blob::readObject(blobFile);
    return blob;
}

void Repository::printCommit(Commit commit) {
    cout << "===" << endl;
    cout << "commit " << commit.getId() << endl;
    if (commit.getParents().size() > 1) {
        vector<string> parents = commit.getParents();
        cout << "Merge: " << parents[0].substr(0, 8) << " "
            << parents[1].substr(0, 8) << endl;
    }
    cout << "Date: " << commit.getDate() << endl;
    cout << commit.getMessage() << endl;
    cout << endl;
}

void Repository::printBranches() {
    vector<string> branchList = plainFilenamesIn(HEADS_DIR);
    string curBranch = readContentsAsString(HEAD);
    cout << "=== Branches ===" << endl;
    cout << "*" << curBranch << endl;
    for (string branch : branchList) {
        if (branch != curBranch) {
            cout << branch << endl;
        }
    }
    cout << endl;
}

void Repository::printStagedFiles() {
    cout << "=== Staged Files ===" << endl;
    StageArea stageArea = readStageArea();
    unordered_map<string, string> added = stageArea.getAdded();
    vector<string> fileList;
    for (auto it : added) {
        fileList.push_back(it.first);
    }
    printFileList(fileList);
}

void Repository::printRemovedFiles() {
    cout << "=== Removed Files ===" << endl;
    StageArea stageArea = readStageArea();
    unordered_set<string> removed = stageArea.getRemoved();
    vector<string> fileList(removed.begin(), removed.end());
    printFileList(fileList);
}

void Repository::printModificationsNotStagedForCommit() {
    cout << "=== Modifications Not Staged For Commit ===" << endl;
    cout << endl;
}

void Repository::printUntrackedFiles() {
    cout << "=== Untracked Files ===" << endl;
    cout << endl;
}

void Repository::printFileList(vector<string> fileList) {
    for (string file : fileList) {
        cout << getFileName(file) << endl;
    }
    cout << endl;
}

void Repository::changeCommitTo(string commitId) {
    Commit curCommit = getCurCommit();
    Commit newCommit = getCommitById(commitId);
    unordered_map<string, string> curBlobs = curCommit.getBlobs();
    unordered_map<string, string> newBlobs = newCommit.getBlobs();

    for (auto it : newBlobs) {
        string file = it.first;
        if (!curBlobs.count(file)) {
            string untrackedFile = file;
            if (access(untrackedFile.c_str(), 0) != -1) {
                Exit("There is an untracked file in the way; delete it, or add and commit it first.");
            }
        }
    }

    for (auto it : curBlobs) {
        string file = it.first;
        if (!newBlobs.count(file)) {
            std::remove(file.c_str());
        }
    }

    for (auto it : newBlobs) {
        string blobId = it.second;
        Blob blob = getBlobById(blobId);
        blob.writeContentToSource();
    }

    StageArea stageArea = readStageArea();
    stageArea.clear();
}

Commit Repository::getCommonCommit(Commit a, Commit b) {
    unordered_set<string> S;
    queue<Commit> Q;

    Q.push(a);
    while (!Q.empty()) {
        Commit commit = Q.front();
        Q.pop();
        S.insert(commit.getId());
        vector<string> parents = commit.getParents();
        for (string parent : parents) {
            Q.push(getCommitById(parent));
        }
    }

    Q.push(b);
    while (!Q.empty()) {
        Commit commit = Q.front();
        Q.pop();
        string commitId = commit.getId();
        if (S.count(commitId)) {
            return getCommitById(commitId);
        } else {
            S.insert(commitId);
        }
        vector<string> parents = commit.getParents();
        for (string parent : parents) {
            Q.push(getCommitById(parent));
        }
    }

    return Commit();
}

unordered_set<string> Repository::getAllFiles(unordered_map<string, string> a, 
unordered_map<string, string> b, unordered_map<string, string> c) {
    unordered_set<string> S;
    for (auto it : a) {
        S.insert(it.first);
    }    
    for (auto it : b) {
        S.insert(it.first);
    }
    for (auto it : c) {
        S.insert(it.first);
    }
    return S;
}

string Repository::dealWithConflict(string file, string curBlobId, string givBlobId) {
    string curBlobContent = (curBlobId != "") ? getBlobById(curBlobId).getContentAsString() : "";
    string givBlobContent = (givBlobId != "") ? getBlobById(givBlobId).getContentAsString() : "";
    string mergeContent = "<<<<<<< HEAD\n" + curBlobContent + "=======\n"
        + givBlobContent + ">>>>>>>\n";
    writeContents(file, mergeContent);

    Blob newBlob(file);
    newBlob.save();
    return newBlob.getId();
}