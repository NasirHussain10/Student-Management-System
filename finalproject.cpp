#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;
// ======================= Helper Functions =======================

// Helper function to validate alphabetic names
bool isAlphabetic(const string& name) {
    for (char c : name) {
        if (!isalpha(c) && c != ' ') { // Allow spaces in names
            return false;
        }
    }
    return !name.empty();
}

// Helper function to convert a string to lowercase
string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Helper function to trim spaces
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return ""; // Empty string
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// Function to get a valid name from the user
string getValidName() {
    string name;
    while (true) {
        cout << "Enter Name of Student : ";
        cin.ignore();
        getline(cin, name);
        name = trim(name);
        if (isAlphabetic(name)) {
            return name;
        }
        cout << "Invalid input! Name should contain only alphabetic characters. Please try again.\n";
    }
}

// Function to get a valid CGPA from the user
float getValidCGPA() {
    float cgpa;
    while (true) {
        cout << "Enter CGPA of Student (0.0 - 4.0): ";
        cin >> cgpa;
        if (!cin.fail() && cgpa >= 0.0 && cgpa <= 4.0) {
            return cgpa;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! CGPA must be a number between 0.0 and 4.0. Please try again.\n";
    }
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

// ======================= Student Class =======================

class Student {
private:
    string Name, RollNo;
    float CGPA;

public:
    Student() : Name(""), RollNo(""), CGPA(0.0) {}

    Student(string name, string rollNo, float cgpa)
        : Name(move(name)), RollNo(move(rollNo)), CGPA(cgpa) {}

    void setName(const string& name) {
        Name = name;
    }

    void setRollNo(const string& rollNo) {
        RollNo = rollNo;
    }

    void setCGPA(float cgpa) {
        CGPA = cgpa;
    }

    string getName() const {
        return Name;
    }

    string getRollNo() const {
        return RollNo;
    }

    float getCGPA() const {
        return CGPA;
    }

    string toString() const {
        stringstream ss;
        ss << Name << " : " << RollNo << " : " << fixed << setprecision(2) << CGPA;
        return ss.str();
    }
};


// ======================= Student Management Functions =======================
// General search function
void searchStudents(const vector<Student>& students, const string& searchTerm, const string& searchBy) {
    bool found = false;
    for (const auto& student : students) {
        if ((searchBy == "name" && toLower(student.getName()) == searchTerm) ||
            (searchBy == "rollno" && toLower(student.getRollNo()) == searchTerm) ||
            (searchBy == "cgpa" && to_string(student.getCGPA()) == searchTerm)) {
            cout << student.toString() << endl;
            found = true;
        }
    }
    if (!found) cout << "Student not found.\n";
}

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

// Function to search student by roll number
void searchByRollNo(const vector<Student>& students) {
    cout << "Enter RollNo of Student: ";
    string rollNo;
    cin >> rollNo;

    searchStudents(students, toLower(trim(rollNo)), "rollno");
}

// Function to search students by CGPA
void searchByCGPA(const vector<Student>& students) {
    float cgpa = getValidCGPA();
    bool found = false;
    for (const auto& student : students) {
        if (student.getCGPA() == cgpa) {
            cout << student.toString() << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No students found with CGPA: " << cgpa << endl;
    }
}

// Function to update a student's information
void updateStudent(vector<Student>& students) {
    cout << "Enter RollNo of Student: ";
    string rollNo;
    cin >> rollNo;

    for (auto& student : students) {
        if (toLower(student.getRollNo()) == toLower(rollNo)) {
            cout << "Select the detail to update:\n";
            cout << "1. Name\n2. RollNo\n3. CGPA\n";
            int updateChoice;
            cin >> updateChoice;

            switch (updateChoice) {
                case 1:
                    student.setName(getValidName());
                    cout << "Name updated successfully.\n";
                    break;
                case 2:
                    {
                        string newRollNo;
                        cout << "Enter new RollNo: ";
                        cin >> newRollNo;
                        student.setRollNo(newRollNo);
                        cout << "RollNo updated successfully.\n";
                    }
                    break;
                case 3:
                    student.setCGPA(getValidCGPA());
                    cout << "CGPA updated successfully.\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
                    break;
            }
            return;
        }
    }
    cout << "Student not found.\n";
}

// Function to display all students
void displayAll(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No students found!\n";
        return;
    }

    cout << left << setw(20) << "Name" 
         << left << setw(15) << "RollNo" 
         << left << setw(10) << "CGPA" 
         << "\n";
    cout << string(45, '-') << "\n";

    for (const auto& student : students) {
        cout << left << setw(20) << student.getName()
             << left << setw(15) << student.getRollNo()
             << left << setw(10) << fixed << setprecision(2) << student.getCGPA()
             << "\n";
    }
}

// Function to remove a student
void removeStudent(vector<Student>& students) {
    cout << "Enter RollNo of Student to Remove: ";
    string rollNo;
    cin >> rollNo;

    auto it = find_if(students.begin(), students.end(), [&rollNo](const Student& s) {
        return toLower(s.getRollNo()) == toLower(rollNo);
    });

    if (it != students.end()) {
        char confirm;
        cout << "Are you sure you want to remove this student? (Y/N): ";
        cin >> confirm;
        if (toupper(confirm) == 'Y') {
            students.erase(it);
            cout << "Student removed successfully.\n";
        }
    } else {
        cout << "Student not found.\n";
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
            case 2: searchByName(students); break;
            case 3: searchByRollNo(students); break;
            case 4: searchByCGPA(students); break;
            case 5: updateStudent(students); break;
            case 6: displayAll(students); break;
            case 7: removeStudent(students); break;
            case 8: exit = true; break;
        }

  }
