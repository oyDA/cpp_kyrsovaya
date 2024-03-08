#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

//class Student_error:public exception{ // TODO �� ������� �������� ���������� (��������� �����)
//
//};


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
    int id; // TODO ��� ������ ������� � ���������� ��������
    string sex;
    // TODO ������ �������� ������ ��� ������
};

class StudentManager {
private:
    vector<Student> studentsDefault;
    vector<Student> studentsOne;
    vector<Student> studentsTwo;
    unsigned short groupOnePar;

public:
    static string getFormatDate(unsigned short day, unsigned short month, unsigned short year) {
        string dateDay = (day > 9 ? (to_string(day) + '.') : ('0' + to_string(day) + '.'));
        string dateMonth = (month > 9 ? (to_string(month) + '.') : ('0' + to_string(month) + '.'));
        string date = dateDay + dateMonth + to_string(year);
        return date;
    }

    static void saveToFile(const string& filename, vector<Student>& studentsVector) {
        ofstream file(filename, ios::binary);

        for (const auto& student : studentsVector) {
            file << student.firstName << " " << student.lastName << " " << student.patronymic << " " << student.birthDate << " " <<
                 student.admissionYear << " " << student.faculty << " " << student.department << " " << student.group <<
                 " " << student.id << " " << student.sex << endl;
        }

        file.close();
    }

    static void sortStudentsById(vector<Student>& studentsVector) {
        sort(studentsVector.begin(), studentsVector.end(), [](const Student& a, const Student& b) { return a.id < b.id; });
    }

    void readStudentsFromFile(const string& filename, const string& groupOneName, const string& groupTwoName) {
        ifstream file(filename, ios::binary);

        while (file) {
            Student student;
            file >> student.firstName >> student.lastName >> student.patronymic >> student.birthDate >> student.admissionYear >>
                 student.faculty >> student.department >> student.group >> student.id >> student.sex;
            if (file) {
                studentsDefault.push_back(student);
            }
        }

        file.close();

        groupOnePar = studentsDefault.front().admissionYear;
        ifstream groupOne(groupOneName, ios::binary);
        ifstream groupTwo(groupTwoName, ios::binary);

        for (auto& student : studentsDefault) {
            if (student.admissionYear == groupOnePar) {
                studentsOne.push_back(student);
            } else {
                studentsTwo.push_back(student);
            }
        }

        sortStudentsById(studentsOne);
        sortStudentsById(studentsTwo);

        saveToFile(groupOneName, studentsOne);
        saveToFile(groupTwoName, studentsTwo);

        groupOne.close();
        groupTwo.close();
    }

    int updateStudentData(const string& groupOneName, const string& groupTwoName, const string& firstName, const string& lastName,
                          const string& patronymic, const string& birthDate, unsigned short admissionYear, const string& faculty,
                          const string& department, const string& group, int id, const string& sex) {
        if (admissionYear == groupOnePar) {
            for (auto& student : studentsOne) {
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
                    saveToFile(groupOneName, studentsOne);
                    sortStudentsById(studentsOne);
                    return 0;
                }
            }
        } else {
            for (auto& student : studentsTwo) {
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
                    saveToFile(groupTwoName, studentsTwo);
                    sortStudentsById(studentsTwo);
                    return 0;
                }
            }
        }
        cout << "������ ������ �� ����������." << endl;
        return 1;
    }

    void getStudentData(const int id) { // TODO �������� ��� ������ ���� ���
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
                    unsigned short admissionYear, const string& faculty, const string& department, const string& group, int id, const string& sex) {
        Student newStudent = {firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex};
        // TODO ������� ������ ����������� id
        //      ����� ������� ������ ��������� ��� ���� id, ��� ��� �� ��������� ������ ����� ���� 1
        studentsDefault.push_back(newStudent);
        cout << "������� ������� ��������." << endl;
    }

    void deleteStudent(int id) {
        studentsDefault.erase(
                remove_if(studentsDefault.begin(), studentsDefault.end(), [id](const Student& student) { return student.id == id; }),
                studentsDefault.end()
        );
    }

    void showStudents() const {
        int counter = 1;
        for (const auto& student : studentsDefault) {
            cout << counter << ". ���: " << student.firstName << " " << student.lastName << " " << student.patronymic << ", ���� ��������: "
            << student.birthDate << ", ��� �����������: " << student.admissionYear << ", ��������: " << student.faculty << ", �������: " <<
            student.department << ", ������: " << student.group << ", ����� �������� ������: " << student.id << ", ���: " << student.sex << endl;
            counter += 1;
        }
    }

    void showMenu() {
        unsigned short choice;
        do {
            cout << "����:" << endl;
            cout << "1. �������� ������ ��������" << endl; // ��������
//            cout << "2. �������� ������ ��������" << endl; // TODO ��������
//            cout << "3. ������� ��������" << endl; // TODO ��������
//            cout << "4. �������� ������ �������� �� ������ �������� ������" << endl; // TODO ��������
//            cout << "7. ������� ���� ������ ��������� �� ������ ������" << endl; // TODO ��������
            cout << "9. �����" << endl; // �������� �������))
            cout << "������� �������:";
            cin >> choice;

            switch (choice) { // TODO �� ��� ������� �������� �������� � ����� ������ ���� ����������
                case 1: {
                    int id;
                    unsigned short admissionYear, birthDay, birthMonth, birthYear;
                    string firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "������� ����� �������� ������ ��������:";
                    cin >> id;
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
                    updateStudentData("groupOne.txt", "groupTwo.txt", firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                    break;
                }
//                case 2: {
//                    int id;
//                    unsigned short admissionYear, birthDay, birthMonth, birthYear;
//                    string firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
//                    cout << "������� ����� �������� ������ ��������:";
//                    cin >> id;
//                    cout << "������� ��� ��������:";
//                    cin >> firstName;
//                    cout << "������� ������� ��������:";
//                    cin >> lastName;
//                    cout << "������� �������� ��������:";
//                    cin >> patronymic;
//                    cout << "������� ���� �������� ��������:" << endl;
//                    cout << "����:";
//                    cin >> birthDay;
//                    cout << "�����:";
//                    cin >> birthMonth;
//                    cout << "���:";
//                    cin >> birthYear;
//                    birthDate = getFormatDate(birthDay, birthMonth, birthYear);
//                    cout << "������� ��� ����������� ��������:";
//                    cin >> admissionYear;
//                    cout << "������� ���������/�������� ��������:";
//                    cin >> faculty;
//                    cout << "������� ������� ��������:";
//                    cin >> department;
//                    cout << "������� ������ ��������:";
//                    cin >> group;
//                    cout << "������� ��� ��������:";
//                    cin >> sex;
//                    addStudent(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
//                    break;
//                }
//                case 3: {
//                    int id;
//                    cout << "������� ����� �������� ������ �������� ��� ��������:";
//                    cin >> id;
//                    deleteStudent(id);
//                    break;
//                }
//                case 4:
//                    int id;
//                    cout << "������� ����� �������� ������ �������� ��� ��������� ������:";
//                    cin >> id;
//                    getStudentData(id);
//                    cout << "������ �������� ���� ��������." << endl;
//                    break;
//                case 7:
//                    showStudents();
//                    break;
                case 9:
                    cout << "����� �� ���������." << endl;
                    break;
                default:
                    cout << "�������� ����. ���������� ��� ���." << endl;
            }
        } while (choice != 9); // TODO ��������� ���� ������ (����������� ������������)
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    StudentManager studentManager;

    studentManager.readStudentsFromFile("students.txt", "groupOne.txt", "groupTwo.txt");
    studentManager.showMenu();


    return 0;
}

// TODO �������� ����� ����

// TODO �����-�� ������� ���� ������� ��+����������
