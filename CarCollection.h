#ifndef CAR_COLLECTION_H
#define CAR_COLLECTION_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <limits>


enum class CarType {
    SCALE_MODEL,
    DIE_CAST,
    RADIO_CONTROLLED,
    ELECTRIC_MODEL,
    CUSTOM_BUILD
};

enum class Condition {
    MINT,
    EXCELLENT,
    GOOD,
    FAIR,
    POOR
};

namespace EnumUtils {
    // Функции для консоли (русский язык)
    inline std::string carTypeToStr(CarType type) {
        static const std::map<CarType, std::string> carTypeToString = {
            {CarType::SCALE_MODEL, "Масштабная модель"},
            {CarType::DIE_CAST, "Литой металл"},
            {CarType::RADIO_CONTROLLED, "Радиоуправляемая"},
            {CarType::ELECTRIC_MODEL, "Электрическая модель"},
            {CarType::CUSTOM_BUILD, "Кастомная сборка"}
        };
        auto it = carTypeToString.find(type);
        return it != carTypeToString.end() ? it->second : "Неизвестно";
    }

    inline std::string conditionToStr(Condition cond) {
        static const std::map<Condition, std::string> conditionToString = {
            {Condition::MINT, "Идеальное"},
            {Condition::EXCELLENT, "Отличное"},
            {Condition::GOOD, "Хорошее"},
            {Condition::FAIR, "Удовлетворительное"},
            {Condition::POOR, "Плохое"}
        };
        auto it = conditionToString.find(cond);
        return it != conditionToString.end() ? it->second : "Неизвестно";
    }

    // Функции для CSV (английский язык)
    inline std::string carTypeToStrEN(CarType type) {
        static const std::map<CarType, std::string> carTypeToStringEN = {
            {CarType::SCALE_MODEL, "Scale Model"},
            {CarType::DIE_CAST, "Die Cast"},
            {CarType::RADIO_CONTROLLED, "Radio Controlled"},
            {CarType::ELECTRIC_MODEL, "Electric Model"},
            {CarType::CUSTOM_BUILD, "Custom Build"}
        };
        auto it = carTypeToStringEN.find(type);
        return it != carTypeToStringEN.end() ? it->second : "Unknown";
    }

    inline std::string conditionToStrEN(Condition cond) {
        static const std::map<Condition, std::string> conditionToStringEN = {
            {Condition::MINT, "Mint"},
            {Condition::EXCELLENT, "Excellent"},
            {Condition::GOOD, "Good"},
            {Condition::FAIR, "Fair"},
            {Condition::POOR, "Poor"}
        };
        auto it = conditionToStringEN.find(cond);
        return it != conditionToStringEN.end() ? it->second : "Unknown";
    }

    inline CarType stringToCarType(const std::string& str) {
        if (str == "Scale Model") return CarType::SCALE_MODEL;
        if (str == "Die Cast") return CarType::DIE_CAST;
        if (str == "Radio Controlled") return CarType::RADIO_CONTROLLED;
        if (str == "Electric Model") return CarType::ELECTRIC_MODEL;
        if (str == "Custom Build") return CarType::CUSTOM_BUILD;
        return CarType::SCALE_MODEL;
    }

    inline Condition stringToCondition(const std::string& str) {
        if (str == "Mint") return Condition::MINT;
        if (str == "Excellent") return Condition::EXCELLENT;
        if (str == "Good") return Condition::GOOD;
        if (str == "Fair") return Condition::FAIR;
        if (str == "Poor") return Condition::POOR;
        return Condition::GOOD;
    }
}

// Базовый класс Vehicle 
class Vehicle {
protected:
    std::string manufacturer;
    std::string model;
    int year;
    double price;

public:
    Vehicle();
    Vehicle(const std::string& manufacturer, const std::string& model,
        int year, double price);
    Vehicle(const Vehicle& other);
    Vehicle(Vehicle&& other) noexcept;
    virtual ~Vehicle() = default;

    Vehicle& operator=(const Vehicle& other);
    Vehicle& operator=(Vehicle&& other) noexcept;
    bool operator==(const Vehicle& other) const;
    bool operator!=(const Vehicle& other) const;
    bool operator<(const Vehicle& other) const;

    friend inline std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);

    std::string getManufacturer() const { return manufacturer; }
    std::string getModel() const { return model; }
    int getYear() const { return year; }
    double getPrice() const { return price; }

    void setManufacturer(const std::string& manufacturer) { this->manufacturer = manufacturer; }
    void setModel(const std::string& model) { this->model = model; }
    void setYear(int year) { this->year = year; }
    void setPrice(double price) { this->price = price; }

    virtual void displayInfo() const = 0;
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<Vehicle> clone() const = 0;

    virtual void updateInfo(const std::string& manufacturer, const std::string& model,
        int year, double price);
    virtual void updateInfo(const std::string& manufacturer, const std::string& model);

    static int getVehicleCount();

protected:
    virtual void print(std::ostream& os) const;

private:
    static int vehicleCount;
};

//  Класс Car 
class Car final : public Vehicle {
private:
    CarType type;
    Condition condition;
    std::string scale;
    std::string color;
    bool limitedEdition;

public:
    Car();
    Car(const std::string& manufacturer, const std::string& model,
        int year, double price, CarType type, Condition condition,
        const std::string& scale, const std::string& color, bool limitedEdition);
    Car(const Car& other);
    Car(Car&& other) noexcept;
    ~Car() override = default;

    Car& operator=(const Car& other);
    Car& operator=(Car&& other) noexcept;

    friend inline std::ostream& operator<<(std::ostream& os, const Car& car);

    CarType getType() const { return type; }
    Condition getCondition() const { return condition; }
    std::string getScale() const { return scale; }
    std::string getColor() const { return color; }
    bool isLimitedEdition() const { return limitedEdition; }

    void setType(CarType type) { this->type = type; }
    void setCondition(Condition condition) { this->condition = condition; }
    void setScale(const std::string& scale) { this->scale = scale; }
    void setColor(const std::string& color) { this->color = color; }
    void setLimitedEdition(bool limited) { limitedEdition = limited; }

    void displayInfo() const override;
    std::string toString() const override;
    std::unique_ptr<Vehicle> clone() const override;

    using Vehicle::updateInfo;
    void updateInfo(const std::string& manufacturer, const std::string& model,
        int year, double price, CarType type, Condition condition,
        const std::string& scale, const std::string& color, bool limitedEdition);

    double calculateValue() const;
    bool isValuable() const;

    static constexpr double RARE_MULTIPLIER = 1.5;
    static constexpr double MINT_CONDITION_BONUS = 1.3;

private:
    void print(std::ostream& os) const override;
};

// Шаблонный класс Collection 
template<typename T>
class Collection {
private:
    std::vector<std::shared_ptr<T>> items;
    std::string name;

public:
    Collection() = default;
    explicit Collection(const std::string& name) : name(name) {}
    ~Collection() = default;

    void addItem(std::shared_ptr<T> item);
    bool removeItem(size_t index);
    void clear();

    std::vector<std::shared_ptr<T>> findByManufacturer(const std::string& manufacturer) const;
    std::vector<std::shared_ptr<T>> filterByCondition(Condition condition) const;
    std::vector<std::shared_ptr<T>> filterByType(CarType type) const;

    void sortByYear(bool ascending = true);
    void sortByPrice(bool ascending = true);
    void sortByManufacturer(bool ascending = true);

    std::map<std::string, std::vector<std::shared_ptr<T>>> groupByManufacturer() const;
    std::map<CarType, std::vector<std::shared_ptr<T>>> groupByType() const;
    std::map<Condition, std::vector<std::shared_ptr<T>>> groupByCondition() const;

    size_t size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    double totalValue() const;

    std::shared_ptr<T> operator[](size_t index) const;
    Collection<T>& operator+=(std::shared_ptr<T> item);
    Collection<T>& operator-=(size_t index);

    auto begin() { return items.begin(); }
    auto end() { return items.end(); }
    auto begin() const { return items.begin(); }
    auto end() const { return items.end(); }
    auto cbegin() const { return items.cbegin(); }
    auto cend() const { return items.cend(); }

    std::string getName() const { return name; }
    void setName(const std::string& name) { this->name = name; }

    void displayAll() const;

    // Метод для редактирования элемента
    bool editItem(size_t index, std::shared_ptr<T> newItem) {
        if (index >= items.size() || !newItem) {
            return false;
        }
        items[index] = newItem;
        return true;
    }

private:
    void checkIndex(size_t index) const;
};

// Реализация методов шаблонного класса 
template<typename T>
void Collection<T>::addItem(std::shared_ptr<T> item) {
    if (!item) {
        throw std::invalid_argument("Cannot add null item to collection");
    }
    items.push_back(item);
}

template<typename T>
bool Collection<T>::removeItem(size_t index) {
    checkIndex(index);
    items.erase(items.begin() + index);
    return true;
}

template<typename T>
void Collection<T>::clear() {
    items.clear();
}

template<typename T>
std::vector<std::shared_ptr<T>> Collection<T>::findByManufacturer(const std::string& manufacturer) const {
    std::vector<std::shared_ptr<T>> result;
    std::copy_if(items.begin(), items.end(), std::back_inserter(result),
        [&manufacturer](const std::shared_ptr<T>& item) {
            return item->getManufacturer() == manufacturer;
        });
    return result;
}

template<typename T>
std::vector<std::shared_ptr<T>> Collection<T>::filterByCondition(Condition condition) const {
    std::vector<std::shared_ptr<T>> result;
    std::copy_if(items.begin(), items.end(), std::back_inserter(result),
        [condition](const std::shared_ptr<T>& item) {
            Car* car = dynamic_cast<Car*>(item.get());
            return car && car->getCondition() == condition;
        });
    return result;
}

template<typename T>
std::vector<std::shared_ptr<T>> Collection<T>::filterByType(CarType type) const {
    std::vector<std::shared_ptr<T>> result;
    std::copy_if(items.begin(), items.end(), std::back_inserter(result),
        [type](const std::shared_ptr<T>& item) {
            Car* car = dynamic_cast<Car*>(item.get());
            return car && car->getType() == type;
        });
    return result;
}

template<typename T>
void Collection<T>::sortByYear(bool ascending) {
    std::sort(items.begin(), items.end(),
        [ascending](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return ascending ? a->getYear() < b->getYear() : a->getYear() > b->getYear();
        });
}

template<typename T>
void Collection<T>::sortByPrice(bool ascending) {
    std::sort(items.begin(), items.end(),
        [ascending](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return ascending ? a->getPrice() < b->getPrice() : a->getPrice() > b->getPrice();
        });
}

template<typename T>
void Collection<T>::sortByManufacturer(bool ascending) {
    std::sort(items.begin(), items.end(),
        [ascending](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return ascending ? a->getManufacturer() < b->getManufacturer()
                : a->getManufacturer() > b->getManufacturer();
        });
}

template<typename T>
std::map<std::string, std::vector<std::shared_ptr<T>>> Collection<T>::groupByManufacturer() const {
    std::map<std::string, std::vector<std::shared_ptr<T>>> groups;
    for (const auto& item : items) {
        groups[item->getManufacturer()].push_back(item);
    }
    return groups;
}

template<typename T>
std::map<CarType, std::vector<std::shared_ptr<T>>> Collection<T>::groupByType() const {
    std::map<CarType, std::vector<std::shared_ptr<T>>> groups;
    for (const auto& item : items) {
        Car* car = dynamic_cast<Car*>(item.get());
        if (car) {
            groups[car->getType()].push_back(item);
        }
    }
    return groups;
}

template<typename T>
std::map<Condition, std::vector<std::shared_ptr<T>>> Collection<T>::groupByCondition() const {
    std::map<Condition, std::vector<std::shared_ptr<T>>> groups;
    for (const auto& item : items) {
        Car* car = dynamic_cast<Car*>(item.get());
        if (car) {
            groups[car->getCondition()].push_back(item);
        }
    }
    return groups;
}

template<typename T>
double Collection<T>::totalValue() const {
    double total = 0.0;
    for (const auto& item : items) {
        total += item->getPrice();
    }
    return total;
}

template<typename T>
std::shared_ptr<T> Collection<T>::operator[](size_t index) const {
    checkIndex(index);
    return items[index];
}

template<typename T>
Collection<T>& Collection<T>::operator+=(std::shared_ptr<T> item) {
    addItem(item);
    return *this;
}

template<typename T>
Collection<T>& Collection<T>::operator-=(size_t index) {
    removeItem(index);
    return *this;
}

template<typename T>
void Collection<T>::displayAll() const {
    std::cout << "\n=== Коллекция: " << name << " ===\n";
    std::cout << "Количество машинок: " << items.size() << "\n";
    std::cout << "Общая стоимость: " << std::fixed << std::setprecision(2) << totalValue() << " руб.\n";
    std::cout << "========================================\n";

    if (items.empty()) {
        std::cout << "Коллекция пуста.\n";
        return;
    }

    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << i + 1 << ". ";
        items[i]->displayInfo();
        std::cout << "\n";
    }
}

template<typename T>
void Collection<T>::checkIndex(size_t index) const {
    if (index >= items.size()) {
        throw std::out_of_range("Index out of range");
    }
}

//  Класс FileHandler
class FileHandler {
public:
    static bool exportToCSV(const Collection<Car>& collection, const std::string& filename);
    static bool importFromCSV(Collection<Car>& collection, const std::string& filename);
    static bool saveToBinary(const Collection<Car>& collection, const std::string& filename);
    static bool loadFromBinary(Collection<Car>& collection, const std::string& filename);
};

#endif // CAR_COLLECTION_H