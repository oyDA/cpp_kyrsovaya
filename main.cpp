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

vector <vector <string>> TermSubjects{
        {"Eng", "History", "IT", "LineAlg", "MathAn", "PE", "Physics", "ProgLang", "Russian", "VPD"},
        {"DiscMath", "Economics", "Eng", "History", "LineAlg", "MathAn", "PE", "Physics", "ProgLang"},
        {"AI", "DiffEq", "ElTech", "Eng", "Law", "LineAlg", "MathAn", "MathLog", "PE", "SocPsycho"},
        {"BigData", "BusModel", "ElTech", "Eng", "OS", "OrgEVM", "PE", "Phyl", "TeorVer", "UgrAndUya"},
        {"ASModel", "BioSys", "BusModel", "Crypto", "Networks", "TeorDB", "UgrAndUya"}, //5 сем
        {"BZD", "BioSys", "Crypto", "DBSafety", "PAMethods", "TechSafeMethods", "VSSafety"},
        {"DBSafety", "PAMethods", "PaySafety", "TeorRel"},
        {"ISAdmin", "InfProtection", "MakeAS", "MarkIB", "PAMethods"},
        {"ASSafety", "DiagCZ", "MakeAS", "ManageIB", "ProtectGos"},
};

map<string, string> subjectsInterpretations{
        pair <string, string>{"LineAlg", "Линейная алгебра"},
        pair <string, string>{"MathAn", "Математический анализ"},
        pair <string, string>{"Physics", "Физика"},
        pair <string, string>{"History", "История"},
        pair <string, string>{"IT", "Информатика"},
        pair <string, string>{"Russian", "Русский язык"},
        pair <string, string>{"VPD", "ВПД"},
        pair <string, string>{"PE", "Физ-ра"},
        pair <string, string>{"Eng", "Иностранный язык"},
        pair <string, string>{"ProgLang", "Языки программирования"},
        pair <string, string>{"Economics", "Экономическая культура"},
        pair <string, string>{"DiscMath", "Дискретная математика"},
        pair <string, string>{"DiffEq", "Дифференциальные уравнения"},
        pair <string, string>{"MathLog", "Математическая логика"},
        pair <string, string>{"Law", "Правоведение"},
        pair <string, string>{"ElTech", "Электротехника"},
        pair <string, string>{"AI", "ИИ"},
        pair <string, string>{"SocPsycho", "Социальная психология"},
        pair <string, string>{"Phyl", "Философия"},
        pair <string, string>{"Electronics", "Электроника"},
        pair <string, string>{"TeorVer", "Теория вероятностей"},
        pair <string, string>{"OS", "ОС"},
        pair <string, string>{"BigData", "Большие данные"},
        pair <string, string>{"OrgEVM", "Организация ЭВМ"},
        pair <string, string>{"UgrAndUya", "Угрозы и уязвимости"},
        pair <string, string>{"BusModel", "Моделирование бизнес-процессов"},
        pair <string, string>{"Crypto", "Криптография"},
        pair <string, string>{"Networks", "Сети"},
        pair <string, string>{"TeorDB", "Теория баз данных"},
        pair <string, string>{"BioSys", "Биометрические системы"},
        pair <string, string>{"ASModel", "Моделирование АС"},
        pair <string, string>{"BZD", "БЖД"},
        pair <string, string>{"PAMethods", "Программно-аппаратные средства защиты"},
        pair <string, string>{"VSSafety", "Безопасность ВС"},
        pair <string, string>{"DBSafety", "Безопасность баз данных"},
        pair <string, string>{"TechSafeMethods", "Технические средства защиты"},
        pair <string, string>{"TeorRel", "Основы теории надёжности"},
        pair <string, string>{"PaySafety", "Безопасность платёжных систем"},
        pair <string, string>{"InfProtection", "Защита информации"},
        pair <string, string>{"ISAdmin", "Администрирование ИС"},
        pair <string, string>{"MarkIB", "Оценка ИБ"},
        pair <string, string>{"MakeAS", "Разработка АС"},
        pair <string, string>{"ManageIB", "Управление ИБ"},
        pair <string, string>{"DiagCZ", "Диагностика систем защиты"},
        pair <string, string>{"ProtectGos", "Защита Гос. ИС"},
        pair <string, string>{"ASSafety", "Безопасность АС"},
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

    Semester(int index) : index(index), next(nullptr) {}

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

    bool hasGrades() {
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
            subjectsBySemester[semesterIndex].insert(subject);
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
            subjectsBySemester[semesterIndex].insert(subject);
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
            cout << "Semester not found: " << semesterIndex << endl;
        }
    }

    string getGrade() { // TODO добавить тег и типа узнать определенную оценку
        int semesterIndex;
        string subject, studentId;

        cout << "Enter semester index: ";
        cin >> semesterIndex;
        cin.ignore(); // Ignore newline character

        cout << "Enter subject: ";
        getline(cin, subject);

        cout << "Enter student ID: ";
        getline(cin, studentId);

        Semester* current = head;
        while (current != nullptr) {
            if (current->index == semesterIndex) {
                return current->getGrade(subject, studentId);
            }
            current = current->next;
        }
        cout << "Semester not found: " << semesterIndex << endl;
        return "";
    }

    void addGradeInteractively() {
        int semesterIndex;
        string subject, studentId, grade;

        cout << "Enter semester index: ";
        cin >> semesterIndex;
        cin.ignore(); // Ignore newline character

        // Check if the semester index is valid
        if (semesterIndex < 1 || semesterIndex > 9) {
            cout << "Invalid semester index. Semester index must be between 1 and 9." << endl;
            return;
        }

        // Check if the subject is valid for the selected semester
        bool subjectFound = false;
        for (const auto& subjectInSemester : TermSubjects[semesterIndex - 1]) {
            if (subjectInSemester == subject) {
                subjectFound = true;
                break;
            }
        }
        if (!subjectFound) {
            cout << "Subject '" << subject << "' is not valid for semester " << semesterIndex << ". Please try again." << endl;
            return;
        }

        cout << "Enter subject: ";
        getline(cin, subject);

        cout << "Enter student ID: ";
        getline(cin, studentId);

        cout << "Enter grade: ";
        getline(cin, grade);

        addGrade(semesterIndex, subject, studentId, grade);
    }

    void removeGradeInteractively() {
        int semesterIndex;
        string subject, studentId;

        cout << "Enter semester index: ";
        cin >> semesterIndex;
        cin.ignore(); // Ignore newline character

        cout << "Enter subject: ";
        getline(cin, subject);

        cout << "Enter student ID: ";
        getline(cin, studentId);

        removeGrade(semesterIndex, subject, studentId);
    }

    void printGrades() {
        Semester* current = head;
        while (current != nullptr) {
            if (current->hasGrades()) {
                cout << "Grades for semester: " << current->index << endl;
                cout << string(12, '-') << string(12 * subjectsBySemester[current->index].size(), '-') << endl;

                // Print the header row
                cout << setw(12) << "Student ID";
                for (const auto& subject : subjectsBySemester[current->index]) {
                    cout << setw(12) << subject;
                }
                cout << endl;

                // Print the separator row
                cout << setw(12) << string(12, '-');
                for (size_t i = 0; i < subjectsBySemester[current->index].size(); i++) {
                    cout << setw(12) << string(12, '-');
                }
                cout << endl;

                // Print the grades
                bool hasGrades = false;
                for (const auto& [studentId, index] : studentIdToIndex) {
                    bool hasStudentGrades = false;
                    for (const auto& subject : subjectsBySemester[current->index]) {
                        string grade = current->getGrade(subject, studentId);
                        if (!grade.empty()) {
                            hasStudentGrades = true;
                            hasGrades = true;
                            break;
                        }
                    }
                    if (hasStudentGrades) {
                        cout << setw(12) << studentId;
                        for (const auto& subject : subjectsBySemester[current->index]) {
                            string grade = current->getGrade(subject, studentId);
                            if (!grade.empty()) {
                                cout << setw(12) << grade;
                            } else {
                                cout << setw(12) << "N/A";
                            }
                        }
                        cout << endl;
                    }
                }
                // Print the separator row
                cout << string(12, '-') << string(12 * subjectsBySemester[current->index].size(), '-') << endl;
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
            cerr << "Unable to open file: " << filename << endl;
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
    void createGrades() {
        studentGrades.addSemester();
    }

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
        studentGrades.addStudent(newStudent.id);
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
        vector<string> keys = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
        cout << "Меню:" << endl;
        cout << "1. Разделить на две группы по году поступления и отсортировать" << endl;
        cout << "2. Обновить данные студента" << endl;
        cout << "3. Добавить нового студента" << endl;
        cout << "4. Удалить студента" << endl;
        cout << "5. Получить данные студента по номеру зачётной книжки" << endl;
        cout << "6. Вывести весь список студентов на данный момент" << endl;
        cout << "7. Добавить оценку" << endl;
        cout << "8. Удалить оценку" << endl;
        cout << "9. Вывести оценки" << endl;
        cout << "0. Выход" << endl;
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
        if (tag == "5") { // TODO пофиксить
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
            studentGrades.addGradeInteractively();
            cout << "Оценка была добавлена." << endl;
            printMainMenu();
        }
        if (tag == "8") {
            studentGrades.removeGradeInteractively();
            cout << "Оценка была удалена." << endl;
            printMainMenu();
        }
        if (tag == "9") {
            studentGrades.printGrades();
            printMainMenu();
        }
        if (tag == "0") {
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

// TODO доделать проверки ввода