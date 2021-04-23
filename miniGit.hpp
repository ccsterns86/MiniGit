/****************************************************************/
/*                     Minigit Definition                       */
/****************************************************************/
#ifndef MINIGIT_HPP
#define MINIGIT_HPP

#include <iostream>
#include <string>
using namespace std;

//singly linked list node for the files in each commit
struct singlyNode {
    string fileName = "";    // Name of local file
    string fileVersion = ""; // Name of file in .minigit folder
    singlyNode * next = nullptr;
};

//doubly linked list node for each commit
struct doublyNode {
    int commitNumber = 0;
    singlyNode * head = nullptr;
    doublyNode * previous = nullptr;
    doublyNode * next = nullptr;
};

//class that will make up the branches of the repository
class Branch {
    public:
        void DeleteBranch();
        void init();
        void addFile(string fileName);
        void removeFile(string fileName);
        void commit();
        void checkout(int commitNumber);
        void populate(); // serialization
        void addCommit(int commitNumber); // also serialization
        int getLastCommitNum();
    private:
        doublyNode* root = nullptr;
        doublyNode* currCommit = nullptr;
};

#endif