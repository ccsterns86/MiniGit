#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "miniGit.hpp"
using namespace std;

//extra function declarations
bool hasChanged(singlyNode *node);
string readFileIntoString(string filename);

/* ------------------------------------------------------ */
void Branch::init()
{
    // Initialize structure
    if (root == nullptr)
    {
        root = new doublyNode;
        currCommit = root;
        if (!filesystem::exists(".minigit"))
        {
            // Create the folder
            filesystem::create_directory(".minigit");
        }
        cout << "Repository successfuly initialized! " << endl;
    }
    else
    {
        cerr << "This repository has already been initialized!" << endl;
    }
}

Branch::~Branch() // TODO: fix crashing bug for when nodes are populated
// Also: do we care if there are already files in the minigit, or should we delete them all? This will make the reopening previous a little harder, unless you want to make an additional folder
{
    doublyNode* curr = root;
    doublyNode* prev = nullptr;

    if (curr == nullptr) // Case for empty branch
    {
        return;
    }
    if (root->head == nullptr) 
    {
        delete root;
        return;
    }
    singlyNode* singleCurr = root->head;
    singlyNode* singlePrev = nullptr;
    while (curr != nullptr)
    {
        while(singleCurr != nullptr)
        {
            singlePrev = singleCurr;
            singleCurr = singleCurr->next;
            cout << "Deleting: " << singlePrev->fileName << endl;
            delete singlePrev;
        }
        prev = curr;
        curr = curr->next;
        singleCurr = curr->head;
        delete prev;
    }
}

void Branch::addFile(string fileName) // TODO: check to see if file has already been added
{
    //I did check to make sure that the file is in the current directory :)
    cout << "Adding " << fileName << "..." << endl; 
    //I didn't account for if they are trying to add a folder, don't know what to do about this
    
    // My current interpretation:
    singlyNode* newFile = new singlyNode;
    newFile->fileName = fileName;
    newFile->fileVersion = "01" + fileName; // NOTE: we probably need some way to update this or get this value
    //fileVersion is the name of the file in the .minigit file and it does need to be updated
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
        cout << curr->fileName << " has been added" << endl; // Temporary debug cout for your reference
        prev = curr;
        curr = curr->next;
    }
    prev->next = newFile;
    cout << newFile->fileName << " has been added" << endl; // Another temporary debug cout
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
            if (cursor->next = nullptr) // If it's the only node in the LL
            {
                delete currCommit->head;
                currCommit->head = 0;
            }
            else // Deleting the head w/ other nodes in the LL
            {
                singlyNode* forward = cursor->next;
                delete cursor;
                currCommit->head = forward;
            }
        }
        else // Was not the head node
        {
            singlyNode* forward = cursor->next;
            delete cursor;
            prev->next = forward;
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

    cout << "~Committed~" << endl <<  "Commit Number: " << currCommit->previous->commitNumber << endl;
    newCommit = nullptr;
    return;
}

void Branch::checkout(int commitNumber)
{
    cout << "Checking out commit number " << commitNumber << endl;
    return;
    // Also note that you must disallow add, remove, and commit operations when the current version is different from the most recent commit (the last DLL node)
}

int Branch::getLastCommitNum() { //returns most current commit number
    return currCommit->commitNumber;
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