/*
 * ============================================================
 *         ONLINE EXAMINATION SYSTEM (OES)
 * ============================================================
 * Course   : CS-112 Object Oriented Programming and Design
 * Institute: Ghulam Ishaq Khan Institute of Engineering & Technology
 *
 * Headers used: <iostream>  <fstream>  <string>
 *
 * OOP Concepts Covered:
 *   a. Classes, Objects, Data Members, Access Specifiers, Encapsulation
 *   b. Default, Parameterized, Copy Constructors | Destructor | Init Lists
 *   c. Inheritance (Single/Multilevel/Hierarchical) | Overriding |
 *      Polymorphism | Virtual Functions | Abstract Classes
 *   d. Friend Functions/Classes | Static Members | Operator Overloading |
 *      Inline Functions | Nested Classes
 *   e. Passing/Returning Objects | Pointers | new/delete | this pointer
 *   f. Array of Objects | Searching | Sorting
 *   g. ifstream | ofstream | fstream | Text & Binary Files
 *   h. Custom Exceptions | try/catch/throw | Multiple Catch Blocks
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ============================================================
//  HELPER FUNCTIONS (no <algorithm> or <cctype>)
// ============================================================

// Convert a character to lowercase manually
char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}


    // Convert a whole string to lowercase manually
string toLowerStr(const string& s) {
    string result = s;
    for (int i = 0; i < (int)result.size(); i++)
        result[i] = toLowerChar(result[i]);
    return result;
}

// Convert int to string manually (no <sstream> or to_string)
string intToStr(int n) {
    if (n == 0) return "0";
    bool neg = (n < 0);
    if (neg) n = -n;
    string res = "";
    while (n > 0) {
        res = (char)('0' + n % 10) + res;
        n /= 10;
    }
    if (neg) res = "-" + res;
    return res;
}

// Print a line of repeated characters
void printLine(char ch = '=', int len = 50) {
    for (int i = 0; i < len; i++) cout << ch;
    cout << endl;
}

void printHeader(const string& title) {
    printLine();
    cout << "  " << title << endl;
    printLine();
}

void pauseScreen() {
    cout << "\n  [Press Enter to continue...]";
    cin.ignore();
    cin.get();
}

// ============================================================
//  h. CUSTOM EXCEPTION CLASSES
//     (standalone, no <stdexcept> or <exception> needed)
// ============================================================

class OESException {
protected:
    string message;
public:
    // b. Default Constructor
    OESException() : message("Unknown error") {}

    // b. Parameterized Constructor
    OESException(const string& msg) : message(msg) {}

    // b. Copy Constructor
    OESException(const OESException& other) : message(other.message) {}

    // b. Destructor
    virtual ~OESException() {}

    // Returns the error message
    virtual string what() const { return message; }
};

// Thrown when login username or password is wrong
class AuthException : public OESException {
public:
    AuthException(const string& msg)
        : OESException("[Login Error] " + msg) {}
};

// Thrown when an exam ID is not found
class ExamNotFoundException : public OESException {
public:
    ExamNotFoundException(const string& msg)
        : OESException("[Exam Not Found] " + msg) {}
};

// Thrown when the user enters bad values
class InvalidInputException : public OESException {
public:
    InvalidInputException(const string& msg)
        : OESException("[Invalid Input] " + msg) {}
};

// Thrown when a file cannot be opened
class FileException : public OESException {
public:
    FileException(const string& msg)
        : OESException("[File Error] " + msg) {}
};

// Thrown when a duplicate username is detected
class DuplicateException : public OESException {
public:
    DuplicateException(const string& msg)
        : OESException("[Duplicate] " + msg) {}
};

// ============================================================
//  a, b, c. ABSTRACT BASE CLASS: Person
// ============================================================


