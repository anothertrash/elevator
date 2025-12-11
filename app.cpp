
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <algorithm>

class Room {
public:
    Room(const std::string &n) : name(n) {};
    const std::string& get_name() const { 
        return name;
    }
private:
    std::string name;
};

class Floor {
public:
    void addRoom(const Room& r) {
        rooms.push_back(r);
    }
    int get_level() {
        return level;
    }
    void printInfo() {
        std::cout << " Этаж " << level << ", помещения:";
        if (rooms.empty()) {
            std::cout << "Нет помещений" << std::endl;
            return;
        }
        for (auto &r : rooms) 
            std::cout << " " << r.get_name() << " .";
        std::cout << "\n";
    }
private:
    int level;
    std::vector<Room> rooms;
};

class Elevator;

class Building {
public:
    static Building& getInstance() {
        static Building instance;
        return instance;
    }

    Building(const Building&) = delete;
    Building& operator=(const Building&) = delete;
    Building(Building&&) = delete;
    Building& operator=(Building&&) = delete;

    void addFloor(int level) {
        for (auto &f : floors) if (f.get_level() == level) return;
        floors.emplace_back(level);
    }

    void addRoomToFloor(int level, const Room& room) {
        for (auto &f : floors) {
            if (f.get_level() == level) {
                f.addRoom(room);
                return;
            }
        }
        floors.emplace_back(level);
        floors.back().addRoom(room);
    }

    int minFloor() {
        if (floors.empty()) return 0;
        int mn = floors[0].get_level();
        for (auto &f : floors) 
            if (f.get_level() < mn) 
                mn = f.get_level();
        return mn;
    }

    int maxFloor() {
        if (floors.empty()) return 0;
        int mx = floors[0].get_level();
        for (auto &f : floors) 
            if (f.get_level() > mx) 
                mx = f.get_level();
        return mx;
    }

    void printInfo() {
        std::cout << " Здание\n";
        if (floors.empty()) {
            std::cout << "(нет этажей)\n";
            return;
        }
        std::vector<Floor> copy = floors;
        std::sort(copy.begin(), copy.end(), [](Floor& a, Floor& b) { 
            return a.get_level() < b.get_level(); 
        });
        for (auto &f : copy) 
            f.printInfo();
    }

private:
    Building() = default;
    ~Building() = default;

    std::vector<Floor> floors;
};

class Elevator {
public:
    static Elevator& getInstance() {
        static Elevator instance;
        return instance;
    }

    Elevator(const Elevator&) = delete;
    Elevator& operator=(const Elevator&) = delete;
    Elevator(Elevator&&) = delete;
    Elevator& operator=(Elevator&&) = delete;

    void callTo(int floor) {
        Building& b = Building::getInstance();
        int minF = b.minFloor();
        int maxF = b.maxFloor();
        if (bHasNoFloors) {
            std::cerr << "Здание не содержит этажей. Нельзя вызвать лифт\n";
        } else {
            if (floor < minF || floor > maxF) {
                throw std::out_of_range("Запрошенный этаж вне диапазона здания.");
            }
        }
        std::cout << "Лифт вызывается на этаж " << floor << "...\n";
        moveTo(floor);
    }

    void moveTo(int floor) {
        Building& b = Building::getInstance();
        int minF = b.minFloor();
        int maxF = b.maxFloor();
        if (!bHasNoFloors && (floor < minF || floor > maxF)) {
            throw std::out_of_range("moveTo: этаж вне диапазона здания.");
        }
        if (currentFloor == floor) {
            std::cout << "Лифт уже на этаже " << floor << "\n";
            return;
        }
        std::cout << "Лифт перемещается с " << currentFloor << " на " << floor << "\n";
        currentFloor = floor;
        std::cout << "Лифт прибыл на этаж " << currentFloor << "\n";
    }

    int get_currentFloor() const { 
        return currentFloor; 
    }

    void setInitialIfBuildingEmpty(bool val) { 
        bHasNoFloors = val; 
    }

    void printStatus() const {
        std::cout << " Лифт \n";
        std::cout << " Текущий этаж: " << currentFloor << "\n";
    }

private:
    Elevator() : currentFloor(0), bHasNoFloors(false) {}
    ~Elevator() = default;

    int currentFloor = 0;
    bool bHasNoFloors = false;
};

int main() {
    try {
        Building& building = Building::getInstance();

        building.addFloor(1);
        building.addFloor(2);
        building.addFloor(3);
        building.addRoomToFloor(1, Room("101"));
        building.addRoomToFloor(1, Room("102"));
        building.addRoomToFloor(2, Room("201"));
        building.addRoomToFloor(3, Room("301"));

        building.printInfo();
        Elevator& lift = Elevator::getInstance();
        lift.callTo(2);
        lift.moveTo(3);
        lift.printStatus();

        try {
            lift.callTo(10);
        } catch (const std::exception &ex) {
            std::cout << "Ошибка: " << ex.what() << "\n";
        }

    } catch (const std::exception &ex) {
        std::cerr << "Исключение: " << ex.what() << "\n";
    }
    return 0;
}
