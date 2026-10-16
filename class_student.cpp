#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

class Student {
private:
    std::string name;
    // Контейнер ключ - предмет, значение - список оценок 
    std::map<std::string, std::vector<int>> grade_book;

public:
    Student() : name("Безымянный") {}
    explicit Student(std::string student_name) : name(std::move(student_name)) {}

    // Метод добавления оценки по предмету
    void add_grade(const std::string& subject, int grade) {
        if (grade >= 1 && grade <= 5) {
            grade_book[subject].push_back(grade);
        } else {
            std::cout << "Ошибка: оценка " << grade << " по предмету \"" << subject 
                      << "\" должна быть в диапазоне от 1 до 5.\n";
        }
    }

    // Метод просмотра всех оценок из контейнера
    void print_grades() const {
        std::cout << "Журнал успеваемости студента: " << name << "\n";
        if (grade_book.empty()) {
            std::cout << "  (записей пока нет)\n";
            return;
        }

        for (const auto& [subject, grades] : grade_book) {
            std::cout << "  " << subject << ": ";
            for (int grade : grades) {
                std::cout << grade << " ";
            }
            std::cout << "\n";
        }
    }

    // Метод просмотра оценок по одному конкретному предмету
    void print_subject_grades(const std::string& subject) const {
        auto it = grade_book.find(subject);
        if (it != grade_book.end()) {
            std::cout << "Оценки по предмету " << subject << ": ";
            for (int grade : it->second) {
                std::cout << grade << " ";
            }
            std::cout << "\n";
        } else {
            std::cout << "Предмет \"" << subject << "\" не найден в журнале.\n";
        }
    }
};

int main() {
    Student student("Данила");
    student.add_grade("Математика", 5);
    student.add_grade("Математика", 4);
    student.add_grade("Информатика", 5);
    student.add_grade("Физика", 3);
    student.add_grade("Физика", 4);

    student.print_grades();

    std::cout << "\n";

    student.print_subject_grades("Математика");

    return 0;
}
