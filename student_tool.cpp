// students.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

const char* STUD_FILE = "students.txt";
const char* CRE_FILE  = "credentials.txt";

struct Student {
    int roll;
    std::string name;
    float mark;
};

class Auth {
public:
    std::string currentUser;
    std::string currentRole;

    bool login() {
        std::string inUser, inPass;
        std::cout << "USERNAME: ";
        std::cin >> inUser;
        std::cout << "PASSWORD: ";
        std::cin >> inPass;

        std::ifstream fin(CRE_FILE);
        if (!fin) {
            std::cerr << "Credential file missing!\n";
            return false;
        }
        std::string u, p, r;
        while (fin >> u >> p >> r) {
            if (u == inUser && p == inPass) {
                currentUser = u;
                currentRole = r;
                return true;
            }
        }
        return false;
    }
};

class StudentDB {
    std::vector<Student> students;

    void load() {
        students.clear();
        std::ifstream fin(STUD_FILE);
        if (!fin) return; // no file -> empty list
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            std::istringstream ss(line);
            Student s;
            // Name may contain spaces; we assume format: roll name-without-newline mark
            // We'll parse roll, then read name tokens until last token is a float.
            if (!(ss >> s.roll)) continue;

            // read remaining tokens into a vector
            std::vector<std::string> toks;
            std::string tok;
            while (ss >> tok) toks.push_back(tok);
            if (toks.empty()) continue;

            // last token should be mark
            try {
                s.mark = std::stof(toks.back());
            } catch (...) {
                continue;
            }
            toks.pop_back(); // remove mark token
            // join remaining as name
            s.name.clear();
            for (size_t i = 0; i < toks.size(); ++i) {
                if (i) s.name += " ";
                s.name += toks[i];
            }
            students.push_back(s);
        }
    }

    void save() {
        std::ofstream fout(STUD_FILE, std::ios::trunc);
        for (const auto &s : students) {
            // keep same simple format: roll name mark
            fout << s.roll << " " << s.name << " " << std::fixed << std::setprecision(2) << s.mark << "\n";
        }
    }

public:
    StudentDB() { load(); }

    void addStudent() {
        Student s;
        std::cout << "Roll: ";
        std::cin >> s.roll;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Name: ";
        std::getline(std::cin, s.name);
        std::cout << "Mark: ";
        std::cin >> s.mark;
        students.push_back(s);
        save();
        std::cout << "Student added!\n";
    }

    void displayStudents() {
        if (students.empty()) {
            std::cout << "No student file or no records!\n";
            return;
        }
        std::cout << "Roll\tName\tMark\n";
        std::cout << "----\t----\t----\n";
        for (const auto &s : students) {
            std::cout << s.roll << "\t" << s.name << "\t" << std::fixed << std::setprecision(2) << s.mark << "\n";
        }
    }

    void searchStudent() {
        int find;
        std::cout << "Enter roll to search: ";
        std::cin >> find;
        for (const auto &s : students) {
            if (s.roll == find) {
                std::cout << "Found: " << s.roll << " " << s.name << " " << std::fixed << std::setprecision(2) << s.mark << "\n";
                return;
            }
        }
        std::cout << "Student not found!\n";
    }

    void deleteStudent() {
        int delRoll;
        std::cout << "Enter roll to delete: ";
        std::cin >> delRoll;
        auto it = std::remove_if(students.begin(), students.end(), [&](const Student &s){ return s.roll == delRoll; });
        if (it != students.end()) {
            students.erase(it, students.end());
            save();
            std::cout << "Student deleted!\n";
        } else {
            std::cout << "Roll not found!\n";
        }
    }

    void updateStudent() {
        int updateRoll;
        std::cout << "Enter roll to update: ";
        std::cin >> updateRoll;
        for (auto &s : students) {
            if (s.roll == updateRoll) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "New Name: ";
                std::getline(std::cin, s.name);
                std::cout << "New Mark: ";
                std::cin >> s.mark;
                save();
                std::cout << "Student updated!\n";
                return;
            }
        }
        std::cout << "Roll not found!\n";
    }
};

void adminMenu(StudentDB &db) {
    while (true) {
        std::cout << "\nADMIN MENU\n";
        std::cout << "1.Add\n2.Display\n3.Search\n4.Update\n5.Delete\n6.Logout\n";
        int c; std::cin >> c;
        switch (c) {
            case 1: db.addStudent(); break;
            case 2: db.displayStudents(); break;
            case 3: db.searchStudent(); break;
            case 4: db.updateStudent(); break;
            case 5: db.deleteStudent(); break;
            default: return;
        }
    }
}

void staffMenu(StudentDB &db) {
    while (true) {
        std::cout << "\nSTAFF MENU\n";
        std::cout << "1.Add\n2.Display\n3.Search\n4.Update\n5.Logout\n";
        int c; std::cin >> c;
        switch (c) {
            case 1: db.addStudent(); break;
            case 2: db.displayStudents(); break;
            case 3: db.searchStudent(); break;
            case 4: db.updateStudent(); break;
            default: return;
        }
    }
}

void guestMenu(StudentDB &db) {
    while (true) {
        std::cout << "\nGUEST MENU\n";
        std::cout << "1.Display\n2.Search\n3.Logout\n";
        int c; std::cin >> c;
        switch (c) {
            case 1: db.displayStudents(); break;
            case 2: db.searchStudent(); break;
            default: return;
        }
    }
}

int main() {
    Auth auth;
    if (!auth.login()) {
        std::cout << "Invalid login!\n";
        return 0;
    }

    std::cout << "Logged in as: " << auth.currentRole << "\n";
    StudentDB db;

    if (auth.currentRole == "admin") adminMenu(db);
    else if (auth.currentRole == "staff") staffMenu(db);
    else guestMenu(db);

    return 0;
}
