#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <cctype>
#include <conio.h>
#include <limits>


using namespace std;

void clearScreen() {
    // If Windows, use "cls", if Linux or macOS, use "clear"
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// *******************
// Input Validation Functions
// *******************

// Function to check if the input is a valid CGPA (between 0.0 and 4.0)
bool isValidCGPA(float& cgpa) {
    string input;
    cin >> input;
    
    // converting the input to a float and check if it's fully numeric
    char* end;
    cgpa = strtof(input.c_str(), &end);
    
    // If end is not at the end of the string, input contains invalid characters
    if (*end != '\0' || cgpa < 0.0f || cgpa > 4.0f) {
        cin.clear();  // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard bad input
        cout << "\t\t\tInvalid CGPA! Please enter a valid CGPA between 0.0 and 4.0.\n";
        return false;
    }
    
    return true;
}


// Function to check if the input is a valid integer
bool isValidNumericInput(int& input) {
    string strInput;
    cin >> strInput;  // Read input as string

    // Check if all characters are digits (allow negative sign at the start)
    int startIndex = (strInput[0] == '-') ? 1 : 0; // Allow negative numbers
    for (size_t i = startIndex; i < strInput.length(); i++) {
        if (!isdigit(strInput[i])) {
            cout << "\t\t\tInvalid input! Please enter a valid number.\n";
            return false;
        }
    }

    // Convert to integer safely
    try {
        input = stoi(strInput);
    } catch (const out_of_range&) {
        cout << "\t\t\tNumber too large! Please enter a valid number.\n";
        return false;
    }

    return true;
}

//function to check if the input is a valid float
bool isValidNumericInput(float& input) {
    string strInput;
    cin >> strInput;  // Read input as a string

    char* end;  // Pointer to detect invalid characters
    input = strtof(strInput.c_str(), &end);  // Convert to float

    // If there are any non-numeric characters after the number, it's invalid
    if (*end != '\0') { 
        cout << "\t\t\tInvalid input! Please enter a valid number.\n";
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
        cout << "\t\t\tName cannot be empty or just spaces!\n";
        return false;
    }

    // Check if all characters in the trimmed name are alphabetic or spaces
    for (char ch : trimmedName) {
        if (!(isalpha(ch) || ch == ' ')) {  // Allow spaces in the name
            cout << "\t\t\tName must contain only alphabetic characters and spaces!\n";
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
        cout << "\t\t\tEnter Student Name: ";
        cin.ignore(); // Clear previous input buffer
        getline(cin, name);

        // Validate the student name
        if (!isValidName(name)) return;

        // Input roll number and check for duplicates
        bool rollNumberExists = false;
        do {
            cout << "\t\t\tEnter Roll Number: ";
            while (!isValidNumericInput(rollNumber)) {
                cout << "\t\t\tEnter valid Roll Number: ";
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
                cout << "\t\t\tRoll Number already exists! Please enter a unique Roll Number.\n";
            }

        } while (rollNumberExists); // Loop until a unique roll number is entered

        // Input CGPA
        cout << "\t\t\tEnter CGPA (0.0 to 4.0): ";
        while (!isValidCGPA(cgpa)) {
            cout << "\t\t\tEnter valid CGPA (0.0 to 4.0): ";
        }

        // Write student information to the file
        outFile << rollNumber << "|" << name << "|" << cgpa << endl;
        cout << "\t\t\tStudent added successfully!\n";
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in addStudent(): " << e.what() << endl;
    }
    cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to display all students from the file
void displayStudents() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        string line;
        cout << "\n\t\t\tRoll Number  | Name                 | CGPA\n";
        cout << "\t\t\t---------------------------------------\n";

        while (getline(inFile, line)) {
            size_t delimiter1 = line.find('|');
            size_t delimiter2 = line.find('|', delimiter1 + 1);

            string rollNumberStr = line.substr(0, delimiter1);
            string name = line.substr(delimiter1 + 1, delimiter2 - delimiter1 - 1);
            string marksStr = line.substr(delimiter2 + 1);

            int rollNumber = atoi(rollNumberStr.c_str());
            float marks = atof(marksStr.c_str());

            cout << "\t\t\t" << left << setw(12) << rollNumber << " | "
                 << setw(20) << name << " | "
                 << marks << endl;
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in displayStudents(): " << e.what() << endl;
    }
    cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to search for a student by roll number
void searchStudentByRollNumber() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        int rollNumber;
        cout << "\t\t\tEnter Roll Number to search: ";
        while (!isValidNumericInput(rollNumber)) {
            cout << "\t\t\tEnter valid Roll Number: ";
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
                cout << "\n\t\t\tStudent Found:\n";
                cout << "\t\t\tRoll Number: " << fileRollNumber << endl;
                cout << "\t\t\tName: " << name << endl;
                cout << "\t\t\tCGPA: " << marks << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\t\t\tStudent with Roll Number " << rollNumber << " not found.\n"<<endl;
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in searchStudentByRollNumber(): " << e.what() << endl;
    }
    cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to search for a student by name
void searchStudentByName() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        string nameToSearch;
        cout << "\t\t\tEnter Name to search: ";
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
                cout << "\n\t\t\tStudent Found:\n";
                cout << "\t\t\tRoll Number: " << rollNumber << endl;
                cout << "\t\t\tName: " << name << endl;
                cout << "\t\t\tCGPA: " << marks << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "\t\t\tNo students found with the name " << nameToSearch << ".\n";
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in searchStudentByName(): " << e.what() << endl;
    }
        cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to search for a student by CGPA
void searchStudentByCGPA() {
    try {
        ifstream inFile("students.txt");

        if (!inFile) {
            throw ios_base::failure("Error opening file for reading!");
        }

        float cgpaToSearch;
        cout << "\t\t\tEnter CGPA to search: ";
        while (!isValidNumericInput(cgpaToSearch)) {
            cout << "\t\t\tEnter valid CGPA: ";
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
                cout << "\n\t\t\tStudent Found:\n";
                cout << "\t\t\tRoll Number: " << rollNumber << endl;
                cout << "\t\t\tName: " << name << endl;
                cout << "\t\t\tCGPA: " << marks << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "\t\t\tNo students found with CGPA " << cgpaToSearch << ".\n";
        }

        inFile.close();
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in searchStudentByCGPA(): " << e.what() << endl;
    }
    cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to delete a student by roll number
void deleteStudent() {
    try {
        int rollNumber;
        cout << "\t\t\tEnter Roll Number to delete: ";
        while (!isValidNumericInput(rollNumber)) {
            cout << "\t\t\tEnter valid Roll Number: ";
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
            remove("students.xlsx"); // Delete the original file
            rename("temp.xlsx", "students.txt"); // Rename temp file to the original
            cout << "\t\t\tStudent with Roll Number " << rollNumber << " deleted successfully.\n";
        } else {
            cout << "\t\t\tStudent with Roll Number " << rollNumber << " not found.\n";
        }
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in deleteStudent(): " << e.what() << endl;
    }
        cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to update a student by roll number
void updateStudent() {
    try {
        int rollNumber;
        cout << "\t\t\tEnter Roll Number to update: ";
        while (!isValidNumericInput(rollNumber)) {
            cout << "\t\t\tEnter valid Roll Number: ";
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

                // Get the new name and CGPA
                cout << "\t\t\tEnter new Name: ";
                cin.ignore(); // Clear input buffer
                getline(cin, name);
                cout << "\t\t\tEnter new CGPA (0.0 to 4.0): ";
                while (!isValidCGPA(cgpa)) {
                    cout << "\t\t\tEnter valid CGPA (0.0 to 4.0): ";
                }

                // Write the updated student information to the temp file
                tempFile << rollNumber << "|" << name << "|" << cgpa << endl;
            } else {
                // Write the original student info to the temp file if no update
                tempFile << line << endl;
            }
        }

        inFile.close();
        tempFile.close();

        if (found) {
            remove("students.xlsx"); // Delete the original file
            rename("temp.xlsx", "students.xlsx"); // Rename the temp file to original
            cout << "\t\t\tStudent with Roll Number " << rollNumber << " updated successfully.\n";
        } else {
            cout << "\t\t\tStudent with Roll Number " << rollNumber << " not found.\n";
        }
    } catch (const ios_base::failure& e) {
        cout << "\t\t\tFile I/O error in updateStudent(): " << e.what() << endl;
    }
        cout<<"\n\n\t\t\tPress Enter to go to main menu!"<<endl;
    getche();
}

// Function to handle menu options
void menu() {
    int choice;
    
    do {
    	clearScreen();
        cout << "\n\t\t\t=== Student Management System ===\n";
        cout << "\t\t\t1. Add Student\n";
        cout << "\t\t\t2. Display Students\n";
        cout << "\t\t\t3. Search Student by Roll Number\n";
        cout << "\t\t\t4. Search Student by Name\n";
        cout << "\t\t\t5. Search Student by CGPA\n";
        cout << "\t\t\t6. Delete Student\n";
        cout << "\t\t\t7. Update Student\n";
        cout << "\t\t\t8. Exit\n";
        cout << "\t\t\tEnter your choice: ";
        while (!isValidNumericInput(choice)) {
            cout << "\t\t\tEnter valid choice: ";
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents();  break;
            case 3: searchStudentByRollNumber(); break;
            case 4: searchStudentByName();  break;
            case 5: searchStudentByCGPA();  break;
            case 6: deleteStudent();  break;
            case 7: updateStudent();  break;
            case 8: cout << "\t\t\tExiting program...\n"; break;
            default: cout << "\t\t\tInvalid choice! press Enter to try again.\n"; getche(); break;
        }
    } while (choice != 8);
}

const string USER_CREDENTIALS_FILE = "credentials.txt";

// *******************
// User Authentication Functions
// *******************

// Function to check if the input is a valid username (alphabetic and non-empty)
bool isValidUsername(const string& username) {
    for (char ch : username) {
        if (!(isalpha(ch))) {
            return false;
        }
    }
    return !username.empty();
}

// Function to check if the input is a valid password (non-empty)
bool isValidPassword(const string& password) {
    return !password.empty();
}

// Function to sign up (create a new user)
void signup() {
    string username, password;

    cout << "\t\t\tEnter new username: ";
    cin >> username;
    while (!isValidUsername(username)) {
        cout << "\t\t\tInvalid username! Username must be alphabetic and non-empty. Try again: ";
        cin >> username;
    }

    cout << "\t\t\tEnter new password: ";
    cin >> password;
    while (!isValidPassword(password)) {
        cout << "\t\t\tInvalid password! Password cannot be empty. Try again: ";
        cin >> password;
    }

    // Save the credentials to the file
    ofstream outFile(USER_CREDENTIALS_FILE, ios::app);
    if (!outFile) {
        cout << "\t\t\tError opening credentials file for writing.\n";
        return;
    }
    outFile << username << "|" << password << endl;
    outFile.close();

    cout << "\t\t\tSignup successful! You can now login.\n";
}

// Function to login (authenticate user)
bool login() {
    string username, password;

    cout << "\t\t\tEnter username: ";
    cin >> username;

    cout << "\t\t\tEnter password: ";
    cin >> password;

    ifstream inFile(USER_CREDENTIALS_FILE);
    if (!inFile) {
        cout << "\t\t\tError opening credentials file for reading.\n";
        return false;
    }

    string fileUsername, filePassword;
    while (getline(inFile, fileUsername, '|') && getline(inFile, filePassword)) {
        if (fileUsername == username && filePassword == password) {
            inFile.close();
            return true;
        }
    }

    inFile.close();
    cout << "\t\t\tInvalid username or password. Please try again.\n";
    return false;
}

// *******************
// Main Menu with Authentication
// *******************



int main() {
    int option;

    cout << "\t\t\t********** Welcome to the Student Management System **********\n";
    
    // Repeatedly prompt the user for valid input until it's correct
    do {
        cout << "\t\t\t1. Sign Up\n";
        cout << "\t\t\t2. Login\n";
        cout << "\t\t\t3. Exit\n";
        cout << "\t\t\tEnter your option: ";
        
        // Ensure the option is valid (numeric) and in the correct range
        while (!isValidNumericInput(option) || option < 1 || option > 3) {
            cout << "\t\t\tPlease enter a valid option (1, 2, or 3): ";
            // Clear input buffer to prevent infinite loop in case of invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Process the choice based on valid input
        switch (option) {
            case 1:
                signup();
                break;
            case 2:
                if (login()) {
                    menu(); // Proceed to the main menu if login is successful
                } else {
                    cout << "\t\t\tLogin failed. Exiting program.\n";
                }
                break;
            case 3:
                cout << "\t\t\tExiting program. Goodbye!\n";
                return 0;
            default:
                // This case will never be reached because the range is validated above
                cout << "\t\t\tInvalid option! Please select a valid option.\n";
        }
    } while (option < 1 || option > 3); // Keep asking until the user enters a valid option

    return 0;
}