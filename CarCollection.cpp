#include "CarCollection.h"

//  Vehicle реализация 
int Vehicle::vehicleCount = 0;

Vehicle::Vehicle() : manufacturer(""), model(""), year(0), price(0.0) {
    vehicleCount++;
}

Vehicle::Vehicle(const std::string& manufacturer, const std::string& model,
    int year, double price)
    : manufacturer(manufacturer), model(model), year(year), price(price) {
    vehicleCount++;
}

Vehicle::Vehicle(const Vehicle& other)
    : manufacturer(other.manufacturer), model(other.model),
    year(other.year), price(other.price) {
    vehicleCount++;
}

Vehicle::Vehicle(Vehicle&& other) noexcept
    : manufacturer(std::move(other.manufacturer)),
    model(std::move(other.model)),
    year(other.year),
    price(other.price) {
    other.year = 0;
    other.price = 0.0;
    vehicleCount++;
}

Vehicle& Vehicle::operator=(const Vehicle& other) {
    if (this != &other) {
        manufacturer = other.manufacturer;
        model = other.model;
        year = other.year;
        price = other.price;
    }
    return *this;
}

Vehicle& Vehicle::operator=(Vehicle&& other) noexcept {
    if (this != &other) {
        manufacturer = std::move(other.manufacturer);
        model = std::move(other.model);
        year = other.year;
        price = other.price;

        other.year = 0;
        other.price = 0.0;
    }
    return *this;
}

bool Vehicle::operator==(const Vehicle& other) const {
    return manufacturer == other.manufacturer &&
        model == other.model &&
        year == other.year &&
        price == other.price;
}

bool Vehicle::operator!=(const Vehicle& other) const {
    return !(*this == other);
}

bool Vehicle::operator<(const Vehicle& other) const {
    if (manufacturer != other.manufacturer) {
        return manufacturer < other.manufacturer;
    }
    if (model != other.model) {
        return model < other.model;
    }
    return year < other.year;
}

void Vehicle::updateInfo(const std::string& manufacturer, const std::string& model,
    int year, double price) {
    this->manufacturer = manufacturer;
    this->model = model;
    this->year = year;
    this->price = price;
}

void Vehicle::updateInfo(const std::string& manufacturer, const std::string& model) {
    this->manufacturer = manufacturer;
    this->model = model;
}

int Vehicle::getVehicleCount() {
    return vehicleCount;
}

void Vehicle::print(std::ostream& os) const {
    os << manufacturer << " " << model << " (" << year << ") - "
        << std::fixed << std::setprecision(2) << price << " руб.";
}

inline std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle) {
    vehicle.print(os);
    return os;
}

//  Car реализация 
Car::Car() : Vehicle(), type(CarType::SCALE_MODEL),
condition(Condition::GOOD), scale("1:64"),
color("Красный"), limitedEdition(false) {
}

Car::Car(const std::string& manufacturer, const std::string& model,
    int year, double price, CarType type, Condition condition,
    const std::string& scale, const std::string& color, bool limitedEdition)
    : Vehicle(manufacturer, model, year, price), type(type),
    condition(condition), scale(scale), color(color),
    limitedEdition(limitedEdition) {
}

Car::Car(const Car& other)
    : Vehicle(other), type(other.type), condition(other.condition),
    scale(other.scale), color(other.color),
    limitedEdition(other.limitedEdition) {
}

Car::Car(Car&& other) noexcept
    : Vehicle(std::move(other)), type(other.type), condition(other.condition),
    scale(std::move(other.scale)), color(std::move(other.color)),
    limitedEdition(other.limitedEdition) {
}

Car& Car::operator=(const Car& other) {
    if (this != &other) {
        Vehicle::operator=(other);
        type = other.type;
        condition = other.condition;
        scale = other.scale;
        color = other.color;
        limitedEdition = other.limitedEdition;
    }
    return *this;
}

Car& Car::operator=(Car&& other) noexcept {
    if (this != &other) {
        Vehicle::operator=(std::move(other));
        type = other.type;
        condition = other.condition;
        scale = std::move(other.scale);
        color = std::move(other.color);
        limitedEdition = other.limitedEdition;
    }
    return *this;
}

void Car::displayInfo() const {
    std::cout << toString() << std::endl;
}

std::string Car::toString() const {
    std::ostringstream oss;
    oss << manufacturer << " " << model << " (" << year << ")\n"
        << "Тип: " << EnumUtils::carTypeToStr(type) << "\n"
        << "Состояние: " << EnumUtils::conditionToStr(condition) << "\n"
        << "Масштаб: " << scale << "\n"
        << "Цвет: " << color << "\n"
        << "Лимитированная серия: " << (limitedEdition ? "Да" : "Нет") << "\n"
        << "Цена: " << std::fixed << std::setprecision(2) << price << " руб.";
    return oss.str();
}

std::unique_ptr<Vehicle> Car::clone() const {
    return std::make_unique<Car>(*this);
}

void Car::updateInfo(const std::string& manufacturer, const std::string& model,
    int year, double price, CarType type, Condition condition,
    const std::string& scale, const std::string& color, bool limitedEdition) {
    Vehicle::updateInfo(manufacturer, model, year, price);
    this->type = type;
    this->condition = condition;
    this->scale = scale;
    this->color = color;
    this->limitedEdition = limitedEdition;
}

double Car::calculateValue() const {
    double value = price;

    switch (condition) {
    case Condition::MINT:
        value *= MINT_CONDITION_BONUS;
        break;
    case Condition::EXCELLENT:
        value *= 1.1;
        break;
    case Condition::GOOD:
        break;
    case Condition::FAIR:
        value *= 0.8;
        break;
    case Condition::POOR:
        value *= 0.5;
        break;
    }

    if (limitedEdition) {
        value *= RARE_MULTIPLIER;
    }

    return value;
}

bool Car::isValuable() const {
    return calculateValue() > 10000.0;
}

void Car::print(std::ostream& os) const {
    Vehicle::print(os);
    os << " [" << EnumUtils::carTypeToStr(type)
        << ", " << EnumUtils::conditionToStr(condition) << "]";
}

inline std::ostream& operator<<(std::ostream& os, const Car& car) {
    car.print(os);
    return os;
}

//  FileHandler реализация 
bool FileHandler::exportToCSV(const Collection<Car>& collection, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    // Заголовок CSV на английском
    file << "Manufacturer;Model;Year;Price;Type;Condition;Scale;Color;LimitedEdition\n";

    // Данные на английском
    for (const auto& carPtr : collection) {
        const Car* car = dynamic_cast<Car*>(carPtr.get());
        if (car) {
            file << car->getManufacturer() << ";"
                << car->getModel() << ";"
                << car->getYear() << ";"
                << std::fixed << std::setprecision(2) << car->getPrice() << ";"
                << EnumUtils::carTypeToStrEN(car->getType()) << ";"
                << EnumUtils::conditionToStrEN(car->getCondition()) << ";"
                << car->getScale() << ";"
                << car->getColor() << ";"
                << (car->isLimitedEdition() ? "Yes" : "No") << "\n";
        }
    }

    file.close();
    return file.good();
}

bool FileHandler::importFromCSV(Collection<Car>& collection, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    std::string line;
    // Пропускаем заголовок
    if (!std::getline(file, line)) {
        std::cerr << "Файл пуст или не содержит заголовка\n";
        return false;
    }

    int lineNum = 1;
    while (std::getline(file, line)) {
        lineNum++;
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ';')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 9) {
            try {
                auto car = std::make_shared<Car>(
                    tokens[0], // manufacturer
                    tokens[1], // model
                    std::stoi(tokens[2]), // year
                    std::stod(tokens[3]), // price
                    EnumUtils::stringToCarType(tokens[4]), // type
                    EnumUtils::stringToCondition(tokens[5]), // condition
                    tokens[6], // scale
                    tokens[7], // color
                    tokens[8] == "Yes" || tokens[8] == "1" // limitedEdition
                );
                collection.addItem(car);
            }
            catch (const std::exception& e) {
                std::cerr << "Ошибка парсинга строки " << lineNum << ": " << line
                    << " - " << e.what() << std::endl;
                continue;
            }
        }
        else {
            std::cerr << "Неверное количество полей в строке " << lineNum
                << ": " << tokens.size() << " вместо 9\n";
        }
    }

    file.close();
    return true;
}

bool FileHandler::saveToBinary(const Collection<Car>& collection, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    size_t size = collection.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    for (const auto& carPtr : collection) {
        const Car* car = dynamic_cast<Car*>(carPtr.get());
        if (car) {
            std::string manufacturer = car->getManufacturer();
            std::string model = car->getModel();
            int year = car->getYear();
            double price = car->getPrice();
            CarType type = car->getType();
            Condition condition = car->getCondition();
            std::string scale = car->getScale();
            std::string color = car->getColor();
            bool limitedEdition = car->isLimitedEdition();

            size_t manufSize = manufacturer.size();
            file.write(reinterpret_cast<const char*>(&manufSize), sizeof(manufSize));
            file.write(manufacturer.c_str(), manufSize);

            size_t modelSize = model.size();
            file.write(reinterpret_cast<const char*>(&modelSize), sizeof(modelSize));
            file.write(model.c_str(), modelSize);

            file.write(reinterpret_cast<const char*>(&year), sizeof(year));
            file.write(reinterpret_cast<const char*>(&price), sizeof(price));
            file.write(reinterpret_cast<const char*>(&type), sizeof(type));
            file.write(reinterpret_cast<const char*>(&condition), sizeof(condition));

            size_t scaleSize = scale.size();
            file.write(reinterpret_cast<const char*>(&scaleSize), sizeof(scaleSize));
            file.write(scale.c_str(), scaleSize);

            size_t colorSize = color.size();
            file.write(reinterpret_cast<const char*>(&colorSize), sizeof(colorSize));
            file.write(color.c_str(), colorSize);

            file.write(reinterpret_cast<const char*>(&limitedEdition), sizeof(limitedEdition));
        }
    }

    file.close();
    return file.good();
}

bool FileHandler::loadFromBinary(Collection<Car>& collection, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));

    for (size_t i = 0; i < size; ++i) {
        size_t manufSize;
        file.read(reinterpret_cast<char*>(&manufSize), sizeof(manufSize));
        std::string manufacturer(manufSize, '\0');
        file.read(&manufacturer[0], manufSize);

        size_t modelSize;
        file.read(reinterpret_cast<char*>(&modelSize), sizeof(modelSize));
        std::string model(modelSize, '\0');
        file.read(&model[0], modelSize);

        int year;
        double price;
        CarType type;
        Condition condition;

        file.read(reinterpret_cast<char*>(&year), sizeof(year));
        file.read(reinterpret_cast<char*>(&price), sizeof(price));
        file.read(reinterpret_cast<char*>(&type), sizeof(type));
        file.read(reinterpret_cast<char*>(&condition), sizeof(condition));

        size_t scaleSize;
        file.read(reinterpret_cast<char*>(&scaleSize), sizeof(scaleSize));
        std::string scale(scaleSize, '\0');
        file.read(&scale[0], scaleSize);

        size_t colorSize;
        file.read(reinterpret_cast<char*>(&colorSize), sizeof(colorSize));
        std::string color(colorSize, '\0');
        file.read(&color[0], colorSize);

        bool limitedEdition;
        file.read(reinterpret_cast<char*>(&limitedEdition), sizeof(limitedEdition));

        auto car = std::make_shared<Car>(manufacturer, model, year, price,
            type, condition, scale, color, limitedEdition);
        collection.addItem(car);
    }

    file.close();
    return file.good();
}