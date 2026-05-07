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

class Person {
protected:
    string name;    // a. protected: accessible by derived classes
    int    id;
    string email;
    static int personCount; // d. Static Data Member

public:
    // b. Default Constructor with Initialization List
    Person() : name("Unknown"), id(0), email("") {
        personCount++;
    }

    // b. Parameterized Constructor with Initialization List
    Person(const string& n, int i, const string& e)
        : name(n), id(i), email(e) {
        personCount++;
    }

    // b. Copy Constructor
    Person(const Person& other)
        : name(other.name), id(other.id), email(other.email) {
        personCount++;
    }

    // b. Virtual Destructor
    virtual ~Person() {
        personCount--;
    }

    // c. Pure Virtual Functions — makes Person an Abstract Class
    virtual void   displayInfo() const = 0;
    virtual string getRole()    const = 0;

    // d. Inline Getters (Encapsulation / Data Hiding)
    inline string getName()  const { return name;  }
    inline int    getId()    const { return id;    }
    inline string getEmail() const { return email; }

    // Setters
    void setName(const string& n)  { name  = n; }
    void setEmail(const string& e) { email = e; }

    // d. Static Member Function
    static int getPersonCount() { return personCount; }

    // d. Operator Overloading
    bool operator==(const Person& other) const { return id    == other.id;   }
    bool operator< (const Person& other) const { return name  <  other.name; }

    // d. Friend Function
    friend ostream& operator<<(ostream& os, const Person& p);
};

int Person::personCount = 0;

// d. Friend function — accesses private id
ostream& operator<<(ostream& os, const Person& p) {
    os << "[ID:" << p.id << "] " << p.name << " <" << p.email << ">";
    return os;
}

// ============================================================
//  USER CLASS — Single Inheritance from Person
// ============================================================

class User : public Person {
protected:
    string username;
    string password;
    bool   isLoggedIn;
    static int userCount; // d. Static

public:
    // b. Default Constructor
    User() : Person(), username(""), password(""), isLoggedIn(false) {
        userCount++;
    }

    // b. Parameterized Constructor
    User(int i, const string& n, const string& e,
         const string& uname, const string& pwd)
        : Person(n, i, e), username(uname), password(pwd), isLoggedIn(false) {
        userCount++;
    }

    // b. Copy Constructor
    User(const User& other)
        : Person(other), username(other.username),
          password(other.password), isLoggedIn(false) {
        userCount++;
    }

    // b. Destructor
    virtual ~User() { userCount--; }

    // h. Exception Handling inside login
    bool login(const string& uname, const string& pwd) {
        try {
            if (uname != username)
                throw AuthException("Username not found.");
            if (pwd   != password)
                throw AuthException("Incorrect password.");
            isLoggedIn = true;
            return true;
        } catch (AuthException& ex) {
            // h. Catch block
            cout << "\n  " << ex.what() << endl;
            return false;
        }
    }

    void logout() { isLoggedIn = false; }

    inline bool   getIsLoggedIn() const { return isLoggedIn; }
    inline string getUsername()   const { return username;   }
    inline string getPassword()   const { return password;   }
    static int    getUserCount()        { return userCount;  }

    // c. Override pure virtual functions
    void displayInfo() const override {
        cout << "  Name     : " << name     << endl;
        cout << "  Username : " << username << endl;
        cout << "  Email    : " << email    << endl;
    }

    string getRole() const override { return "User"; }
};

int User::userCount = 0;

// ============================================================
//  QUESTION CLASSES — Abstract Base + Hierarchical Inheritance
// ============================================================

class Question {
protected:
    int    questionId;
    string questionText;
    int    marks;
    string subject;
    static int questionCount; // d. Static

public:
    // d. Nested Class
    class Metadata {
    public:
        string createdBy;
        string createdDate;
        string difficulty;  // Easy / Medium / Hard

        // b. Default Constructor
        Metadata() : createdBy(""), createdDate(""), difficulty("Medium") {}

        // b. Parameterized Constructor
        Metadata(const string& by, const string& date, const string& diff)
            : createdBy(by), createdDate(date), difficulty(diff) {}

        // b. Copy Constructor
        Metadata(const Metadata& other)
            : createdBy(other.createdBy), createdDate(other.createdDate),
              difficulty(other.difficulty) {}

        void display() const {
            cout << "   By: " << createdBy
                 << "  Date: " << createdDate
                 << "  [" << difficulty << "]" << endl;
        }
    };

    Metadata metadata; // Each question carries metadata

    // b. Default Constructor
    Question() : questionId(0), questionText(""), marks(1), subject("General") {
        questionCount++;
    }

    // b. Parameterized Constructor
    Question(int id, const string& text, int m, const string& subj)
        : questionId(id), questionText(text), marks(m), subject(subj) {
        questionCount++;
    }

    // b. Copy Constructor
    Question(const Question& other)
        : questionId(other.questionId), questionText(other.questionText),
          marks(other.marks), subject(other.subject), metadata(other.metadata) {
        questionCount++;
    }

    // b. Virtual Destructor
    virtual ~Question() { questionCount--; }

    // c. Pure Virtual Functions — Abstract Class
    virtual void   display()                         const = 0;
    virtual bool   checkAnswer(const string& answer) const = 0;
    virtual string getType()                         const = 0;
    virtual string getCorrectAnswer()                const = 0;

    // d. Inline Getters
    inline int    getQuestionId()   const { return questionId;   }
    inline string getQuestionText() const { return questionText; }
    inline int    getMarks()        const { return marks;        }
    inline string getSubject()      const { return subject;      }

    static int getQuestionCount() { return questionCount; }

    // d. Operator Overloading (used in sort/search)
    bool operator==(const Question& other) const { return questionId == other.questionId; }
    bool operator< (const Question& other) const { return questionId <  other.questionId; }

    // d. Friend Class
    friend class QuestionBank;
};

int Question::questionCount = 0;

// ─── MCQ Question ─────────────────────────────────────────
class MCQQuestion : public Question {
private:
    string options[4];    // f. Array of strings inside object
    char   correctOption; // 'A', 'B', 'C', 'D'

public:
    // b. Default Constructor
    MCQQuestion() : Question(), correctOption('A') {
        for (int i = 0; i < 4; i++) options[i] = "";
    }

    // b. Parameterized Constructor
    MCQQuestion(int id, const string& text, int m,
                const string& subj, const string opts[4], char correct)
        : Question(id, text, m, subj), correctOption((char)toLowerChar(correct) - 32 < 0 ? correct : (char)(toLowerChar(correct) - 32)) {
        correctOption = (correct >= 'a' && correct <= 'z') ? (correct - 32) : correct;
        for (int i = 0; i < 4; i++) options[i] = opts[i];
    }

    // b. Copy Constructor
    MCQQuestion(const MCQQuestion& other)
        : Question(other), correctOption(other.correctOption) {
        for (int i = 0; i < 4; i++) options[i] = other.options[i];
    }

    ~MCQQuestion() override {}

    // c. Function Overriding
    void display() const override {
        cout << "\n  [MCQ - " << marks << " mark(s)]" << endl;
        cout << "  Q: " << questionText << endl;
        cout << "    A) " << options[0] << endl;
        cout << "    B) " << options[1] << endl;
        cout << "    C) " << options[2] << endl;
        cout << "    D) " << options[3] << endl;
    }

    bool checkAnswer(const string& answer) const override {
        if (answer.empty()) return false;
        char a = answer[0];
        if (a >= 'a' && a <= 'z') a = a - 32; // manual toUpper
        return a == correctOption;
    }

    string getType()          const override { return "MCQ"; }
    string getCorrectAnswer() const override { return string(1, correctOption); }
    string getOption(int i)   const { return (i >= 0 && i < 4) ? options[i] : ""; }
};

// ─── True/False Question ──────────────────────────────────
class TrueFalseQuestion : public Question {
private:
    bool correctAnswer; // true = True, false = False

public:
    TrueFalseQuestion() : Question(), correctAnswer(true) {}

    TrueFalseQuestion(int id, const string& text, int m,
                      const string& subj, bool correct)
        : Question(id, text, m, subj), correctAnswer(correct) {}

    TrueFalseQuestion(const TrueFalseQuestion& other)
        : Question(other), correctAnswer(other.correctAnswer) {}

    ~TrueFalseQuestion() override {}

    void display() const override {
        cout << "\n  [True/False - " << marks << " mark(s)]" << endl;
        cout << "  Q: " << questionText << endl;
        cout << "    Enter T for True  or  F for False" << endl;
    }

    bool checkAnswer(const string& answer) const override {
        if (answer.empty()) return false;
        char a = answer[0];
        if (a >= 'a' && a <= 'z') a = a - 32;
        return correctAnswer ? (a == 'T') : (a == 'F');
    }

    string getType()          const override { return "TF"; }
    string getCorrectAnswer() const override {
        return correctAnswer ? "True" : "False";
    }
};

// ─── Short Answer Question ────────────────────────────────
class ShortAnswerQuestion : public Question {
private:
    string expectedAnswer;

public:
    ShortAnswerQuestion() : Question(), expectedAnswer("") {}

    ShortAnswerQuestion(int id, const string& text, int m,
                        const string& subj, const string& ans)
        : Question(id, text, m, subj), expectedAnswer(ans) {}

    ShortAnswerQuestion(const ShortAnswerQuestion& other)
        : Question(other), expectedAnswer(other.expectedAnswer) {}

    ~ShortAnswerQuestion() override {}

    void display() const override {
        cout << "\n  [Short Answer - " << marks << " mark(s)]" << endl;
        cout << "  Q: " << questionText << endl;
        cout << "    Your Answer: ";
    }

    bool checkAnswer(const string& answer) const override {
        // Manual lowercase comparison (no <algorithm>)
        return toLowerStr(answer) == toLowerStr(expectedAnswer);
    }

    string getType()          const override { return "SA";            }
    string getCorrectAnswer() const override { return expectedAnswer;  }
};

// ============================================================
//  QUESTION BANK — Friend of Question
//  f. Sorting (Bubble Sort) + Searching (Binary Search)
//  e. Dynamic Memory Allocation
// ============================================================

class QuestionBank {
private:
    string     subject;
    Question** questions; // e. Pointer to array of Question pointers
    int        capacity;
    int        count;

    // Resize the array when full
    void resize() {
        capacity *= 2;
        Question** temp = new Question*[capacity]; // e. new
        for (int i = 0; i < count; i++) temp[i] = questions[i];
        delete[] questions; // e. delete[]
        questions = temp;
    }

public:
    // b. Parameterized Constructor
    QuestionBank(const string& subj = "General", int cap = 10)
        : subject(subj), capacity(cap), count(0) {
        questions = new Question*[capacity]; // e. Dynamic Allocation
    }

    // b. Copy Constructor
    QuestionBank(const QuestionBank& other)
        : subject(other.subject), capacity(other.capacity), count(other.count) {
        questions = new Question*[capacity];
        for (int i = 0; i < count; i++) questions[i] = other.questions[i];
    }

    // b. Destructor
    ~QuestionBank() {
        delete[] questions; // e. delete
    }

    void addQuestion(Question* q) {
        if (count >= capacity) resize();
        questions[count++] = q;
    }

    // e. Returning Object pointer
    Question* getQuestion(int index) const {
        if (index < 0 || index >= count)
            throw InvalidInputException("Question index out of range.");
        return questions[index];
    }

    int    getCount()   const { return count;   }
    string getSubject() const { return subject; }

    // f. Bubble Sort by Question ID
    void sortByID() {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (*questions[j + 1] < *questions[j]) {
                    Question* temp      = questions[j];
                    questions[j]        = questions[j + 1];
                    questions[j + 1]    = temp;
                }
            }
        }
    }

    // f. Binary Search by Question ID
    Question* searchByID(int id) {
        sortByID();
        int low = 0, high = count - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            // d. Friend access to private questionId
            if (questions[mid]->questionId == id) return questions[mid];
            else if (questions[mid]->questionId < id) low  = mid + 1;
            else                                       high = mid - 1;
        }
        return nullptr;
    }

    void displayAll() const {
        printHeader("Question Bank: " + subject);
        cout << "  Total Questions: " << count << endl;
        for (int i = 0; i < count; i++) {
            questions[i]->display();
            questions[i]->metadata.display();
        }
    }
};

// ============================================================
//  EXAM RESULT
// ============================================================

class ExamResult {
private:
    int    studentId;
    string studentName;
    int    examId;
    string examTitle;
    int    obtainedMarks;
    int    totalMarks;
    bool   passed;
    string attemptDate;

public:
    // b. Default Constructor
    ExamResult()
        : studentId(0), studentName(""), examId(0), examTitle(""),
          obtainedMarks(0), totalMarks(0), passed(false), attemptDate("") {}

    // b. Parameterized Constructor
    ExamResult(int sId, const string& sName, int eId, const string& eTitle,
               int obtained, int total, bool pass, const string& date)
        : studentId(sId), studentName(sName), examId(eId), examTitle(eTitle),
          obtainedMarks(obtained), totalMarks(total), passed(pass), attemptDate(date) {}

    // b. Copy Constructor
    ExamResult(const ExamResult& other)
        : studentId(other.studentId), studentName(other.studentName),
          examId(other.examId), examTitle(other.examTitle),
          obtainedMarks(other.obtainedMarks), totalMarks(other.totalMarks),
          passed(other.passed), attemptDate(other.attemptDate) {}

    // b. Destructor
    ~ExamResult() {}

    // Getters
    inline int    getStudentId()     const { return studentId;     }
    inline string getStudentName()   const { return studentName;   }
    inline int    getExamId()        const { return examId;        }
    inline string getExamTitle()     const { return examTitle;     }
    inline int    getObtainedMarks() const { return obtainedMarks; }
    inline int    getTotalMarks()    const { return totalMarks;    }
    inline bool   getPassed()        const { return passed;        }
    inline string getAttemptDate()   const { return attemptDate;   }

    // Simple percentage as integer
    int getPercentage() const {
        if (totalMarks == 0) return 0;
        return (obtainedMarks * 100) / totalMarks;
    }

    // d. Operator Overloading
    bool operator< (const ExamResult& o) const {
        return obtainedMarks > o.obtainedMarks; // descending order
    }
    bool operator==(const ExamResult& o) const {
        return studentId == o.studentId && examId == o.examId;
    }

    // d. Friend operator<<
    friend ostream& operator<<(ostream& os, const ExamResult& r) {
        os << r.studentName << " | " << r.examTitle
           << " | " << r.obtainedMarks << "/" << r.totalMarks
           << " (" << r.getPercentage() << "%)";
        return os;
    }

    void display() const {
        printLine('-', 45);
        cout << "  Student : " << studentName << " (ID: " << studentId << ")" << endl;
        cout << "  Exam    : " << examTitle   << " (ID: " << examId    << ")" << endl;
        cout << "  Score   : " << obtainedMarks << " / " << totalMarks
             << "  (" << getPercentage() << "%)" << endl;
        cout << "  Status  : " << (passed ? "*** PASSED ***" : "--- FAILED ---") << endl;
        cout << "  Date    : " << attemptDate  << endl;
        printLine('-', 45);
    }
};

// ============================================================
//  EXAM CLASS
// ============================================================

class Exam {
private:
    int       examId;
    string    title;
    string    subject;
    int       durationMinutes;
    int       totalMarks;
    int       passingMarks;
    bool      isActive;
    string    createdBy;
    string    examDate;
    Question** questions;    // e. Dynamic array of pointers
    int       questionCount;
    int       questionCap;
    static int examCount;    // d. Static

public:
    // b. Default Constructor
    Exam() : examId(0), title(""), subject(""), durationMinutes(60),
             totalMarks(0), passingMarks(0), isActive(false),
             createdBy(""), examDate(""), questionCount(0), questionCap(10) {
        questions = new Question*[questionCap]; // e. new
        examCount++;
    }

    // b. Parameterized Constructor
    Exam(int id, const string& t, const string& subj, int dur,
         int passing, const string& by, const string& date)
        : examId(id), title(t), subject(subj), durationMinutes(dur),
          totalMarks(0), passingMarks(passing), isActive(true),
          createdBy(by), examDate(date), questionCount(0), questionCap(10) {
        questions = new Question*[questionCap];
        examCount++;
    }

    // b. Copy Constructor
    Exam(const Exam& other)
        : examId(other.examId), title(other.title), subject(other.subject),
          durationMinutes(other.durationMinutes), totalMarks(other.totalMarks),
          passingMarks(other.passingMarks), isActive(other.isActive),
          createdBy(other.createdBy), examDate(other.examDate),
          questionCount(other.questionCount), questionCap(other.questionCap) {
        questions = new Question*[questionCap];
        for (int i = 0; i < questionCount; i++) questions[i] = other.questions[i];
        examCount++;
    }

    // b. Destructor
    ~Exam() {
        delete[] questions; // e. delete
        examCount--;
    }

    void addQuestion(Question* q) {
        if (questionCount >= questionCap) {
            questionCap *= 2;
            Question** tmp = new Question*[questionCap];
            for (int i = 0; i < questionCount; i++) tmp[i] = questions[i];
            delete[] questions;
            questions = tmp;
        }
        questions[questionCount++] = q;
        totalMarks += q->getMarks();
    }

    // Getters
    inline int    getExamId()        const { return examId;         }
    inline string getTitle()         const { return title;          }
    inline string getSubject()       const { return subject;        }
    inline int    getDuration()      const { return durationMinutes;}
    inline int    getTotalMarks()    const { return totalMarks;     }
    inline int    getPassingMarks()  const { return passingMarks;   }
    inline bool   getIsActive()      const { return isActive;       }
    inline string getCreatedBy()     const { return createdBy;      }
    inline string getExamDate()      const { return examDate;       }
    inline int    getQuestionCount() const { return questionCount;  }
    Question*     getQuestion(int i) const { return questions[i];   }

    void setIsActive(bool a) { isActive = a; }
    static int getExamCount() { return examCount; }

    // d. Operator Overloading
    bool operator==(const Exam& o) const { return examId == o.examId; }
    bool operator< (const Exam& o) const { return examId <  o.examId; }

    void displayInfo() const {
        cout << "\n  [ID:" << examId << "] " << title << endl;
        cout << "  Subject  : " << subject
             << "   Duration : " << durationMinutes << " min" << endl;
        cout << "  Total    : " << totalMarks
             << "   Passing  : " << passingMarks << endl;
        cout << "  Date     : " << examDate
             << "   By       : " << createdBy << endl;
        cout << "  Status   : " << (isActive ? "ACTIVE" : "INACTIVE") << endl;
    }

    void displayWithQuestions() const {
        displayInfo();
        cout << "\n  Questions:" << endl;
        for (int i = 0; i < questionCount; i++) questions[i]->display();
    }
};

int Exam::examCount = 0;

// ============================================================
//  STUDENT — Multilevel Inheritance (Person -> User -> Student)
// ============================================================

class Student : public User {
private:
    string      enrollmentNo;
    string      program;
    int         semester;
    ExamResult* results;      // e. Dynamic array of objects
    int         resultCount;
    int         resultCap;

public:
    // b. Default Constructor
    Student() : User(), enrollmentNo(""), program(""), semester(1),
                resultCount(0), resultCap(5) {
        results = new ExamResult[resultCap]; // e. new
    }

    // b. Parameterized Constructor
    Student(int id, const string& n, const string& e,
            const string& uname, const string& pwd,
            const string& enroll, const string& prog, int sem)
        : User(id, n, e, uname, pwd),
          enrollmentNo(enroll), program(prog), semester(sem),
          resultCount(0), resultCap(5) {
        results = new ExamResult[resultCap];
    }

    // b. Copy Constructor
    Student(const Student& other)
        : User(other), enrollmentNo(other.enrollmentNo),
          program(other.program), semester(other.semester),
          resultCount(other.resultCount), resultCap(other.resultCap) {
        results = new ExamResult[resultCap]; // e. Deep copy
        for (int i = 0; i < resultCount; i++) results[i] = other.results[i];
    }

    // b. Destructor
    ~Student() override { delete[] results; } // e. delete[]

    // e. Passing Object to Function (by const reference)
    void addResult(const ExamResult& r) {
        if (resultCount >= resultCap) {
            resultCap *= 2;
            ExamResult* tmp = new ExamResult[resultCap]; // e. new
            for (int i = 0; i < resultCount; i++) tmp[i] = results[i];
            delete[] results; // e. delete[]
            results = tmp;
        }
        results[resultCount++] = r; // f. Store object in array
    }

    bool hasAttempted(int eid) const {
        for (int i = 0; i < resultCount; i++)
            if (results[i].getExamId() == eid) return true;
        return false;
    }

    // f. Bubble Sort on Array of Objects
    void sortResultsByScore() {
        for (int i = 0; i < resultCount - 1; i++) {
            for (int j = 0; j < resultCount - i - 1; j++) {
                if (results[j] < results[j + 1]) {
                    ExamResult tmp  = results[j];
                    results[j]      = results[j + 1];
                    results[j + 1]  = tmp;
                }
            }
        }
    }

    // Getters
    inline string getEnrollmentNo() const { return enrollmentNo; }
    inline string getProgram()      const { return program;      }
    inline int    getSemester()     const { return semester;     }
    inline int    getResultCount()  const { return resultCount;  }
    ExamResult&   getResult(int i)        { return results[i];   }

    // c. Override Pure Virtual Functions
    void displayInfo() const override {
        printHeader("Student Profile");
        cout << "  Name       : " << name         << endl;
        cout << "  ID         : " << id           << endl;
        cout << "  Email      : " << email        << endl;
        cout << "  Username   : " << username     << endl;
        cout << "  Enrollment : " << enrollmentNo << endl;
        cout << "  Program    : " << program      << endl;
        cout << "  Semester   : " << semester     << endl;
        cout << "  Exams Done : " << resultCount  << endl;
    }

    string getRole() const override { return "Student"; }

    void viewResults() {
        if (resultCount == 0) {
            cout << "\n  No results yet." << endl;
            return;
        }
        sortResultsByScore();
        printHeader("Your Exam Results");
        for (int i = 0; i < resultCount; i++) results[i].display();
    }
};

// ============================================================
//  EXAMINER — Hierarchical Inheritance from User
// ============================================================

class Examiner : public User {
private:
    string  department;
    string  specialization;
    Exam**  myExams;    // e. Dynamic array of pointers
    int     examCount;
    int     examCap;

public:
    // b. Default Constructor
    Examiner() : User(), department(""), specialization(""),
                 examCount(0), examCap(5) {
        myExams = new Exam*[examCap];
    }

    // b. Parameterized Constructor
    Examiner(int id, const string& n, const string& e,
             const string& uname, const string& pwd,
             const string& dept, const string& spec)
        : User(id, n, e, uname, pwd), department(dept),
          specialization(spec), examCount(0), examCap(5) {
        myExams = new Exam*[examCap];
    }

    // b. Copy Constructor
    Examiner(const Examiner& other)
        : User(other), department(other.department),
          specialization(other.specialization),
          examCount(other.examCount), examCap(other.examCap) {
        myExams = new Exam*[examCap];
        for (int i = 0; i < examCount; i++) myExams[i] = other.myExams[i];
    }

    // b. Destructor
    ~Examiner() override { delete[] myExams; }

    void addExam(Exam* e) {
        if (examCount >= examCap) {
            examCap *= 2;
            Exam** tmp = new Exam*[examCap];
            for (int i = 0; i < examCount; i++) tmp[i] = myExams[i];
            delete[] myExams;
            myExams = tmp;
        }
        myExams[examCount++] = e;
    }

    inline string getDepartment()     const { return department;     }
    inline string getSpecialization() const { return specialization; }
    inline int    getExamCount()      const { return examCount;      }
    Exam*         getExam(int i)      const { return myExams[i];     }

    // c. Override Pure Virtual Functions
    void displayInfo() const override {
        printHeader("Examiner Profile");
        cout << "  Name           : " << name           << endl;
        cout << "  ID             : " << id             << endl;
        cout << "  Email          : " << email          << endl;
        cout << "  Username       : " << username       << endl;
        cout << "  Department     : " << department     << endl;
        cout << "  Specialization : " << specialization << endl;
        cout << "  Exams Created  : " << examCount      << endl;
    }

    string getRole() const override { return "Examiner"; }
};

// ============================================================
//  ADMIN — Hierarchical Inheritance from User
// ============================================================

class Admin : public User {
private:
    int    adminLevel;
    string adminCode;

public:
    // b. Default Constructor
    Admin() : User(), adminLevel(1), adminCode("") {}

    // b. Parameterized Constructor
    Admin(int id, const string& n, const string& e,
          const string& uname, const string& pwd,
          int level, const string& code)
        : User(id, n, e, uname, pwd), adminLevel(level), adminCode(code) {}

    // b. Copy Constructor
    Admin(const Admin& other)
        : User(other), adminLevel(other.adminLevel), adminCode(other.adminCode) {}

    // b. Destructor
    ~Admin() override {}

    inline int    getAdminLevel() const { return adminLevel; }
    inline string getAdminCode()  const { return adminCode;  }

    // c. Override Pure Virtual Functions
    void displayInfo() const override {
        printHeader("Admin Profile");
        cout << "  Name       : " << name       << endl;
        cout << "  ID         : " << id         << endl;
        cout << "  Email      : " << email      << endl;
        cout << "  Username   : " << username   << endl;
        cout << "  Admin Level: " << adminLevel << endl;
    }

    string getRole() const override { return "Admin"; }
};

// ============================================================
//  g. FILE MANAGER — All File Handling
// ============================================================

class FileManager {
private:
    string dir;

public:
    FileManager(const string& directory = "data/") : dir(directory) {}

    // g. ofstream — Write student record to text file
    void saveStudent(const Student& s) {
        try {
            ofstream file(dir + "students.txt", ios::app);
            if (!file.is_open())
                throw FileException("Cannot open students.txt for writing.");
            file << s.getId()           << ","
                 << s.getName()         << ","
                 << s.getEmail()        << ","
                 << s.getUsername()     << ","
                 << s.getPassword()     << ","
                 << s.getEnrollmentNo() << ","
                 << s.getProgram()      << ","
                 << s.getSemester()     << "\n";
            file.close();
        } catch (FileException& fe) {
            cerr << fe.what() << endl;
        }
    }

    // g. ofstream — Write examiner record to text file
    void saveExaminer(const Examiner& ex) {
        try {
            ofstream file(dir + "examiners.txt", ios::app);
            if (!file.is_open())
                throw FileException("Cannot open examiners.txt for writing.");
            file << ex.getId()             << ","
                 << ex.getName()           << ","
                 << ex.getEmail()          << ","
                 << ex.getUsername()       << ","
                 << ex.getPassword()       << ","
                 << ex.getDepartment()     << ","
                 << ex.getSpecialization() << "\n";
            file.close();
        } catch (FileException& fe) {
            cerr << fe.what() << endl;
        }
    }

    // g. ofstream — Write exam result to text file
    void saveResult(const ExamResult& r) {
        try {
            ofstream file(dir + "results.txt", ios::app);
            if (!file.is_open())
                throw FileException("Cannot open results.txt for writing.");
            file << r.getStudentId()     << ","
                 << r.getStudentName()   << ","
                 << r.getExamId()        << ","
                 << r.getExamTitle()     << ","
                 << r.getObtainedMarks() << ","
                 << r.getTotalMarks()    << ","
                 << (r.getPassed() ? "1" : "0") << ","
                 << r.getAttemptDate()   << "\n";
            file.close();
        } catch (FileException& fe) {
            cerr << fe.what() << endl;
        }
    }

    // g. ifstream — Read and print results from text file
    void printResultsFromFile() const {
        try {
            ifstream file(dir + "results.txt");
            if (!file.is_open()) {
                cout << "  No results file found yet." << endl;
                return;
            }
            string line;
            int count = 0;
            printHeader("All Results (Loaded From File)");
            while (getline(file, line)) {
                if (!line.empty()) {
                    cout << "  " << line << endl;
                    count++;
                }
            }
            cout << "  Total records: " << count << endl;
            file.close();
        } catch (...) {
            cerr << "Error reading results file." << endl;
        }
    }

    // g. ifstream — Read and print activity log
    void printActivityLog() const {
        try {
            ifstream file(dir + "activity.log");
            if (!file.is_open()) {
                cout << "  No activity log found yet." << endl;
                return;
            }
            string line;
            printHeader("Activity Log");
            while (getline(file, line))
                if (!line.empty()) cout << "  " << line << endl;
            file.close();
        } catch (...) {
            cerr << "Error reading activity log." << endl;
        }
    }

    // g. fstream (binary) — Write exam summary to binary file
    void saveExamBinary(const string& filename, int examId,
                        const string& title, int totalMarks) {
        try {
            fstream file(dir + filename,
                         ios::binary | ios::app | ios::out);
            if (!file.is_open())
                throw FileException("Cannot open binary file: " + filename);

            // Write two integers as raw bytes
            file.write(reinterpret_cast<const char*>(&examId),     sizeof(int));
            file.write(reinterpret_cast<const char*>(&totalMarks), sizeof(int));

            // Write string: length first, then characters
            int len = (int)title.size();
            file.write(reinterpret_cast<const char*>(&len), sizeof(int));
            file.write(title.c_str(), len);

            file.close();
        } catch (FileException& fe) {
            cerr << fe.what() << endl;
        }
    }

    // g. fstream (binary) — Read exam summaries from binary file
    void printExamsBinary(const string& filename) const {
        try {
            fstream file(dir + filename, ios::binary | ios::in);
            if (!file.is_open()) {
                cout << "  No binary exam file found." << endl;
                return;
            }
            printHeader("Exams (From Binary File)");
            int examId, totalMarks;
            while (file.read(reinterpret_cast<char*>(&examId), sizeof(int))) {
                file.read(reinterpret_cast<char*>(&totalMarks), sizeof(int));
                int len;
                file.read(reinterpret_cast<char*>(&len), sizeof(int));
                string title(len, '\0');
                file.read(&title[0], len);
                cout << "  Exam ID: " << examId
                     << "  |  " << title
                     << "  |  Total Marks: " << totalMarks << endl;
            }
            file.close();
        } catch (...) {
            cerr << "Error reading binary exam file." << endl;
        }
    }

    // g. fstream (append) — Log an activity with timestamp
    void logActivity(const string& activity, const string& date) {
        try {
            fstream file(dir + "activity.log",
                         ios::app | ios::out);
            if (!file.is_open())
                throw FileException("Cannot open activity.log");
            file << "[" << date << "] " << activity << "\n";
            file.close();
        } catch (FileException& fe) {
            cerr << fe.what() << endl;
        }
    }
};

// ============================================================
//  EXAMINATION SYSTEM — Main Controller
//  d. Static  e. Dynamic Memory + this pointer
//  c. Runtime Polymorphism  f. Array of Objects
// ============================================================

class ExaminationSystem {
private:
    // f. Arrays of Object Pointers
    Student*      students[200];
    Examiner*     examiners[50];
    Admin*        admins[10];
    Exam*         exams[200];
    QuestionBank* banks[20];

    int studentCount;
    int examinerCount;
    int adminCount;
    int examCount;
    int bankCount;

    int nextStudentId;
    int nextExaminerId;
    int nextExamId;
    int nextQuestionId;

    static int instanceCount; // d. Static Member

    FileManager fileManager;
    User*       currentUser;  // e. Pointer to current user

    string currentDate; // set by admin or default

    // e. this pointer usage
    ExaminationSystem* getThis() { return this; }

    // ── HELPERS ──────────────────────────────────────────────

    Student* findStudent(const string& uname) const {
        for (int i = 0; i < studentCount; i++)
            if (students[i]->getUsername() == uname) return students[i];
        return nullptr;
    }

    Examiner* findExaminer(const string& uname) const {
        for (int i = 0; i < examinerCount; i++)
            if (examiners[i]->getUsername() == uname) return examiners[i];
        return nullptr;
    }

    Exam* findExam(int id) const {
        for (int i = 0; i < examCount; i++)
            if (exams[i] && exams[i]->getExamId() == id) return exams[i];
        return nullptr;
    }

    QuestionBank* getOrCreateBank(const string& subject) {
        for (int i = 0; i < bankCount; i++)
            if (banks[i]->getSubject() == subject) return banks[i];
        if (bankCount < 20) {
            banks[bankCount] = new QuestionBank(subject);
            return banks[bankCount++];
        }
        return banks[0];
    }

    // ── SEED DEFAULT DATA ─────────────────────────────────────
    void seedData() {
        // Default Admin
        admins[adminCount++] = new Admin(
            9001, "System Admin", "admin@oes.edu",
            "admin", "admin123", 5, "OES-ADMIN"
        );

        // Default Examiner
        Examiner* ex = new Examiner(
            2001, "Dr. Said Nabi", "snabi@giki.edu.pk",
            "snabi", "examiner123",
            "Computer Science", "OOP & Algorithms"
        );
        examiners[examinerCount++] = ex;

        // Default Students
        Student* s1 = new Student(
            1001, "Ali Hassan", "ali@giki.edu.pk",
            "ali", "student123", "2021-CS-001", "BS-SE", 4
        );
        Student* s2 = new Student(
            1002, "Sara Khan", "sara@giki.edu.pk",
            "sara", "student123", "2021-CS-002", "BS-SE", 4
        );
        students[studentCount++] = s1;
        students[studentCount++] = s2;
        fileManager.saveStudent(*s1);
        fileManager.saveStudent(*s2);

        // Question Bank
        QuestionBank* bank = new QuestionBank("OOP", 20);
        banks[bankCount++] = bank;

        // MCQ Questions
        string o1[4] = {
            "To achieve runtime polymorphism",
            "To prevent inheritance",
            "To overload functions",
            "To define static members"
        };
        MCQQuestion* q1 = new MCQQuestion(
            nextQuestionId++,
            "What is the purpose of a virtual function in C++?",
            2, "OOP", o1, 'A');
        q1->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        string o2[4] = {"private", "protected", "public", "static"};
        MCQQuestion* q2 = new MCQQuestion(
            nextQuestionId++,
            "Which access specifier allows access only within the class?",
            2, "OOP", o2, 'A');
        q2->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Easy");

        string o3[4] = {
            "Single Inheritance",
            "Multiple Inheritance",
            "Hierarchical Inheritance",
            "Multilevel Inheritance"
        };
        MCQQuestion* q3 = new MCQQuestion(
            nextQuestionId++,
            "When one class inherits from multiple base classes, it is called:",
            3, "OOP", o3, 'B');
        q3->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        string o4[4] = {"new", "malloc()", "alloc()", "create()"};
        MCQQuestion* q4 = new MCQQuestion(
            nextQuestionId++,
            "Which operator allocates memory dynamically in C++?",
            2, "OOP", o4, 'A');
        q4->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Easy");

        string o5[4] = {
            "Compile-time Polymorphism",
            "Runtime Polymorphism",
            "Inheritance",
            "Encapsulation"
        };
        MCQQuestion* q5 = new MCQQuestion(
            nextQuestionId++,
            "Function Overloading is an example of which OOP concept?",
            3, "OOP", o5, 'A');
        q5->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        string o6[4] = {
            "A class with at least one pure virtual function",
            "A class with no constructors",
            "A class that cannot be inherited",
            "A class with only static members"
        };
        MCQQuestion* q6 = new MCQQuestion(
            nextQuestionId++,
            "What is an abstract class in C++?",
            3, "OOP", o6, 'A');
        q6->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Hard");

        string o7[4] = {
            "Constructor Overloading",
            "Function Overriding",
            "Operator Overloading",
            "Template specialization"
        };
        MCQQuestion* q7 = new MCQQuestion(
            nextQuestionId++,
            "Redefining a base class function in a derived class is called:",
            2, "OOP", o7, 'B');
        q7->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Easy");

        // True/False Questions
        TrueFalseQuestion* q8 = new TrueFalseQuestion(
            nextQuestionId++,
            "A destructor can be overloaded in C++.",
            2, "OOP", false);
        q8->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Hard");

        TrueFalseQuestion* q9 = new TrueFalseQuestion(
            nextQuestionId++,
            "The 'this' pointer in C++ points to the current object.",
            2, "OOP", true);
        q9->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Easy");

        TrueFalseQuestion* q10 = new TrueFalseQuestion(
            nextQuestionId++,
            "A friend function can access private members of a class.",
            2, "OOP", true);
        q10->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        TrueFalseQuestion* q11 = new TrueFalseQuestion(
            nextQuestionId++,
            "Static member functions can access non-static data members directly.",
            2, "OOP", false);
        q11->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Hard");

        // Short Answer Questions
        ShortAnswerQuestion* q12 = new ShortAnswerQuestion(
            nextQuestionId++,
            "What keyword makes a member function constant in C++?",
            2, "OOP", "const");
        q12->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        ShortAnswerQuestion* q13 = new ShortAnswerQuestion(
            nextQuestionId++,
            "What is the special pointer that always points to the current object?",
            2, "OOP", "this");
        q13->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Easy");

        ShortAnswerQuestion* q14 = new ShortAnswerQuestion(
            nextQuestionId++,
            "Which OOP principle binds data and functions together inside a class?",
            3, "OOP", "encapsulation");
        q14->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        ShortAnswerQuestion* q15 = new ShortAnswerQuestion(
            nextQuestionId++,
            "What is called when the same function name works differently based on parameters?",
            2, "OOP", "overloading");
        q15->metadata = Question::Metadata("Dr. Said Nabi", currentDate, "Medium");

        // Add all questions to bank
        Question* allQ[15] = {
            q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14,q15
        };
        for (int i = 0; i < 15; i++) bank->addQuestion(allQ[i]);

        // Create default exam
        Exam* exam = new Exam(
            nextExamId++, "OOP Mid-Term Examination", "OOP",
            45, 18, "Dr. Said Nabi", currentDate
        );
        for (int i = 0; i < bank->getCount(); i++)
            exam->addQuestion(bank->getQuestion(i));

        exams[examCount++] = exam;
        ex->addExam(exam);

        fileManager.saveExamBinary("exams.bin",
            exam->getExamId(), exam->getTitle(), exam->getTotalMarks());
        fileManager.logActivity(
            "System started. Exam created: " + exam->getTitle(), currentDate);

        cout << "\n  System ready! Default exam loaded with "
             << bank->getCount() << " questions ("
             << exam->getTotalMarks() << " total marks)." << endl;
    }

    // ── REGISTER STUDENT ─────────────────────────────────────
    void registerStudent() {
        try {
            printHeader("Register New Student");
            string name, email, uname, pwd, enroll, prog;
            int sem;

            cout << "  Full Name     : "; cin.ignore(); getline(cin, name);
            cout << "  Email         : "; getline(cin, email);
            cout << "  Username      : "; getline(cin, uname);
            cout << "  Password      : "; getline(cin, pwd);
            cout << "  Enrollment No : "; getline(cin, enroll);
            cout << "  Program       : "; getline(cin, prog);
            cout << "  Semester(1-8) : "; cin >> sem;

            // h. Validate and throw exceptions
            if (name.empty() || uname.empty() || pwd.empty())
                throw InvalidInputException("Name, username, password cannot be empty.");
            if (sem < 1 || sem > 8)
                throw InvalidInputException("Semester must be between 1 and 8.");
            if (findStudent(uname) != nullptr)
                throw DuplicateException("Username '" + uname + "' already exists.");

            Student* s = new Student(
                nextStudentId++, name, email, uname, pwd, enroll, prog, sem
            );
            students[studentCount++] = s;
            fileManager.saveStudent(*s);
            fileManager.logActivity("Student registered: " + name, currentDate);
            cout << "\n  Student registered! ID: " << s->getId() << endl;

        } catch (InvalidInputException& e) { // h. Multiple catch blocks
            cout << "\n  " << e.what() << endl;
        } catch (DuplicateException& e) {
            cout << "\n  " << e.what() << endl;
        } catch (OESException& e) {
            cout << "\n  " << e.what() << endl;
        }
    }

    // ── REGISTER EXAMINER ────────────────────────────────────
    void registerExaminer() {
        try {
            printHeader("Register New Examiner");
            string name, email, uname, pwd, dept, spec;

            cout << "  Full Name      : "; cin.ignore(); getline(cin, name);
            cout << "  Email          : "; getline(cin, email);
            cout << "  Username       : "; getline(cin, uname);
            cout << "  Password       : "; getline(cin, pwd);
            cout << "  Department     : "; getline(cin, dept);
            cout << "  Specialization : "; getline(cin, spec);

            if (name.empty() || uname.empty() || pwd.empty())
                throw InvalidInputException("Name, username, password cannot be empty.");
            if (findExaminer(uname) != nullptr)
                throw DuplicateException("Username '" + uname + "' already exists.");

            Examiner* ex = new Examiner(
                nextExaminerId++, name, email, uname, pwd, dept, spec
            );
            examiners[examinerCount++] = ex;
            fileManager.saveExaminer(*ex);
            fileManager.logActivity("Examiner registered: " + name, currentDate);
            cout << "\n  Examiner registered! ID: " << ex->getId() << endl;

        } catch (InvalidInputException& e) {
            cout << "\n  " << e.what() << endl;
        } catch (DuplicateException& e) {
            cout << "\n  " << e.what() << endl;
        } catch (OESException& e) {
            cout << "\n  " << e.what() << endl;
        }
    }

    // ── LOGIN ────────────────────────────────────────────────
    User* performLogin() {
        printHeader("Login");
        string uname, pwd;
        cout << "  Username : "; cin >> uname;
        cout << "  Password : "; cin >> pwd;

        // Check admins
        for (int i = 0; i < adminCount; i++) {
            if (admins[i]->getUsername() == uname) {
                if (admins[i]->login(uname, pwd)) {
                    currentUser = admins[i];
                    fileManager.logActivity("Admin logged in: " + uname, currentDate);
                    return admins[i];
                }
                return nullptr;
            }
        }

        // Check examiners
        for (int i = 0; i < examinerCount; i++) {
            if (examiners[i]->getUsername() == uname) {
                if (examiners[i]->login(uname, pwd)) {
                    currentUser = examiners[i];
                    fileManager.logActivity("Examiner logged in: " + uname, currentDate);
                    return examiners[i];
                }
                return nullptr;
            }
        }

        // Check students
        for (int i = 0; i < studentCount; i++) {
            if (students[i]->getUsername() == uname) {
                if (students[i]->login(uname, pwd)) {
                    currentUser = students[i];
                    fileManager.logActivity("Student logged in: " + uname, currentDate);
                    return students[i];
                }
                return nullptr;
            }
        }

        cout << "  User not found." << endl;
        return nullptr;
    }

    // ── CREATE EXAM ──────────────────────────────────────────
    void createExam(Examiner* ex) {
        try {
            printHeader("Create New Exam");
            string title, subject, date;
            int duration, passing;

            cout << "  Exam Title     : "; cin.ignore(); getline(cin, title);
            cout << "  Subject        : "; getline(cin, subject);
            cout << "  Duration (min) : "; cin >> duration;
            cout << "  Passing Marks  : "; cin >> passing;
            cout << "  Exam Date      : "; cin >> date;

            if (title.empty() || subject.empty())
                throw InvalidInputException("Title and subject cannot be empty.");
            if (duration <= 0)
                throw InvalidInputException("Duration must be positive.");
            if (passing < 0)
                throw InvalidInputException("Passing marks cannot be negative.");

            Exam* e = new Exam(
                nextExamId++, title, subject, duration, passing, ex->getName(), date
            );

            QuestionBank* bank = getOrCreateBank(subject);

            cout << "\n  Add Questions:" << endl;
            cout << "  1=MCQ  2=True/False  3=Short Answer  0=Done" << endl;

            int choice;
            while (true) {
                cout << "\n  Type (0 to finish): "; cin >> choice;
                if (choice == 0) break;

                if (choice == 1) {
                    string text, opts[4]; int marks; char correct;
                    cout << "  Question : "; cin.ignore(); getline(cin, text);
                    cout << "  Option A : "; getline(cin, opts[0]);
                    cout << "  Option B : "; getline(cin, opts[1]);
                    cout << "  Option C : "; getline(cin, opts[2]);
                    cout << "  Option D : "; getline(cin, opts[3]);
                    cout << "  Correct (A/B/C/D): "; cin >> correct;
                    cout << "  Marks    : "; cin >> marks;
                    if (marks <= 0) throw InvalidInputException("Marks must be positive.");
                    MCQQuestion* q = new MCQQuestion(
                        nextQuestionId++, text, marks, subject, opts, correct
                    );
                    q->metadata = Question::Metadata(ex->getName(), date, "Medium");
                    e->addQuestion(q);
                    bank->addQuestion(q);
                    cout << "  MCQ added." << endl;

                } else if (choice == 2) {
                    string text; int marks; char ans;
                    cout << "  Question     : "; cin.ignore(); getline(cin, text);
                    cout << "  Correct (T/F): "; cin >> ans;
                    cout << "  Marks        : "; cin >> marks;
                    if (marks <= 0) throw InvalidInputException("Marks must be positive.");
                    bool correct = (ans == 'T' || ans == 't');
                    TrueFalseQuestion* q = new TrueFalseQuestion(
                        nextQuestionId++, text, marks, subject, correct
                    );
                    q->metadata = Question::Metadata(ex->getName(), date, "Medium");
                    e->addQuestion(q);
                    bank->addQuestion(q);
                    cout << "  True/False question added." << endl;

                } else if (choice == 3) {
                    string text, answer; int marks;
                    cout << "  Question        : "; cin.ignore(); getline(cin, text);
                    cout << "  Expected Answer : "; getline(cin, answer);
                    cout << "  Marks           : "; cin >> marks;
                    if (marks <= 0) throw InvalidInputException("Marks must be positive.");
                    ShortAnswerQuestion* q = new ShortAnswerQuestion(
                        nextQuestionId++, text, marks, subject, answer
                    );
                    q->metadata = Question::Metadata(ex->getName(), date, "Medium");
                    e->addQuestion(q);
                    bank->addQuestion(q);
                    cout << "  Short answer question added." << endl;

                } else {
                    cout << "  Invalid type. Enter 1, 2, 3, or 0." << endl;
                }
            }

            if (e->getQuestionCount() == 0) {
                delete e;
                throw InvalidInputException("Exam must have at least one question.");
            }

            exams[examCount++] = e;
            ex->addExam(e);
            fileManager.saveExamBinary(
                "exams.bin", e->getExamId(), e->getTitle(), e->getTotalMarks()
            );
            fileManager.logActivity("Exam created: " + title, currentDate);
            cout << "\n  Exam created! ID: " << e->getExamId()
                 << "  Total Marks: " << e->getTotalMarks() << endl;

        } catch (InvalidInputException& e) {
            cout << "\n  " << e.what() << endl;
        } catch (OESException& e) {
            cout << "\n  " << e.what() << endl;
        }
    }

    // ── TAKE EXAM ────────────────────────────────────────────
    void takeExam(Student* st) {
        try {
            printHeader("Available Exams");
            if (examCount == 0)
                throw ExamNotFoundException("No exams exist in the system.");

            bool anyOpen = false;
            for (int i = 0; i < examCount; i++) {
                if (exams[i] && exams[i]->getIsActive()) {
                    if (st->hasAttempted(exams[i]->getExamId()))
                        cout << "  [Done]  ";
                    else {
                        cout << "  [Open]  ";
                        anyOpen = true;
                    }
                    exams[i]->displayInfo();
                }
            }

            if (!anyOpen) {
                cout << "\n  You have attempted all available exams." << endl;
                return;
            }

            cout << "\n  Enter Exam ID to attempt (0 to cancel): ";
            int examId; cin >> examId;
            if (examId == 0) return;

            Exam* e = findExam(examId);
            if (!e)
                throw ExamNotFoundException("Exam ID " + intToStr(examId) + " not found.");
            if (!e->getIsActive())
                throw ExamNotFoundException("That exam is currently inactive.");
            if (st->hasAttempted(examId))
                throw InvalidInputException("You have already attempted this exam.");

            printHeader("EXAM: " + e->getTitle());
            cout << "  Questions    : " << e->getQuestionCount() << endl;
            cout << "  Duration     : " << e->getDuration() << " minutes" << endl;
            cout << "  Total Marks  : " << e->getTotalMarks() << endl;
            cout << "  Passing Marks: " << e->getPassingMarks() << endl;
            cout << "\n  Press Enter to begin...";
            cin.ignore(); cin.get();

            int obtained = 0;

            for (int i = 0; i < e->getQuestionCount(); i++) {
                Question* q = e->getQuestion(i); // e. Pointer to Object

                cout << "\n  Question " << (i + 1)
                     << " of " << e->getQuestionCount() << ":";

                // c. Virtual Function — correct display() runs based on type
                q->display();

                string ans;
                if (q->getType() != "SA") cout << "  Your Answer: ";
                cin >> ans;

                // c. Virtual Function — correct checkAnswer() runs
                if (q->checkAnswer(ans)) {
                    obtained += q->getMarks();
                    cout << "  [CORRECT] +" << q->getMarks() << " marks" << endl;
                } else {
                    cout << "  [WRONG]   Correct: " << q->getCorrectAnswer() << endl;
                }
            }

            bool passed = (obtained >= e->getPassingMarks());

            // e. Passing Object to Function + Returning Object
            ExamResult result(
                st->getId(), st->getName(),
                e->getExamId(), e->getTitle(),
                obtained, e->getTotalMarks(),
                passed, currentDate
            );

            st->addResult(result); // e. Pass by const reference
            fileManager.saveResult(result);
            fileManager.logActivity(
                st->getName() + " completed " + e->getTitle() +
                " Score:" + intToStr(obtained) + "/" + intToStr(e->getTotalMarks()),
                currentDate
            );

            cout << endl;
            result.display();
            cout << "\n  Thank you for completing the exam!" << endl;

        } catch (ExamNotFoundException& e) {  // h. Multiple catch
            cout << "\n  " << e.what() << endl;
        } catch (InvalidInputException& e) {
            cout << "\n  " << e.what() << endl;
        } catch (OESException& e) {
            cout << "\n  " << e.what() << endl;
        }
    }

    // ── LEADERBOARD ──────────────────────────────────────────
    void displayLeaderboard(int examId) {
        Exam* e = findExam(examId);
        if (!e) { cout << "  Exam not found." << endl; return; }

        // f. Array of Objects for sorting
        ExamResult board[200];
        int count = 0;

        for (int i = 0; i < studentCount; i++) {
            Student* s = students[i];
            for (int j = 0; j < s->getResultCount(); j++) {
                if (s->getResult(j).getExamId() == examId)
                    board[count++] = s->getResult(j);
            }
        }

        if (count == 0) { cout << "  No results yet for this exam." << endl; return; }

        // f. Bubble Sort descending by marks
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (board[j + 1].getObtainedMarks() > board[j].getObtainedMarks()) {
                    ExamResult tmp = board[j];
                    board[j]       = board[j + 1];
                    board[j + 1]   = tmp;
                }
            }
        }

        printHeader("Leaderboard: " + e->getTitle());
        cout << "  Rank   Student              Score     %      Status" << endl;
        printLine('-', 55);
        for (int i = 0; i < count; i++) {
            cout << "  " << (i + 1) << "      "
                 << board[i].getStudentName();
            // pad name to 20 chars manually
            int pad = 20 - (int)board[i].getStudentName().size();
            for (int p = 0; p < pad; p++) cout << " ";
            cout << board[i].getObtainedMarks()
                 << "/"  << board[i].getTotalMarks()
                 << "    " << board[i].getPercentage() << "%"
                 << "   " << (board[i].getPassed() ? "PASS" : "FAIL")
                 << endl;
        }
    }

    // ── VIEW HELPERS ─────────────────────────────────────────
    void viewAllStudents() {
        printHeader("All Students (" + intToStr(studentCount) + ")");
        if (studentCount == 0) { cout << "  None registered." << endl; return; }
        for (int i = 0; i < studentCount; i++) students[i]->displayInfo();
    }

    void viewAllExaminers() {
        printHeader("All Examiners (" + intToStr(examinerCount) + ")");
        for (int i = 0; i < examinerCount; i++) examiners[i]->displayInfo();
    }

    void viewAllExams() {
        printHeader("All Exams (" + intToStr(examCount) + ")");
        if (examCount == 0) { cout << "  No exams yet." << endl; return; }
        for (int i = 0; i < examCount; i++) if (exams[i]) exams[i]->displayInfo();
    }

    void deleteStudent(int id) {
        for (int i = 0; i < studentCount; i++) {
            if (students[i]->getId() == id) {
                string n = students[i]->getName();
                delete students[i]; // e. delete
                for (int j = i; j < studentCount - 1; j++)
                    students[j] = students[j + 1];
                students[--studentCount] = nullptr;
                fileManager.logActivity("Student deleted: " + n, currentDate);
                cout << "  '" << n << "' deleted." << endl;
                return;
            }
        }
        cout << "  Student ID " << id << " not found." << endl;
    }

    void toggleExam(int examId) {
        Exam* e = findExam(examId);
        if (!e) { cout << "  Exam not found." << endl; return; }
        e->setIsActive(!e->getIsActive());
        string status = e->getIsActive() ? "ACTIVE" : "INACTIVE";
        cout << "  Exam '" << e->getTitle() << "' is now " << status << endl;
        fileManager.logActivity("Exam toggled: " + e->getTitle() + " -> " + status, currentDate);
    }

    void systemStats() {
        printHeader("System Statistics");
        cout << "  Total Students       : " << studentCount                << endl;
        cout << "  Total Examiners      : " << examinerCount               << endl;
        cout << "  Total Admins         : " << adminCount                  << endl;
        cout << "  Total Exams          : " << examCount                   << endl;
        cout << "  Total Question Banks : " << bankCount                   << endl;
        cout << "  Total Questions      : " << Question::getQuestionCount() << endl;
        cout << "  Total Person Objects : " << Person::getPersonCount()     << endl;
        cout << "  System Instances     : " << instanceCount               << endl;
    }

    // ── MENUS ────────────────────────────────────────────────

    void adminMenu(Admin* admin) {
        int ch;
        do {
            cout << "\n";
            printLine('=', 40);
            cout << "  ADMIN PANEL  -  " << admin->getName() << endl;
            printLine('=', 40);
            cout << "   1.  Register Student\n";
            cout << "   2.  Register Examiner\n";
            cout << "   3.  View All Students\n";
            cout << "   4.  View All Examiners\n";
            cout << "   5.  View All Exams\n";
            cout << "   6.  Delete Student\n";
            cout << "   7.  Toggle Exam Active/Inactive\n";
            cout << "   8.  View Leaderboard\n";
            cout << "   9.  System Statistics\n";
            cout << "  10.  View Activity Log\n";
            cout << "  11.  View Results File\n";
            cout << "  12.  View Binary Exam File\n";
            cout << "  13.  My Profile\n";
            cout << "   0.  Logout\n";
            printLine('-', 40);
            cout << "  Choice: "; cin >> ch;

            switch (ch) {
                case  1: registerStudent();  pauseScreen(); break;
                case  2: registerExaminer(); pauseScreen(); break;
                case  3: viewAllStudents();  pauseScreen(); break;
                case  4: viewAllExaminers(); pauseScreen(); break;
                case  5: viewAllExams();     pauseScreen(); break;
                case  6: {
                    cout << "  Enter Student ID: "; int id; cin >> id;
                    deleteStudent(id); pauseScreen(); break;
                }
                case  7: {
                    cout << "  Enter Exam ID: "; int id; cin >> id;
                    toggleExam(id); pauseScreen(); break;
                }
                case  8: {
                    cout << "  Enter Exam ID: "; int id; cin >> id;
                    displayLeaderboard(id); pauseScreen(); break;
                }
                case  9: systemStats(); pauseScreen(); break;
                case 10: fileManager.printActivityLog(); pauseScreen(); break;
                case 11: fileManager.printResultsFromFile(); pauseScreen(); break;
                case 12: fileManager.printExamsBinary("exams.bin"); pauseScreen(); break;
                case 13: admin->displayInfo(); pauseScreen(); break;
                case  0: admin->logout(); cout << "\n  Logged out.\n"; break;
                default: cout << "  Invalid choice.\n";
            }
        } while (ch != 0);
    }

    void examinerMenu(Examiner* ex) {
        int ch;
        do {
            cout << "\n";
            printLine('=', 40);
            cout << "  EXAMINER PANEL  -  " << ex->getName() << endl;
            printLine('=', 40);
            cout << "  1.  Create New Exam\n";
            cout << "  2.  View My Exams\n";
            cout << "  3.  View Exam Details & Questions\n";
            cout << "  4.  Browse Question Banks\n";
            cout << "  5.  View Leaderboard\n";
            cout << "  6.  View All Students\n";
            cout << "  7.  My Profile\n";
            cout << "  0.  Logout\n";
            printLine('-', 40);
            cout << "  Choice: "; cin >> ch;

            switch (ch) {
                case 1: createExam(ex); pauseScreen(); break;
                case 2: {
                    printHeader("Your Exams");
                    if (ex->getExamCount() == 0) {
                        cout << "  No exams created yet.\n";
                    } else {
                        for (int i = 0; i < ex->getExamCount(); i++)
                            ex->getExam(i)->displayInfo();
                    }
                    pauseScreen(); break;
                }
                case 3: {
                    cout << "  Enter Exam ID: "; int id; cin >> id;
                    Exam* e = findExam(id);
                    if (e) e->displayWithQuestions();
                    else   cout << "  Exam not found.\n";
                    pauseScreen(); break;
                }
                case 4: {
                    for (int i = 0; i < bankCount; i++)
                        if (banks[i]) banks[i]->displayAll();
                    pauseScreen(); break;
                }
                case 5: {
                    cout << "  Enter Exam ID: "; int id; cin >> id;
                    displayLeaderboard(id); pauseScreen(); break;
                }
                case 6: viewAllStudents(); pauseScreen(); break;
                case 7: ex->displayInfo(); pauseScreen(); break;
                case 0: ex->logout(); cout << "\n  Logged out.\n"; break;
                default: cout << "  Invalid choice.\n";
            }
        } while (ch != 0);
    }

    void studentMenu(Student* st) {
        int ch;
        do {
            cout << "\n";
            printLine('=', 40);
            cout << "  STUDENT PORTAL  -  " << st->getName() << endl;
            printLine('=', 40);
            cout << "  1.  View Available Exams\n";
            cout << "  2.  Take an Exam\n";
            cout << "  3.  View My Results\n";
            cout << "  4.  View Leaderboard\n";
            cout << "  5.  My Profile\n";
            cout << "  0.  Logout\n";
            printLine('-', 40);
            cout << "  Choice: "; cin >> ch;

            switch (ch) {
                case 1: viewAllExams(); pauseScreen(); break;
                case 2: takeExam(st); pauseScreen(); break;
                case 3: st->viewResults(); pauseScreen(); break;
                case 4: {
                    cout << "  Enter Exam ID: "; int id; cin >> id;
                    displayLeaderboard(id); pauseScreen(); break;
                }
                case 5: st->displayInfo(); pauseScreen(); break;
                case 0: st->logout(); cout << "\n  Logged out.\n"; break;
                default: cout << "  Invalid choice.\n";
            }
        } while (ch != 0);
    }

public:
    // b. Constructor
    ExaminationSystem()
        : studentCount(0), examinerCount(0), adminCount(0),
          examCount(0), bankCount(0),
          nextStudentId(1001), nextExaminerId(2001),
          nextExamId(3001), nextQuestionId(4001),
          fileManager("data/"), currentUser(nullptr),
          currentDate("2026-05-08") { // fixed date — no <ctime> needed

        instanceCount++;

        // Initialize arrays to nullptr
        for (int i = 0; i < 200; i++) students[i]  = nullptr;
        for (int i = 0; i < 50;  i++) examiners[i] = nullptr;
        for (int i = 0; i < 10;  i++) admins[i]    = nullptr;
        for (int i = 0; i < 200; i++) exams[i]     = nullptr;
        for (int i = 0; i < 20;  i++) banks[i]     = nullptr;

        seedData();
    }

    // b. Destructor
    ~ExaminationSystem() {
        for (int i = 0; i < studentCount;  i++) delete students[i];
        for (int i = 0; i < examinerCount; i++) delete examiners[i];
        for (int i = 0; i < adminCount;    i++) delete admins[i];
        for (int i = 0; i < examCount;     i++) delete exams[i];
        for (int i = 0; i < bankCount;     i++) delete banks[i];
        instanceCount--;
    }

    static int getInstanceCount() { return instanceCount; } // d. Static

    void run() {
        printLine('*');
        cout << "     ONLINE EXAMINATION SYSTEM  (OES)  v1.0\n";
        cout << "     CS-112  |  OOP & Design  |  GIKI\n";
        printLine('*');

        cout << "\n  Default Login Credentials:\n";
        cout << "    Admin    ->  admin    /  admin123\n";
        cout << "    Examiner ->  snabi    /  examiner123\n";
        cout << "    Student  ->  ali      /  student123\n";
        cout << "    Student  ->  sara     /  student123\n";

        int ch;
        do {
            cout << "\n";
            printLine('=', 40);
            cout << "          MAIN MENU\n";
            printLine('=', 40);
            cout << "  1.  Login\n";
            cout << "  2.  About System\n";
            cout << "  0.  Exit\n";
            printLine('-', 40);
            cout << "  Choice: "; cin >> ch;

            if (ch == 1) {
                // c. Runtime Polymorphism — virtual getRole() determines dispatch
                User* user = performLogin();
                if (user) {
                    cout << "\n  Welcome, " << user->getName()
                         << "!  Role: " << user->getRole() << endl;

                    // c. dynamic_cast — safe downcast for menu dispatch
                    if (user->getRole() == "Admin")
                        adminMenu(dynamic_cast<Admin*>(user));
                    else if (user->getRole() == "Examiner")
                        examinerMenu(dynamic_cast<Examiner*>(user));
                    else if (user->getRole() == "Student")
                        studentMenu(dynamic_cast<Student*>(user));
                }

            } else if (ch == 2) {
                printHeader("About Online Examination System");
                cout << "  Project  : Online Examination System\n";
                cout << "  Course   : CS-112 OOP & Design\n";
                cout << "  Institute: GIKI\n\n";
                cout << "  User Roles:\n";
                cout << "    Admin    -> Registers users, manages exams\n";
                cout << "    Examiner -> Creates exams and questions\n";
                cout << "    Student  -> Takes exams, views results\n\n";
                cout << "  Headers  : iostream  fstream  string\n";

            } else if (ch == 0) {
                cout << "\n  Thank you for using OES. Goodbye!\n";
            } else {
                cout << "  Invalid choice.\n";
            }
        } while (ch != 0);
    }
};

int ExaminationSystem::instanceCount = 0;

// ============================================================
//  MAIN FUNCTION
// ============================================================

int main() {
    try {
        // e. Dynamic Memory Allocation
        ExaminationSystem* system = new ExaminationSystem();

        // e. Using 'this' pointer internally via run()
        system->run();

        // e. delete — calls destructor, frees all memory
        delete system;

    } catch (OESException& e) { // h. Exception Handling
        cerr << "\nSystem Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
