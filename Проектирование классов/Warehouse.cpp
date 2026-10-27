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

    // Виртуальный деструктор для безопасного полиморфного удаления (delete item) ===
    virtual ~Item() = default;

    // возврат константной ссылки 
    const std::string& get_sku() const { return sku; }
    const std::string& get_title() const { return title; }
    const std::string& get_category() const { return category; }

    // Вычисление объема 
    double get_volume() const {
        return width * height * length;
    }

    // добавлено слово virtual, чтобы метод мог переопределяться в наследниках ===
    virtual void print_info() const {
        std::cout << "[" << sku << "] " << title 
                  << " | Категория: " << category 
                  << " | Объем: " << get_volume() << " м3\n";
    }
};

// класс FragileItem (одиночное наследование от Item)
class FragileItem : public Item {
private:
    double max_impact_force = 0.0;     // Предельная нагрузка / сила удара (в Н)
    bool requires_cushioning = true;   // Требуется ли амортизация (пупырка/поролон)

public:
    // конструктор - инициализирует базовый класс Item, затем свои поля
    FragileItem(std::string item_sku, std::string item_title, std::string item_cat,
                double w, double h, double l, 
                double max_force, bool need_cushioning = true)
        : Item(std::move(item_sku), std::move(item_title), std::move(item_cat), w, h, l),
          max_impact_force(max_force),
          requires_cushioning(need_cushioning) {}

    // конструктор для кубического хрупкого товара
    explicit FragileItem(std::string item_sku, std::string item_title, std::string item_cat,
                         double side, double max_force)
        : FragileItem(std::move(item_sku), std::move(item_title), std::move(item_cat),
                      side, side, side, max_force, true) {}

    double get_max_impact_force() const { return max_impact_force; }
    bool get_requires_cushioning() const { return requires_cushioning; }

    // переопределение виртуального метода базового класса
    void print_info() const override {
        Item::print_info();
        std::cout << "    └── [ХРУПКИЙ ГРУЗ] Нагрузка до: " 
                  << max_impact_force << " Н | Амортизация: " 
                  << (requires_cushioning ? "Да" : "Нет") << "\n";
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

bool transfer_item(Warehouse& from, Warehouse& to, const std::string& sku) {
    Item* item = from.take_item(sku);
    if (!item) {
        std::cout << "Перемещение не удалось: товар с артикулом " << sku << " не найден.\n";
        return false;
    }

    if (to.add_item(item)) {
        std::cout << "Успешно перемещен товар [" << sku << "] " << item->get_title() << "\n\n";
        return true;
    }

    // Если на целевом складе не хватило места, возвращаем назад
    from.add_item(item);
    std::cout << "Откат операции: товар возвращен на исходный склад.\n\n";
    return false;
}

// демонстрации работы базового и производного классов
int main() {
    SetConsoleOutputCP(CP_UTF8);

    Warehouse central_hub("Центральный склад", 50.0);
    Warehouse local_point("Пункт выдачи", 10.0);

    // Добавляем обычный товар базового класса Item
    central_hub.add_item(new Item("A-101", "Холодильник", "Бытовая техника", 0.8, 2.0, 0.7));

    // Добавляем хрупкий товар производного класса FragileItem
    central_hub.add_item(new FragileItem("GL-501", "Люстра хрустальная", "Освещение", 0.5, 0.5, 0.5, 25.0, true));

    std::cout << "--- Исходное состояние складов ---\n";
    central_hub.print_inventory();
    local_point.print_inventory();

    // Перемещение хрупкого товара
    std::cout << ">>> Перемещаем 'GL-501' в пункт выдачи <<<\n";
    transfer_item(central_hub, local_point, "GL-501");

    std::cout << "--- Состояние после перемещения ---\n";
    central_hub.print_inventory();
    local_point.print_inventory();

    return 0;
}
