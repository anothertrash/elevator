#include <iostream>
#include <string>
#include <memory>

// 5.	Паттерн Decorator. Реализуйте модуль для интернет-магазин, который предлагает различные способы доставки заказов, 
// включая курьерскую доставку, почтовую доставку и самовывоз. Добавьте поддержку экспресс-доставки, которая обеспечивает 
// быструю доставку заказов в течение нескольких дней. 
// Guideline: Сначала опишите абстрактный класс "DeliverySystem", который будет определять общие методы для системы доставки 
// для заказа. Затем создайте класс-потомок "ExpressDeliverySystem", который будет наследоваться от абстрактного класса и 
// реализовывать его. Класс-потомок "ExpressDeliverySystem" будет иметь свои собственные методы для отслеживания статуса 
// доставки и расчета стоимости экспресс-доставки. Опишите заглушки для использования API курьерской службы для отслеживания 
// статуса доставки и расчета стоимости. Декоратор "DeliverySystem" будет добавлять новый метод для отслеживания статуса доставки 
// и расчета стоимости экспресс-доставки в систему доставки. Этот метод будет использовать класс-потомок "ExpressDeliverySystem" 
// для отслеживания статуса доставки и расчета стоимости экспресс-доставки. Добавьте остальные методы доставки без изменения основной 
// структуры системы доставки. 



class DeliverySystem {
public:
    virtual ~DeliverySystem() = default;

    virtual std::string getDescription() const = 0;
    virtual double calculateCost(double weight) const = 0;
    virtual void deliverOrder(int orderId) = 0;
};

class CourierDelivery : public DeliverySystem {
public:
    int baseCost = 400;
    std::string getDescription() const override {
        return "Курьерская доставка";
    }

    double calculateCost(double weight) const override {
        return baseCost + weight * 20;
    }

    void deliverOrder(int orderId) override {
        std::cout << " Доставка заказа: " << orderId << "\n";
    }
};

class PostalDelivery : public DeliverySystem {
public:
    int baseCost = 200;
    std::string getDescription() const override {
        return "Доставка почтой";
    }

    double calculateCost(double weight) const override {
        return baseCost + weight * 10;
    }

    void deliverOrder(int orderId) override {
        std::cout << "Заказ: " << orderId << " отправлен с почты\n";
    }
};

class PickupDelivery : public DeliverySystem {
public:
    std::string getDescription() const override {
        return "Самовывоз";
    }

    double calculateCost(double weight) const override {
        return 0.0;
    }

    void deliverOrder(int orderId) override {
        std::cout << "Заказ: " << orderId << " готов к выдаче\n";
    }
};


class ExpressDeliverySystem : public DeliverySystem {
public:
    std::string getDescription() const override {
        return "Экспресс доставка";
    }

    double calculateCost(double weight) const override {
        double baseCost = 800;
        double sum_to_kg = 50;
        return baseCost + weight * sum_to_kg;
    }

    void deliverOrder(int orderId) override {
        std::cout << "Заказ: " << orderId << " будет доставлен эксперссом\n";
    }

    std::string trackStatus(int orderId) const {
        // Заглушка для настоящего API
        return "Заказ: " + std::to_string(orderId) + " в процессе перевозки жкспрессом";
    }

    int estimateDeliveryDays() const {
        // Заглушка ответа API
        return 2;
    }
};

class DeliveryDecorator : public DeliverySystem {
protected:
    std::unique_ptr<DeliverySystem> base_;
public:
    DeliveryDecorator(std::unique_ptr<DeliverySystem> base)
        : base_(std::move(base)) {}
};

class ExpressDeliveryDecorator : public DeliveryDecorator {
private:
    ExpressDeliverySystem expressSystem;
public:
    ExpressDeliveryDecorator(std::unique_ptr<DeliverySystem> base)
        : DeliveryDecorator(std::move(base)) {}

    std::string getDescription() const override {
        return base_->getDescription() + " + Экспресс";
    }

    double calculateCost(double weight) const override {
        return base_->calculateCost(weight) + expressSystem.calculateCost(weight);
    }

    void deliverOrder(int orderId) override {
        base_->deliverOrder(orderId);
        std::cout << "Экспресс добавлен\n";
        expressSystem.deliverOrder(orderId);
    }

    std::string trackExpressStatus(int orderId) const {
        return expressSystem.trackStatus(orderId);
    }

    int getExpressDays() const {
        return expressSystem.estimateDeliveryDays();
    }
};
int main() {
    std::unique_ptr<DeliverySystem> delivery =
        std::make_unique<CourierDelivery>();

    ExpressDeliveryDecorator express(std::move(delivery));

    std::cout << express.getDescription() << "\n";

    double cost = express.calculateCost(3.0);
    std::cout << "Общая сумма: " << cost << "\n";

    express.deliverOrder(101);

    std::cout << "Экспресс статус: " << express.trackExpressStatus(101) << "\n";
    std::cout << "Осталось дней: " << express.getExpressDays() << "\n";

    return 0;
}