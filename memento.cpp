#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
// 7. Паттерн Memento. Реализовать систему сохранения состояния корзины покупок в 
// интернет-магазине. Пользователь должен иметь возможность отменить изменения и 
// вернуться к предыдущему состоянию корзины. Создайте класс ShoppingCart (корзина покупок)
// с методами для добавления/удаления товаров и сохранения состояния. Реализуйте класс 
// Memento для хранения состояния корзины и класс Caretaker для управления мементами 
// (сохранение и восстановление состояния). 
// Вопрос: Как вы будете хранить несколько 
// точек сохранения (например, для отмены нескольких действий)? Какие ограничения могут 
// возникнуть при использовании этого паттерна?

class Memento {
private:
    std::vector<std::string> items;

public:
    Memento(std::vector<std::string>& state)
        : items(state) {}

    std::vector<std::string>& getState() {
        return items;
    }
};

class ShoppingCart {
private:
    std::vector<std::string> items;

public:
    void addItem(std::string item) {
        items.push_back(item);
    }
    void removeItem(std::string& item) {
        items.erase(
            std::remove(items.begin(), items.end(), item),
            items.end()
        );
    }
    std::unique_ptr<Memento> save() {
        return std::make_unique<Memento>(items);
    }
    void restore(Memento& m) {
        items = m.getState();
    }
    void print() {
        if (items.empty()) {
            std::cout << "Корзина пустая\n";
            return;
        }

        std::cout << "Корзина: ";
        for (auto& item : items)
            std::cout << item << " ";
        std::cout << "\n";
    }
};

class Caretaker {
private:
    std::vector<std::unique_ptr<Memento>> history;

public:
    void save(std::unique_ptr<Memento> m) {
        history.push_back(std::move(m));
    }

    Memento* undo() {
        if (history.empty()) return nullptr;

        Memento* last = history.back().get();
        history.pop_back();
        return last;
    }

    bool hasHistory() {
        return !history.empty();
    }
};
int main() {
    ShoppingCart cart;
    Caretaker caretaker;

    cart.addItem("Apple");
    caretaker.save(cart.save());
    cart.print();

    cart.addItem("Banana");
    caretaker.save(cart.save());
    cart.print();

    cart.addItem("Milk");
    caretaker.save(cart.save());
    cart.print();

    std::cout << "\n--- Undo last two actions ---\n";

    if (caretaker.hasHistory()) {
        cart.restore(*caretaker.undo());
        cart.print();
    }

    if (caretaker.hasHistory()) {
        cart.restore(*caretaker.undo());
        cart.print();
    }
    return 0;
}

// Ответ на вопрос: хранить несколько точек сохранения можно с помощью вектора. 
// Ограничение: большой расход памяти. 