#include <iostream>
#include <vector>
#include <memory>
#include <string>
//паттерн, который объединяет
//объекты в дерево, простые 
//объекты помещает в композиции - 
//контейнеры, которые могут содержать
//простые объекты + другие контейнеры
//обращаемся к ним через единый интерфейс

//Задача:
//Паттерн Composite. Реализовать систему объектов для склада Продукт и Коробка. 
//Коробка может содержать несколько Продуктов и других Коробок поменьше. Те, 
//в свою очередь, тоже содержат либо Продукты, либо Коробки и так далее. Продукты 
//и Коробки могут быть частью заказов. Каждый заказ может содержать как простые 
//Продукты без упаковки, так и составные Коробки. Заказ может состоять из различных 
//продуктов, упакованных в собственные коробки. Ваша задача состоит в том, чтобы у
//знать цену всего заказа.

//Немного усложнил себе задачу. Добавил имена и идентификаторы. Добавил реализацию
//кол-ва товаров, вес и объем коробок, приоритет заказов, гибкую статистику
//(подсчет сколько типов продуктов в заказе, макс и мин цена продукта, средняя цена коробки)

enum orderType {
    standart = 1,
    opt = 2, // -5% на продукты
    priority = 3 // +15% к цене
};

class product {
public:
    virtual int getPrice() = 0;
    virtual int getCount() = 0;
    virtual std::string getName() = 0;
    virtual void printInfo() = 0;
    virtual ~product() = default;
};

class vegetable : public product {
    std::string name = "";
    int price = 0;
    int count = 0;
public:
    vegetable(std::string n, int p) {
        name = n;
        price = p;
    }
    vegetable (std::string n, int p, int c) {
        name = n;
        price = p;
        if (c > 0)
            count = c;
    }
    int getPrice() override {
        if(count != 0)
            return count*price;
        else
            return price;
    }
    int getCount() override {
        return count;
    }
    std::string getName() override {
        return name;
    }
    void printInfo() override {
        std::cout << "-Название: " << getName() << " , количество: " << 
                getCount() << " , сумма: " << getPrice() << "\n";
    }
};

class fruit : public product {
    std::string name = "";
    int price = 0;
    int count = 0;
public:
    fruit(std::string n, int p) {
        name = n;
        price = p;
        count = 1;
    }
    fruit(std::string n, int p, int c) {
        name = n;
        price = p;
        if (c > 0)
            count = c;
    }
    int getPrice() override {
        if(count != 0)
            return count*price;
        else
            return price;
    }
    int getCount() override {
        return count;
    }
    std::string getName() override {
        return name;
    }
    void printInfo() override {
        std::cout << "-Название: " << getName() << " , количество: " << 
                getCount() << " , сумма: " << getPrice() << "\n";
    }
};

class box : public product {
    int id = 0;
    std::vector<std::shared_ptr<product>> items;
    int price = 0;
    int weight = 0;
    int maxWeight = 10;
public:
    box() {
        id++;
    }
    void add(const std::shared_ptr<product>& p) {
        if(weight + p->getCount() < maxWeight) {
            items.push_back(p);
            weight += p->getCount();
        }
        else
            std::cout << "Нельзя добавить продукт. Превышен максимальный вес коробки" 
                    << "\n";
    }
    int getCount() override {
        return 1;
    }
    int getPrice() override {
        int total = 0;
        for (auto &i : items)
            total += i->getPrice();
        price += total;
        return total;
    }
    std::string getName() override {
        return "box id: " + std::to_string(id);
    }
    void printInfo() override {
        std::cout << "-Название: " << getName() << " , количество: " << 
            getCount() << " , сумма: " << getPrice() << "\n";
        for (auto &p : items) {
            std::cout << "  ";
            p->printInfo();
        }
    }
};

class order : product {
    bool is_deliviry = false;
    int distance = 0;
    int id = 0;
    int price = 0;
    std::vector<std::shared_ptr<product>> items;
    orderType type;
public:
    order() : type(orderType::standart){
        id++;
    }
    order(bool is_deliv, int dist, orderType t) {
        id++;
        distance = dist;
        is_deliviry = is_deliv;
        type = t;
    }
    void add(const std::shared_ptr<product>& p) {
            items.push_back(p);
    }
    std::string getName() {
        return "order id: " + std::to_string(id);
    }
    int getPrice() override {
        int total_price = 0;
        int total = 0;
        for (auto &i : items)
            total_price += i->getPrice();
        price += total + total_price;
        if(is_deliviry) {
            int tarif = distance * 7;
            if(type == orderType::priority)
                total = (total_price + tarif) * 1.15;
            else if(type == orderType::opt)
                total = total_price * 0.95 + tarif;
            std::cout << "Доставка от магазина до пункта назначения: " <<
            distance << " км. " << "Цена тарифа за доставку: " << tarif <<
            " . Цена заказа: " << total << "\n";
            return total;
        } else
            return total_price;
    }
    int getCount() override {
        return 1;
    }
    bool findProductByName(std::string s) {
        bool is = false;
        for(auto &p : items)
            if(p->getName() == s)
                is = true;
        if (is)
            std::cout << s << " есть в заказе" << "\n";
        else
            std::cout << s << " нет в заказе" << "\n";
        return is;
    }
    int getMaxPrice() {
        int max_price = 0;
        for (auto &p : items)
            if(p->getPrice() > max_price)
                max_price = p->getPrice();
        return max_price;
    }
    int getMinPrice() {
        int min_price = 9999999;
        for (auto &p : items)
            if(p->getPrice() < min_price)
                min_price = p->getPrice();
        return min_price;
    }
    int getAveragePrice() {
        int avg = 0;
        int counter = 0;
        for (auto &p : items) {
            avg += p->getPrice();
            counter++;
        }
        int total = avg / counter;
        return total;
    }
    void printInfo() override {
        std::cout << "Состав заказа " << getName() << ":\n";
        for (auto &p : items) {
            std::cout << "  ";
            p->printInfo();
        }
    }
};

int main() {
    auto banana = std::make_shared<fruit>("Банан", 120, 2);
    auto botat = std::make_shared<vegetable>("Ботат", 300);
    auto apple = std::make_shared<fruit>("Яблоко", 190);
    auto tomato = std::make_shared<vegetable>("Томат",490, 2);

    auto fruitBox = std::make_shared<box>();
    fruitBox->add(banana);
    fruitBox->add(apple);
    
    auto vegetableBox = std::make_shared<box>();
    vegetableBox->add(botat);
    vegetableBox->add(tomato);

    auto myOrder = std::make_shared<order>();
    myOrder->add(fruitBox);
    myOrder->add(vegetableBox);
    auto ogurets = std::make_shared<vegetable>("Огурец", 120);
    myOrder->add(ogurets);

    myOrder->findProductByName("Огурец");

    vegetableBox->add(std::make_shared<vegetable>("Морковь",490, 13));

    std::cout << "Сумма вашего заказа: " << myOrder->getPrice() << "\n";
    std::cout << "-===========-" << "\n";

    auto deliveryOrder = std::make_shared<order>(true, 30, orderType::priority);
    deliveryOrder->add(std::make_shared<vegetable>("Огурец", 120, 2));
    deliveryOrder->add(fruitBox);
    std::cout << "Максимальная цена продукта: " << deliveryOrder->getMaxPrice() << "\n";
    std::cout << "Минимальная цена продукта: " << deliveryOrder->getMinPrice() << "\n";
    std::cout << "Средняя цена продукта: " << deliveryOrder->getAveragePrice() << "\n";
    deliveryOrder->getPrice();
    deliveryOrder->printInfo();
    return 0;
}