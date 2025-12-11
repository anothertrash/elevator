#include <iostream>
#include <vector>
#include <string>
#include <memory>
// Паттерн Observer. Реализовать систему уведомлений для интернет-магазина, 
// где клиенты могут подписываться на обновления статуса заказа. Создайте класс Order 
// (заказ), который будет иметь методы AddObserver(), RemoveObserver(), и NotifyObservers().  
// Реализуйте наблюдателей: ClientNotification(уведомление клиента), ManagerNotification(уведомление менеджера), 
// и AnalyticsSystem(аналитическая система).  Каждый раз, когда статус заказа меняется (например, 
// "Оформлен", "В обработке", "Отправлен", "Доставлен"), система должна уведомлять 
// всех подписанных наблюдателей. 
// Вопрос: Есть ли необходимость добавлениядополнительных 
// классов или методов для обеспечения безопасности? Почему?

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update(const std::string& status) = 0;
};


class Order {
private:
    std::vector<IObserver*> observers;
    std::string status;

public:
    void AddObserver(IObserver* obs) {
        observers.push_back(obs);
    }

    void RemoveObserver(IObserver* obs) {
        observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void SetStatus(const std::string& newStatus) {
        status = newStatus;
        NotifyObservers();
    }

    std::string GetStatus() const {
        return status;
    }

    void NotifyObservers() {
        for (auto* obs : observers) {
            obs->Update(status);
        }
    }
};


class ClientNotification : public IObserver {
public:
    void Update(const std::string& status) override {
        std::cout << "[Уведомление клиенту] Новый статус: " << status << "\n";
    }
};

class ManagerNotification : public IObserver {
public:
    void Update(const std::string& status) override {
        std::cout << "[Уведомление менеджеру] Статус заказа обновлён: " << status << "\n";
    }
};

class AnalyticsSystem : public IObserver {
public:
    void Update(const std::string& status) override {
        std::cout << "[Аналитика] Зафиксировано изменение статуса: " << status << "\n";
    }
};


int main() {
    Order order;

    ClientNotification client;
    ManagerNotification manager;
    AnalyticsSystem analytics;

    order.AddObserver(&client);
    order.AddObserver(&manager);
    order.AddObserver(&analytics);

    order.SetStatus("Оформлен");
    order.SetStatus("В обработке");
    order.SetStatus("Отправлен");
    order.SetStatus("Доставлен");

    order.RemoveObserver(&analytics);

    order.SetStatus("Завершён");

    return 0;
}

// Ответ на вопрос: да, желательно добавить. например, один пользователь может подписаться
// дважды. также если наблюдателя удалили, он все еще может получать уведомления итд. 