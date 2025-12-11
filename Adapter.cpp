#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
//Паттерн, с помощью которого можно встроить в систему новый объект, не подходя-
//щий к ней, чтобы он мог работать.

//Задача:
//Реализовать систему объектов «Транспорт», «Машина», «Дорога», «Осёл», «Седло». 
//Абстракция «Транспорт» содержит метод «Ехать», принимающая в себя интерфейс «Дорога». 
//Объёкт «Осёл», помимо своих свойств, из методов содержит только метод «Кушать». 
//Он не является "Транспортом", но при этом, благодаря объекту «Седло», 
//он приобретает его свойство. 


//от себя добавил механику скорости, сделал ослу энергию и голод, ограничение 
//скорости дороги, механику возможности везти груз.

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

class road {
public:
    virtual std::string getType() = 0;
    virtual int getSpeedLimit() = 0;
    virtual ~road() = default;
};

class asphalt : public road {
public:
    std::string getType() override { return "Асфальт"; }
    int getSpeedLimit() override { return 120; }
};

class dirt : public road {
public:
    std::string getType() override { return "Грунтовка"; }
    int getSpeedLimit() override { return 40; }
};

class transport {
public:
    virtual ~transport() = default;
    virtual void go(road& r, int s, int cargoWeight = 0) = 0;
};

class car : public transport {
    std::string name;
    int max_speed;
    int carWeight;
public:
    car(std::string model, int speed, int weight)
        : name(model), max_speed(speed), carWeight(weight) {}

    void go(road& r, int s, int cargoWeight = 0) override {
        if (cargoWeight > carWeight) {
            std::cout << name << " не может ехать: превышен вес груза (" << cargoWeight << " кг)\n";
            return;
        }
        int road_limit = r.getSpeedLimit();
        if (s > road_limit) {
            std::cout << name << " не может ехать быстрее " << road_limit << " км/ч (ограничение дороги)\n";
            s = road_limit;
        }
        if (s > max_speed) s = max_speed;
        std::cout << name << " едет по " << r.getType() << " со скоростью " << s
                  << " км/ч, везёт груз " << cargoWeight << " кг\n";
    }
};

class donkey {
    std::string name;
    int hunger = 100;
    int energy = 100;
    int maxWeight = 80;
public:
    donkey(std::string n) : name(n) {}

    bool canCarry(int weight) {
        return weight <= maxWeight;
    }

    void eat() {
        std::cout << name << " кушает траву...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        hunger += 25;
        if (hunger > 100) hunger = 100;
        std::cout << name << " поел. Голод: " << hunger << "\n";
    }

    void walkStep() {
        if (energy > 0 && hunger > 0) {
            energy -= 10;
            hunger -= 10;
            std::cout << name << " идёт... Энергия: " << energy << ", Голод: " << hunger << "\n";
        } else {
            std::cout << name << " устал или голоден. Нужен отдых или еда.\n";
        }
    }

    void rest() {
        std::cout << name << " отдыхает...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        energy += 20;
        if (energy > 100) energy = 100;
        std::cout << name << " восстановился. Энергия: " << energy << "\n";
    }
};

class saddle : public transport {
    std::shared_ptr<donkey> donk;
public:
    saddle(std::shared_ptr<donkey> d) : donk(d) {}

    void go(road& r, int s, int cargoWeight = 0) override {
        std::cout << "Осёл с седлом едет по " << r.getType() << "...\n";
        if (!donk->canCarry(cargoWeight)) {
            std::cout << "Осел не может нести такой груз (" << cargoWeight
                      << " кг). Макс: 80 кг\n";
            return;
        }
        int road_limit = r.getSpeedLimit();
        if (s > road_limit) {
            std::cout << "Осел не может идти быстрее " << road_limit
                      << " км/ч - максимальная скорость\n";
            s = road_limit;
        }
        if (s > 10) {
            std::cout << "Осел не может идти быстрее 10 км/ч. Скорость снижена.\n";
            s = 10;
        }
        std::cout << "Осел везет груз " << cargoWeight << " кг по " << r.getType()
                  << " со скоростью " << s << " км/ч\n";
        for (int i = 0; i < 3; ++i) {
            donk->walkStep();
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
        }
        std::cout << "Поездка завершена!\n";
    }
};

int main() {
    asphalt asphaltRoad;
    dirt dirtRoad;

    car bmw("BMW X5", 250, 500);
    bmw.go(asphaltRoad, 180, 300);
    bmw.go(dirtRoad, 70, 100);

    auto donk = std::make_shared<donkey>("Осёл");
    donk->eat();
    donk->rest();

    saddle sed(donk);
    sed.go(asphaltRoad, 20, 60);
    sed.go(dirtRoad, 8, 90);

    return 0;
}
