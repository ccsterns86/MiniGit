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