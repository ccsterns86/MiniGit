#include <iostream>
#include <fstream>
#include "miniGit.hpp"
using namespace std;

int main() {
    bool quit = false;
	while (!quit)
	{
		int option;
		string input;
        //print menu options
		cout << "======Main Menu======" << endl;
		cout << "1. init" << endl;
		cout << "2. add" << endl;
        cout << "3. rm" << endl;
		cout << "4. commit" << endl;
        cout << "5. checkout" << endl;
        cout << "6. quit" << endl;

		getline(cin, input);
		option = stoi(input); //convert to number
		switch (option) {
			case 1:
			{
				cout << "Here is where we initialized a repository" << endl;
                break;
			}
			case 2:
			{
				cout << "Adding a file" << endl;
				break;
			}
            case 3:
            {
                cout << "Removing a file" << endl;
                break;
            }
			case 4:
            {
                cout << "Commiting a file" << endl;
                break;
            }
            case 5:
            {
                cout << "Checkout a commit number" << endl;
                break;
            }
            case 6:
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
}