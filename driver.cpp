#include <iostream>
#include <fstream>
#include <filesystem>
#include "miniGit.hpp"
using namespace std;

int main() {
    string input; 
    //the first menu to start the initialize process
    cout << "======Menu======" << endl;
    cout << "1. initialize an empty repository" << endl;
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
	while (!quit)
	{
		int option;
        string filename;
        //print menu options
		cout << "======Main Menu======" << endl;
		cout << "1. add" << endl;
        cout << "2. rm" << endl;
		cout << "3. commit" << endl;
        cout << "4. checkout" << endl;
        cout << "5. quit" << endl;

		getline(cin, input);
		option = stoi(input); //convert to number
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
                repository.commit();
                break;
            }
            case 4:
            {
                int comNum = 0;
                while(!(comNum > 0 && comNum <= repository.getLastCommitNum())) { //user isn't in valid commit number range
                    cout << "Enter the commit number you would like to checkout: " << endl;
                    getline(cin, input);
                    comNum = stoi(input);
                    if (!(comNum > 0 && comNum <= repository.getLastCommitNum())) {
                        cout << "Invalid commit number: try again" << endl;
                    }
                }
                cout << "Warning: all local changes will be erased if you continue." << endl;
                cout << "Enter 1 if you would like to continue" << endl;
                getline(cin, input);
                if (input == "1") {
                    repository.checkout(comNum);
                }
                break;
            }
            case 5:
			{
				quit = true;
                break;
			}
            default:
            {
                cerr << "Invalid option : " << input << endl;
                break;
            }
        }
	}
    cout << "Goodbye" << endl;
}