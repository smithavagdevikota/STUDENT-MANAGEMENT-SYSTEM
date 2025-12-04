# Student Record Management System (C++)

This is a simple console-based **Student Record Management System** built using C++.  
It allows users to manage student information such as roll number, name, and marks.  
The system uses file handling so that all data is stored permanently.

---

## 🚀 Features
- **Login System** (Admin, Staff, Guest)
- **Admin**
  - Add Student  
  - Display All Students  
  - Search Student  
  - Update Student  
  - Delete Student  
- **Staff**
  - Add, Display, Search, Update
- **Guest**
  - Display, Search
- Data stored using text files (`students.txt`, `credentials.txt`)

---

## 📁 Files
- `student_tool.cpp` — Main source code  
- `students.txt` — Student records  
- `credentials.txt` — Login credentials  

---

## 🛠️ How to Compile and Run
```bash
g++ student_tool.cpp -o student_tool
./student_tool
🔐 Sample Credentials
Add these inside credentials.txt:
admin admin123 admin
staff staff123 staff
guest guest123 guest

🎯 Project Purpose
This project demonstrates:

File handling in C++

Role-based access

Basic CRUD operations

Simple menu-driven user interfaces

📜 License
This project is free to use for learning and academic purposes.
