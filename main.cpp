/*
============================================================
 ONLINE EXAMINATION SYSTEM
 Course : CS-112 Object Oriented Programming - GIK
 Headers : #include <iostream>
           #include <fstream>
           #include <string>
============================================================
 DEFAULT CREDENTIALS:
 Role        | Username   | Password
 ------------|------------|----------
 Admin       | admin      | admin123
 Instructor  | instructor | inst123
 Student     | register   | you choose
============================================================
 All OOP concepts covered:
 a. Classes, Objects, Encapsulation, Access Specifiers
 b. Default/Parameterized/Copy Constructor, Destructor, Init Lists
 c. Hierarchical Inheritance (Student, Admin, Instructor <- User),
    Function Overriding, Runtime Polymorphism, Virtual Functions,
    Abstract Class, Pure Virtual Functions
 d. Friend Functions, Static Members, Operator Overloading,
    Inline Functions
 e. Pointers to Objects, Dynamic Memory (new/delete), this pointer
 f. Array of Objects, Searching & Sorting with Objects,
    Structures vs Classes
 g. File Handling (ifstream, ofstream, text files)
 h. Exception Handling (try/catch/throw, Custom Exception Classes)
============================================================
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Array size limits
const int MAX_Q = 100;   // max questions in bank
const int MAX_U = 50;    // max users
const int MAX_R = 200;   // max results


// ============================================================
// SECTION h: CUSTOM EXCEPTION CLASS
// ============================================================
class AppException {
    string message;
public:
    AppException(const string& m) : message(m) {}
    string what() const { return message; }
};


// ============================================================
// SECTION a,b,d,f: QUESTION CLASS
// Demonstrates: Encapsulation, Constructors, Static Member,
//               Operator Overloading, Friend Functions,
//               Array of Objects (options[4])
// ============================================================
class Question {
private:                          // a: private access specifier
    string text;
    string options[4];            // f: fixed array member
    int    correct;               // 1 to 4
    string topic;

public:                           // a: public access specifier
    static int count;             // d: static data member

    // b: Default Constructor with initialization list
    Question() : text(""), correct(0), topic("") { count++; }

    // b: Parameterized Constructor
    Question(const string& t, const string opts[4], int c, const string& top)
        : text(t), correct(c), topic(top)
    {
        for (int i = 0; i < 4; i++) options[i] = opts[i];
        count++;
    }

    // b: Copy Constructor
    Question(const Question& q)
        : text(q.text), correct(q.correct), topic(q.topic)
    {
        for (int i = 0; i < 4; i++) options[i] = q.options[i];
        count++;
    }

    // b: Destructor
    ~Question() { count--; }

    // Getters - Encapsulation / Data Hiding
    string getText()      const { return text; }
    string getTopic()     const { return topic; }
    int    getCorrect()   const { return correct; }
    string getOption(int i) const { return options[i]; }

    // d: Inline member function
    inline void setTopic(const string& t) { topic = t; }

    void display(int num) const {
        cout << "\nQ" << num << ". [" << topic << "] " << text << "\n";
        for (int i = 0; i < 4; i++)
            cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }

    // d: Operator Overloading - used for sorting and comparing
    bool operator< (const Question& q) const { return topic < q.topic; }
    bool operator==(const Question& q) const { return topic == q.topic; }

    // d: Friend function declarations (defined outside class)
    friend void     saveQ(ofstream& f, const Question& q);
    friend Question loadQ(ifstream& f);
};

int Question::count = 0;   // d: static member definition outside class

// d: Friend functions - access private members directly
void saveQ(ofstream& f, const Question& q) {
    f << q.text << "\n";
    for (int i = 0; i < 4; i++) f << q.options[i] << "\n";
    f << q.correct << "\n" << q.topic << "\n";
}

Question loadQ(ifstream& f) {
    Question q;
    getline(f, q.text);
    for (int i = 0; i < 4; i++) getline(f, q.options[i]);
    f >> q.correct; f.ignore();
    getline(f, q.topic);
    return q;
}


// ============================================================
// SECTION f: RESULT - Struct vs Class demonstration
// ============================================================
struct Result {                   // f: struct (public by default)
    string name;
    string topic;
    int    score;
    int    total;

    // Struct constructor
    Result() : name(""), topic(""), score(0), total(0) {}
    Result(const string& n, const string& t, int s, int tot)
        : name(n), topic(t), score(s), total(tot) {}

    // d: Operator overloading - for sorting by score
    bool operator>(const Result& r) const { return score > r.score; }

    int percent() const { return total > 0 ? score * 100 / total : 0; }

    void display() const {
        cout << "  Student: " << name
             << "  Topic: "   << topic
             << "  Score: "   << score << "/" << total
             << " ("          << percent() << "%) "
             << (percent() >= 50 ? "PASSED" : "FAILED") << "\n";
    }
};


// ============================================================
// SECTION c: ABSTRACT BASE CLASS - User
// Demonstrates: Abstract Class, Pure Virtual Functions,
//               Virtual Functions, Static Member
// ============================================================
class User {
protected:                        // a: protected access specifier
    string name, username, password;
    static int userCount;         // d: static member for all users

public:
    // b: Parameterized Constructor with initialization list
    User(const string& n, const string& u, const string& p)
        : name(n), username(u), password(p) { userCount++; }

    // b: Virtual Destructor - needed for correct polymorphic cleanup
    virtual ~User() { userCount--; }

    // c: Pure Virtual Functions - makes User an abstract class
    virtual void showMenu() = 0;
    virtual string getRole() const = 0;

    // c: Virtual Function - overridable by derived classes
    virtual void displayInfo() const {
        cout << "  Name: "  << name
             << "  User: "  << username
             << "  Role: "  << getRole() << "\n";
    }

    bool authenticate(const string& u, const string& p) const {
        return username == u && password == p;
    }

    // d: Inline getters (encapsulation)
    inline string getName()     const { return name; }
    inline string getUsername() const { return username; }
    inline string getPassword() const { return password; }

    // d: Static member function
    static int getUserCount() { return userCount; }

    // e: 'this' pointer - explicitly refers to calling object
    User& setName(const string& n) { this->name = n; return *this; }
};

int User::userCount = 0;


// ============================================================
// SECTION c: STUDENT - Hierarchical Inheritance from User
// ============================================================
class Student : public User {
private:
    Result results[50];           // f: array of objects
    int    resultCount;

public:
    // b: Default Constructor
    Student() : User("", "", ""), resultCount(0) {}

    // b: Parameterized Constructor
    Student(const string& n, const string& u, const string& p)
        : User(n, u, p), resultCount(0) {}

    // b: Copy Constructor
    Student(const Student& s)
        : User(s.name, s.username, s.password), resultCount(s.resultCount)
    {
        for (int i = 0; i < resultCount; i++) results[i] = s.results[i];
    }

    ~Student() override {}

    // c: Override pure virtual functions
    string getRole() const override { return "Student"; }

    void showMenu() override {
        cout << "\n======== STUDENT MENU ========\n"
             << "  1. Take Exam\n"
             << "  2. View My Results\n"
             << "  3. Logout\n"
             << "==============================\n";
    }

    // e: Passing object to function
    void addResult(const Result& r) {
        if (resultCount < 50) results[resultCount++] = r;
    }

    void viewResults() const {
        if (resultCount == 0) { cout << "  No exams taken yet.\n"; return; }
        cout << "\n--- Your Results (" << resultCount << " exam(s)) ---\n";
        for (int i = 0; i < resultCount; i++) results[i].display();
    }
};


// ============================================================
// SECTION c: ADMIN - Hierarchical Inheritance from User
// ============================================================
class Admin : public User {
public:
    Admin(const string& n, const string& u, const string& p)
        : User(n, u, p) {}

    // c: Override pure virtual functions
    string getRole() const override { return "Admin"; }

    void showMenu() override {
        cout << "\n========= ADMIN MENU =========\n"
             << "  1. Add Question\n"
             << "  2. View All Questions\n"
             << "  3. View All Results\n"
             << "  4. View All Users\n"
             << "  5. Logout\n"
             << "==============================\n";
    }

    // c: Function Overriding
    void displayInfo() const override {
        cout << "  [ADMIN] "; User::displayInfo();
    }
};


// ============================================================
// SECTION c: INSTRUCTOR - Hierarchical Inheritance from User
// ============================================================
class Instructor : public User {
private:
    string subject;

public:
    // b: Parameterized Constructor
    Instructor(const string& n, const string& u, const string& p,
               const string& sub = "General")
        : User(n, u, p), subject(sub) {}

    // b: Copy Constructor
    Instructor(const Instructor& i)
        : User(i.name, i.username, i.password), subject(i.subject) {}

    ~Instructor() override {}

    // c: Override pure virtual functions
    string getRole() const override { return "Instructor"; }

    // d: Inline getter
    inline string getSubject() const { return subject; }

    void showMenu() override {
        cout << "\n====== INSTRUCTOR MENU =======\n"
             << "  Subject: " << subject << "\n"
             << "  -----------------------------\n"
             << "  1. Add Question\n"
             << "  2. View Questions by Topic\n"
             << "  3. View Student Results\n"
             << "  4. Logout\n"
             << "==============================\n";
    }

    // c: Function Overriding
    void displayInfo() const override {
        cout << "  [INSTRUCTOR | " << subject << "] ";
        User::displayInfo();
    }
};


// ============================================================
// SECTION g,f: QUESTION BANK
// Demonstrates: File Handling, Array of Objects, Sorting, Searching
// ============================================================
class QuestionBank {
private:
    Question questions[MAX_Q];    // f: array of objects
    int      qCount;
    string   filename;

    // f: Bubble Sort using overloaded operator<
    void bubbleSort() {
        for (int i = 0; i < qCount - 1; i++)
            for (int j = 0; j < qCount - i - 1; j++)
                if (questions[j + 1] < questions[j]) {
                    Question tmp  = questions[j];
                    questions[j]  = questions[j + 1];
                    questions[j + 1] = tmp;
                }
    }

public:
    QuestionBank(const string& f = "questions.txt") : qCount(0), filename(f) {
        loadFromFile();
    }

    ~QuestionBank() { saveToFile(); }

    void add(const Question& q, bool silent = false) {
        if (qCount < MAX_Q) {
            questions[qCount++] = q;
            if (!silent)
                cout << "  Question added successfully.\n";
        }
    }

    // f: Linear search by topic - returns matched array
    int getByTopic(const string& topic, Question out[], int maxOut) const {
        int n = 0;
        for (int i = 0; i < qCount && n < maxOut; i++)
            if (questions[i].getTopic() == topic) out[n++] = questions[i];
        return n;
    }

    void sortByTopic() { bubbleSort(); }

    void displayAll() const {
        if (qCount == 0) { cout << "  Bank is empty.\n"; return; }
        for (int i = 0; i < qCount; i++) questions[i].display(i + 1);
    }

    int size() const { return qCount; }

    // g: Write objects to text file using ofstream
    void saveToFile() {
        try {
            ofstream f(filename.c_str());
            if (!f) throw AppException("Cannot write to " + filename);
            f << qCount << "\n";
            for (int i = 0; i < qCount; i++) saveQ(f, questions[i]);
        } catch (AppException& e) {
            cerr << "  [File Error] " << e.what() << "\n";
        }
    }

    // g: Read objects from text file using ifstream
    void loadFromFile() {
        try {
            ifstream f(filename.c_str());
            if (!f) return;
            int n; f >> n; f.ignore();
            for (int i = 0; i < n && qCount < MAX_Q; i++)
                questions[qCount++] = loadQ(f);
        } catch (AppException& e) {
            cerr << "  [Load Error] " << e.what() << "\n";
        }
    }
};


// ============================================================
// SECTION e: EXAM ENGINE
// Demonstrates: Pointer to Object, Passing/Returning Objects
// ============================================================
class ExamEngine {
private:
    QuestionBank* bank;           // e: pointer to object
    Result        allResults[MAX_R];
    int           rCount;
    string        filename;

    void saveResults() {
        try {
            ofstream f(filename.c_str());
            if (!f) throw AppException("Cannot save results.");
            f << rCount << "\n";
            for (int i = 0; i < rCount; i++)
                f << allResults[i].name  << "\n"
                  << allResults[i].topic << "\n"
                  << allResults[i].score << "\n"
                  << allResults[i].total << "\n";
        } catch (AppException& e) {
            cerr << "  [File Error] " << e.what() << "\n";
        }
    }

    void loadResults() {
        try {
            ifstream f(filename.c_str());
            if (!f) return;
            int n; f >> n; f.ignore();
            for (int i = 0; i < n && rCount < MAX_R; i++) {
                string nm, top; int s, t;
                getline(f, nm); getline(f, top);
                f >> s >> t; f.ignore();
                allResults[rCount++] = Result(nm, top, s, t);
            }
        } catch (AppException& e) {
            cerr << "  [Load Error] " << e.what() << "\n";
        }
    }

public:
    ExamEngine(QuestionBank* b, const string& f = "results.txt")
        : bank(b), rCount(0), filename(f) { loadResults(); }

    ~ExamEngine() { saveResults(); }

    // e: Takes pointer to Student, returns a Result object
    Result conductExam(Student* student, const string& topic) {
        Question qs[MAX_Q];
        int n = bank->getByTopic(topic, qs, MAX_Q);
        if (n == 0) throw AppException("No questions for topic: " + topic);

        int score = 0;
        cout << "\n======== EXAM STARTED ========\n"
             << "  Topic: " << topic << "  |  Questions: " << n << "\n"
             << "==============================\n";

        for (int i = 0; i < n; i++) {
            qs[i].display(i + 1);
            int ans = 0;
            cout << "  Your answer (1-4): ";
            cin >> ans;

            // h: Exception Handling
            if (cin.fail() || ans < 1 || ans > 4) {
                cin.clear(); cin.ignore(1000, '\n');
                throw AppException("Answer must be between 1 and 4.");
            }
            if (ans == qs[i].getCorrect()) score++;
        }

        Result r(student->getName(), topic, score, n);
        allResults[rCount++] = r;
        student->addResult(r);    // e: passing object to function

        cout << "\n======== EXAM RESULT =========\n";
        r.display();
        cout << "==============================\n";
        return r;                 // e: returning object from function
    }

    void viewAll() const {
        if (rCount == 0) { cout << "  No results yet.\n"; return; }
        cout << "\n--- All Exam Results ---\n";
        for (int i = 0; i < rCount; i++) allResults[i].display();
    }

    void viewByTopic(const string& topic) const {
        bool found = false;
        cout << "\n--- Results for: " << topic << " ---\n";
        for (int i = 0; i < rCount; i++)
            if (allResults[i].topic == topic) { allResults[i].display(); found = true; }
        if (!found) cout << "  No results for: " << topic << "\n";
    }
};


// ============================================================
// SECTION e: USER MANAGER
// Demonstrates: Dynamic Memory (new/delete), Polymorphism via User*
// ============================================================
class UserManager {
private:
    User*  users[MAX_U];          // e: array of pointers (polymorphism)
    int    uCount;
    string filename;

    void saveUsers() {
        ofstream f(filename.c_str());
        if (!f) return;
        f << uCount << "\n";
        for (int i = 0; i < uCount; i++) {
            f << users[i]->getRole()     << "\n"
              << users[i]->getName()     << "\n"
              << users[i]->getUsername() << "\n"
              << users[i]->getPassword() << "\n";
            if (users[i]->getRole() == "Instructor")
                f << dynamic_cast<Instructor*>(users[i])->getSubject() << "\n";
        }
    }

    void loadUsers() {
        ifstream f(filename.c_str());
        if (!f) return;
        int n; f >> n; f.ignore();
        for (int i = 0; i < n && uCount < MAX_U; i++) {
            string role, nm, u, p;
            getline(f, role); getline(f, nm);
            getline(f, u);    getline(f, p);
            if (role == "Admin")
                users[uCount++] = new Admin(nm, u, p);          // e: new
            else if (role == "Instructor") {
                string sub; getline(f, sub);
                users[uCount++] = new Instructor(nm, u, p, sub);
            } else
                users[uCount++] = new Student(nm, u, p);
        }
    }

public:
    UserManager(const string& f = "users.txt") : uCount(0), filename(f) {
        loadUsers();
        if (uCount == 0) {
            users[uCount++] = new Admin("Administrator", "admin", "admin123");
            users[uCount++] = new Instructor("Said Nabi", "instructor", "inst123", "OOP & DS");
        }
    }

    ~UserManager() {
        saveUsers();
        for (int i = 0; i < uCount; i++) delete users[i];       // e: delete
        uCount = 0;
    }

    void registerStudent(const string& n, const string& u, const string& p) {
        for (int i = 0; i < uCount; i++)
            if (users[i]->getUsername() == u)
                throw AppException("Username '" + u + "' already taken.");
        users[uCount++] = new Student(n, u, p);
        cout << "  Registered! Login: " << u << " / " << p << "\n";
    }

    // c: Runtime Polymorphism - works for any role
    User* login(const string& u, const string& p) {
        for (int i = 0; i < uCount; i++)
            if (users[i]->authenticate(u, p)) return users[i];
        throw AppException("Invalid username or password.");
    }

    void displayAll() const {
        cout << "\n--- All Users (Active: " << User::getUserCount() << ") ---\n";
        for (int i = 0; i < uCount; i++) users[i]->displayInfo();
    }
};


// ============================================================
// HELPER: Seed sample questions on first run
// ============================================================
void seedQuestions(QuestionBank& bank) {
    string o1[4] = {"int", "float", "char", "bool"};
    bank.add(Question("Which type stores a whole number in C++?", o1, 1, "Programming"), true);

    string o2[4] = {"for", "while", "do-while", "All of these"};
    bank.add(Question("Which of the following is a loop in C++?", o2, 4, "Programming"), true);

    string o3[4] = {"Abstraction", "Encapsulation", "Inheritance", "Polymorphism"};
    bank.add(Question("Which OOP concept hides internal data?", o3, 2, "OOP"), true);

    string o4[4] = {"Hiding", "Overriding", "Overloading", "None"};
    bank.add(Question("Redefining a base class function in derived class is?", o4, 2, "OOP"), true);

    string o5[4] = {"new", "malloc", "alloc", "create"};
    bank.add(Question("Which keyword allocates heap memory in C++?", o5, 1, "OOP"), true);

    string o6[4] = {"O(n)", "O(log n)", "O(1)", "O(n^2)"};
    bank.add(Question("Time complexity of Binary Search is?", o6, 2, "DS"), true);

    string o7[4] = {"Stack", "Queue", "Tree", "Graph"};
    bank.add(Question("Which data structure follows LIFO order?", o7, 1, "DS"), true);

    string o8[4] = {"Stack", "Queue", "Array", "Linked List"};
    bank.add(Question("Which structure follows FIFO order?", o8, 2, "DS"), true);
}


// ============================================================
// HELPER: Add question (shared by Admin and Instructor)
// ============================================================
void addQuestionFlow(QuestionBank& bank) {
    string text, opts[4], topic; int correct;
    cout << "  Question text  : "; getline(cin, text);
    for (int i = 0; i < 4; i++) {
        cout << "  Option " << (i + 1) << "       : ";
        getline(cin, opts[i]);
    }
    cout << "  Correct (1-4)  : "; cin >> correct; cin.ignore();
    cout << "  Topic          : "; getline(cin, topic);
    bank.add(Question(text, opts, correct, topic));
}


// ============================================================
// MAIN FUNCTION
// ============================================================
int main() {
    cout << "\n";
    cout << "  ****************************************************\n";
    cout << "  *         ONLINE EXAMINATION SYSTEM               *\n";
    cout << "  *   CS-112 Object Oriented Programming -- GIK     *\n";
    cout << "  ****************************************************\n";
    cout << "\n";
    cout << "  ====================================================\n";
    cout << "  ***             LOGIN CREDENTIALS               ***\n";
    cout << "  ====================================================\n";
    cout << "  Role          Username        Password\n";
    cout << "  ----------------------------------------------------\n";
    cout << "  Admin         admin           admin123\n";
    cout << "  Instructor    instructor      inst123\n";
    cout << "  Student       register first  (you choose)\n";
    cout << "  ====================================================\n";
    cout << "\n";
    cout << "  HOW TO USE:\n";
    cout << "  1 = Login    2 = Register as Student    3 = Exit\n";
    cout << "  TOPICS: OOP | DS | Programming\n";
    cout << "  ====================================================\n\n";

    QuestionBank bank;
    UserManager  manager;
    ExamEngine   engine(&bank);   // e: passing pointer to object

    if (bank.size() == 0) seedQuestions(bank);

    int choice;
    do {
        cout << "\n===== MAIN MENU =====\n"
             << "  1. Login\n"
             << "  2. Register as Student\n"
             << "  3. Exit\n"
             << "  Choice: ";
        cin >> choice; cin.ignore();

        // h: try / catch / throw
        if (choice == 1) {
            try {
                string u, p;
                cout << "  Username: "; getline(cin, u);
                cout << "  Password: "; getline(cin, p);

                User* user = manager.login(u, p);   // c: runtime polymorphism
                cout << "\n  Welcome, " << user->getName()
                     << " [" << user->getRole() << "]!\n";

                // ---------- ADMIN ----------
                if (user->getRole() == "Admin") {
                    Admin* admin = dynamic_cast<Admin*>(user);
                    int ac;
                    do {
                        admin->showMenu();
                        cout << "  Choice: "; cin >> ac; cin.ignore();
                        if      (ac == 1) addQuestionFlow(bank);
                        else if (ac == 2) { bank.sortByTopic(); bank.displayAll(); }
                        else if (ac == 3) engine.viewAll();
                        else if (ac == 4) manager.displayAll();
                        else if (ac == 5) cout << "  Logged out.\n";
                        else              cout << "  Invalid choice.\n";
                    } while (ac != 5);

                // ---------- INSTRUCTOR ----------
                } else if (user->getRole() == "Instructor") {
                    Instructor* inst = dynamic_cast<Instructor*>(user);
                    int ic;
                    do {
                        inst->showMenu();
                        cout << "  Choice: "; cin >> ic; cin.ignore();
                        if (ic == 1) {
                            addQuestionFlow(bank);
                        } else if (ic == 2) {
                            string t;
                            cout << "  Topic (OOP / DS / Programming): ";
                            getline(cin, t);
                            Question qs[MAX_Q];
                            int n = bank.getByTopic(t, qs, MAX_Q);
                            if (n == 0) cout << "  No questions found for: " << t << "\n";
                            else for (int i = 0; i < n; i++) qs[i].display(i + 1);
                        } else if (ic == 3) {
                            string t;
                            cout << "  Topic (or ALL): "; getline(cin, t);
                            if (t == "ALL") engine.viewAll();
                            else            engine.viewByTopic(t);
                        } else if (ic == 4) {
                            cout << "  Logged out.\n";
                        } else {
                            cout << "  Invalid choice.\n";
                        }
                    } while (ic != 4);

                // ---------- STUDENT ----------
                } else {
                    Student* student = dynamic_cast<Student*>(user);
                    int sc;
                    do {
                        student->showMenu();
                        cout << "  Choice: "; cin >> sc; cin.ignore();
                        if (sc == 1) {
                            try {
                                string t;
                                cout << "  Topics: OOP | DS | Programming\n";
                                cout << "  Enter topic: "; getline(cin, t);
                                engine.conductExam(student, t);
                            } catch (AppException& e) {
                                cout << "  [Error] " << e.what() << "\n";
                            }
                        } else if (sc == 2) {
                            student->viewResults();
                        } else if (sc == 3) {
                            cout << "  Logged out.\n";
                        } else {
                            cout << "  Invalid choice.\n";
                        }
                    } while (sc != 3);
                }

            } catch (AppException& e) {
                cout << "  [Error] " << e.what() << "\n";
            }

        } else if (choice == 2) {
            try {
                string n, u, p;
                cout << "  Full Name : "; getline(cin, n);
                cout << "  Username  : "; getline(cin, u);
                cout << "  Password  : "; getline(cin, p);
                manager.registerStudent(n, u, p);
            } catch (AppException& e) {
                cout << "  [Error] " << e.what() << "\n";
            }

        } else if (choice != 3) {
            cout << "  Invalid choice. Try again.\n";
        }

    } while (choice != 3);

    cout << "\n  Thank you for using the Online Examination System!\n"
         << "  Goodbye.\n\n";
    return 0;
}
