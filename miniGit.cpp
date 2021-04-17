#include <iostream>
#include <fstream>
#include "miniGit.hpp"
using namespace std;

/* ------------------------------------------------------ */
Branch::Branch()
{
    // Initialize structure
    if (root == nullptr)
    {
        root = new doublyNode;
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

}

void Branch::removeFile(string fileName)
{

}

void Branch::commit()
{

}

void Branch::checkout(int commitNumber)
{

}