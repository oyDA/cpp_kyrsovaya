#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

//class Student_error:public exception{ // TODO ну понятно запилить исключения (миссклики сасат)
//
//};

class Student {
public:
    string firstName;
    string lastName;
    string patronymic; // TODO а вдруг у типули нет отчества?
    string birthDate;
    int admissionYear;
    string faculty;
    string department;
    string group;
    int id;
    string sex;
//    сессия предметы там
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

    // TODO реализовать функцию следующего содержания: (Разбить группу на 2 части, с указанием интервала года рождения:
    //                                                      1) студентов, поступивших в ВУЗ в одном и том же году;
    //                                                      2) студентов, поступивших в ВУЗ в др. годы, отличные от части 1.
    //                                                  Отсортировать каждую часть по номеру зачетных книжек.)

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

    void getStudentData(int id) { // TODO выводить еще оценки надо ему
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
        // TODO пресечь случаи одинакового id
        //      можно сделать список отдельный для всех id, как раз по критериям список нужен хоть 1
        students.push_back(newStudent);
        cout << "Студент успешно добавлен." << endl;
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
            cout << counter << ". ФИО: " << student.firstName << " " << student.lastName << " " << student.patronymic << ", Дата рождения: "
            << student.birthDate << ", Год поступления: " << student.admissionYear << ", Институт: " << student.faculty << ", Кафедра: " <<
            student.department << ", Группа: " << student.group << ", Номер зачётной книжки: " << student.id << ", Пол: " << student.sex << endl;
            counter += 1;
        }
    }

    void showMenu() {
        int choice;
        do {
            cout << "Меню:" << endl;
            cout << "1. Обновить данные студента" << endl;
            cout << "2. Добавить нового студента" << endl;
            cout << "3. Удалить студента" << endl;
            cout << "4. Отсортировать студентов по номерам зачётных книжек" << endl;
            cout << "5. Сохранить студентов в новый файл" << endl;
            cout << "6. Получить данные студента по номеру зачётной книжки" << endl;
            cout << "7. Вывести весь список студентов на данный момент" << endl;
            cout << "8. Сохранить изменения в исходный файл" << endl;
            cout << "9. Выход" << endl;
            cout << "Введите команду:";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int id, admissionYear;
                    string firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "Введите номер зачётной книжки студента:";
                    cin >> id;
                    cout << "Введите новое имя:";
                    cin >> firstName;
                    cout << "Введите новую фамилию:";
                    cin >> lastName;
                    cout << "Введите новое отчество:";
                    cin >> patronymic;
                    cout << "Введите новую дату рождения:";
                    cin >> birthDate;
                    cout << "Введите новый год поступления:";
                    cin >> admissionYear;
                    cout << "Введите новый факультет:";
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
                case 2: {
                    int id, admissionYear;
                    string firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
                    cout << "Введите номер зачётной книжки студента:";
                    cin >> id;
                    cout << "Введите имя студента:";
                    cin >> firstName;
                    cout << "Введите фамилию студента:";
                    cin >> lastName;
                    cout << "Введите отчество студента:";
                    cin >> patronymic;
                    cout << "Введите дату рождения студента:";
                    cin >> birthDate;
                    cout << "Введите год поступления студента:";
                    cin >> admissionYear;
                    cout << "Введите факультет студента:";
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
                case 3: {
                    int id;
                    cout << "Введите номер зачётной книжки студента для удаления:";
                    cin >> id;
                    deleteStudent(id);
                    break;
                }
                case 4:
                    sortStudentsById();
                    break;
                case 5:
                    saveToFile("sorted_students.txt");
                    cout << "Студенты сохранены в файл." << endl;
                    break;
                case 6:
                    int id;
                    cout << "Введите номер зачётной книжки студента для получения данных:";
                    cin >> id;
                    getStudentData(id);
                    cout << "Данные студента были получены." << endl;
                    break;
                case 7:
                    showStudents();
                    break;
                case 8:
                    saveToFile("students.txt");
                    cout << "Исходный файл был успешно перезаписан." << endl;
                    break;
                case 9:
                    cout << "Выход из программы." << endl;
                    break;
                default:
                    cout << "Неверный ввод. Попробуйте ещё раз." << endl;
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

// TODO бинарные файлы ыыыы

// TODO каким-то макаром надо сделать де+шифрование