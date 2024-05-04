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
    char firstName[51];
    char lastName[51];
    char patronymic[51];
    char birthDate[11];
    unsigned short admissionYear;
    char faculty[6];
    char department[51];
    char group[12];
    char id[11];
    char sex[20];
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

    void saveGradesToFile(const string& filename) {
        ofstream file(filename, ios::binary | ios::trunc);

        Semester* current = head;
        while (current != nullptr) {
            for (const auto& [subject, grades] : current->marks) {
                for (const auto& [studentId, grade] : grades) {
                    file.write(reinterpret_cast<const char*>(&current->index), sizeof(int));
                    file.write(subject.c_str(), subject.length() + 1);
                    file.write(studentId.c_str(), studentId.length() + 1);
                    file.write(grade.c_str(), grade.length() + 1);
                }
            }
            current = current->next;
        }

        file.close();
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
            current->addGrade(subject, studentId, grade);
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
            bool found = false;
            for (auto it = current->marks.begin(); it != current->marks.end(); ++it) {
                if (subjectsInterpretations[it->first] == subject && it->second.count(studentId)) {
                    it->second.erase(studentId);
                    if (it->second.empty()) {
                        current->marks.erase(it);
                        auto subjectIt = subjectsBySemester[semesterIndex].find(it->first);
                        if (subjectIt != subjectsBySemester[semesterIndex].end()) {
                            subjectsBySemester[semesterIndex].erase(subjectIt);
                        }
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "������ ��� �������� " << studentId << " �� �������� " << subject << " �� �������." << endl;
            }
        } else {
            cout << "������� �� ������: " << semesterIndex << endl;
        }
    }

    string getGrade() {
        int semesterIndex;
        string subject, studentId;

        cout << "������� ������ �������� (1-9): " << endl;
        cin >> semesterIndex;
        cin.ignore();

        cout << "������� �������� ��������: " << endl;
        getline(cin, subject);

        cout << "������� ����� �������� ������ ��������: " << endl;
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

    void addGradeInteractively(const string& studentId) {
        int semesterIndex;
        string subject, grade;

        cout << "������� ������ ��������: " << endl;
        cin >> semesterIndex;
        cin.ignore();

        if (semesterIndex < 1 || semesterIndex > 9) {
            cout << "�������� ������ ��������. ������ �������� ������ ���� �� 1 �� 9." << endl;
            return;
        }

        cout << "������� �������: " << endl;
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

        cout << "������� ������: " << endl;
        getline(cin, grade);

        addGrade(semesterIndex, subject, studentId, grade);
    }

    void removeGradeInteractively() {
        int semesterIndex;
        string subject, studentId;

        cout << "������� ������ ��������:" << endl;
        cin >> semesterIndex;
        cin.ignore();

        cout << "������� �������:" << endl;
        getline(cin, subject);

        cout << "������� ����� �������� ������ ��������:" << endl;
        getline(cin, studentId);

        removeGrade(semesterIndex, subject, studentId);
    }

    void printGrades() {
        Semester* current = head;
        while (current != nullptr) {
            if (current->hasGrades()) {
                cout << "\n������ �� �������: " << current->index << endl;
                cout << endl;

                // �������� ������ ���� ��������� � ���� ��������
                vector<string> subjects;
                for (auto & mark : current->marks) {
                    subjects.push_back(mark.first);
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
                cout << " |" << endl;

                // ��������� ������ ������ ������
                int firstLineWidth = 10 + maxWidth * subjects.size() + (subjects.size()) * 3;
                cout << string(firstLineWidth + 4, '-') << endl;

                // ������� ������ �� ���������
                for (auto & it : studentIdToIndex) {
                    bool hasGrades = false;
                    for (const auto& subject : subjects) {
                        string grade = current->getGrade(subject, it.first);
                        if (!grade.empty()) {
                            hasGrades = true;
                            break;
                        }
                    }
                    if (hasGrades) {
                        cout << "| " << setw(10) << left << it.first;
                        for (const auto& subject : subjects) {
                            string grade = current->getGrade(subject, it.first);
                            if (!grade.empty()) {
                                cout << " | " << setw(maxWidth) << left << grade;
                            }
                            else {
                                cout << " | " << setw(maxWidth) << left << " ";
                            }
                        }
                        cout << " |" << endl;
                    }
                }

                cout << string(firstLineWidth + 4, '-') << endl;
            }
            current = current->next;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (file.is_open()) {
            int semesterIndex;
            string subject, studentId, grade;

            while (true) {
                file.read(reinterpret_cast<char*>(&semesterIndex), sizeof(int));
                if (file.eof()) break;

                getline(file, subject, '\0');
                getline(file, studentId, '\0');
                getline(file, grade, '\0');

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

    static int saveToFile(const std::string& filename, std::vector<Student>& studentsVector, bool checker) {
        if (checker == 1) {
            std::ofstream file(filename, ios::binary | ios::trunc);

            for (const auto& student : studentsVector) {
                file.write(reinterpret_cast<const char*>(&student), sizeof(Student));
            }

            file.close();
            return 0;
        } else if (checker == 0) {
            std::ofstream file(filename, ios::binary | ios::app);

            for (const auto& student : studentsVector) {
                file.write(reinterpret_cast<const char*>(&student), sizeof(Student));
            }

            file.close();
            return 0;
        }
        return 1;
    }

    static bool compareStudentsById(const Student& a, const Student& b) {
        std::string aId(a.id, 12);
        std::string bId(b.id, 12);
        return aId < bId;
    }

    static void sortStudentsById(vector<Student>& studentsVector) {
        std::sort(studentsVector.begin(), studentsVector.end(), compareStudentsById);
    }

    int readStudentsFromFile(const string& filename) {
        ifstream file(filename, ios::binary);

        if (!file.is_open()) {
            std::cerr << "�� ������� ������� ���� students.bin" << std::endl;
            return 1;
        }

        Student student;

        while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
            studentsDefault.push_back(student);
            idSet.insert(student.id);
            studentGrades.addStudent(student.id);
        }

        file.close();
        return 0;
    }

    void loadGradesFromFile(const string& filename) {
        studentGrades.loadFromFile(filename);
    }

    void splitStudents(const string& groupOneName, const string& groupTwoName) {
        vector<int> grOneDates;
        vector<int> grTwoDates;
        groupOnePar = studentsDefault.front().admissionYear;
        ofstream grOne(groupOneName, ios::binary | ios::trunc);
        ofstream grTwo(groupTwoName, ios::binary | ios::trunc);

        for (auto& student : studentsDefault) {
            if (student.admissionYear == groupOnePar) {
                studentsOne.push_back(student);
                grOneDates.push_back(atoi(student.birthDate + 6));
            } else {
                studentsTwo.push_back(student);
                grTwoDates.push_back(atoi(student.birthDate + 6));
            }
        }

        auto minmaxOne = std::minmax_element(begin(grOneDates), end(grOneDates));
        auto minOne = *minmaxOne.first;
        auto maxOne = *minmaxOne.second;

        auto minmaxTwo = std::minmax_element(begin(grTwoDates), end(grTwoDates));
        auto minTwo = *minmaxTwo.first;
        auto maxTwo = *minmaxTwo.second;

        grOne << "�������� " << minOne << "-" << maxOne << " ���� �������� (��� ����������� - " << groupOnePar << ")\n";
        grTwo << "�������� " << minTwo << "-" << maxTwo << " ���� ��������\n";

        sortStudentsById(studentsOne);
        sortStudentsById(studentsTwo);

        saveToFile(groupOneName, studentsOne, 0);
        saveToFile(groupTwoName, studentsTwo, 0);

        grOne.close();
        grTwo.close();

        cout << "\n������ ������ � " << groupOnePar << " ����� �����������:\n\n";
        showStudents(studentsOne);
        cout << "\n������ ������:\n\n";
        showStudents(studentsTwo);

        studentsOne.clear();
        studentsTwo.clear();
        grOneDates.clear();
        grTwoDates.clear();
    }

    int updateStudentData(const string& firstName, const string& lastName,
                                          const string& patronymic, const string& birthDate, unsigned short admissionYear, const string& faculty,
                                          const string& department, const string& group, const string& id, const string& sex, const string& newId) {
        for (auto& student : studentsDefault) {
            if (strcmp(student.id, id.c_str()) == 0) {
                strncpy(student.firstName, firstName.c_str(), sizeof(student.firstName) - 1);
                strncpy(student.lastName, lastName.c_str(), sizeof(student.lastName) - 1);
                strncpy(student.patronymic, patronymic.c_str(), sizeof(student.patronymic) - 1);
                strncpy(student.birthDate, birthDate.c_str(), sizeof(student.birthDate) - 1);
                student.admissionYear = admissionYear;
                strncpy(student.faculty, faculty.c_str(), sizeof(student.faculty) - 1);
                strncpy(student.department, department.c_str(), sizeof(student.department) - 1);
                strncpy(student.group, group.c_str(), sizeof(student.group) - 1);
                strncpy(student.sex, sex.c_str(), sizeof(student.sex) - 1);
                strncpy(student.id, newId.c_str(), sizeof(student.id) - 1);
                if (newId != id) {
                    idSet.insert(newId);
                    idSet.erase(id);
                }
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
        Student newStudent;
        strncpy(newStudent.firstName, firstName.c_str(), sizeof(newStudent.firstName) - 1);
        strncpy(newStudent.lastName, lastName.c_str(), sizeof(newStudent.lastName) - 1);
        strncpy(newStudent.patronymic, patronymic.c_str(), sizeof(newStudent.patronymic) - 1);
        strncpy(newStudent.birthDate, birthDate.c_str(), sizeof(newStudent.birthDate) - 1);
        newStudent.admissionYear = admissionYear;
        strncpy(newStudent.faculty, faculty.c_str(), sizeof(newStudent.faculty) - 1);
        strncpy(newStudent.department, department.c_str(), sizeof(newStudent.department) - 1);
        strncpy(newStudent.group, group.c_str(), sizeof(newStudent.group) - 1);
        strncpy(newStudent.id, id.c_str(), sizeof(newStudent.id) - 1);
        strncpy(newStudent.sex, sex.c_str(), sizeof(newStudent.sex) - 1);
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
        idSet.erase(id);
    }

    void showStudents(const vector<Student>& vectorStudent) const {
        // �������� ������ ���� ���������� ��������
        vector<string> parameters = {"���", "�������", "��������", "���� ��������", "��� �����������", "��������", "�������", "������", "�����", "���"};

        // ��������� ������������ ������ ��������
        int maxWidth = 15;
        for (const auto& param : parameters) {
            maxWidth = max(maxWidth, static_cast<int>(param.length()));
        }
        maxWidth = min(maxWidth, 20);

        // ������� ������ �� ��������� ���������, �� 10 ��������� � ������ ������
        for (size_t i = 0; i < vectorStudent.size(); i += 10) {
            // ������� ��������� �������
            for (const auto& param : parameters) {
                cout << "| " << setw(maxWidth) << left << param;
            }
            cout << "|" << endl;

            int totalWidth = maxWidth * parameters.size() + (parameters.size() - 1) * 3 + 1; // ����� ������ �������
            cout << string(totalWidth - 7, '-') << endl;

            // ������� ������ �� ���������
            for (size_t j = i; j < min(i + 10, vectorStudent.size()); ++j) {
                const auto& student = vectorStudent[j];
                for (const auto& param : parameters) {
                    if (param == "���") {
                        cout << "| " << setw(maxWidth) << left << student.firstName;
                    } else if (param == "�������") {
                        cout << "| " << setw(maxWidth) << left << student.lastName;
                    } else if (param == "��������") {
                        cout << "| " << setw(maxWidth) << left << student.patronymic;
                    } else if (param == "���� ��������") {
                        cout << "| " << setw(maxWidth) << left << student.birthDate;
                    } else if (param == "��� �����������") {
                        cout << "| " << setw(maxWidth) << left << student.admissionYear;
                    } else if (param == "��������") {
                        cout << "| " << setw(maxWidth) << left << student.faculty;
                    } else if (param == "�������") {
                        cout << "| " << setw(maxWidth) << left << student.department;
                    } else if (param == "������") {
                        cout << "| " << setw(maxWidth) << left << student.group;
                    } else if (param == "�����") {
                        cout << "| " << setw(maxWidth) << left << student.id;
                    } else if (param == "���") {
                        cout << "| " << setw(maxWidth) << left << student.sex;
                    }
                }
                cout << "|" << endl;
            }

            cout << string(totalWidth - 7, '-') << endl;
            cout << endl; // ��������� ������ ������ ����� �������
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
            cout << "������� ����� �������� ������ ��������:" << endl;
            cin >> id;
            if (!idSet.count(id)) {
                cerr << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                cout << "������� ����� ���:" << endl;
                cin >> firstName;
                cout << "������� ����� �������:" << endl;
                cin >> lastName;
                cout << "������� ����� ��������:" << endl;
                cin >> patronymic;
                cout << "������� ����� ���� ��������:" << endl;
                cout << "����:" << endl;
                cin >> birthDay;
                cout << "�����:" << endl;
                cin >> birthMonth;
                cout << "���:" << endl;
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "������� ����� ��� �����������:" << endl;
                cin >> admissionYear;
                cout << "������� ����� ���������/��������:" << endl;
                cin >> faculty;
                cout << "������� ����� �������:" << endl;
                cin >> department;
                cout << "������� ����� ������:" << endl;
                cin >> group;
                cout << "������� ����� ���:" << endl;
                cin >> sex;
                cout << "������� ����� ����� �������� ������:" << endl;
                cin >> newId;
                updateStudentData(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex, newId);
                cout << "������ �������� ���� ���������." << endl;
            }
            printMainMenu();
        }
        if (tag == "3") {
            unsigned short admissionYear, birthDay, birthMonth, birthYear;
            string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
            cout << "������� ����� �������� ������ ��������:" << endl;
            cin >> id;
            if (idSet.count(id)) {
                cerr << "����� ����� �������� ������ ��� ����������." << endl;
            } else {
                cout << "������� ��� ��������:" << endl;
                cin >> firstName;
                cout << "������� ������� ��������:" << endl;
                cin >> lastName;
                cout << "������� �������� ��������:" << endl;
                cin >> patronymic;
                cout << "������� ���� �������� ��������:" << endl;
                cout << "����:" << endl;
                cin >> birthDay;
                cout << "�����:" << endl;
                cin >> birthMonth;
                cout << "���:" << endl;
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "������� ��� ����������� ��������:" << endl;
                cin >> admissionYear;
                cout << "������� ���������/�������� ��������:" << endl;
                cin >> faculty;
                cout << "������� ������� ��������:" << endl;
                cin >> department;
                cout << "������� ������ ��������:" << endl;
                cin >> group;
                cout << "������� ��� ��������:" << endl;
                cin >> sex;
                addStudent(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                cout << "������ �������� ���� ���������." << endl;
            }
            printMainMenu();
        }
        if (tag == "4") {
            string id;
            cout << "������� ����� �������� ������ �������� ��� ��������:" << endl;
            cin >> id;
            if (!idSet.count(id)) {
                cerr << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                deleteStudent(id);
                cout << "������ �������� ���� �������." << endl;
            }
            printMainMenu();
        }
        if (tag == "5") {
            string id;
            cout << "������� ����� �������� ������ �������� ��� ��������� ������:" << endl;
            cin >> id;
            if (!idSet.count(id)) {
                cerr << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                getStudentData(id);
            }
            printMainMenu();
        }
        if (tag == "6") {
            showStudents(studentsDefault);
            printMainMenu();
        }
        if (tag == "7") {
            string id;
            cout << "������� ����� �������� ������:" << endl;
            getline(cin, id);
            if (!idSet.count(id)) {
                cerr << "������ ������ �������� ������ �� ����������." << endl;
            } else {
                studentGrades.addGradeInteractively(id);
            }
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
            saveToFile("new_students.bin", studentsDefault, 1);
            studentGrades.saveGradesToFile("new_grades.bin");
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
