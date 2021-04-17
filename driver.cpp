#include <iostream>
#include <fstream>
#include "miniGit.hpp"
using namespace std;

int main() {
    string input;
    cout << "======Menu======" << endl;
    cout << "1. initialize an empty repository" << endl;
    cout << "2. quit" << endl;
    getline(cin, input);
    while (input != "1" && input != "2") {
        cerr << "Invalid option : " << input << endl;
        cout << "Try again" << endl;
        getline(cin, input);
    }
    if (input == "2") { //if the choose to quit
        cout << "Goodbye" << endl;
        return 0;
    }
    //else initialize
    Branch repository;
    repository.init();
    //then give the next options
    bool quit = false;
	while (!quit)
	{
		int option;
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
				cout << "Adding a file" << endl;
				break;
			}
            case 2:
            {
                cout << "Removing a file" << endl;
                break;
            }
			case 3:
            {
                cout << "Commiting a file" << endl;
                break;
            }
            case 4:
            {
                cout << "Checkout a commit number" << endl;
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