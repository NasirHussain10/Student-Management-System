#include <iostream>
using namespace std;

// Function to get a valid menu choice from the user
int getValidMenuChoice() {
    int choice;
    while (true) {
        cout << "Enter Choice: ";
        cin >> choice;
        if (!cin.fail() && choice >= 1 && choice <= 8) {
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice! Please enter a number between 1 and 8.\n";
    }
}

  int main (){
  	
  	cout << "Welcome To Student Management System\n";
        cout << "************************************\n";
        cout << "1. Add Student.\n";
        cout << "2. Search Student by Name.\n";
        cout << "3. Search Student by RollNo.\n";
        cout << "4. Search Student by CGPA.\n";
        cout << "5. Update Student.\n";
        cout << "6. Display All Students.\n";
        cout << "7. Remove Student.\n";
        cout << "8. Exit.\n";
        
        int choice = getValidMenuChoice();
  }
