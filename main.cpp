#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <string>
#include <limits>

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
    // TODO добавить сессию+оценки
};

class StudentManager {
private:
    vector<Student> studentsDefault;
    vector<Student> studentsOne;
    vector<Student> studentsTwo;
    unsigned short groupOnePar, count;

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
            }
        }

        count = size(studentsDefault);

        file.close();
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
        const auto [minTwo, maxTwo] = std::minmax_element(begin(grOneDates), end(grOneDates));

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
                          const string& department, const string& group, const string& id, const string& sex) {
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
        // TODO пресечь случаи одинакового id
        studentsDefault.push_back(newStudent);
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

    void showMenu() {
        int choice;
        do {
            cout << "Меню:" << endl;
            cout << "1. Разделить на две группы по году поступления и отсортировать" << endl;
            cout << "2. Обновить данные студента" << endl; // РАБОТАЕТ
            cout << "3. Добавить нового студента" << endl;
            cout << "4. Удалить студента" << endl;
            cout << "5. Получить данные студента по номеру зачётной книжки" << endl;
            cout << "6. Вывести весь список студентов на данный момент" << endl;
            cout << "7. Выход" << endl;
            cout << "Введите команду:";
            cin >> choice;

            switch (choice) {
                case 1: {
                    splitStudents("groupOne.txt", "groupTwo.txt");
                    break;
                }
                case 2: {
                    unsigned short admissionYear, birthDay, birthMonth, birthYear;
                    string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "Введите номер зачётной книжки студента:";
                    cin >> id; // TODO если такого id нет -> break
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
                    updateStudentData(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                    break;
                }
                case 3: {
                    unsigned short admissionYear, birthDay, birthMonth, birthYear;
                    string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "Введите номер зачётной книжки студента:";
                    cin >> id; // TODO если такое id есть -> break
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
                    break;
                }
                case 4: {
                    string id;
                    cout << "Введите номер зачётной книжки студента для удаления:";
                    cin >> id;
                    deleteStudent(id);
                    break;
                }
                case 5: {
                    string id;
                    cout << "Введите номер зачётной книжки студента для получения данных:";
                    cin >> id;
                    getStudentData(id);
                    cout << "Данные студента были получены." << endl;
                }
                    break;
                case 6:
                    showStudents();
                    break;
                case 7:
                    cout << "Выход из программы." << endl;
                    break;
                default:
                    cout << "Неверный ввод. Попробуйте ещё раз." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
            }
        } while (choice != 7);
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

// TODO бинарные файлы

// TODO сделать де+шифрование

// TODO у каждого кейса подписать успешное/неудачное выполнение действия