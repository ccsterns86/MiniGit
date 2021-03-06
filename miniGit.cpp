#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "miniGit.hpp"
using namespace std;

//extra function declarations
bool hasChanged(singlyNode *node);
string readFileIntoString(string filename);
singlyNode* retreiveCommitHead(int commitNum, doublyNode* root);
void printOutDifferences(singlyNode *node);

/* ------------------------------------------------------ */
void Branch::init()
{
        // Initialize structure
        filesystem::create_directory(".minigit");
        root = new doublyNode;
        currCommit = root;
        cout << "Repository successfuly initialized! " << endl;
    }

void Branch::DeleteBranch() // TODO: fix crashing bug for when nodes are populated
// Also: do we care if there are already files in the minigit, or should we delete them all? This will make the reopening previous a little harder, unless you want to make an additional folder
{
    doublyNode* curr = root;
    doublyNode* prev = nullptr;
    singlyNode* singleCurr = root->head;
    singlyNode* singlePrev = nullptr;

    if (root == nullptr) // Case for empty branch
    {
        return;
    }
    while (root != nullptr) {
        while (curr->head != nullptr) {
            //cout << "deleting: " << curr->head->fileVersion << endl;
            curr->head = curr->head->next;
            delete singleCurr;
            singleCurr = curr->head;
        }
        //cout << "~Deleted Commit: " << curr->commitNumber << endl;
        root = root->next;
        delete curr;
        curr = root;
    }
    //cout << "Deleted all commits" << endl;  

    //delete all of the minigit
    filesystem::remove_all(".minigit"); // A cool debug line, but if we do saving, it can't be used :(
}

bool isNewNode(singlyNode* cursor, string key)
{
    while(cursor != nullptr)
    {
        if (cursor->fileName == key)
        {
            return false;
        }
        cursor = cursor->next;
    }
    return true;
}

void Branch::addFile(string fileName)
{
    //I did check to make sure that the file is in the current directory :)
    cout << "Adding " << fileName << "..." << endl; 
    //I didn't account for if they are trying to add a folder, don't know what to do about this

    if (!isNewNode(currCommit->head, fileName)) // Case for if that file has already been added
    {
        cout << "This file has already been added!" << endl;
        return;
    }
    
    // My current interpretation:
    singlyNode* newFile = new singlyNode;
    newFile->fileName = fileName;
    newFile->fileVersion = "01" + fileName; // NOTE: don't worry about updating, I do this in the commit section

    singlyNode* curr = currCommit->head;
    singlyNode* prev = nullptr;

    // Case: new node is the first node
    if (curr == nullptr)
    {
        currCommit->head = newFile;
        return;
    }
    //TODO: need to check if the file has already been added, check while already looping through. Maybe check if it isn't a repeat before making a node?
    while (curr != nullptr)
    {
        //cout << curr->fileName << " has been added" << endl; // Temporary debug cout for your reference
        prev = curr;
        curr = curr->next;
    }
    prev->next = newFile;
    //cout << newFile->fileName << " has been added" << endl; // Another temporary debug cout
    cout << endl;
    return;
}

void Branch::removeFile(string fileName)
{
    //I didn't do any sort of checking if the file existed here.
    //might want to have a message that says that file wasn't found, or success in removal
    if(filesystem::exists(fileName))
    {
        singlyNode* prev = nullptr; 
        singlyNode* cursor = currCommit->head;

        if (cursor == nullptr)
        {
            cout << "No files have been added to this commit!" << endl;
        }
        while (true)
        {
            if (cursor->fileName == fileName) break;
            else
            {
                prev = cursor;
                cursor = cursor->next;
            }
        }
        if (cursor == currCommit->head) // Delete the head node
        {
            currCommit->head = cursor->next;
            delete cursor;
        }
        else // Was not the head node
        {
            prev->next = cursor->next;
            delete cursor;
        }
    }
    else
    {
        cout << "File does not exist! " << endl;
        return;
    }
    cout << "Removed " << fileName << " successfully." << endl;
    return;
}

void Branch::commit()
{
    if (currCommit->head == nullptr) { //if there is nothing in the list yet
        cout << "Nothing to commit. Add something to commit." << endl;
        return;
    }
    //make a new DLL node
    doublyNode *newCommit = new doublyNode;
    newCommit->commitNumber = currCommit->commitNumber + 1;
    newCommit->previous = currCommit;
    currCommit->next = newCommit;

    string minifilename;
    singlyNode *curr = currCommit->head;
    singlyNode *newCurr = new singlyNode;
    newCommit->head = newCurr;
    singlyNode *newPrev;

    while (curr != nullptr) {
        minifilename = ".minigit/" + curr->fileVersion;
        if(!filesystem::exists(minifilename)) { //if the file version number doesn't exist yet
            ofstream file(minifilename);
            string text = readFileIntoString(curr->fileName);
            file << text;
            file.close();
        }
        else { //check to see if the file has changed since last commit
            bool changed = hasChanged(curr);
            if (changed == true) { //it has changed: make new file, and update SLL
                string version = to_string(stoi(curr->fileVersion.substr(0, 2)) + 1);
                if (version.length() == 1) {
                    version = "0" + version;
                }
                minifilename = version + curr->fileName;
                ofstream file(".minigit/" + minifilename);
                string text = readFileIntoString(curr->fileName);
                file << text;
                file.close();
                curr->fileVersion = minifilename;
            }
        }
        
        //add this SLL node to the new commit
        if (newCurr != newCommit->head) { //if this isn't the head
            newCurr = new singlyNode;
            newPrev->next = newCurr;
        }
        newCurr->fileName = curr->fileName;
        newCurr->fileVersion = curr->fileVersion;
        newCurr->next = nullptr;
        newPrev = newCurr;
        newCurr = nullptr;
        curr = curr->next;
    }
    //add the DLL node to the end
    currCommit->next = newCommit;
    currCommit = newCommit;

    cout << "~*~Committed~*~" << endl <<  "Commit Number: " << currCommit->previous->commitNumber << endl;
    newCommit = nullptr;
    return;
}

void Branch::checkout(int commitNumber) //TODO: figure out why it deletes the subdirectory? Find out why it isn't finding the correct file contents
//TODO: delete all changes made to the commit final node
{
    singlyNode *curr = retreiveCommitHead(commitNumber, root);
    string file;
    string compFile;
    string contents;

    while (curr != nullptr) { //going through each of the items in the chosen commit
        compFile = "./" + curr->fileName;
        for (auto & p : filesystem::directory_iterator(".")) { //going through the directory to see if they exist
            file = p.path();
            if (compFile == file) { //file found in the directory
                //delete the old file
                filesystem::remove(file);
                break;
            }
        }
        //create the document
        contents = readFileIntoString(".minigit/" + curr->fileVersion);
        ofstream newFile(curr->fileName); //fix this eventually
        newFile << contents;
        newFile.close();
        //go to next node in the SLL
        curr = curr->next;
    }
    cout << "Checking out commit number " << commitNumber << endl;
    return;
}

int Branch::getLastCommitNum() { //returns most current commit number
    return currCommit->previous->commitNumber;
}

void Branch::printStatus() {
    if (currCommit->head == nullptr) { //if nothing in the repository
        cout << "Nothing in the repository, add something to see status" << endl;
        return;
    }

    string minifile;
    singlyNode *curr = currCommit->head;
    bool modified = false;
    bool newFile = false;
    bool changed;
    
    while (curr != nullptr) { //go through each node in the SLL to see if it has changed
        minifile = ".minigit/" + curr->fileVersion;
        if (!filesystem::exists(minifile)) { //file just added, so there are changes
            cout << "new file: " << curr->fileName << endl;
            newFile = true;
        }
        else { //check if the file has been modified
            changed = hasChanged(curr);
            if (changed == true) {
                cout << "modified: " << curr->fileName << endl;
                modified = true;
            }
        }
        curr = curr->next;
    }
    if (modified == false && newFile == false) { //nothing was modified
        cout << "commit up to date" << endl;
    }
    else if (newFile == true) {
        cout << "commit changes to commit file" << endl;
    }
    else { //check to see if they want to see the differences
        cout << "If you would like to see the differences enter 1, else enter anything else" << endl;
        string input;
        getline(cin, input);
        if (input == "1") { //want to see the differences
            curr = currCommit->head;
            while (curr != nullptr) {
                minifile = ".minigit/" + curr->fileVersion;
                if(filesystem::exists(minifile)) { //if the file exists
                    changed = hasChanged(curr);
                    if (changed == true) { //if the file has changed
                        printOutDifferences(curr);
                    }
                }
                curr = curr->next;
            }
        }
    }
}

/* -------------------Extra functions------------------- */
string readFileIntoString(string filename) {
    ifstream input_file(filename);
    return string((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
}

bool hasChanged(singlyNode *node) {
    string dirFile = readFileIntoString(node->fileName);
    string miniFile = readFileIntoString(".minigit/" + node->fileVersion);
    if (dirFile == miniFile) { //same contents
        return false;
    }
    else return true;
}

singlyNode* retreiveCommitHead(int commitNum, doublyNode* root) {
    doublyNode *curr = root;
    while(curr->commitNumber != commitNum) {
        curr = curr->next;
    }
    return curr->head;
}

void printOutDifferences(singlyNode *node) {
    string minifileName = ".minigit/" + node->fileVersion;
    string commitLine = "";
    string currentLine = "";
    fstream currentFile;
    currentFile.open(node->fileName);
    fstream commitFile;
    commitFile.open(minifileName);
    while (getline(commitFile, commitLine) && getline(currentFile, currentLine)) {
        if (commitLine != currentLine) {
            cout << "File: " << node->fileName << endl;
            cout << "Difference (if the two are the same, a line has been added or removed): " << endl;
            cout << "commited: " << commitLine << endl;
            cout << "current: " << currentLine << endl;
        }
    }
    currentFile.close();
    commitFile.close();
}