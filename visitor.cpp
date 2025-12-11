// 8. Паттерн Visitor. Реализовать систему расчёта стоимости доставки и налогов для 
// заказов, используя паттерн посетитель на базе иерархииклассов товаров: Product, 
// Box из лабораторной работы №2. Создайте абстрактный класс Visitor с методами 
// VisitProduct(), VisitBox(), которые будут использоваться для расчёта стоимости 
// доставки и налогов. Реализуйте конкретные посетители: DeliveryCostCalculator 
// (расчёт стоимости доставки), TaxCalculator (расчёт налогов). Вопрос: Как вы 
// будете расширять систему, добавляя новые типы расчетов (например, скидки)? 
// Какие изменения потребуются в коде?

#include <iostream>
#include <vector>
#include <memory>
#include <string>


class Visitor;

class Product {
public:
    virtual int getPrice() = 0;
    virtual int getCount() = 0;
    virtual std::string getName() = 0;
    virtual void printInfo() = 0;

    virtual void Accept(Visitor* v) = 0;

    virtual ~Product() = default;
};

class Vegetable : public Product {
    std::string name;
    int price;
    int count;
public:
    Vegetable(std::string n, int p, int c = 1) : name(n), price(p), count(c) {}

    int getPrice() override { 
        return price * count; 
    }
    int getCount() override { 
        return count; 
    }
    std::string getName() override { 
        return name; 
    }

    void printInfo() override {
        std::cout << "Овощ: " << name << ", кол-во: " << count << ", цена: " << getPrice() << "\n";
    }

    void Accept(Visitor* v) override;
};

class Fruit : public Product {
    std::string name;
    int price;
    int count;
public:
    Fruit(std::string n, int p, int c = 1) : name(n), price(p), count(c) {}

    int getPrice() override { 
        return price * count; 
    }
    int getCount() override {
        return count; 
        }
    std::string getName() override {
        return name;
    }

    void printInfo() override {
        std::cout << "Фрукт: " << name << ", кол-во: " << count << ", цена: " << getPrice() << "\n";
    }

    void Accept(Visitor* v) override;
};

class Box : public Product {
    std::vector<std::shared_ptr<Product>> items;

public:
    void add(const std::shared_ptr<Product>& p) {
        items.push_back(p);
    }

    int getCount() override {
        return 1;
    }

    int getPrice() override {
        int total = 0;
        for (auto& i : items) total += i->getPrice();
        return total;
    }

    std::string getName() override {
        return "Коробка";
    }

    void printInfo() override {
        std::cout << "Коробка: сумма = " << getPrice() << "\n";
        for (auto& p : items) {
            std::cout << " ";
            p->printInfo();
        }
    }

    const std::vector<std::shared_ptr<Product>>& getItems() const {
        return items;
    }

    void Accept(Visitor* v) override;
};

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual void VisitProduct(Product* p) = 0;
    virtual void VisitBox(Box* b) = 0;
};

class DeliveryCostCalculator : public Visitor {
public:
    int deliveryCost = 0;

    void VisitProduct(Product* p) override {
        deliveryCost += 5;
    }

    void VisitBox(Box* b) override {
        deliveryCost += 10;
        for (auto& item : b->getItems()) {
            item->Accept(this);
        }
    }
};

class TaxCalculator : public Visitor {
public:
    int tax = 0;

    void VisitProduct(Product* p) override {
        tax += p->getPrice() * 0.1;
    }

    void VisitBox(Box* b) override {
        for (auto& item : b->getItems()) {
            item->Accept(this);
        }
    }
};

void Vegetable::Accept(Visitor* v) {
    v->VisitProduct(this);
}
void Fruit::Accept(Visitor* v) {
    v->VisitProduct(this);
}
void Box::Accept(Visitor* v) {
    v->VisitBox(this);
}

int main() {
    auto apple = std::make_shared<Fruit>("Яблоко", 100, 2);
    auto tomato = std::make_shared<Vegetable>("Томат", 150, 3);

    auto box = std::make_shared<Box>();
    box->add(apple);
    box->add(tomato);

    DeliveryCostCalculator delivery;
    box->Accept(&delivery);

    std::cout << "Стоимость доставки: " << delivery.deliveryCost << " руб\n";

    TaxCalculator taxCalc;
    box->Accept(&taxCalc);

    std::cout << "Налог: " << taxCalc.tax << " руб\n";

    return 0;
}

// Ответ на вопрос: паттерн позволяет добавлять новые виды операций без изменения
// box и product. но при добавлении нового типа расчета потребуются изменения в 
// интерфейсе visitor