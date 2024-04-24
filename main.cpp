#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <string>
#include <set>

using namespace std;

// TODO сделать проверки ввода

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
    // TODO добавить оценки
};

class StudentManager {
private:
    vector<Student> studentsDefault;
    vector<Student> studentsOne;
    vector<Student> studentsTwo;
    unsigned short groupOnePar;
    set<string> idSet;

public:
    static string getFormatDate(unsigned short day, unsigned short month, unsigned short year) {
        string dateDay = (day > 9 ? (to_string(day) + '.') : ('0' + to_string(day) + '.'));
        string dateMonth = (month > 9 ? (to_string(month) + '.') : ('0' + to_string(month) + '.'));
        string date = dateDay + dateMonth + to_string(year);
        return date;
    }

    static void saveToFile(const string& filename, vector<Student>& studentsVector) {
        ofstream file(filename, ios::binary | ios::app);

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

    void readStudentsFromFile(const string& filename) {
        ifstream file(filename, ios::binary);

        while (file) {
            Student student;
            file >> student.firstName >> student.lastName >> student.patronymic >> student.birthDate >> student.admissionYear >>
                 student.faculty >> student.department >> student.group >> student.id >> student.sex;
            if (file) {
                studentsDefault.push_back(student);
                idSet.insert(student.id);
            }
        }
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

        grOne << "Студенты " << *minOne << "-" << *maxOne << " года рождения (год поступления - " << groupOnePar << ")" << endl;
        grTwo << "Студенты " << *minTwo << "-" << *maxTwo << " года рождения" << endl;

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
        cout << "Такого номера не существует." << endl;
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
        cout << "Студент успешно добавлен." << endl;
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
            cout << counter << ". ФИО: " << student.firstName << " " << student.lastName << " " << student.patronymic
                 << ", Дата рождения: " << student.birthDate << ", Год поступления: " << student.admissionYear << ", Институт: "
                 << student.faculty << ", Кафедра: " << student.department << ", Группа: " << student.group
                 << ", Номер зачётной книжки: " << student.id << ", Пол: " << student.sex << endl;
            counter += 1;
        }
    }

    void printMainMenu() {
        string tag;
        vector<string> keys = {"1", "2", "3", "4", "5", "6", "7", "q"};
        cout << "Меню:" << endl;
        cout << "1. Разделить на две группы по году поступления и отсортировать" << endl;
        cout << "2. Обновить данные студента" << endl;
        cout << "3. Добавить нового студента" << endl;
        cout << "4. Удалить студента" << endl;
        cout << "5. Получить данные студента по номеру зачётной книжки" << endl;
        cout << "6. Вывести весь список студентов на данный момент" << endl;
        cout << "7. Выход" << endl;
        cout << "Введите команду:";
        getline(cin, tag);
        if (tag.empty()) getline(cin, tag);
        while (find(keys.begin(), keys.end(), tag) == keys.end()) {
            cerr << "Неверный ввод. Попробуйте ещё раз." << endl;
            getline(cin, tag);
        }
        if (tag == "1") {
            splitStudents("groupOne.bin", "groupTwo.bin");
            cout << "Данные студентов были записаны в соответствующие файлы." << endl;
            printMainMenu();
        }
        if (tag == "2") {
            unsigned short admissionYear, birthDay, birthMonth, birthYear;
            string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex, newId;
            cout << "Введите номер зачётной книжки студента:";
            cin >> id;
            if (!idSet.count(id)) {
                cout << "Такого номера зачётной книжки не существует." << endl;
            } else {
                cout << "Введите новое имя:";
                cin >> firstName;
                cout << "Введите новую фамилию:";
                cin >> lastName;
                cout << "Введите новое отчество:";
                cin >> patronymic;
                cout << "Введите новую дату рождения:" << endl;
                cout << "День:";
                cin >> birthDay;
                cout << "Месяц:";
                cin >> birthMonth;
                cout << "Год:";
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "Введите новый год поступления:";
                cin >> admissionYear;
                cout << "Введите новый факультет/институт:";
                cin >> faculty;
                cout << "Введите новую кафедру:";
                cin >> department;
                cout << "Введите новую группу:";
                cin >> group;
                cout << "Введите новый пол:";
                cin >> sex;
                cout << "Введите новый номер зачётной книжки:";
                cin >> newId;
                updateStudentData(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex, newId);
                cout << "Данные студента были обновлены." << endl;
            }
            printMainMenu();
        }
        if (tag == "3") {
            unsigned short admissionYear, birthDay, birthMonth, birthYear;
            string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
            cout << "Введите номер зачётной книжки студента:";
            cin >> id;
            if (idSet.count(id)) {
                cout << "Такой номер зачётной книжки уже существует." << endl;
            } else {
                cout << "Введите имя студента:";
                cin >> firstName;
                cout << "Введите фамилию студента:";
                cin >> lastName;
                cout << "Введите отчество студента:";
                cin >> patronymic;
                cout << "Введите дату рождения студента:" << endl;
                cout << "День:";
                cin >> birthDay;
                cout << "Месяц:";
                cin >> birthMonth;
                cout << "Год:";
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "Введите год поступления студента:";
                cin >> admissionYear;
                cout << "Введите факультет/институт студента:";
                cin >> faculty;
                cout << "Введите кафедру студента:";
                cin >> department;
                cout << "Введите группу студента:";
                cin >> group;
                cout << "Введите пол студента:";
                cin >> sex;
                addStudent(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                cout << "Данные студента был добавлен." << endl;
            }
            printMainMenu();
        }
        if (tag == "4") {
            string id;
            cout << "Введите номер зачётной книжки студента для удаления:";
            cin >> id;
            if (!idSet.count(id)) {
                cout << "Такого номера зачётной книжки не существует." << endl;
            } else {
                deleteStudent(id);
                cout << "Данные студента были удалены." << endl;
            }
            printMainMenu();
        }
        if (tag == "5") {
            string id;
            cout << "Введите номер зачётной книжки студента для получения данных:";
            cin >> id;
            if (!idSet.count(id)) {
                cout << "Такого номера зачётной книжки не существует." << endl;
            } else {
                getStudentData(id);
                cout << "Данные студента были получены." << endl;
            }
            printMainMenu();
        }
        if (tag == "6") {
            showStudents();
            printMainMenu();
        }
        if (tag == "7") {
            exit(0);
        }
    }

};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    StudentManager studentManager;

    studentManager.readStudentsFromFile("students.bin");
    studentManager.printMainMenu();


    return 0;
}

// TODO сделать де+шифрование
