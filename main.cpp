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
    string patronymic; // TODO � ����� � ������ ��� ��������?
    string birthDate;
    int admissionYear;
    string faculty;
    string department;
    string group;
    int id;
    string sex;
//    ������ �������� ���
};

class StudentManager {
private:
    vector<Student> students;

public:
    void readStudentsFromFile(const string& filename) {
        ifstream file(filename, ios::binary);

        while (file) {
            Student student;
            file >> student.firstName >> student.lastName >> student.patronymic >> student.birthDate >> student.admissionYear >>
                 student.faculty >> student.department >> student.group >> student.id >> student.sex;
            if (file) {
                students.push_back(student);
            }
        }

        file.close();
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename, ios::binary);

        for (const auto& student : students) {
            file << student.firstName << " " << student.lastName << " " << student.patronymic << " " << student.birthDate << " " <<
                 student.admissionYear << " " << student.faculty << " " << student.department << " " << student.group <<
                 " " << student.id << " " << student.sex << endl;
        }

        file.close();
    }

    // TODO ����������� ������� ���������� ����������: (������� ������ �� 2 �����, � ��������� ��������� ���� ��������:
    //                                                      1) ���������, ����������� � ��� � ����� � ��� �� ����;
    //                                                      2) ���������, ����������� � ��� � ��. ����, �������� �� ����� 1.
    //                                                  ������������� ������ ����� �� ������ �������� ������.)

    void updateStudentData(const string& firstName, const string& lastName, const string& patronymic, const string& birthDate,
                           int admissionYear, const string& faculty, const string& department, const string& group, int id, const string& sex) {
        for (auto& student : students) {
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
                break;
            }
        }
    }

    void getStudentData(int id) { // TODO �������� ��� ������ ���� ���
        for (auto& student : students) {
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
                    int admissionYear, const string& faculty, const string& department, const string& group, int id, const string& sex) {
        Student newStudent = {firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex};
        // TODO ������� ������ ����������� id
        //      ����� ������� ������ ��������� ��� ���� id, ��� ��� �� ��������� ������ ����� ���� 1
        students.push_back(newStudent);
        cout << "������� ������� ��������." << endl;
    }

    void deleteStudent(int id) {
        students.erase(
                remove_if(students.begin(), students.end(), [id](const Student& student) { return student.id == id; }),
                students.end()
        );
    }

    void sortStudentsById() {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) { return a.id < b.id; });
    }

    void showStudents() const {
        int counter = 1;
        for (const auto& student : students) {
            cout << counter << ". ���: " << student.firstName << " " << student.lastName << " " << student.patronymic << ", ���� ��������: "
            << student.birthDate << ", ��� �����������: " << student.admissionYear << ", ��������: " << student.faculty << ", �������: " <<
            student.department << ", ������: " << student.group << ", ����� �������� ������: " << student.id << ", ���: " << student.sex << endl;
            counter += 1;
        }
    }

    void showMenu() {
        int choice;
        do {
            cout << "����:" << endl;
            cout << "1. �������� ������ ��������" << endl;
            cout << "2. �������� ������ ��������" << endl;
            cout << "3. ������� ��������" << endl;
            cout << "4. ������������� ��������� �� ������� �������� ������" << endl;
            cout << "5. ��������� ��������� � ����� ����" << endl;
            cout << "6. �������� ������ �������� �� ������ �������� ������" << endl;
            cout << "7. ������� ���� ������ ��������� �� ������ ������" << endl;
            cout << "8. ��������� ��������� � �������� ����" << endl;
            cout << "9. �����" << endl;
            cout << "������� �������:";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int id, admissionYear;
                    string firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "������� ����� �������� ������ ��������:";
                    cin >> id;
                    cout << "������� ����� ���:";
                    cin >> firstName;
                    cout << "������� ����� �������:";
                    cin >> lastName;
                    cout << "������� ����� ��������:";
                    cin >> patronymic;
                    cout << "������� ����� ���� ��������:";
                    cin >> birthDate;
                    cout << "������� ����� ��� �����������:";
                    cin >> admissionYear;
                    cout << "������� ����� ���������:";
                    cin >> faculty;
                    cout << "������� ����� �������:";
                    cin >> department;
                    cout << "������� ����� ������:";
                    cin >> group;
                    cout << "������� ����� ���:";
                    cin >> sex;
                    updateStudentData(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                    break;
                }
                case 2: {
                    int id, admissionYear;
                    string firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "������� ����� �������� ������ ��������:";
                    cin >> id;
                    cout << "������� ��� ��������:";
                    cin >> firstName;
                    cout << "������� ������� ��������:";
                    cin >> lastName;
                    cout << "������� �������� ��������:";
                    cin >> patronymic;
                    cout << "������� ���� �������� ��������:";
                    cin >> birthDate;
                    cout << "������� ��� ����������� ��������:";
                    cin >> admissionYear;
                    cout << "������� ��������� ��������:";
                    cin >> faculty;
                    cout << "������� ������� ��������:";
                    cin >> department;
                    cout << "������� ������ ��������:";
                    cin >> group;
                    cout << "������� ��� ��������:";
                    cin >> sex;
                    addStudent(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                    break;
                }
                case 3: {
                    int id;
                    cout << "������� ����� �������� ������ �������� ��� ��������:";
                    cin >> id;
                    deleteStudent(id);
                    break;
                }
                case 4:
                    sortStudentsById();
                    break;
                case 5:
                    saveToFile("sorted_students.txt");
                    cout << "�������� ��������� � ����." << endl;
                    break;
                case 6:
                    int id;
                    cout << "������� ����� �������� ������ �������� ��� ��������� ������:";
                    cin >> id;
                    getStudentData(id);
                    cout << "������ �������� ���� ��������." << endl;
                    break;
                case 7:
                    showStudents();
                    break;
                case 8:
                    saveToFile("students.txt");
                    cout << "�������� ���� ��� ������� �����������." << endl;
                    break;
                case 9:
                    cout << "����� �� ���������." << endl;
                    break;
                default:
                    cout << "�������� ����. ���������� ��� ���." << endl;
            }
        } while (choice != 9);
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    StudentManager studentManager;

    studentManager.readStudentsFromFile("students.txt");
    studentManager.showMenu();

    return 0;
}

// TODO �������� ����� ����

// TODO �����-�� ������� ���� ������� ��+����������