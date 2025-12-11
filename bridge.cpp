#include <iostream>
#include <fstream>
#include <memory>
// паттерн, который разделяет абстракцию от реализации, чтобы их можно было
// изменять независимо друг от друга. 

// Паттерн Bridge. Создайте механизм логирования в интернет-магазине, используя 
// механизм создания объект-потомка, который будет реализовывать интерфейс логирования. 
// Этот объект -потомок может использовать абстрактный класс для определения общего 
// поведения логирования, а затем реализовывать этот объект -родитель с помощью 
// специфических методов логирования. 

class loggerRealization {
public:
    virtual void writeLog(std::string msg) = 0;
    virtual ~loggerRealization() = default;
};

class consoleLogger : public loggerRealization {
public:
    void writeLog(std::string msg) override {
        std::cout << "console: log: " << msg << "\n";
    }
};

class fileLogger : public loggerRealization {
public:
    void writeLog(std::string msg) override {
        std::ofstream logs("logs.txt", std::ios::app);
        if(logs.is_open()) {
            logs << msg << std::endl;
            logs.close();
        }
        else 
            std::cout << "Ошибка в открытии файла" << "\n";
    }
};

class logger {
protected:
    //доступ только у логгера и его наследников
    std::shared_ptr<loggerRealization> loggerRealiz;
public:
    logger(std::shared_ptr<loggerRealization> l) {
        loggerRealiz = l;
    }
    virtual void log(std::string msg) = 0;
    virtual ~logger() = default;
};

class orderLogger : public logger {
public: 
    orderLogger(std::shared_ptr<loggerRealization> l) : logger(l) {};
    void log(std::string msg) override {
        loggerRealiz->writeLog("Order: " + msg);
    }
};

class payLogger : public logger {
public: 
    payLogger(std::shared_ptr<loggerRealization> l) : logger(l) {};
    void log(std::string msg) override {
        loggerRealiz->writeLog("Payment: " + msg);
    }
};

int main() {
    auto console_logger = std::make_shared<consoleLogger>();
    auto file_logger = std::make_shared<fileLogger>();

    orderLogger order_logger(console_logger);
    payLogger pay_logger(file_logger);

    order_logger.log("Создан заказ: №1");
    pay_logger.log("Оплачен заказ: №1: успешно");
    return 0;
}