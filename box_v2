#include <iostream>
#include <vector>
#include <string>

class Ball {
private:
    std::string color;

public:
    explicit Ball(std::string ball_color) : color(std::move(ball_color)) {}

    const std::string& getColor() const {
        return color;
    }
};

class Box {
private:
    std::string name;
    std::vector<Ball*> storage; // Хранилище указателей на шарики в куче

public:
    explicit Box(std::string boxName) : name(std::move(boxName)) {}

    // запрет копирования
    Box(const Box&) = delete;
    Box& operator=(const Box&) = delete;

    // очищаем выделенную в куче память
    ~Box() {
        for (Ball* ptr : storage) {
            delete ptr;
        }
    }

    void putBall(Ball* ball) {
        if (ball != nullptr) {
            storage.push_back(ball);
        }
    }

    // забрать шарик из коробки не удаляя из памяти
    Ball* takeBall() {
        if (storage.empty()) {
            return nullptr;
        }
        Ball* last = storage.back();
        storage.pop_back();
        return last;
    }

    void printStatus() const {
        std::cout << "--- " << name << " ---\n";
        std::cout << "Всего шариков: " << storage.size() << "\n";

        if (storage.empty()) {
            std::cout << "Пусто\n\n";
            return;
        }

        std::cout << "Цвета: ";
        for (size_t i = 0; i < storage.size(); ++i) {
            std::cout << storage[i]->getColor();
            if (i + 1 < storage.size()) std::cout << ", ";
        }
        std::cout << "\n\n";
    }
};

int main() {

    Box firstBox("Первая коробка");
    Box secondBox("Вторая коробка");

    // выделяем через new в куче
    firstBox.putBall(new Ball("Красный"));
    firstBox.putBall(new Ball("Синий"));
    firstBox.putBall(new Ball("Зеленый"));
    firstBox.putBall(new Ball("Желтый"));
    firstBox.putBall(new Ball("Белый"));
    firstBox.putBall(new Ball("Фиолетовый"));

    secondBox.putBall(new Ball("Черный"));
    secondBox.putBall(new Ball("Оранжевый"));
    secondBox.putBall(new Ball("Розовый"));
    secondBox.putBall(new Ball("Серый"));

    firstBox.printStatus();
    secondBox.printStatus();

    // достаем один шарик из первой и кладем во вторую
    Ball* movedBall = firstBox.takeBall();
    if (movedBall) {
        secondBox.putBall(movedBall);
    }

    std::cout << ">>> Переложили один шарик из первой во вторую <<<\n\n";
    firstBox.printStatus();
    secondBox.printStatus();

    return 0;
}
