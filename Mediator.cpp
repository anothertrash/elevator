#include <iostream>
#include <string>
// Паттерн Mediator. Реализовать систему управления заказами, где разные компоненты 
// (клиент, менеджер, склад) взаимодействуют через посредника.
// Создайте абстрактный класс Mediator с методами Notify() для взаимодействия между компонентами. 
// Реализуйте класс OrderMediator (посредник), который будет управлять взаимодействием 
// между Client (клиент), Manager (менеджер) и Warehouse(склад). 
// Каждый компонент должен иметь методы для отправки сообщений через посредника и 
// обработки сообщений от других компонентов. 
// Вопрос: Как вы обеспечите безопасность при обработке сообщений между компонентами? 
// Какие дополнительные проверки добавите?
class Component;

class Mediator {
public:
    virtual void notify(Component* sender, const std::string& message) = 0;
    virtual ~Mediator() = default;
};

class Component {
protected:
    Mediator* mediator;
public:
    Component(Mediator* med = nullptr) : mediator(med) {}
    void setMediator(Mediator* med) {
        mediator = med;
    }
    virtual void receiveMessage(const std::string& message) = 0;
    virtual ~Component() = default;
};

class Client : public Component {
public:
    using Component::Component;
    void sendMessage(const std::string& message) {
        if(mediator) 
            mediator->notify(this, message);
    }
    void receiveMessage(const std::string& message) override {
        std::cout << "Клиент получил: : " << message << "\n";
    }
};

class Manager : public Component {
public:
    using Component::Component;
    void sendMessage(const std::string& message) {
        if(mediator) 
            mediator->notify(this, message);
    }
    void receiveMessage(const std::string& message) override {
        std::cout << "Менеджер получил: " << message << "\n";
    }
};

class Warehouse : public Component {
public:
    using Component::Component;
    void sendMessage(const std::string& message) {
        if(mediator) mediator->notify(this, message);
    }
    void receiveMessage(const std::string& message) override {
        std::cout << "Склад получил: " << message << "\n";
    }
};

class OrderMediator : public Mediator {
private:
    Client* client = nullptr;
    Manager* manager = nullptr;
    Warehouse* warehouse = nullptr;

    bool IsSafeMessage(const std::string& message) {
        if(message.empty())
            return false;
        if(message.size() > 500) 
            return false;
        return true;
    }

public:
    void setClient(Client* c) {
        client = c;
    }
    void setManager(Manager* m) {
        manager = m;
    }
    void setWarehouse(Warehouse* w) {
        warehouse = w;
    }

    void notify(Component* sender, const std::string& message) override {
        if(!IsSafeMessage(message)) {
            std::cerr << "Ошибка сообщения\n";
            return;
        }

        if(sender == client && manager) {
            manager->receiveMessage("От клиента: " + message);
        } else if(sender == manager) {
            if(client)
                client->receiveMessage("От менеджера: " + message);
            if(warehouse)
                warehouse->receiveMessage("От менеджера: " + message);
        } else if(sender == warehouse && manager) {
            manager->receiveMessage("От склада: " + message);
        } else {
            std::cerr << "Неизвестный отправитель\n";
        }
    }
};

int main() {
    OrderMediator mediator;

    Client client(&mediator);
    Manager manager(&mediator);
    Warehouse warehouse(&mediator);

    mediator.setClient(&client);
    mediator.setManager(&manager);
    mediator.setWarehouse(&warehouse);

    client.sendMessage("Хочу сделать заказ");
    manager.sendMessage("Заказ получен, обработка");
    warehouse.sendMessage("Заказ собран и готов к доставке");

    client.sendMessage("");
    manager.sendMessage(std::string(600, ')'));

    return 0;
}

// Ответ на вопрос: проверка сообщения на пустоту и на длину, чтобы не переполнить
// буфер. Проверка на отправку от объекта. Можно все сообщения пихать в лог, например
// с указанием кто отправил, кто получил и время отправки и получения.