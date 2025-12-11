#include <iostream>
#include <string>
#include <memory>
// Паттерн Template Method. Реализовать систему оформления заказа в интернет-магазине, которая использует 
// шаблонный метод для определения общего процесса оформления заказа.Создайте абстрактный класс OrderProcessing 
// с методом ProcessOrder(), который определяет общий алгоритм: выбор товара, оформление, оплата, доставка. 
// Некоторые шаги (например, выбор способа доставки) должны быть переопределены в подклассах: StandardOrderProcessing 
// и ExpressOrderProcessing. Создайте класс Order (заказ), который будет использовать подходящий шаблонный метод 
// для обработки заказа.
// Вопрос: Как вы можете расширить систему, добавив новый тип заказа (например, заказ с предоплатой)? Какие изменения потребуются 
// в коде? Также, как вы реализуете функционал для обработки заказов с предоплатой в методе ProcessOrder()
class OrderProcessing {
public:
    virtual ~OrderProcessing() = default;

    void ProcessOrder(const std::string& product, double price) {
        SelectProduct(product);
        PlaceOrder();
        MakePayment(price);
        ChooseDelivery();
        CompleteOrder();
    }

protected:
    void SelectProduct(const std::string& product) {
        std::cout << "Выбран товар: " << product << "\n";
    }

    void PlaceOrder() {
        std::cout << "Оформление заказа...\n";
    }

    void MakePayment(double price) {
        std::cout << "Оплата заказа на сумму " << price << " руб.\n";
    }

    virtual void ChooseDelivery() = 0;

    void CompleteOrder() {
        std::cout << "Заказ успешно обработан!\n";
    }
};

class StandardOrderProcessing : public OrderProcessing {
protected:
    void ChooseDelivery() override {
        std::cout << "Выбрана стандартная доставка (3-7 дней).\n";
    }
};

class ExpressOrderProcessing : public OrderProcessing {
protected:
    void ChooseDelivery() override {
        std::cout << "Выбрана экспресс доставка (1-2 дня).\n";
    }
};

class Order {
private:
    std::unique_ptr<OrderProcessing> processor;

public:
    Order(std::unique_ptr<OrderProcessing> proc) 
        : processor(std::move(proc)) {}

    void Process(const std::string& product, double price) {
        processor->ProcessOrder(product, price);
    }
};

int main() {
    std::cout << "Обработка заказа со стандартной доставкой\n";
    Order standardOrder(std::make_unique<StandardOrderProcessing>());
    standardOrder.Process("Ноутбук", 75000);

    std::cout << "\nОбработка заказа с экспресс доставкой\n";
    Order expressOrder(std::make_unique<ExpressOrderProcessing>());
    expressOrder.Process("Смартфон", 50000);
    
    return 0;
}

// Ответ на вопрос
// OrderProcessing остается без изменений. Паттерн позволяет расширять систему, используя полиморфизм. 
// Создаем новый подкласс, изменяем только те методы, которые этого требуют и используем новый подкласс
// в клиентсокм коде.
