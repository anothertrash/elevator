#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// Паттерн Iterator. Реализовать систему каталога интернет-магазина, которая поддерживает 
// разные способы обхода товаров: по категориям, по цене, по популярности.
//  Создайте интерфейс CatalogIterator с методами HasNext(), Next(), Next(int count) 
//  и Reset(). Реализуйте классы итераторов: CategoryIterator(обход по категориям), 
//  PriceIterator (обход по цене), PopularityIterator (обход по популярности).  
//  Создайте класс Catalog (каталог), который будет использовать текущий итератор 
//  для обхода товаров.  Вопрос: Как вы будете обрабатывать ситуацию, когда в 
//  каталоге нет товаров, соответствующих определенному критерию? Какие изменения 
//  внесете в систему?


struct Product {
    std::string name;
    std::string category;
    double price;
    int popularity;
};

class CatalogIterator {
public:
    virtual ~CatalogIterator() = default;

    virtual bool HasNext() = 0;
    virtual Product Next() = 0;

    virtual std::vector<Product> Next(int count) = 0;

    virtual void Reset() = 0;
};

class CategoryIterator : public CatalogIterator {
private:
    std::vector<Product> filtered;
    int index = 0;

public:
    CategoryIterator(const std::vector<Product>& products, const std::string& category) {
        for (const auto& p : products)
            if (p.category == category)
                filtered.push_back(p);
    }

    bool HasNext() override {
        return index < filtered.size();
    }

    Product Next() override {
        return filtered[index++];
    }

    std::vector<Product> Next(int count) override {
        std::vector<Product> chunk;
        while (HasNext() && count--) {
            chunk.push_back(filtered[index++]);
        }
        return chunk;
    }

    void Reset() override {
        index = 0;
    }
};

class PriceIterator : public CatalogIterator {
private:
    std::vector<Product> sorted;
    int index = 0;

public:
    PriceIterator(std::vector<Product> products) {
        std::sort(products.begin(), products.end(),
                  [](const Product& a, const Product& b) { return a.price < b.price; });
        sorted = products;
    }

    bool HasNext() override {
        return index < sorted.size();
    }

    Product Next() override {
        return sorted[index++];
    }

    std::vector<Product> Next(int count) override {
        std::vector<Product> chunk;
        while (HasNext() && count--) {
            chunk.push_back(sorted[index++]);
        }
        return chunk;
    }

    void Reset() override {
        index = 0;
    }
};

class PopularityIterator : public CatalogIterator {
private:
    std::vector<Product> sorted;
    int index = 0;

public:
    PopularityIterator(std::vector<Product> products) {
        std::sort(products.begin(), products.end(),
                  [](const Product& a, const Product& b) { return a.popularity > b.popularity; });
        sorted = products;
    }

    bool HasNext() override {
        return index < sorted.size();
    }

    Product Next() override {
        return sorted[index++];
    }

    std::vector<Product> Next(int count) override {
        std::vector<Product> chunk;
        while (HasNext() && count--) {
            chunk.push_back(sorted[index++]);
        }
        return chunk;
    }

    void Reset() override {
        index = 0;
    }
};

class Catalog {
private:
    std::vector<Product> products;
    CatalogIterator* iterator = nullptr;

public:
    void AddProduct(const Product& p) {
        products.push_back(p);
    }

    void SetIterator(CatalogIterator* it) {
        iterator = it;
    }

    void ShowAll() {
        if (!iterator) {
            std::cout << "Итератор не установлен\n";
            return;
        }

        iterator->Reset();

        if (!iterator->HasNext()) {
            std::cout << "Нет товаров для отображения\n";
            return;
        }

        while (iterator->HasNext()) {
            Product p = iterator->Next();
            std::cout << p.name << " | " << p.category 
                      << " | " << p.price << " | популярность: " << p.popularity << "\n";
        }
    }

    std::vector<Product> GetProducts() const {
        return products;
    }
};

int main() {
    Catalog catalog;

    catalog.AddProduct({ "iPhone", "Электроника", 1000, 90 });
    catalog.AddProduct({ "Samsung", "Электроника", 800, 70 });
    catalog.AddProduct({ "Хлеб", "Food", 2, 50 });
    catalog.AddProduct({ "Молоко", "Food", 1, 40 });
    catalog.AddProduct({ "PS5 pro", "Электроника", 500, 95 });

    CategoryIterator catIt(catalog.GetProducts(), "Электроника");
    PriceIterator priceIt(catalog.GetProducts());
    PopularityIterator popIt(catalog.GetProducts());

    std::cout << "\n По категориям \n";
    catalog.SetIterator(&catIt);
    catalog.ShowAll();

    std::cout << "\n По цене \n";
    catalog.SetIterator(&priceIt);
    catalog.ShowAll();

    std::cout << "\n По популярности \n";
    catalog.SetIterator(&popIt);
    catalog.ShowAll();

    std::cout << "\n Категория 'овощи' (пусто) \n";
    CategoryIterator emptyIt(catalog.GetProducts(), "овощи");
    catalog.SetIterator(&emptyIt);
    catalog.ShowAll();

    return 0;
}

// Ответ на вопрос:чтобы это делать, надо чтобы итератор умел работать с пустым списком,
// каталог олжен проверять пустой результат перед выводом, никаких исключений кидать не надо.