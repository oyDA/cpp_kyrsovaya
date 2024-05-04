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
        pair <string, string>{"LineAlg", "Линейная алгебра"},
        pair <string, string>{"MathAn", "Математический анализ"},
        pair <string, string>{"Physics", "Физика"},
        pair <string, string>{"History", "История"},
        pair <string, string>{"IT", "Информатика"},
        pair <string, string>{"Russian", "Русский язык"},
        pair <string, string>{"VPD", "ВПД"},
        pair <string, string>{"PE", "Физ-ра"},
        pair <string, string>{"English", "Иностранный язык"},
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
                cout << "Оценка для студента " << studentId << " по предмету " << subject << " не найдена." << endl;
            }
        } else {
            cout << "Семестр не найден: " << semesterIndex << endl;
        }
    }

    string getGrade() {
        int semesterIndex;
        string subject, studentId;

        cout << "Введите индекс семестра (1-9): " << endl;
        cin >> semesterIndex;
        cin.ignore();

        cout << "Введите название предмета: " << endl;
        getline(cin, subject);

        cout << "Введите номер зачётной книжки студента: " << endl;
        getline(cin, studentId);

        Semester* current = head;
        while (current != nullptr) {
            if (current->index == semesterIndex) {
                return current->getGrade(subject, studentId);
            }
            current = current->next;
        }
        cout << "Семестр не найден: " << semesterIndex << endl;
        return "";
    }

    void addGradeInteractively(const string& studentId) {
        int semesterIndex;
        string subject, grade;

        cout << "Введите индекс семестра: " << endl;
        cin >> semesterIndex;
        cin.ignore();

        if (semesterIndex < 1 || semesterIndex > 9) {
            cout << "Неверный индекс семестра. Индекс семестра должен быть от 1 до 9." << endl;
            return;
        }

        cout << "Введите предмет: " << endl;
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
            cout << "Предмет '" << subject << "' не является действительным для семестра " << semesterIndex << ". Пожалуйста, попробуйте снова." << endl;
            return;
        }

        cout << "Введите оценку: " << endl;
        getline(cin, grade);

        addGrade(semesterIndex, subject, studentId, grade);
    }

    void removeGradeInteractively() {
        int semesterIndex;
        string subject, studentId;

        cout << "Введите индекс семестра:" << endl;
        cin >> semesterIndex;
        cin.ignore();

        cout << "Введите предмет:" << endl;
        getline(cin, subject);

        cout << "Введите номер зачётной книжки студента:" << endl;
        getline(cin, studentId);

        removeGrade(semesterIndex, subject, studentId);
    }

    void printGrades() {
        Semester* current = head;
        while (current != nullptr) {
            if (current->hasGrades()) {
                cout << "\nОценки за семестр: " << current->index << endl;
                cout << endl;

                // Собираем список всех предметов в этом семестре
                vector<string> subjects;
                for (auto & mark : current->marks) {
                    subjects.push_back(mark.first);
                }

                // Вычисляем максимальную ширину столбцов
                int maxWidth = 10;
                for (const auto& subject : subjects) {
                    maxWidth = max(maxWidth, static_cast<int>(subjectsInterpretations[subject].length()));
                }
                maxWidth = max(maxWidth, 10);

                // Выводим заголовок таблицы
                cout << "| " << setw(10) << left << "Номер";
                for (const auto& subject : subjects) {
                    cout << " | " << setw(maxWidth) << left << subjectsInterpretations[subject];
                }
                cout << " |" << endl;

                // Вычисляем ширину первой строки
                int firstLineWidth = 10 + maxWidth * subjects.size() + (subjects.size()) * 3;
                cout << string(firstLineWidth + 4, '-') << endl;

                // Выводим данные по студентам
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
            cerr << "Невозможно открыть файл: " << filename << endl;
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
            std::cerr << "Не удалось открыть файл students.bin" << std::endl;
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

        grOne << "Студенты " << minOne << "-" << maxOne << " года рождения (год поступления - " << groupOnePar << ")\n";
        grTwo << "Студенты " << minTwo << "-" << maxTwo << " года рождения\n";

        sortStudentsById(studentsOne);
        sortStudentsById(studentsTwo);

        saveToFile(groupOneName, studentsOne, 0);
        saveToFile(groupTwoName, studentsTwo, 0);

        grOne.close();
        grTwo.close();

        cout << "\nПервая группа с " << groupOnePar << " годом поступления:\n\n";
        showStudents(studentsOne);
        cout << "\nВторая группа:\n\n";
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
        cout << "Студент успешно добавлен." << endl;
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
        // Собираем список всех параметров студента
        vector<string> parameters = {"Имя", "Фамилия", "Отчество", "Дата рождения", "Год поступления", "Институт", "Кафедра", "Группа", "Номер", "Пол"};

        // Вычисляем максимальную ширину столбцов
        int maxWidth = 15;
        for (const auto& param : parameters) {
            maxWidth = max(maxWidth, static_cast<int>(param.length()));
        }
        maxWidth = min(maxWidth, 20);

        // Выводим данные по студентам построчно, по 10 студентов в каждой строке
        for (size_t i = 0; i < vectorStudent.size(); i += 10) {
            // Выводим заголовок таблицы
            for (const auto& param : parameters) {
                cout << "| " << setw(maxWidth) << left << param;
            }
            cout << "|" << endl;

            int totalWidth = maxWidth * parameters.size() + (parameters.size() - 1) * 3 + 1; // Общая ширина таблицы
            cout << string(totalWidth - 7, '-') << endl;

            // Выводим данные по студентам
            for (size_t j = i; j < min(i + 10, vectorStudent.size()); ++j) {
                const auto& student = vectorStudent[j];
                for (const auto& param : parameters) {
                    if (param == "Имя") {
                        cout << "| " << setw(maxWidth) << left << student.firstName;
                    } else if (param == "Фамилия") {
                        cout << "| " << setw(maxWidth) << left << student.lastName;
                    } else if (param == "Отчество") {
                        cout << "| " << setw(maxWidth) << left << student.patronymic;
                    } else if (param == "Дата рождения") {
                        cout << "| " << setw(maxWidth) << left << student.birthDate;
                    } else if (param == "Год поступления") {
                        cout << "| " << setw(maxWidth) << left << student.admissionYear;
                    } else if (param == "Институт") {
                        cout << "| " << setw(maxWidth) << left << student.faculty;
                    } else if (param == "Кафедра") {
                        cout << "| " << setw(maxWidth) << left << student.department;
                    } else if (param == "Группа") {
                        cout << "| " << setw(maxWidth) << left << student.group;
                    } else if (param == "Номер") {
                        cout << "| " << setw(maxWidth) << left << student.id;
                    } else if (param == "Пол") {
                        cout << "| " << setw(maxWidth) << left << student.sex;
                    }
                }
                cout << "|" << endl;
            }

            cout << string(totalWidth - 7, '-') << endl;
            cout << endl; // Добавляем пустую строку между блоками
        }
    }

    void printMainMenu() {
        string tag;
        vector<string> keys = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "q"};
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
        cout << "0. Узнать оценку студента по предмету" << endl;
        cout << "q. Выход" << endl;
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
            cout << "Введите номер зачётной книжки студента:" << endl;
            cin >> id;
            if (!idSet.count(id)) {
                cerr << "Такого номера зачётной книжки не существует." << endl;
            } else {
                cout << "Введите новое имя:" << endl;
                cin >> firstName;
                cout << "Введите новую фамилию:" << endl;
                cin >> lastName;
                cout << "Введите новое отчество:" << endl;
                cin >> patronymic;
                cout << "Введите новую дату рождения:" << endl;
                cout << "День:" << endl;
                cin >> birthDay;
                cout << "Месяц:" << endl;
                cin >> birthMonth;
                cout << "Год:" << endl;
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "Введите новый год поступления:" << endl;
                cin >> admissionYear;
                cout << "Введите новый факультет/институт:" << endl;
                cin >> faculty;
                cout << "Введите новую кафедру:" << endl;
                cin >> department;
                cout << "Введите новую группу:" << endl;
                cin >> group;
                cout << "Введите новый пол:" << endl;
                cin >> sex;
                cout << "Введите новый номер зачётной книжки:" << endl;
                cin >> newId;
                updateStudentData(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex, newId);
                cout << "Данные студента были обновлены." << endl;
            }
            printMainMenu();
        }
        if (tag == "3") {
            unsigned short admissionYear, birthDay, birthMonth, birthYear;
            string id, firstName, lastName, patronymic, birthDate, faculty, department, group, sex;
            cout << "Введите номер зачётной книжки студента:" << endl;
            cin >> id;
            if (idSet.count(id)) {
                cerr << "Такой номер зачётной книжки уже существует." << endl;
            } else {
                cout << "Введите имя студента:" << endl;
                cin >> firstName;
                cout << "Введите фамилию студента:" << endl;
                cin >> lastName;
                cout << "Введите отчество студента:" << endl;
                cin >> patronymic;
                cout << "Введите дату рождения студента:" << endl;
                cout << "День:" << endl;
                cin >> birthDay;
                cout << "Месяц:" << endl;
                cin >> birthMonth;
                cout << "Год:" << endl;
                cin >> birthYear;
                birthDate = getFormatDate(birthDay, birthMonth, birthYear);
                cout << "Введите год поступления студента:" << endl;
                cin >> admissionYear;
                cout << "Введите факультет/институт студента:" << endl;
                cin >> faculty;
                cout << "Введите кафедру студента:" << endl;
                cin >> department;
                cout << "Введите группу студента:" << endl;
                cin >> group;
                cout << "Введите пол студента:" << endl;
                cin >> sex;
                addStudent(firstName, lastName, patronymic, birthDate, admissionYear, faculty, department, group, id, sex);
                cout << "Данные студента были добавлены." << endl;
            }
            printMainMenu();
        }
        if (tag == "4") {
            string id;
            cout << "Введите номер зачётной книжки студента для удаления:" << endl;
            cin >> id;
            if (!idSet.count(id)) {
                cerr << "Такого номера зачётной книжки не существует." << endl;
            } else {
                deleteStudent(id);
                cout << "Данные студента были удалены." << endl;
            }
            printMainMenu();
        }
        if (tag == "5") {
            string id;
            cout << "Введите номер зачётной книжки студента для получения данных:" << endl;
            cin >> id;
            if (!idSet.count(id)) {
                cerr << "Такого номера зачётной книжки не существует." << endl;
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
            cout << "Введите номер зачётной книжки:" << endl;
            getline(cin, id);
            if (!idSet.count(id)) {
                cerr << "Такого номера зачётной книжки не существует." << endl;
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

// TODO шифрование
