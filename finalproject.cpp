#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <cctype>

using namespace std;

// *******************
// Input Validation Functions
// *******************

// Function to check if the input is a valid CGPA (between 0.0 and 4.0)
bool isValidCGPA(float& cgpa) {
    if (!(cin >> cgpa) || cgpa < 0.0f || cgpa > 4.0f) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid CGPA! Please enter a valid CGPA between 0.0 and 4.0.\n";
        return false;
    }
    return true;
}

// Function to check if the input is a valid integer
bool isValidNumericInput(int& input) {
    if (!(cin >> input)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a valid number.\n";
        return false;
    }
    return true;
}

// Function to check if the input is a valid float
bool isValidNumericInput(float& input) {
    if (!(cin >> input)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a valid number.\n";
        return false;
    }
    return true;
}

// Function to validate the student name (only alphabetic characters and spaces, no empty or spaces-only input)
bool isValidName(const string& name) {
    // Remove leading and trailing spaces
    string trimmedName = name;
    trimmedName.erase(0, trimmedName.find_first_not_of(' ')); // Trim leading spaces
    trimmedName.erase(trimmedName.find_last_not_of(' ') + 1); // Trim trailing spaces

    if (trimmedName.empty()) {
        cout << "Name cannot be empty or just spaces!\n";
        return false;
    }

    // Check if all characters in the trimmed name are alphabetic or spaces
    for (char ch : trimmedName) {
        if (!(isalpha(ch) || ch == ' ')) {  // Allow spaces in the name
            cout << "Name must contain only alphabetic characters and spaces!\n";
            return false;
        }
    }

    return true;
}

// *******************
// Student Operations Functions
// *******************

// Function to add a student
void addStudent() {
    try {
        ofstream outFile("students.txt", ios::app);
        if (!outFile) {
            throw ios_base::failure("Error opening file for writing!");
        }

        string name;
        int rollNumber;
        float cgpa;

        // Input student name
        cout << "Enter Student Name: ";
        cin.ignore(); // Clear previous input buffer
        getline(cin, name);

        // Validate the student name
        if (!isValidName(name)) return;

        // Input roll number and check for duplicates
        bool rollNumberExists = false;
        do {
            cout << "Enter Roll Number: ";
            while (!isValidNumericInput(rollNumber)) {
                cout << "Enter valid Roll Number: ";
            }

            // Check if the roll number already exists
            ifstream inFile("students.txt");
            string line;
            rollNumberExists = false; // Reset flag
            while (getline(inFile, line)) {
                size_t delimiter1 = line.find('|');
                string rollNumberStr = line.substr(0, delimiter1);
                int fileRollNumber = atoi(rollNumberStr.c_str());

                if (fileRollNumber == rollNumber) {
                    rollNumberExists = true;  // Roll number already exists
                    break;
                }
            }

            inFile.close();

            if (rollNumberExists) {
                cout << "Roll Number already exists! Please enter a unique Roll Number.\n";
            }

        } while (rollNumberExists); // Loop until a unique roll number is entered

        // Input CGPA
        cout << "Enter CGPA (0.0 to 4.0): ";
        while (!isValidCGPA(cgpa)) {
            cout << "Enter valid CGPA (0.0 to 4.0): ";
        }

        // Write student information to the file
        outFile << rollNumber << "|" << name << "|" << cgpa << endl;
        cout << "Student added successfully!\n";
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in addStudent(): " << e.what() << endl;
    }
}

// Function to display all students from the file
void displayStudents() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        string line;
        cout << "\nRoll Number | Name                | CGPA\n";
        cout << "---------------------------------------\n";

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            size_t delimiter2 = line.find('|', delimiter1 + 1);

            string rollNumberStr = line.substr(0, delimiter1);
            string name = line.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
            string marksStr = line.substr(delimiter2 + 1);

            int rollNumber = atoi(rollNumberStr.c_str());
            float marks = atof(marksStr.c_str());

            cout << left << setw(12) << rollNumber << " | "
                 << setw(20) << name << " | "
                 << marks << endl;
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in displayStudents(): " << e.what() << endl;
    }
}

// Function to search for a student by roll number
void searchStudentByRollNumber() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        int rollNumber;
        cout << "Enter Roll Number to search: ";
        while (!isValidNumericInput(rollNumber)) {
            cout << "Enter valid Roll Number: ";
        }

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            size_t delimiter2 = line.find('|', delimiter1 + 1);

            string rollNumberStr = line.substr(0, delimiter1);
            string name = line.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
            string marksStr = line.substr(delimiter2 + 1);

            int fileRollNumber = atoi(rollNumberStr.c_str());
            float marks = atof(marksStr.c_str());

            if (fileRollNumber == rollNumber) {
                cout << "\nStudent Found:\n";
                cout << "Roll Number: " << fileRollNumber << endl;
                cout << "Name: " << name << endl;
                cout << "CGPA: " << marks << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Student with Roll Number " << rollNumber << " not found.\n";
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in searchStudentByRollNumber(): " << e.what() << endl;
    }
}

// Function to search for a student by name
void searchStudentByName() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        string nameToSearch;
        cout << "Enter Name to search: ";
        cin.ignore();  // Clear the input buffer
        getline(cin, nameToSearch);

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            size_t delimiter2 = line.find('|', delimiter1 + 1);

            string rollNumberStr = line.substr(0, delimiter1);
            string name = line.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
            string marksStr = line.substr(delimiter2 + 1);

            int rollNumber = atoi(rollNumberStr.c_str());
            float marks = atof(marksStr.c_str());

            if (name == nameToSearch) {
                cout << "\nStudent Found:\n";
                cout << "Roll Number: " << rollNumber << endl;
                cout << "Name: " << name << endl;
                cout << "CGPA: " << marks << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No students found with the name " << nameToSearch << ".\n";
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in searchStudentByName(): " << e.what() << endl;
    }
}

// Function to search for a student by CGPA
void searchStudentByCGPA() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        float cgpaToSearch;
        cout << "Enter CGPA to search: ";
        while (!isValidNumericInput(cgpaToSearch)) {
            cout << "Enter valid CGPA: ";
        }

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            size_t delimiter2 = line.find('|', delimiter1 + 1);

            string rollNumberStr = line.substr(0, delimiter1);
            string name = line.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
            string marksStr = line.substr(delimiter2 + 1);

            int rollNumber = atoi(rollNumberStr.c_str());
            float marks = atof(marksStr.c_str());

            if (marks == cgpaToSearch) {
                cout << "\nStudent Found:\n";
                cout << "Roll Number: " << rollNumber << endl;
                cout << "Name: " << name << endl;
                cout << "CGPA: " << marks << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "No students found with CGPA " << cgpaToSearch << ".\n";
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in searchStudentByCGPA(): " << e.what() << endl;
    }
}

// Function to delete a student by roll number
void deleteStudent() {
    try {
        int rollNumber;
        cout << "Enter Roll Number to delete: ";
        while (!isValidNumericInput(rollNumber)) {
            cout << "Enter valid Roll Number: ";
        }

        ifstream inFile("students.txt");
        ofstream tempFile("temp.txt");

        if (!inFile || !tempFile) {
            throw ios_base::failure("Error opening file for reading or writing!");
        }

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            string rollNumberStr = line.substr(0, delimiter1);
            int fileRollNumber = atoi(rollNumberStr.c_str());

            if (fileRollNumber != rollNumber) {
                tempFile << line << endl; // Write the line to the temp file if it's not the one to delete
            } else {
                found = true;
            }
        }

        inFile.close();
        tempFile.close();

        if (found) {
            remove("students.txt"); // Delete the original file
            rename("temp.txt", "students.txt"); // Rename temp file to the original
            cout << "Student with Roll Number " << rollNumber << " deleted successfully.\n";
        } else {
            cout << "Student with Roll Number " << rollNumber << " not found.\n";
        }
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in deleteStudent(): " << e.what() << endl;
    }
}

// Function to update a student by roll number
void updateStudent() {
    try {
        int rollNumber;
        cout << "Enter Roll Number to update: ";
        while (!isValidNumericInput(rollNumber)) {
            cout << "Enter valid Roll Number: ";
        }

        ifstream inFile("students.txt");
        ofstream tempFile("temp.txt");

        if (!inFile || !tempFile) {
            throw ios_base::failure("Error opening file for reading or writing!");
        }

        string line;
        bool found = false;

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            string rollNumberStr = line.substr(0, delimiter1);
            int fileRollNumber = atoi(rollNumberStr.c_str());

            if (fileRollNumber == rollNumber) {
                found = true;
                string name;
                float cgpa;

                // Input new name and CGPA
                cout << "Enter new name: ";
                cin.ignore();  // Clear input buffer
                getline(cin, name);
                cout << "Enter new CGPA (0.0 to 4.0): ";
                while (!isValidCGPA(cgpa)) {
                    cout << "Enter valid CGPA (0.0 to 4.0): ";
                }

                // Write the updated information to temp file
                tempFile << rollNumber << "|" << name << "|" << cgpa << endl;
            } else {
                tempFile << line << endl;  // Copy the line to the temp file unchanged
            }
        }

        inFile.close();
        tempFile.close();

        if (found) {
            remove("students.txt"); // Delete the original file
            rename("temp.txt", "students.txt"); // Rename temp file to the original
            cout << "Student with Roll Number " << rollNumber << " updated successfully.\n";
        } else {
            cout << "Student with Roll Number " << rollNumber << " not found.\n";
        }
    } catch (const ios_base::failure& e) {
        cout << "File I/O error in updateStudent(): " << e.what() << endl;
    }
}

// *******************
// Main Function
// *******************

int main() {
    int choice;

    do {
        cout << "\n********** Student Management System **********\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by Roll Number\n";
        cout << "4. Search Student by Name\n";
        cout << "5. Search Student by CGPA\n";
        cout << "6. Delete Student\n";
        cout << "7. Update Student\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";

        if (!isValidNumericInput(choice)) {
            cout << "Please enter a valid menu option.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudentByRollNumber();
                break;
            case 4:
                searchStudentByName();
                break;
            case 5:
                searchStudentByCGPA();
                break;
            case 6:
                deleteStudent();
                break;
            case 7:
                updateStudent();
                break;
            case 8:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please select a valid option.\n";
        }
    } while (choice != 8);

    return 0;
}
