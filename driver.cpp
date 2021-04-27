#include <iostream>
#include <fstream>
#include <filesystem>
#include "miniGit.hpp"
using namespace std;

//function declaration
void checkout(Branch repository, bool revertToHead, bool &isCurrent);

int main() {
    string input; 
    //the first menu to start the initialize process
    cout << "======Menu======" << endl;
    cout << "1. initialize" << endl;  // Changed to initialize because it will do so regardless of the directory's existence
    cout << "2. quit" << endl;

    getline(cin, input); //user input
    while (input != "1" && input != "2") { //wrong input
        cerr << "Invalid option : " << input << endl;
        cout << "Try again" << endl;
        getline(cin, input);
    }
    if (input == "2") { //if the choose to quit
        cout << "Goodbye" << endl;
        return 0;
    }
    //else: initialize
    Branch repository;
    repository.init();
    //then give the next options
    bool quit = false;
    bool isCurrent = true;
	while (!quit)
	{
		int option;
        string filename;
        
        if (!isCurrent) { // if the user isn't on the most current commit
            cout << "======Menu======" << endl;
            cout << "1. checkout" << endl;
            cout << "2. return to head" << endl;

            getline(cin, input);
            option = stoi(input); //convert to number
            switch (option)
            {
            case 1:
                checkout(repository, false, isCurrent);
                break;
            case 2:
                checkout(repository, true, isCurrent);
                break;
            default:
                cerr << "Invalid option : " << input << endl;
                break;
            }
        }
        else {
            //print menu options
            cout << "======Menu======" << endl;
            cout << "1. add" << endl;
            cout << "2. rm" << endl;
            cout << "3. commit" << endl;
            cout << "4. checkout" << endl;
            cout << "5. status" << endl;
            cout << "6. quit" << endl;

            getline(cin, input);
            if (input[0] < '0' || input [0] > '9' || input.size() > 1)
            {
                option = 99;
            }
            else option = stoi(input); //convert to number
            
            switch (option) {
                case 1:
                {   
                    string temp;
                    while (true) { //check to see if the file is in the directory
                        cout << "Enter a filename: " << endl;
                        getline(cin, filename);
                        if (!filesystem::exists(filename)) 
                        { // filename not found
                            cout << "Invalid filename: try again" << endl;
                        }
                        else break;
                    }	
                    repository.addFile(filename);			
                    break;
                }
                case 2:
                {
                    cout << "Enter a filename: " << endl;
                    getline(cin, filename);
                    repository.removeFile(filename);
                    break;
                }
                case 3:
                {
                    repository.commit(false);
                    break;
                }
                case 4:
                {
                    checkout(repository, false, isCurrent);             
                    break;
                }
                case 5:
                {
                    repository.printStatus();
                    break;
                }
                case 6:
                {
                    quit = true;
                    repository.DeleteBranch();
                    break;
                }
                default:
                {
                    cerr << "Invalid option : " << input << endl;
                    break;
                }
            }
        }
	}
    cout << "Goodbye" << endl;
}

void checkout(Branch repository, bool revertToHead, bool &isCurrent) {
    int comNum = -1;
    string input;
    if (revertToHead == true) {
        comNum = repository.getLastCommitNum();
    }
    else {
        while(!(comNum >= 0 && comNum <= repository.getLastCommitNum())) { //user isn't in valid commit number range
            cout << "Enter the commit number you would like to checkout: " << endl;
            getline(cin, input);
            comNum = stoi(input);
            if (!(comNum >= 0 && comNum <= repository.getLastCommitNum())) {
                cout << "Invalid commit number: try again" << endl;
            }
        }
    }
    cout << "Warning: all local changes will be erased if you continue." << endl;
    cout << "Enter 1 if you would like to continue" << endl;
    getline(cin, input);
    if (input == "1") {
        if (comNum == repository.getLastCommitNum()) {
            isCurrent = true;
        }
        else {
            isCurrent = false;
        }
        repository.checkout(comNum);
    }
}