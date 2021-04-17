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
    string fileName;    // Name of local file
    string fileVersion; // Name of file in .minigit folder
    singlyNode * next;
};

//doubly linked list node for each commit
struct doublyNode {
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;
};

//class that will make up the branches of the repository
class Branch {
    public:
        ~Branch();
        void init();
        void addFile(string fileName);
        void removeFile(string fileName);
        void commit();
        void checkout(int commitNumber);
    private:
        doublyNode* root = nullptr;
        doublyNode* currCommit = nullptr;
};

#endif