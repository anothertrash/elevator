#include <iostream>
#include <memory>
#include <string>
// Паттерн Chain of Responsibility. Реализовать систему обработки запросов на 
// возврат товара. Запрос должен передаваться по цепочке обработчиков: менеджер, 
// руководитель, служба поддержки. Создайте абстрактный класс Handler с методом 
// HandleRequest(). Реализуйте конкретные обработчики: ManagerHandler, SupervisorHandler, 
// SupportHandler. Каждый обработчик должен решать, обрабатывать ли текущий запрос 
// или передать его следующему обработчику. 
// Вопрос: Как вы будете обрабатывать ситуацию, когда запрос не обрабатывается ни 
// одним из обработчиков? Какие изменения внесете в систему?

class Handler {
protected:
    std::shared_ptr<Handler> nextHandler;
public:
    void setNext(std::shared_ptr<Handler> next) {
        nextHandler = next;
    }
    virtual void handleRequest(const std::string& request) = 0;
    virtual ~Handler() = default;
};

class ManagerHandler : public Handler {
public:
    void handleRequest(const std::string& request) override {
        if(request == "simple_return") {
            std::cout << "Менеджер обрабатывет запрос \n";
        } else if(nextHandler) {
            nextHandler->handleRequest(request);
        } else {
            std::cout << "Запрос не обработан \n";
        }
    }
};

class SupervisorHandler : public Handler {
public:
    void handleRequest(const std::string& request) override {
        if(request == "medium_return") {
            std::cout << "Руководитель обрабатывет запрос \n";
        } else if(nextHandler) {
            nextHandler->handleRequest(request);
        } else {
            std::cout << "Запрос не обработан \n";
        }
    }
};

class SupportHandler : public Handler {
public:
    void handleRequest(const std::string& request) override {
        if(request == "complex_return") {
            std::cout << "Поддержка обрабатывает запрос \n";
        } else if(nextHandler) {
            nextHandler->handleRequest(request);
        } else {
            std::cout << "Запрос не обработан \n";
        }
    }
};

class DefaultHandler : public Handler {
public:
    void handleRequest(const std::string& request) override {
        std::cout << "Запрос требует ручной проверки \n";
    }
};
int main() {
    auto manager = std::make_shared<ManagerHandler>();
    auto supervisor = std::make_shared<SupervisorHandler>();
    auto support = std::make_shared<SupportHandler>();
    auto defaultHandler = std::make_shared<DefaultHandler>();

    manager->setNext(supervisor);
    supervisor->setNext(support);
    support->setNext(defaultHandler);

    manager->handleRequest("simple_return");
    manager->handleRequest("medium_return");
    manager->handleRequest("complex_return");
    manager->handleRequest("unknown_request");
    return 0;
}

// Ответ на вопрос: добавил специальный обработчик котороый принимает все необработанные
// запросы. В дальнейшем их все можно запихивать в лог с указанием времени и типа запроса.
// При надобности можно уведомлять медежжера о необходимости ручной обработки запроса.