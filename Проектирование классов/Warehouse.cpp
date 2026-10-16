#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <windows.h> 

//предмет
class Item {
private:
    std::string sku;       // артикул 
    std::string title;     // название
    std::string category;  // категория товара
    double width = 0.0;
    double height = 0.0;
    double length = 0.0;

public:
    // основной конструктор
    Item(std::string item_sku, std::string item_title, std::string item_cat,
         double w, double h, double l)
        : sku(std::move(item_sku)),
          title(std::move(item_title)),
          category(std::move(item_cat)),
          width(w), height(h), length(l) {}

    // конструктор для кубических товаров
    explicit Item(std::string item_sku, std::string item_title, std::string item_cat, double side)
        : Item(std::move(item_sku), std::move(item_title), std::move(item_cat), side, side, side) {}

    // возврат константной ссылки 
    const std::string& get_sku() const { return sku; }
    const std::string& get_title() const { return title; }
    const std::string& get_category() const { return category; }

    // Вычисление объема 
    double get_volume() const {
        return width * height * length;
    }

    void print_info() const {
        std::cout << "[" << sku << "] " << title 
                  << " | Категория: " << category 
                  << " | Объем: " << get_volume() << " м3\n";
    }
};


// 2. склад (контейнер-владелец)
class Warehouse {
private:
    std::string name;
    double max_capacity = 0.0;       // Максимальный вмещаемый объем
    std::vector<Item*> storage;      // Хранилище указателей на объекты в куче

public:
    Warehouse(std::string wh_name, double capacity)
        : name(std::move(wh_name)), max_capacity(capacity) {}

    // запрет копирования объекта склада
    Warehouse(const Warehouse&) = delete;
    Warehouse& operator=(const Warehouse&) = delete;

    // деструктор
    ~Warehouse() {
        for (Item* item : storage) {
            delete item;
        }
    }

    // занятый объем
    double get_current_volume() const {
        double total = 0.0;
        for (const Item* item : storage) {
            total += item->get_volume();
        }
        return total;
    }

    // прием товара на склад
    bool add_item(Item* item) {
        if (!item) return false;

        if (get_current_volume() + item->get_volume() > max_capacity) {
            std::cout << "Ошибка: Склад '" << name << "' переполнен! Нельзя принять " 
                      << item->get_title() << "\n";
            return false;
        }

        storage.push_back(item);
        return true;
    }

    // передача товара по артикулу
    Item* take_item(const std::string& sku) {
        for (auto it = storage.begin(); it != storage.end(); ++it) {
            if ((*it)->get_sku() == sku) {
                Item* target = *it;
                storage.erase(it); // Убираем из вектора склада
                return target;     // Отдаем наружу
            }
        }
        return nullptr;
    }

    // подсчет товаров по категориям
    std::map<std::string, int> get_category_stats() const {
        std::map<std::string, int> stats;
        for (const Item* item : storage) {
            stats[item->get_category()]++;
        }
        return stats;
    }

    void print_inventory() const {
        std::cout << "========================================\n";
        std::cout << "Склад: " << name << "\n";
        std::cout << "Загрузка: " << get_current_volume() << " / " << max_capacity << " м3\n";
        std::cout << "Всего единиц груза: " << storage.size() << "\n";
        
        std::cout << "\nСписок товаров:\n";
        if (storage.empty()) {
            std::cout << "  (склад пуст)\n";
        } else {
            for (const Item* item : storage) {
                std::cout << "  ";
                item->print_info();
            }
        }

        std::cout << "\nРаспределение по категориям:\n";
        auto stats = get_category_stats();
        for (const auto& [cat, count] : stats) {
            std::cout << "  - " << cat << ": " << count << " шт.\n";
        }
        std::cout << "========================================\n\n";
    }
};
