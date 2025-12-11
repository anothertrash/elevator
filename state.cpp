#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
// 6. Паттерн State. Реализовать систему управления состоянием заказа в интернет-
// магазине. Заказ может находиться в следующих состояниях: "Новый", "В обработке", 
// "Отправлен", "Доставлен", "Отменен". Создайте абстрактный класс OrderState с 
// методами ProcessOrder() и GetStatus(). Реализуйте конкретные состояния, каждое 
// из которых определяет свое поведение при переходе в следующее состояние. Создайте 
// класс Order (заказ), который будет использовать текущее состояние для обработки 
// заказа. 
// Вопрос: Как вы обеспечите корректность переходов между состояниями (например, 
// нельзя вернуться из "Доставлен" в "В обработке")? Какие методы добавите для 
// управления состояниями?

class Order;

class OrderState {
public:
    virtual ~OrderState() = default;

    virtual void processOrder(Order& order) = 0;
    virtual std::string getStatus() = 0;
    virtual void cancel(Order& order) {
        throw std::logic_error("Ошибка. Нельзя отменить заказ на данном этапе");
    }
    virtual void ship(Order& order) {
        throw std::logic_error("Ошибка. Нельзя отправить заказ на данном этапе");
    }
    virtual void deliver(Order& order) {
        throw std::logic_error("Ошибка. Нельзя доставить заказ на данном этапе");
    }
};

class Order {
private:
    std::unique_ptr<OrderState> state;

public:
    Order(std::unique_ptr<OrderState> initState) : state(std::move(initState)) {}

    void setState(std::unique_ptr<OrderState> newState) {
        state = std::move(newState);
    }
    void processOrder() {
        state->processOrder(*this);
    }
    void cancel() {
        state->cancel(*this);
    }
    void ship() {
        state->ship(*this);
    }
    void deliver() {
        state->deliver(*this);
    }
    std::string getStatus() {
        return state->getStatus();
    }
};

class NewState : public OrderState {
public:
    void processOrder(Order& order) override;
    std::string getStatus() override {
        return "Новый заказ";
    }
    void cancel(Order& order) override;
};

class ProcessingState : public OrderState {
public:
    void processOrder(Order& order) override;
    std::string getStatus() override {
        return "В обработке"; 
    }
    void cancel(Order& order) override;
    void ship(Order& order) override;
};

class ShippedState : public OrderState {
public:
    void processOrder(Order& order) override {
        throw std::logic_error("Заказ уже отправлен. Используйте Deliver().");
    }
    std::string getStatus() override {
        return "Отправлен"; 
    }
    void deliver(Order& order) override;
};

class DeliveredState : public OrderState {
public:
    void processOrder(Order& order) override {
        throw std::logic_error("Заказ уже доставлен.");
    }
    std::string getStatus() override {
        return "Доставлен"; 
    }
};

class CanceledState : public OrderState {
public:
    void processOrder(Order& order) override {
        throw std::logic_error("Заказ отменен.");
    }
    std::string getStatus() override { 
        return "Отменен"; 
    }
};

void NewState::processOrder(Order& order) {
    std::cout << "Переводим заказ в обработку\n";
    order.setState(std::make_unique<ProcessingState>());
}

void NewState::cancel(Order& order) {
    std::cout << "Отмена нового заказа\n";
    order.setState(std::make_unique<CanceledState>());
}

void ProcessingState::processOrder(Order& order) {
    std::cout << "Обработка завершена\n";
    order.setState(std::make_unique<ShippedState>());
}

void ProcessingState::cancel(Order& order) {
    std::cout << "Отмена заказа\n";
    order.setState(std::make_unique<CanceledState>());
}

void ProcessingState::ship(Order& order) {
    std::cout << "Отправляем заказ\n";
    order.setState(std::make_unique<ShippedState>());
}

void ShippedState::deliver(Order& order) {
    std::cout << "Доставляем заказ\n";
    order.setState(std::make_unique<DeliveredState>());
}

int main() {
    Order order(std::make_unique<NewState>());

    std::cout << "Текущее состояние: " << order.getStatus() << "\n";

    order.processOrder();
    std::cout << "Текущее состояние: " << order.getStatus() << "\n";

    order.ship();
    std::cout << "Текущее состояние: " << order.getStatus() << "\n";

    order.deliver();
    std::cout << "Текущее состояние: " << order.getStatus() << "\n";

    try {
        order.cancel();
    }
    catch(const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
    return 0;
}

//Ответ на вопрос: Корректность переходов обеспечивается тем, что состояние кон-
//тролирует, какие переходы разрешены, а какие нет. То есть объект определяет, в 
//какое стстояние он может перевести заказ и не похволяет вернуться назад или сделать
//недопустимый переход. Мы не можем например перейти из доставленого в обработку,
//т.к. попросту не реализован такой переход.