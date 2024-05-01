#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <unordered_map>
#include <list>
#include <unordered_set>

using namespace std;

vector <vector <string>> semesterSubjects{
        /* 1 */{"English", "History", "IT", "LineAlg", "MathAn", "PE", "Physics", "ProgLang", "Russian", "VPD"},
        /* 2 */{"DiscMath", "Economics", "English", "History", "LineAlg", "MathAn", "PE", "Physics", "ProgLang"},
        /* 3 */{"AI", "DiffEq", "ElTech", "English", "Law", "LineAlg", "MathAn", "MathLog", "PE", "SocPsycho"},
        /* 4 */{"BigData", "BusModel", "ElTech", "English", "OS", "OrgEVM", "PE", "Phyl", "TeorVer", "UgrAndUya"},
        /* 5 */{"ASModel", "BioSys", "BusModel", "Crypto", "Networks", "TeorDB", "UgrAndUya"},
        /* 6 */{"BZD", "BioSys", "Crypto", "DBSafety", "PAMethods", "TechSafeMethods", "VSSafety"},
        /* 7 */{"DBSafety", "PAMethods", "PaySafety", "TeorRel"},
        /* 8 */{"ISAdmin", "InfProtection", "MakeAS", "MarkIB", "PAMethods"},
        /* 9 */{"ASSafety", "DiagCZ", "MakeAS", "ManageIB", "ProtectGos"},
};

map<string, string> subjectsInterpretations{
        pair <string, string>{"LineAlg", "�������� �������"},
        pair <string, string>{"MathAn", "�������������� ������"},
        pair <string, string>{"Physics", "������"},
        pair <string, string>{"History", "�������"},
        pair <string, string>{"IT", "�����������"},
        pair <string, string>{"Russian", "������� ����"},
        pair <string, string>{"VPD", "���"},
        pair <string, string>{"PE", "���-��"},
        pair <string, string>{"English", "����������� ����"},
        pair <string, string>{"ProgLang", "����� ����������������"},
        pair <string, string>{"Economics", "������������� ��������"},
        pair <string, string>{"DiscMath", "���������� ����������"},
        pair <string, string>{"DiffEq", "���������������� ���������"},
        pair <string, string>{"MathLog", "�������������� ������"},
        pair <string, string>{"Law", "������������"},
        pair <string, string>{"ElTech", "��������������"},
        pair <string, string>{"AI", "��"},
        pair <string, string>{"SocPsycho", "���������� ����������"},
        pair <string, string>{"Phyl", "���������"},
        pair <string, string>{"Electronics", "�����������"},
        pair <string, string>{"TeorVer", "������ ������������"},
        pair <string, string>{"OS", "��"},
        pair <string, string>{"BigData", "������� ������"},
        pair <string, string>{"OrgEVM", "����������� ���"},
        pair <string, string>{"UgrAndUya", "������ � ����������"},
        pair <string, string>{"BusModel", "������������� ������-���������"},
        pair <string, string>{"Crypto", "������������"},
        pair <string, string>{"Networks", "����"},
        pair <string, string>{"TeorDB", "������ ��� ������"},
        pair <string, string>{"BioSys", "�������������� �������"},
        pair <string, string>{"ASModel", "������������� ��"},
        pair <string, string>{"BZD", "���"},
        pair <string, string>{"PAMethods", "����������-���������� �������� ������"},
        pair <string, string>{"VSSafety", "������������ ��"},
        pair <string, string>{"DBSafety", "������������ ��� ������"},
        pair <string, string>{"TechSafeMethods", "����������� �������� ������"},
        pair <string, string>{"TeorRel", "������ ������ ���������"},
        pair <string, string>{"PaySafety", "������������ �������� ������"},
        pair <string, string>{"InfProtection", "������ ����������"},
        pair <string, string>{"ISAdmin", "����������������� ��"},
        pair <string, string>{"MarkIB", "������ ��"},
        pair <string, string>{"MakeAS", "���������� ��"},
        pair <string, string>{"ManageIB", "���������� ��"},
        pair <string, string>{"DiagCZ", "����������� ������ ������"},
        pair <string, string>{"ProtectGos", "������ ���. ��"},
        pair <string, string>{"ASSafety", "������������ ��"},
};

class Student {
public:
    string firstName;
    string lastName;
    string patronymic;
    string birthDate;
    unsigned short admissionYear;
    string faculty;
    string department;
    string group;
    string id;
    string sex;
};

class Semester {
public:
    int index;
    unordered_map<string, unordered_map<string, string>> marks;
    Semester* next;

    explicit Semester(int index) : index(index), next(nullptr) {}

    void addGrade(const string& subject, const string& studentId, const string& grade) {
        marks[subject][studentId] = grade;
    }

    void removeGrade(const string& subject, const string& studentId) {
        if (marks.count(subject) && marks[subject].count(studentId)) {
            marks[subject].erase(studentId);
            if (marks[subject].empty()) {
                marks.erase(subject);
            }
        }
    }

    string getGrade(const string& subject, const string& studentId) {
        if (marks.count(subject) && marks.at(subject).count(studentId)) {
            return marks.at(subject).at(studentId);
        } else {
            return "";
        }
    }

    bool hasGrades() const {
        return !marks.empty();
    }
};

class StudentGrades {
private:
    Semester* head;
    unordered_map<string, int> studentIdToIndex;
    unordered_map<int, unordered_set<string>> subjectsBySemester;

public:
    StudentGrades() : head(nullptr) {}

    void addSemester() {
        int index = 1;
        if (head == nullptr) {
            head = new Semester(index);
        } else {
            Semester* current = head;
            while (current->next != nullptr) {
                current = current->next;
                index++;
            }
            current->next = new Semester(index);
        }
    }

    void addStudent(const string& studentId) {
        if (studentIdToIndex.count(studentId)) {
            return;
        }
        studentIdToIndex[studentId] = studentIdToIndex.size();
    }

    void addGrade(int semesterIndex, const string& subject, const string& studentId, const string& grade) {
        Semester* current = head;
        while (current != nullptr && current->index != semesterIndex) {
            current = current->next;
        }
        if (current != nullptr) {
            bool found = false;
            for (const auto& [id, _] : current->marks[subject]) {
                if (id == studentId) {
                    current->addGrade(subject, studentId, grade);
                    found = true;
                    break;
                }
            }
            if (!found) {
                current->addGrade(subject, studentId, grade);
            }
            for (const auto& subjectInSemester : semesterSubjects[semesterIndex - 1]) {
                if (subjectsInterpretations[subjectInSemester] == subject) {
                    subjectsBySemester[semesterIndex].insert(subjectInSemester);
                    break;
                }
            }
            addStudent(studentId);
        } else {
            while (current == nullptr || current->index < semesterIndex) {
                addSemester();
                current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
            }
            current->addGrade(subject, studentId, grade);
            for (const auto& subjectInSemester : semesterSubjects[semesterIndex - 1]) {
                if (subjectsInterpretations[subjectInSemester] == subject) {
                    subjectsBySemester[semesterIndex].insert(subjectInSemester);
                    break;
                }
            }
            addStudent(studentId);
        }
    }

    void removeGrade(int semesterIndex, const string& subject, const string& studentId) {
        Semester* current = head;
        while (current != nullptr && current->index != semesterIndex) {
            current = current->next;
        }
        if (current != nullptr) {
            current->removeGrade(subject, studentId);
        } else {
            cout << "������� �� ������: " << semesterIndex << endl;
        }
    }

    string getGrade() {
        int semesterIndex;
        string subject, studentId;

        cout << "������� ������ �������� (1-9): ";
        cin >> semesterIndex;
        cin.ignore();

        cout << "������� �������� ��������: ";
        getline(cin, subject);

        cout << "������� ����� �������� ������ ��������: ";
        getline(cin, studentId);

        Semester* current = head;
        while (current != nullptr) {
            if (current->index == semesterIndex) {
                return current->getGrade(subject, studentId);
            }
            current = current->next;
        }
        cout << "������� �� ������: " << semesterIndex << endl;
        return "";
    }

    void addGradeInteractively() {
        int semesterIndex;
        string subject, studentId, grade;

        cout << "������� ������ ��������: ";
        cin >> semesterIndex;
        cin.ignore();

        if (semesterIndex < 1 || semesterIndex > 9) {
            cout << "�������� ������ ��������. ������ �������� ������ ���� �� 1 �� 9." << endl;
            return;
        }

        cout << "������� �������: ";
        getline(cin, subject);

        bool subjectFound = false;
        for (const auto& subjectInSemester : semesterSubjects[semesterIndex - 1]) {
            if (subject == subjectsInterpretations[subjectInSemester]) {
                subjectFound = true;
                subject = subjectInSemester;
                break;
            }
        }
        if (!subjectFound) {
            cout << "������� '" << subject << "' �� �������� �������������� ��� �������� " << semesterIndex << ". ����������, ���������� �����." << endl;
            return;
        }

        cout << "������� ����� �������� ������ ��������: ";
        getline(cin, studentId);

        cout << "������� ������: ";
        getline(cin, grade);

        addGrade(semesterIndex, subject, studentId, grade);
    }

    void removeGradeInteractively() {
        int semesterIndex;
        string subject, studentId;

        cout << "������� ������ ��������: ";
        cin >> semesterIndex;
        cin.ignore();

        cout << "������� �������: ";
        getline(cin, subject);

        cout << "������� ����� �������� ������ ��������: ";
        getline(cin, studentId);

        removeGrade(semesterIndex, subject, studentId);
    }

    void printGrades() {
        Semester* current = head;
        while (current != nullptr) {
            if (current->hasGrades()) {
                cout << "������ �� �������: " << current->index << endl;
                cout << endl;

                // �������� ������ ���� ��������� � ���� ��������
                vector<string> subjects;
                for (const auto& [subject, grades] : current->marks) {
                    subjects.push_back(subject);
                }

                // ��������� ������������ ������ ��������
                int maxWidth = 10;
                for (const auto& subject : subjects) {
                    maxWidth = max(maxWidth, static_cast<int>(subjectsInterpretations[subject].length()));
                }
                maxWidth = max(maxWidth, 10);

                // ������� ��������� �������
                cout << "| " << setw(10) << left << "�����";
                for (const auto& subject : subjects) {
                    cout << " | " << setw(maxWidth) << left << subjectsInterpretations[subject];
                }
                cout << endl;

                cout << string(10 + maxWidth * subjects.size() + (subjects.size() - 1) * 3, '-') << endl;

                // ������� ������ �� ���������
                for (const auto& [studentId, index] : studentIdToIndex) {
                    bool hasGrades = false;
                    for (const auto& subject : subjects) {
                        string grade = current->getGrade(subject, studentId);
                        if (!grade.empty()) {
                            hasGrades = true;
                            break;
                        }
                    }
                    if (hasGrades) {
                        cout << "| " << setw(10) << left << studentId;
                        for (const auto& subject : subjects) {
                            string grade = current->getGrade(subject, studentId);
                            if (!grade.empty()) {
                                cout << " | " << setw(maxWidth) << left << grade;
                            } else {
                                cout << " | " << setw(maxWidth) << left << " ";
                            }
                        }
                        cout << endl;
                    }
                }

                cout << string(10 + maxWidth * subjects.size() + (subjects.size() - 1) * 3, '-') << endl;
            }
            current = current->next;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            int semesterIndex;
            string subject, studentId, grade;
            while (file >> semesterIndex >> subject >> studentId >> grade) {
                addGrade(semesterIndex, subject, studentId, grade);
            }
            file.close();
        } else {
            cerr << "���������� ������� ����: " << filename << endl;
        }
    }
};

class StudentManager {
private:
    vector<Student> studentsDefault, studentsOne, studentsTwo;
    unsigned short groupOnePar;
    set<string> idSet;
    StudentGrades studentGrades;

public:
    static string getFormatDate(unsigned short day, unsigned short month, unsigned short year) {
        string dateDay = (day > 9 ? (to_string(day) + '.') : ('0' + to_string(day) + '.'));
        string dateMonth = (month > 9 ? (to_string(month) + '.') : ('0' + to_string(month) + '.'));
        string date = dateDay + dateMonth + to_string(year);
        return date;
    }

    static void saveToFile(const string& filename, vector<Student>& studentsVector) {
        ofstream file(filename, ios::binary | ios::app);

        for (const auto &student: studentsVector) {
            file << student.firstName << " " << student.lastName << " " << student.patronymic << " "
                 << student.birthDate << " " <<
                 student.admissionYear << " " << student.faculty << " " << student.department << " " << student.group <<
                 " " << student.id << " " << student.sex << endl;
        }
    }

    static void sortStudentsById(vector<Student> &studentsVector) {
        sort(studentsVector.begin(), studentsVector.end(),
             [](const Student &a, const Student &b) { return a.id < b.id; });
    }

    void readStudentsFromFile(const string& filename) {
        ifstream file(filename, ios::binary);

        while (file) {
            Student student;
            file >> student.firstName >> student.lastName >> student.patronymic >> student.birthDate >> student.admissionYear >>
                 student.faculty >> student.department >> student.group >> student.id >> student.sex;
            if (file) {
                studentsDefault.push_back(student);
                idSet.insert(student.id);
                studentGrades.addStudent(student.id);
            }
        }

        file.close();
    }

    void loadGradesFromFile(const string& filename) {
        studentGrades.loadFromFile(filename);
    }

    void splitStudents(const string& groupOneName, const string& groupTwoName) {
        vector<int> grOneDates;
        vector<int> grTwoDates;
        groupOnePar = studentsDefault.front().admissionYear;
        ifstream groupOne(groupOneName, ios::binary);
        ifstream groupTwo(groupTwoName, ios::binary);

        for (auto& student : studentsDefault) {
            if (student.admissionYear == groupOnePar) {
                studentsOne.push_back(student);
                grOneDates.push_back(stoi(student.birthDate.substr(6, 9)));
            } else {
                studentsTwo.push_back(student);
                grTwoDates.push_back(stoi(student.birthDate.substr(6, 9)));
            }
        }

        const auto [minOne, maxOne] = std::minmax_element(begin(grOneDates), end(grOneDates));
        const auto [minTwo, maxTwo] = std::minmax_element(begin(grTwoDates), end(grTwoDates));

        ofstream grOne(groupOneName, ios::binary);
        ofstream grTwo(groupTwoName, ios::binary);

        grOne << "�������� " << *minOne << "-" << *maxOne << " ���� �������� (��� ����������� - " << groupOnePar << ")" << endl;
        grTwo << "�������� " << *minTwo << "-" << *maxTwo << " ���� ��������" << endl;

        grOne.close();
        grTwo.close();

        sortStudentsById(studentsOne);
        sortStudentsById(studentsTwo);

        saveToFile(groupOneName, studentsOne);
        saveToFile(groupTwoName, studentsTwo);

        groupOne.close();
        groupTwo.close();

        studentsOne.clear();
        studentsTwo.clear();
        grOneDates.clear();
        grTwoDates.clear();
    }

    int updateStudentData(const string& firstName, const string& lastName,
                          const string& patronymic, const string& birthDate, unsigned short admissionYear, const string& faculty,
                          const string& department, const string& group, const string& id, const string& sex, const string& newId) {
        for (auto& student : studentsDefault) {
            if (student.id == id) {
                student.firstName = firstName;
                student.lastName = lastName;
                student.patronymic = patronymic;
                student.birthDate = birthDate;
                student.admissionYear = admissionYear;
                student.faculty = faculty;
                student.department = department;
                student.group = group;
                student.sex = sex;
                student.id = newId;
                idSet.insert(newId);
                idSet.erase(id);
                return 0;
            }
        }
        cout << "������ ������ �� ����������." << endl;
        return 1;
    }

    void getStudentData(const string& id) {
        for (auto& student : studentsDefault) {
            if (student.id == id) {
                cout << student.firstName << endl;
                cout << student.lastName << endl;
                cout << student.patronymic << endl;
                cout << student.birthDate << endl;
                cout << student.admissionYear << endl;
                cout << student.faculty << endl;
                cout << student.department << endl;
                cout << student.group << endl;
                cout << student.sex << endl;
                break;
            }
        }
    }

    void addStudent(const string& firstName, const string& lastName, const string& patronymic, const string& birthDate,
                    unsigned short admissionYear, const string& faculty, const string& department, const string& group,
                    const string& id, const string& sex) {
        Student newStudent = {firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex};
        studentsDefault.push_back(newStudent);
        idSet.insert(newStudent.id);
        studentGrades.addStudent(newStudent.id);
        cout << "������� ������� ��������." << endl;
    }

    void deleteStudent(const string& id) {
        studentsDefault.erase(
                remove_if(studentsDefault.begin(), studentsDefault.end(),
                          [id](const Student& student) { return student.id == id; }),
                studentsDefault.end()
        );
    }

    void showStudents() const {
        int counter = 1;
        for (const auto& student : studentsDefault) {
            cout << counter << ". ���: " << student.firstName << " " << student.lastName << " " << student.patronymic
                 << ", ���� ��������: " << student.birthDate << ", ��� �����������: " << student.admissionYear << ", ��������: "
                 << student.faculty << ", �������: " << student.department << ", ������: " << student.group
                 << ", ����� �������� ������: " << student.id << ", ���: " << student.sex << endl;
            counter += 1;
        }
    }

    void printMainMenu() {
        string tag;
        vector<string> keys = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "q"};
        cout << "����:" << endl;
        cout << "1. ��������� �� ��� ������ �� ���� ����������� � �������������" << endl;
        cout << "2. �������� ������ ��������" << endl;
        cout << "3. �������� ������ ��������" << endl;
        cout << "4. ������� ��������" << endl;
        cout << "5. �������� ������ �������� �� ������ �������� ������" << endl;
        cout << "6. ������� ���� ������ ��������� �� ������ ������" << endl;
        cout << "7. �������� ������" << endl;
        cout << "8. ������� ������" << endl;
        cout << "9. ������� ������" << endl;
        cout << "0. ������ ������ �������� �� ��������" << endl;
        cout << "q. �����" << endl;
        cout << "������� �������:";
        getline(cin, tag);
        if (tag.empty()) getline(cin, tag);
        while (find(keys.begin(), keys.end(), tag) == keys.end()) {
            cerr << "�������� ����. ���������� ��� ���." << endl;
            getline(cin, tag);
        }
        if (tag == "1") {
            splitStudents("groupOne.bin", "groupTwo.bin");
            cout << "������ ��������� ���� �������� � ��������������� �����." << endl;
            printMainMenu();
        }
        if (tag == "2") {
            unsigned short admissionYear, birthDay, birthMonth, birthYear;
            string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex, newId;
            cout << "������� ����� �������� ������ ��������:";
            cin >> id;
            if (!idSet.count(id)) {
                cout << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                cout << "������� ����� ���:";
                cin >> firstName;
                cout << "������� ����� �������:";
                cin >> lastName;
                cout << "������� ����� ��������:";
                cin >> patronymic;
                cout << "������� ����� ���� ��������:" << endl;
                cout << "����:";
                cin >> birthDay;
                cout << "�����:";
                cin >> birthMonth;
                cout << "���:";
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "������� ����� ��� �����������:";
                cin >> admissionYear;
                cout << "������� ����� ���������/��������:";
                cin >> faculty;
                cout << "������� ����� �������:";
                cin >> department;
                cout << "������� ����� ������:";
                cin >> group;
                cout << "������� ����� ���:";
                cin >> sex;
                cout << "������� ����� ����� �������� ������:";
                cin >> newId;
                updateStudentData(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex, newId);
                cout << "������ �������� ���� ���������." << endl;
            }
            printMainMenu();
        }
        if (tag == "3") {
            unsigned short admissionYear, birthDay, birthMonth, birthYear;
            string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
            cout << "������� ����� �������� ������ ��������:";
            cin >> id;
            if (idSet.count(id)) {
                cout << "����� ����� �������� ������ ��� ����������." << endl;
            } else {
                cout << "������� ��� ��������:";
                cin >> firstName;
                cout << "������� ������� ��������:";
                cin >> lastName;
                cout << "������� �������� ��������:";
                cin >> patronymic;
                cout << "������� ���� �������� ��������:" << endl;
                cout << "����:";
                cin >> birthDay;
                cout << "�����:";
                cin >> birthMonth;
                cout << "���:";
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "������� ��� ����������� ��������:";
                cin >> admissionYear;
                cout << "������� ���������/�������� ��������:";
                cin >> faculty;
                cout << "������� ������� ��������:";
                cin >> department;
                cout << "������� ������ ��������:";
                cin >> group;
                cout << "������� ��� ��������:";
                cin >> sex;
                addStudent(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                cout << "������ �������� ��� ��������." << endl;
            }
            printMainMenu();
        }
        if (tag == "4") {
            string id;
            cout << "������� ����� �������� ������ �������� ��� ��������:";
            cin >> id;
            if (!idSet.count(id)) {
                cout << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                deleteStudent(id);
                cout << "������ �������� ���� �������." << endl;
            }
            printMainMenu();
        }
        if (tag == "5") {
            string id;
            cout << "������� ����� �������� ������ �������� ��� ��������� ������:";
            cin >> id;
            if (!idSet.count(id)) {
                cout << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                getStudentData(id);
            }
            printMainMenu();
        }
        if (tag == "6") {
            showStudents();
            printMainMenu();
        }
        if (tag == "7") {
            studentGrades.addGradeInteractively();
            printMainMenu();
        }
        if (tag == "8") {
            studentGrades.removeGradeInteractively();
            printMainMenu();
        }
        if (tag == "9") {
            studentGrades.printGrades();
            printMainMenu();
        }
        if (tag == "0") {
            cout << studentGrades.getGrade() << endl;
            printMainMenu();
        }
        if (tag == "q") {
            exit(0);
        }
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    StudentManager studentManager;

    studentManager.readStudentsFromFile("students.bin");
    studentManager.loadGradesFromFile("grades.bin");
    studentManager.printMainMenu();

    return 0;
}

// TODO ����������

// TODO �������� ����� � ��������� ���� ��� ������
