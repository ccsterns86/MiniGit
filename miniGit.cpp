#include <iostream>
#include <fstream>
#include <filesystem>
#include "miniGit.hpp"
using namespace std;

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

Branch::~Branch()
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
            delete singlePrev;
        }
        prev = curr;
        curr = curr->next;
        singleCurr = curr->head;
        delete prev;
    }
}

void Branch::addFile(string fileName)
{
    //I did check to make sure that the file is in the current directory :)
    cout << "Adding " << fileName << "..." << endl; 
    //I didn't account for if they are trying to add a folder, don't know what to do about this
    
    // My current interpretation:
    singlyNode* newFile = new singlyNode;
    newFile->fileName = fileName;
    newFile->fileVersion = 1.0; // NOTE: we probably need some way to update this or get this value
    singlyNode* curr = currCommit->head;
    singlyNode* prev = nullptr;

    // Case: new node is the first node
    if (curr == nullptr)
    {
        currCommit->head = newFile;
        return;
    }

    while (curr != nullptr)
    {
        cout << curr->fileName << " has been added" << endl; // Temporary debug cout for your reference
        prev = curr;
        curr = curr->next;
    }
    prev->next = newFile;
    cout << endl;
    return;
}

void Branch::removeFile(string fileName)
{
    //I didn't do any sort of checking if the file existed here.
    //might want to have a message that says that file wasn't found, or success in removal
    cout << "Removed " << fileName << endl;
    return;
}

void Branch::commit()
{
    cout << "Committing..." << endl;
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