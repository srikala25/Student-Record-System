#include <bits/stdc++.h>
using namespace std;

char classTeacherSection;
char sections[3];
string subject, userName;
int lineNumber;

void loadOperations();
void showMarksOptions(char, int);

void clearScreen() {
    system("clear");
}

bool isValidUsername(string s, string username) {
    string firstName = "", lastName = "";
    int i;
    for(i = 0; i < s.length(); ++i) {
        if(s[i] == ' '){
            break;
        }
        firstName += s[i];
    } 
    ++i;
    if(username.length() != i) {
        return false;
    }
    for( ; i < s.length(); ++i) {
        lastName += s[i];
    }
    for(i = 0; i < firstName.length(); ++i) {
        if(firstName[i] != username[i]) {
            return false;
        }
    }
    if(username[i] != lastName[0]){
        return false;
    }
    return true;
}

bool isValidPassword(string s, string p) {
    if(s.length() != p.length()) {
        return false;
    }
    for(int i = 0; i < s.length(); ++i) {
        if(s[i] != p[i]) return false;
    }
    return true;
}

string readUptoColon(ifstream &file) {
    string s = "";
    char ch;
    while(true) {
        file.get(ch);
        if(ch == ':'){
            break;
        }
        s += ch;
    }
    return s;
}

string readUptoColon(fstream &file) {
    string s = "";
    char ch;
    while(true) {
        file.get(ch);
        if(ch == ':'){
            break;
        }
        s += ch;
    }
    return s;
}

bool isValidRollNo(fstream &file, char section, string rollno) {
    char ch;
    string s;
    while (!file.eof()) {
        ++lineNumber;
        file.get(ch);
        if(file.eof()) break;
        file.seekg(-1, ios::cur);
        s = readUptoColon(file);
        if(s == rollno) {
            return true;
        }
        while(file.get(ch) && ch != '\n');
    }
    return false;
}

void setValues(ifstream &file) {
    subject = readUptoColon(file);
    classTeacherSection = readUptoColon(file)[0];
    sections[0] = readUptoColon(file)[0];
    sections[1] = readUptoColon(file)[0];
    file.get(sections[2]);
}

void displayTeacher() {
    cout << "Welcome " << userName << "\n\n";
    if(classTeacherSection != '_') {
        cout << "You are the class teacher for " << classTeacherSection << " section\n\n";
    }else{
        cout << "You are not alloted any section as a class teacher\n\n";
    }
    cout << "You are the subject teacher for sections : ";
    for(int i = 0; i < 3; ++i) cout << sections[i] << " ";
    cout << "\n\n\n\n";
}

void insertEntryIntoSectionFile(ofstream &file) {
    string s;
    cout << "Enter details of student\n\n";
    cout << "Enter RollNo : ";
    cin >> s;
    file << s;
    file.put(':');
    cout << "Enter Name : ";
    cin >> s;
    s += ":   :   :   :   :   :";
    file << s;
    cout << "Enter attendance(in %%) : ";
    cin >> s;
    file << s;
    file.put('\n');
}

void enrollStudent() {
    string filename = string("section") + classTeacherSection + string(".txt");
    ofstream sectionFile;
    sectionFile.open(filename, ios_base::app);
    insertEntryIntoSectionFile(sectionFile);
    sectionFile.close();
    clearScreen();
    cout << "Student enrolled successfully\n\n\n";
    displayTeacher();
    loadOperations();
}

void deleteStudent() {
    string filename = string("section") + classTeacherSection + string(".txt");
    fstream file(filename);
    if (!file) {
        clearScreen();
        cout << "No data in the section file\n\n";
        displayTeacher();
        loadOperations();
        return;
    }
    string rollno;
    cout << "Deleting a student\n\n";
    cout << "Enter rollno of student: ";
    cin >> rollno;
    char ch;
    lineNumber = 0;
    vector<string> lines;
    if(isValidRollNo(file, classTeacherSection, rollno)) {
        int line = 0;
        file.seekg(ios::beg);
        while(getline(file, rollno)){
            ++line;
            if(line == lineNumber) {
                continue;
            }
            lines.push_back(rollno);
        }
        file.close();
        file.open(filename, ios::out | ios::trunc);
        for (auto & i : lines) file << i << '\n';
        clearScreen();
        cout << "Student deleted successfully.....\n\n";
        displayTeacher();
        loadOperations();
    }else{
        cout << "Invalid Rollno\n\n\n";
    }
    file.close();
}

int getMarksIndex() {
    if(subject == "Hindi") return 0;
    if(subject == "English") return 1;
    if(subject == "Maths") return 2;
    if(subject == "Science") return 3;
    if(subject == "Social") return 4;
}

void viewStudentMarks(char section) {
    clearScreen();
    cout << "RollNo\tName\t\t" << subject << " Marks\n\n";
    ifstream file(string("section") + section + string(".txt"));
    if (!file) {
        clearScreen();
        cout << "No data in the section file\n\n";
        showMarksOptions(section, 1);
        return;
    }
    char ch;
    int k = getMarksIndex();
    while (!file.eof()) {
        file.get(ch);
        if(file.eof()) break;
        file.seekg(-1, ios::cur);
        cout << left << setw(6) << readUptoColon(file) << "\t" << setw(12) << readUptoColon(file) << "\t";
        for(int i = 0; i < k; ++i) readUptoColon(file);
        cout << readUptoColon(file);
        while(file.get(ch) && ch != '\n');
        cout << "\n";
    }
    file.close();
    cout << "\n\n\nPress any key to go back...";
    cin >> ch;
    clearScreen();
    displayTeacher();
    loadOperations();
}

void enterStudentMarks(char section) {
    clearScreen();
    string rollno, marks;
    cout << "Enter rollno : ";
    cin >> rollno;
    fstream file(string("section") + section + string(".txt"));
    if (!file) {
        clearScreen();
        cout << "No data in the section file\n\n";
        showMarksOptions(section, 1);
        return;
    }
    if(isValidRollNo(file, section, rollno)) {
        cout << "Enter marks : ";
        cin >> marks;
        int k = getMarksIndex();
        for(int i = 0; i <= k; ++i) readUptoColon(file);
        int len = marks.length();
        int i, j = 0;
        for(i = 0; i < 3 - len; ++i){
            file.put(' ');
        }
        for(; i < 3; ++i) {
            file.put(marks[j++]);
        }
        cout << "Marks uploaded successfully\n\n\n";
    }else{
        cout << "Invalid Rollno\n\n\n";
    }
    file.close();
    showMarksOptions(section, 1);
}

void showMarksOptions(char section, int firstTime) {
    cout << "You are in the section " << section << " file\n\n";
    int choice;
    cout << "Select option\n";
    cout << "1. View Student Marks\n";
    cout << "2. Enter Student Marks\n";
    cout << "3. Go back to operations\n";
    cout << "\nEnter choice : ";
    cin >> choice;
    clearScreen();
    if(choice == 1) {
        viewStudentMarks(section);
    }else if(choice == 2) {
        enterStudentMarks(section);
    }else if(choice == 3){
        displayTeacher();
        loadOperations();
    }else {
        cout << "Invalid choice\n\n\n";
        showMarksOptions(section, 0);
    }
}

void enterMarks() {
    int choice;
    cout << "Select section\n";
    cout << "1. Section " << sections[0] << "\n";
    cout << "2. Section " << sections[1] << "\n";
    cout << "3. Section " << sections[2] << "\n";
    if(classTeacherSection != '_') {
        cout << "4. Section " << classTeacherSection << "\n";
    }
    cout << "0. Go back... \n";
    cout << "\nEnter choice : ";
    cin >> choice;
    clearScreen();
    if(choice == 1) {
        showMarksOptions(sections[0], 1);
    }else if(choice == 2) {
        showMarksOptions(sections[1], 1);
    }else if(choice == 3) {
        showMarksOptions(sections[2], 1);
    }else if(choice == 4 && classTeacherSection != '_') {
        showMarksOptions(classTeacherSection, 1);
    }else if(choice == 0){
        displayTeacher();
        loadOperations();
    }else {
        cout << "Enter valid choice....\n\n\n";
        enterMarks();
    }
}

void viewStudents() {
    ifstream file(string("section") + classTeacherSection + string(".txt"));
    if (!file) {
        clearScreen();
        cout << "No data in the file\n\n\n";
        return;
    }
    cout << "RollNo\tName\t\thindi\tenglish\tmaths\tscience\tsocial\tattendance\n\n";
    char ch;
    while (!file.eof()) {
        file.get(ch);
        if(file.eof()) break;
        file.seekg(-1, ios::cur);
        cout << left << setw(6) << readUptoColon(file);
        cout << "\t" << setw(12) << readUptoColon(file);
        cout << "\t" << setw(5) << readUptoColon(file);
        cout << "\t" << setw(7) << readUptoColon(file);
        cout << "\t" << setw(5) << readUptoColon(file);
        cout << "\t" << setw(7) << readUptoColon(file);
        cout << "\t" << setw(6) << readUptoColon(file) << "\t";
        while(file.get(ch) && ch != '\n') cout << ch;
        cout << "%\n";
    }
    file.close();
    cout << "\n\n\nPress any key to go back...";
    cin >> ch;
    clearScreen();
}

void loadOperations() {
    int choice;
    cout << "Select Opeartion : \n";
    cout << "1: Enroll a student\n";
    cout << "2: View Students\n";
    cout << "3: Enter Marks\n";
    cout << "4: Delete a student\n";
    cout << "5: Logout\n";
    cout << "\nEnter choice : ";
    cin >> choice;
    if(choice == 1) {
        clearScreen();
        if(classTeacherSection == '_') {
            cout << "You are not a class teacher to enroll a student\n\n";
            displayTeacher();
            loadOperations();
        }else{
            enrollStudent();
        }
    }
    else if(choice == 2) {
        clearScreen();
        if(classTeacherSection == '_') {
            cout << "You are not a class teacher to view students\n\n";
        }else{
            viewStudents();
        }
        displayTeacher();
        loadOperations();
    }
    else if(choice == 3) {
        clearScreen();
        enterMarks();
    }
    else if(choice == 4) {
        clearScreen();
        if(classTeacherSection == '_') {
            cout << "You are not a class teacher to enroll a student\n\n";
            displayTeacher();
            loadOperations();
        }else{
            deleteStudent();
        }
    }
    else if(choice == 5) {
        clearScreen();
        cout << "Logout successfull";
        return;
    }else{
        clearScreen();
        cout << "Invalid choice. Try again \n\n";
        loadOperations();
    }
}

bool login(string &username, string &password, ifstream &loginFile) {
    int cnt = 0;
    string s = "";
    char ch;
    while (!loginFile.eof()) {
        for(int i = 0; i < 6; ++i) {
            string s = readUptoColon(loginFile);
            if(i == 0 and isValidUsername(s, username)) {
                userName = s;
                ++i;
                s = readUptoColon(loginFile);
                if(isValidPassword(s, password)) {
                    setValues(loginFile);
                    return true;
                }
            }
        }
        loginFile.get(ch);
        loginFile.get(ch);
    }
    return false;
}

int main() {
    clearScreen();
    char ch;
    ifstream loginFile("logins.txt");
    if (!loginFile) {
        cout << "No Login file found. Try again";
        return 0;
    }
    string username, password;
    cout << "Login : \n";
    cout << "Enter username : ";
    cin >> username;
    cout << "Enter password : ";
    cin >> password;
    if(login(username, password, loginFile)) {
        clearScreen();
        displayTeacher();
        loadOperations();
    }
    else{
        cout << "Invalid username or password";
    }
    loginFile.close();
}
