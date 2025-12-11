#include <iostream>
#include <memory>
#include <string>
// Паттерн Strategy. Реализовать систему оплаты в интернет-магазине, которая 
// поддерживает разные методы оплаты: безналичный, наличный расчёты, перевод при 
// получении. Система должна быть проектирована с учетом масштабируемости, так как 
// со временем могут появиться новые методы оплаты (например, криптовалюты, электронные 
// кошельки, рассрочка и т.д.). Нужно обеспечить возможность добавления новых 
// методов без изменения существующего кода. Каждый метод оплаты должен быть 
// реализован как отдельная стратегия, и система должна позволять динамически 
// менять метод оплаты в зависимости от предпочтений клиента. Создайте абстрактный 
// класс PaymentStrategy с методом ProcessPayment(), который будет определять 
// общий интерфейс для всех стратегий. Реализуйте конкретные стратегии оплаты. 
// Создайте класс Order (заказ), который будет использовать текущую стратегию 
// оплаты для обработки платежа
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual void ProcessPayment(double amount) = 0;
};

class CashlessPayment : public PaymentStrategy {
public:
    void ProcessPayment(double amount) override {
        std::cout << "Обрабатываем безналичную оплату на сумму: " 
                  << amount << " руб.\n";
    }
};

class CashPayment : public PaymentStrategy {
public:
    void ProcessPayment(double amount) override {
        std::cout << "Оплата наличными на сумму: " 
                  << amount << " руб.\n";
    }
};

class CODPayment : public PaymentStrategy {
public:
    void ProcessPayment(double amount) override {
        std::cout << "Оплата при получении. Сумма к оплате: " 
                  << amount << " руб.\n";
    }
};

class Order {
private:
    double amount;
    std::unique_ptr<PaymentStrategy> strategy;

public:
    Order(double amt, std::unique_ptr<PaymentStrategy> initialStrategy)
        : amount(amt), strategy(std::move(initialStrategy)) {}

    void SetPaymentStrategy(std::unique_ptr<PaymentStrategy> newStrategy) {
        strategy = std::move(newStrategy);
    }

    void ProcessPayment() {
        if (!strategy) {
            std::cout << "Ошибка: стратегия оплаты не выбрана!\n";
            return;
        }
        strategy->ProcessPayment(amount);
    }
};

int main() {
    // как добавить новый метод оплаты
    // 1. создать новый класс наследуемый от PaymentStrategy
    // 2. менять существующий код не нужно
    // 3. в новом месте order.SetPaymentStrategy(std::make_unique<вашКласс>());
    Order order(4999.0, std::make_unique<CashlessPayment>());

    std::cout << "=== Оплата 1 ===\n";
    order.ProcessPayment();

    std::cout << "\nМеняем метод на наличный...\n";
    order.SetPaymentStrategy(std::make_unique<CashPayment>());

    std::cout << "=== Оплата 2 ===\n";
    order.ProcessPayment();

    std::cout << "\nМеняем метод на оплату при получении...\n";
    order.SetPaymentStrategy(std::make_unique<CODPayment>());

    std::cout << "=== Оплата 3 ===\n";
    order.ProcessPayment();

    return 0;
}