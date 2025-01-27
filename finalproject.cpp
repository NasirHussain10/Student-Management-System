#include <iostream>
using namespace std;

// ======================= Student Management Functions =======================
// Function to add a new student
void addStudent(vector<Student>& students) {
    string name = getValidName();

    cout << "Enter RollNo of Student: ";
    string rollNo;
    cin >> rollNo;

    // Check for duplicate roll number
    auto it = find_if(students.begin(), students.end(), [&rollNo](const Student& s) {
        return toLower(s.getRollNo()) == toLower(rollNo);
    });
    if (it != students.end()) {
        cout << "Error: A student with this RollNo already exists.\n";
        return;
    }

    float cgpa = getValidCGPA();
    students.emplace_back(name, rollNo, cgpa);
    cout << "Student added successfully!\n";
}

// Function to search student by name
void searchByName(const vector<Student>& students) {
    cout << "Enter Name of Student: ";
    string name;
    cin.ignore();
    getline(cin, name);
    name = toLower(trim(name));

    searchStudents(students, name, "name");
}


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
        switch (choice) {
            case 1: addStudent(students); break;
            case 2:  break;
            case 3:  break;
            case 4:  break;
            case 5:  break;
            case 6:  break;
            case 7:  break;
            case 8: exit = true; break;
        }

  }
