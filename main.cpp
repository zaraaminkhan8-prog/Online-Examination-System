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
