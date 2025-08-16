#include <iostream>   // For console input/output: std::cout, std::cin
#include <fstream>    // For file input/output: std::fstream, std::ifstream, std::ofstream
using namespace std;

/*
  Simple file-based login system (for learning purposes).

  Data format stored in "loginData.txt":
    username*email*password\n

  IMPORTANT: This stores passwords in plain text.
  This is NOT secure and should not be used in real applications.
*/

class temp {
    // --- Private data members used across methods ---
    string userName, Email, password;         // Holds one record read from file (username, email, password)
    string searchName, searchPass, searchEmail; // Holds values typed by the user at the console
    fstream file;                              // Reusable file stream (opened per method when needed)

public:
    // --- Public operations ---
    void login();   // Try to sign in by matching username + password from the file
    void signUP();  // Append a new "username*email*password" line to the file
    void forgot();  // Find a record by username + email and print the password
} obj;              // Create a global object 'obj' to call methods from main()

int main() {
    char choice;    // Will store the user's menu selection as a single character

    // --- Simple text menu ---
    cout << "\n1- Login";
    cout << "\n2- Sign-Up";
    cout << "\n3- Forgot Password";
    cout << "\n4- Exit";
    cout << "\nEnter Your Choice :: ";
    cin >> choice;  // Reads the choice; NOTE: leaves a '\n' in the input buffer

    // Dispatch to the chosen action
    switch (choice) {
        case '1':
            // Remove the leftover '\n' from the input buffer so that the next getline() works
            cin.ignore();
            obj.login();
            break;

        case '2':
            cin.ignore();  // Same reason as above
            obj.signUP();
            break;

        case '3':
            cin.ignore();  // Same reason as above
            obj.forgot();
            break;

        case '4':
            return 0;      // Exit the program
            break;

        default:           // Handles any other character
            cout << "Invalid Selection...!";
    }
}

/*
  signUP()
  - Asks the user for username, email, password (using getline to keep spaces).
  - Appends a new line to the file in the format: username*email*password\n
*/
void temp::signUP() {
    cout << "\nEnter Your User Name :: ";
    getline(cin, userName);  // getline keeps spaces and stops at '\n'

    cout << "Enter Your Email Address :: ";
    getline(cin, Email);

    cout << "Enter Your Password :: ";
    getline(cin, password);

    // Open the file for output in append mode.
    // ios::app means new data is written at the end; existing content remains.
    file.open("loginData.txt", ios::out | ios::app);

    // Write one record separated by '*' and terminated by newline.
    file << userName << "*" << Email << "*" << password << endl;

    file.close(); // Always close the file when done.
}

/*
  login()
  - Prompts for username and password.
  - Reads the file record-by-record using getline with delimiters ('*' and '\n').
  - If a matching username+password is found, prints account info.
*/
void temp::login() {

    cout << "----------LOGIN---------" << endl;

    cout << "Enter Your User Name :: " << endl;
    getline(cin, searchName);   // Read the username to search

    cout << "Enter Your Password :: " << endl;
    getline(cin, searchPass);   // Read the password to compare

    // Open the file for reading (input mode)
    file.open("loginData.txt", ios::in);

    // Prime-read the first record (username, email, password)
    // getline(file, var, delim) reads until 'delim' and removes it from the stream.
    getline(file, userName, '*');  // read username up to '*'
    getline(file, Email, '*');     // read email up to '*'
    getline(file, password, '\n'); // read password up to end of line

    // Loop until end-of-file (note: while(!eof()) is not ideal, but kept to match your structure)
    while (!file.eof()) {
        // Check if the current record matches the username
        if (userName == searchName) {
            // If the username matches, check the password
            if (password == searchPass) {
                cout << "\nAccount Login Succesfull...!";
                cout << "\nUsername :: " << userName << endl;
                cout << "\nEmail :: " << Email << endl;
                // (No early break here, so it would continue scanning;
                //  for this educational example we keep your structure.)
            } else {
                cout << "Password is Incorrect...!";
            }
        }

        // Read the next record (3 fields again)
        getline(file, userName, '*');
        getline(file, Email, '*');
        getline(file, password, '\n');
    }

    file.close(); // Close the file when finished
}

/*
  forgot()
  - Prompts for username + email.
  - Scans the file; if a record with both values matches, prints the stored password.
*/
void temp::forgot() {
    cout << "\nEnter Your UserName :: ";
    getline(cin, searchName);

    cout << "\nEnter Your Email Address :: ";
    getline(cin, searchEmail);

    // Open the file for reading
    file.open("loginData.txt", ios::in);

    // Read the first record (username, email, password)
    getline(file, userName, '*');
    getline(file, Email, '*');
    getline(file, password, '\n');

    // Scan each record until end-of-file
    while (!file.eof()) {
        // First, match username
        if (userName == searchName) {
            // Then match email
            if (Email == searchEmail) {
                cout << "\nAccount Found...!" << endl;
                cout << "Your Password :: " << password << endl;
            } else {
                cout << "Not found...!\n";  // Username matched but email did not
            }
        } else {
            cout << "\nNot fount...!\n";    // Username did not match (typo kept as in your code)
        }

        // Read the next record
        getline(file, userName, '*');
        getline(file, Email, '*');
        getline(file, password, '\n');
    }

    file.close(); // Close the file
}
