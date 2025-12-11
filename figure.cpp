#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::unique_ptr<Shape> clone() const = 0;
};

class Square : public Shape {
    int size;
public:
    Square(int s=2) : size(s) {}
    void draw() const override {
        std::cout << "Рисуем квадрат размером " << size << "\n";
    }
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Square>(*this);
    }
};

class Line : public Shape {
    int length;
public:
    Line(int l=3) : length(l) {}
    void draw() const override {
        std::cout << "Рисуем линию размером " << length << "\n";
    }
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Line>(*this);
    }
};

class SuperBlock : public Shape {
    int cells;
public:
    SuperBlock(int c=6) : cells(c) {}
    void draw() const override {
        std::cout << "Суперфигура " << cells << " клеток\n";
    }
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<SuperBlock>(*this);
    }
};

class ShapeFactory {
public:
    virtual ~ShapeFactory() = default;
    virtual std::unique_ptr<Shape> createShape() const = 0;
};

class RandomShapeFactory : public ShapeFactory {
    std::vector<std::unique_ptr<Shape>> prototypes;
public:
    RandomShapeFactory() {
        prototypes.push_back(std::make_unique<Square>(2));
        prototypes.push_back(std::make_unique<Line>(3));
        prototypes.push_back(std::make_unique<SuperBlock>(6));
    }

    std::unique_ptr<Shape> createShape() const override {
        int idx = rand() % prototypes.size();
        return prototypes[idx]->clone();
    }
};

int main() {
    srand((unsigned)time(nullptr));
    RandomShapeFactory factory;
    std::vector<std::unique_ptr<Shape>> shapes;
    for (int i=0; i<5; i++) {
        shapes.push_back(factory.createShape());
    }
    for (auto &s : shapes) {
        s->draw();
    }
    auto copy = shapes[0]->clone();
    copy->draw();

    return 0;
}
