#include "CarCollection.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <cassert>
#include <fstream>
#include <sstream>

//  Вспомогательные функции для тестирования 
void printTestResult(const std::string& testName, bool passed) {
    if (passed) {
        std::cout << "  [PASS] " << testName << "\n";
    } else {
        std::cout << "  [FAIL] " << testName << "\n";
    }
}

void printSectionHeader(const std::string& sectionName) {
    std::cout << "\n" << sectionName << "\n";
    std::cout << std::string(sectionName.length(), '-') << "\n";
}


void runUnitTests() {
    std::cout << "=== ЗАПУСК UNIT-ТЕСТОВ ===\n\n";
    
    int passedTests = 0;
    int totalTests = 0;
    
    try {
        // ТЕСТ 1: Vehicle =
        printSectionHeader("1. ТЕСТИРОВАНИЕ VEHICLE");
        
        // Тест 1.1: Проверка Vehicle через Car
        {
            totalTests++;
            
            Car vehicle("Test", "Model", 2020, 1000.0,
                CarType::SCALE_MODEL, Condition::GOOD,
                "1:64", "Красный", false);
            
            // Проверяем, что геттеры Vehicle возвращают правильные данные
            assert(vehicle.getManufacturer() == "Test");
            assert(vehicle.getModel() == "Model");
            assert(vehicle.getYear() == 2020);
            assert(vehicle.getPrice() == 1000.0);
            
            // Проверяем, что данные не пустые
            assert(!vehicle.getManufacturer().empty());
            assert(!vehicle.getModel().empty());
            assert(vehicle.getYear() > 1900 && vehicle.getYear() < 2100);
            assert(vehicle.getPrice() >= 0);
            
            std::cout << "  Счетчик Vehicle: " << Vehicle::getVehicleCount() << "\n";
            
            passedTests++;
            printTestResult("Vehicle методы работают через Car", true);
        }
        
        // Тест 1.2: Операторы Vehicle 
        {
            totalTests++;
            Car v1("Toyota", "Supra", 1993, 25000.0,
                CarType::SCALE_MODEL, Condition::GOOD, "1:18", "Red", false);
            
            Car v2("Toyota", "Supra", 1993, 25000.0,
                CarType::SCALE_MODEL, Condition::GOOD, "1:18", "Red", false);
            
            Car v3("Honda", "Civic", 2020, 15000.0,
                CarType::DIE_CAST, Condition::EXCELLENT, "1:24", "Blue", false);
            
            // Приводим к Vehicle& для проверки операторов
            Vehicle& vehicle1 = v1;
            Vehicle& vehicle2 = v2;
            Vehicle& vehicle3 = v3;
            
            assert(vehicle1 == vehicle2);
            assert(vehicle1 != vehicle3);
            assert(vehicle3 < vehicle1); // Honda < Toyota
            
            passedTests++;
            printTestResult("Операторы сравнения Vehicle работают", true);
        }
        
        // ТЕСТ 2: Car 
        printSectionHeader("2. ТЕСТИРОВАНИЕ CAR");
        
        // Тест 2.1: Создание Car
        {
            totalTests++;
            Car car("Ferrari", "F40", 1987, 15000.0,
                CarType::SCALE_MODEL, Condition::MINT,
                "1:18", "Красный", true);
            
            // Проверка базовых полей (унаследованных от Vehicle)
            assert(car.getManufacturer() == "Ferrari");
            assert(car.getModel() == "F40");
            assert(car.getYear() == 1987);
            assert(car.getPrice() == 15000.0);
            
            // Проверка специфичных полей Car
            assert(car.getType() == CarType::SCALE_MODEL);
            assert(car.getCondition() == Condition::MINT);
            assert(car.getScale() == "1:18");
            assert(car.getColor() == "Красный");
            assert(car.isLimitedEdition() == true);
            
            passedTests++;
            printTestResult("Создание Car с полными данными", true);
        }
        
        // Тест 2.2: Методы Car
        {
            totalTests++;
            Car car("Porsche", "911", 1973, 12000.0,
                CarType::DIE_CAST, Condition::EXCELLENT,
                "1:24", "Серебристый", false);
            
            // Проверка displayInfo() - должен выполняться без ошибок
            std::cout << "  DisplayInfo output:\n";
            car.displayInfo(); 
            
            // Проверка toString() - должен возвращать не пустую строку с данными
            std::string carInfo = car.toString();
            assert(!carInfo.empty());
            assert(carInfo.find("Porsche") != std::string::npos);
            assert(carInfo.find("911") != std::string::npos);
            
            // Проверка calculateValue() - должно возвращать положительное число
            double value = car.calculateValue();
            assert(value > 0.0);
            
            // Проверка isValuable() - должен выполняться без ошибок
            bool isValuable = car.isValuable();
            
            passedTests++;
            printTestResult("Методы Car возвращают корректные данные", true);
        }
        
        // Тест 2.3: Клонирование Car
        {
            totalTests++;
            Car original("Lamborghini", "Countach", 1974, 18000.0,
                CarType::RADIO_CONTROLLED, Condition::GOOD,
                "1:32", "Белый", true);
            
            auto clone = original.clone();
            
            // Проверяем что clone возвращает указатель
            assert(clone != nullptr);
            
            // Динамически приводим к Car*
            Car* clonedCar = dynamic_cast<Car*>(clone.get());
            assert(clonedCar != nullptr);
            
            // Проверяем что данные скопированы правильно
            assert(clonedCar->getManufacturer() == original.getManufacturer());
            assert(clonedCar->getModel() == original.getModel());
            assert(clonedCar->getYear() == original.getYear());
            assert(clonedCar->getType() == original.getType());
            
            passedTests++;
            printTestResult("Клонирование создает идентичную копию", true);
        }
        
        // ТЕСТ 3: Collection 
        printSectionHeader("3. ТЕСТИРОВАНИЕ COLLECTION");
        
        // Тест 3.1: Базовые операции Collection
        {
            totalTests++;
            Collection<Car> collection("Тестовая коллекция");
            
            // Проверка начального состояния
            assert(collection.empty());
            assert(collection.size() == 0);
            assert(collection.getName() == "Тестовая коллекция");
            assert(collection.totalValue() == 0.0);
            
            passedTests++;
            printTestResult("Инициализация Collection", true);
        }
        
        // Тест 3.2: Добавление и удаление элементов
        {
            totalTests++;
            Collection<Car> collection("Операции с элементами");
            
            // Добавление элементов
            auto car1 = std::make_shared<Car>("Ferrari", "F40", 1987, 15000.0,
                CarType::SCALE_MODEL, Condition::MINT,
                "1:18", "Красный", true);
            
            auto car2 = std::make_shared<Car>("Porsche", "911", 1973, 12000.0,
                CarType::DIE_CAST, Condition::EXCELLENT,
                "1:24", "Серебристый", false);
            
            collection.addItem(car1);
            collection.addItem(car2);
            
            // Проверка добавления
            assert(!collection.empty());
            assert(collection.size() == 2);
            assert(collection.totalValue() == 15000.0 + 12000.0);
            
            // Проверка оператора []
            assert(collection[0]->getManufacturer() == "Ferrari");
            assert(collection[1]->getManufacturer() == "Porsche");
            
            // Удаление
            bool removed = collection.removeItem(0);
            assert(removed);
            assert(collection.size() == 1);
            assert(collection[0]->getManufacturer() == "Porsche");
            
            passedTests++;
            printTestResult("Добавление и удаление элементов", true);
        }
        
        // Тест 3.3: Поиск и фильтрация
        {
            totalTests++;
            Collection<Car> collection("Поиск и фильтрация");
            
            collection.addItem(std::make_shared<Car>("Ford", "Mustang", 1967, 8500.0,
                CarType::SCALE_MODEL, Condition::EXCELLENT,
                "1:18", "Синий", false));
            
            collection.addItem(std::make_shared<Car>("Ford", "Focus", 2000, 5000.0,
                CarType::DIE_CAST, Condition::GOOD,
                "1:32", "Красный", false));
            
            collection.addItem(std::make_shared<Car>("Chevrolet", "Camaro", 1969, 9000.0,
                CarType::RADIO_CONTROLLED, Condition::GOOD,
                "1:24", "Желтый", true));
            
            // Поиск по производителю
            auto fordCars = collection.findByManufacturer("Ford");
            assert(fordCars.size() == 2);
            
            // Фильтрация по типу
            auto scaleModels = collection.filterByType(CarType::SCALE_MODEL);
            assert(scaleModels.size() == 1);
            
            // Фильтрация по состоянию
            auto goodCars = collection.filterByCondition(Condition::GOOD);
            assert(goodCars.size() == 2);
            
            passedTests++;
            printTestResult("Поиск и фильтрация работают корректно", true);
        }
        
        // Тест 3.4: Сортировка
        {
            totalTests++;
            Collection<Car> collection("Сортировка");
            
            collection.addItem(std::make_shared<Car>("A", "Model", 2000, 10000.0,
                CarType::SCALE_MODEL, Condition::GOOD, "1:18", "Red", false));
            
            collection.addItem(std::make_shared<Car>("B", "Model", 1990, 8000.0,
                CarType::DIE_CAST, Condition::GOOD, "1:24", "Blue", false));
            
            collection.addItem(std::make_shared<Car>("C", "Model", 2010, 12000.0,
                CarType::RADIO_CONTROLLED, Condition::GOOD, "1:32", "Green", false));
            
            // Сортировка по году (возрастание)
            collection.sortByYear(true);
            assert(collection[0]->getYear() == 1990);
            assert(collection[1]->getYear() == 2000);
            assert(collection[2]->getYear() == 2010);
            
            // Сортировка по цене (убывание)
            collection.sortByPrice(false);
            assert(collection[0]->getPrice() == 12000.0);
            assert(collection[1]->getPrice() == 10000.0);
            assert(collection[2]->getPrice() == 8000.0);
            
            passedTests++;
            printTestResult("Сортировка работает корректно", true);
        }
        
        // Тест 3.5: Группировка
        {
            totalTests++;
            Collection<Car> collection("Группировка");
            
            collection.addItem(std::make_shared<Car>("Ford", "Mustang", 1967, 8500.0,
                CarType::SCALE_MODEL, Condition::EXCELLENT, "1:18", "Blue", false));
            
            collection.addItem(std::make_shared<Car>("Ford", "Focus", 2000, 5000.0,
                CarType::DIE_CAST, Condition::GOOD, "1:32", "Red", false));
            
            collection.addItem(std::make_shared<Car>("Chevrolet", "Camaro", 1969, 9000.0,
                CarType::SCALE_MODEL, Condition::GOOD, "1:24", "Yellow", true));
            
            // Группировка по производителю
            auto byManufacturer = collection.groupByManufacturer();
            assert(byManufacturer.size() == 2);
            assert(byManufacturer["Ford"].size() == 2);
            assert(byManufacturer["Chevrolet"].size() == 1);
            
            // Группировка по типу
            auto byType = collection.groupByType();
            assert(byType[CarType::SCALE_MODEL].size() == 2);
            assert(byType[CarType::DIE_CAST].size() == 1);
            
            // Группировка по состоянию
            auto byCondition = collection.groupByCondition();
            assert(byCondition[Condition::EXCELLENT].size() == 1);
            assert(byCondition[Condition::GOOD].size() == 2);
            
            passedTests++;
            printTestResult("Группировка работает корректно", true);
        }
        
        //  ТЕСТ 4: FileHandler 
        printSectionHeader("4. ТЕСТИРОВАНИЕ FILEHANDLER");
        
        // Тест 4.1: CSV экспорт/импорт
        {
            totalTests++;
            Collection<Car> collection("CSV Тест");
            
            // Используем английские названия для CSV
            collection.addItem(std::make_shared<Car>("Ferrari", "F40", 1987, 15000.0,
                CarType::SCALE_MODEL, Condition::MINT,
                "1:18", "Red", true));
            
            collection.addItem(std::make_shared<Car>("Porsche", "911", 1973, 12000.0,
                CarType::DIE_CAST, Condition::EXCELLENT,
                "1:24", "Silver", false));
            
            // Экспорт в CSV
            bool exportSuccess = FileHandler::exportToCSV(collection, "test_export.csv");
            assert(exportSuccess);
            
            // Проверка, что файл создан
            std::ifstream checkFile("test_export.csv");
            assert(checkFile.is_open());
            
            // Проверка заголовка CSV
            std::string header;
            std::getline(checkFile, header);
            assert(header == "Manufacturer;Model;Year;Price;Type;Condition;Scale;Color;LimitedEdition");
            checkFile.close();
            
            // Импорт из CSV
            Collection<Car> importedCollection("Импортированная");
            bool importSuccess = FileHandler::importFromCSV(importedCollection, "test_export.csv");
            assert(importSuccess);
            assert(importedCollection.size() == 2);
            
            // Проверка импортированных данных
            assert(importedCollection[0]->getManufacturer() == "Ferrari");
            assert(importedCollection[1]->getManufacturer() == "Porsche");
            
            // Очистка тестовых файлов
            remove("test_export.csv");
            
            passedTests++;
            printTestResult("CSV экспорт/импорт работает корректно", true);
        }
        
        // Тест 4.2: Бинарный формат
        {
            totalTests++;
            Collection<Car> collection("Бинарный тест");
            
            collection.addItem(std::make_shared<Car>("Toyota", "Supra", 1993, 25000.0,
                CarType::SCALE_MODEL, Condition::EXCELLENT,
                "1:18", "Red", true));
            
            // Сохранение в бинарный формат
            bool saveSuccess = FileHandler::saveToBinary(collection, "test_binary.bin");
            assert(saveSuccess);
            
            // Проверка, что файл создан
            std::ifstream checkFile("test_binary.bin", std::ios::binary);
            assert(checkFile.is_open());
            checkFile.close();
            
            // Загрузка из бинарного файла
            Collection<Car> loadedCollection("Загруженная");
            bool loadSuccess = FileHandler::loadFromBinary(loadedCollection, "test_binary.bin");
            assert(loadSuccess);
            assert(loadedCollection.size() == 1);
            
            // Проверка загруженных данных
            assert(loadedCollection[0]->getManufacturer() == "Toyota");
            assert(loadedCollection[0]->getModel() == "Supra");
            assert(loadedCollection[0]->getYear() == 1993);
            
            // Очистка тестовых файлов
            remove("test_binary.bin");
            
            passedTests++;
            printTestResult("Бинарный формат работает корректно", true);
        }
        
        // Тест 4.3: Проверка структуры CSV файла
        {
            totalTests++;
            Collection<Car> collection("Структура CSV");
            
            collection.addItem(std::make_shared<Car>("Test", "Model", 2023, 9999.99,
                CarType::CUSTOM_BUILD, Condition::GOOD,
                "1:10", "Black", true));
            
            // Экспорт
            bool exportResult = FileHandler::exportToCSV(collection, "test_structure.csv");
            assert(exportResult);
            
            // Проверка структуры файла
            std::ifstream file("test_structure.csv");
            assert(file.is_open());
            
            std::string line;
            int lineCount = 0;
            
            while (std::getline(file, line)) {
                lineCount++;
                if (lineCount == 1) {
                    // Проверка заголовка
                    assert(line.find("Manufacturer") != std::string::npos);
                    assert(line.find("Model") != std::string::npos);
                    assert(line.find("Year") != std::string::npos);
                    assert(line.find("Price") != std::string::npos);
                    assert(line.find("Type") != std::string::npos);
                    assert(line.find("Condition") != std::string::npos);
                    assert(line.find("Scale") != std::string::npos);
                    assert(line.find("Color") != std::string::npos);
                    assert(line.find("LimitedEdition") != std::string::npos);
                } else {
                    // Проверка данных
                    std::stringstream ss(line);
                    std::vector<std::string> columns;
                    std::string column;
                    
                    while (std::getline(ss, column, ';')) {
                        columns.push_back(column);
                    }
                    
                    // Должно быть 9 колонок 
                    assert(columns.size() == 9);
                    
                    // Проверка типов данных
                    assert(!columns[0].empty()); // Manufacturer
                    assert(!columns[1].empty()); // Model
                    assert(std::stoi(columns[2]) == 2023); // Year
                    assert(std::abs(std::stod(columns[3]) - 9999.99) < 0.01); // Price
                    assert(columns[4] == "Custom Build"); // Type
                    assert(columns[5] == "Good"); // Condition
                    assert(columns[6] == "1:10"); // Scale
                    assert(columns[7] == "Black"); // Color
                    assert(columns[8] == "Yes"); // LimitedEdition
                }
            }
            
            file.close();
            remove("test_structure.csv");
            
            passedTests++;
            printTestResult("CSV файл имеет правильную структуру", true);
        }
        
        //  ИТОГИ ТЕСТИРОВАНИЯ 
        printSectionHeader("ИТОГИ ТЕСТИРОВАНИЯ");
        std::cout << "Пройдено тестов: " << passedTests << " из " << totalTests << "\n";
        
        if (passedTests == totalTests) {
            std::cout << "\n=== ВСЕ ТЕСТЫ УСПЕШНО ПРОЙДЕНЫ ===\n";
        } else {
            std::cout << "\n=== НЕКОТОРЫЕ ТЕСТЫ НЕ ПРОЙДЕНЫ ===\n";
        }
        
    } catch (const std::exception& e) {
        std::cout << "\n!!! ОШИБКА В ТЕСТАХ: " << e.what() << " !!!\n";
        std::cout << "Пройдено тестов: " << passedTests << " из " << totalTests << "\n";
    } catch (...) {
        std::cout << "\n!!! НЕИЗВЕСТНАЯ ОШИБКА В ТЕСТАХ !!!\n";
        std::cout << "Пройдено тестов: " << passedTests << " из " << totalTests << "\n";
    }
}

//  Функции для пользовательского интерфейса 
int inputInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else {
            std::cout << "Ошибка ввода! Пожалуйста, введите число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double inputDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else {
            std::cout << "Ошибка ввода! Пожалуйста, введите число.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string inputString(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

CarType selectCarType() {
    std::cout << "\nВыберите тип машинки:\n";
    std::cout << "1. Масштабная модель\n";
    std::cout << "2. Литой металл\n";
    std::cout << "3. Радиоуправляемая\n";
    std::cout << "4. Электрическая модель\n";
    std::cout << "5. Кастомная сборка\n";

    int choice = inputInt("Ваш выбор (1-5): ");
    switch (choice) {
    case 1: return CarType::SCALE_MODEL;
    case 2: return CarType::DIE_CAST;
    case 3: return CarType::RADIO_CONTROLLED;
    case 4: return CarType::ELECTRIC_MODEL;
    case 5: return CarType::CUSTOM_BUILD;
    default: return CarType::SCALE_MODEL;
    }
}

Condition selectCondition() {
    std::cout << "\nВыберите состояние:\n";
    std::cout << "1. Идеальное\n";
    std::cout << "2. Отличное\n";
    std::cout << "3. Хорошее\n";
    std::cout << "4. Удовлетворительное\n";
    std::cout << "5. Плохое\n";

    int choice = inputInt("Ваш выбор (1-5): ");
    switch (choice) {
    case 1: return Condition::MINT;
    case 2: return Condition::EXCELLENT;
    case 3: return Condition::GOOD;
    case 4: return Condition::FAIR;
    case 5: return Condition::POOR;
    default: return Condition::GOOD;
    }
}

void addCar(Collection<Car>& collection) {
    std::cout << "\n=== Добавление новой машинки ===\n";

    std::string manufacturer = inputString("Производитель: ");
    std::string model = inputString("Модель: ");
    int year = inputInt("Год выпуска: ");
    double price = inputDouble("Цена (руб.): ");
    CarType type = selectCarType();
    Condition condition = selectCondition();
    std::string scale = inputString("Масштаб (например, 1:64): ");
    std::string color = inputString("Цвет: ");
    std::string limitedInput = inputString("Лимитированная серия? (да/нет): ");
    bool limitedEdition = (limitedInput == "да" || limitedInput == "yes" || limitedInput == "1" || limitedInput == "y");

    auto car = std::make_shared<Car>(manufacturer, model, year, price,
        type, condition, scale, color, limitedEdition);

    collection.addItem(car);
    std::cout << "Машинка успешно добавлена!\n";
}

void removeCar(Collection<Car>& collection) {
    if (collection.empty()) {
        std::cout << "Коллекция пуста!\n";
        return;
    }

    collection.displayAll();
    int index = inputInt("Введите номер машинки для удаления: ") - 1;

    try {
        if (collection.removeItem(index)) {
            std::cout << "Машинка успешно удалена!\n";
        }
    }
    catch (const std::out_of_range&) {
        std::cout << "Ошибка: неверный индекс!\n";
    }
}

void editCar(Collection<Car>& collection) {
    if (collection.empty()) {
        std::cout << "Коллекция пуста!\n";
        return;
    }

    collection.displayAll();
    int index = inputInt("Введите номер машинки для редактирования: ") - 1;

    try {
        // Получаем текущую машинку
        auto currentCar = collection[index];
        if (!currentCar) {
            std::cout << "Ошибка: машинка не найдена!\n";
            return;
        }

        std::cout << "\n=== Редактирование машинки ===\n";
        std::cout << "Текущие данные:\n";
        currentCar->displayInfo();
        std::cout << "\nВведите новые данные (оставьте поле пустым для сохранения старого значения):\n";

        // Производитель
        std::string manufacturer = inputString("Производитель [" + currentCar->getManufacturer() + "]: ");
        if (manufacturer.empty()) {
            manufacturer = currentCar->getManufacturer();
        }

        // Модель
        std::string model = inputString("Модель [" + currentCar->getModel() + "]: ");
        if (model.empty()) {
            model = currentCar->getModel();
        }

        // Год
        std::string yearStr = inputString("Год выпуска [" + std::to_string(currentCar->getYear()) + "] (введите число или нажмите Enter): ");
        int year = currentCar->getYear();
        if (!yearStr.empty()) {
            try {
                year = std::stoi(yearStr);
            }
            catch (...) {
                std::cout << "Неверный формат года, сохранено старое значение.\n";
            }
        }

        // Цена
        std::string priceStr = inputString("Цена [" + std::to_string(currentCar->getPrice()) + "] (введите число или нажмите Enter): ");
        double price = currentCar->getPrice();
        if (!priceStr.empty()) {
            try {
                price = std::stod(priceStr);
            }
            catch (...) {
                std::cout << "Неверный формат цены, сохранено старое значение.\n";
            }
        }

        // Тип
        std::cout << "\nТекущий тип: " << EnumUtils::carTypeToStr(currentCar->getType()) << "\n";
        std::cout << "Изменить тип? (да/нет): ";
        std::string changeType;
        std::getline(std::cin, changeType);
        CarType type = currentCar->getType();
        if (changeType == "да" || changeType == "yes" || changeType == "1" || changeType == "y") {
            type = selectCarType();
        }

        // Состояние
        std::cout << "\nТекущее состояние: " << EnumUtils::conditionToStr(currentCar->getCondition()) << "\n";
        std::cout << "Изменить состояние? (да/нет): ";
        std::string changeCondition;
        std::getline(std::cin, changeCondition);
        Condition condition = currentCar->getCondition();
        if (changeCondition == "да" || changeCondition == "yes" || changeCondition == "1" || changeCondition == "y") {
            condition = selectCondition();
        }

        // Масштаб
        std::string scale = inputString("Масштаб [" + currentCar->getScale() + "]: ");
        if (scale.empty()) {
            scale = currentCar->getScale();
        }

        // Цвет
        std::string color = inputString("Цвет [" + currentCar->getColor() + "]: ");
        if (color.empty()) {
            color = currentCar->getColor();
        }

        // Лимитированная серия
        std::cout << "\nТекущее значение лимитированной серии: " << (currentCar->isLimitedEdition() ? "Да" : "Нет") << "\n";
        std::cout << "Изменить? (да/нет): ";
        std::string changeLimited;
        std::getline(std::cin, changeLimited);
        bool limitedEdition = currentCar->isLimitedEdition();
        if (changeLimited == "да" || changeLimited == "yes" || changeLimited == "1" || changeLimited == "y") {
            std::string limitedInput = inputString("Лимитированная серия? (да/нет): ");
            limitedEdition = (limitedInput == "да" || limitedInput == "yes" || limitedInput == "1" || limitedInput == "y");
        }

        // Создаем новую машинку с обновленными данными
        auto updatedCar = std::make_shared<Car>(manufacturer, model, year, price,
            type, condition, scale, color, limitedEdition);

        // Заменяем старую машинку на новую
        if (collection.editItem(index, updatedCar)) {
            std::cout << "Машинка успешно отредактирована!\n";
        }
        else {
            std::cout << "Ошибка при редактировании машинки!\n";
        }
    }
    catch (const std::out_of_range&) {
        std::cout << "Ошибка: неверный индекс!\n";
    }
    catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
    }
}

void displayMenu() {
    std::cout << "\n════════════════════════════════════════\n";
    std::cout << "     КАТАЛОГ КОЛЛЕКЦИОННЫХ МАШИНОК      \n";
    std::cout << "════════════════════════════════════════\n";
    std::cout << "1.  Показать все машинки\n";
    std::cout << "2.  Добавить машинку\n";
    std::cout << "3.  Удалить машинку\n";
    std::cout << "4.  Редактировать машинку\n";
    std::cout << "5.  Поиск по производителю\n";
    std::cout << "6.  Фильтр по типу\n";
    std::cout << "7.  Фильтр по состоянию\n";
    std::cout << "8.  Сортировать по году\n";
    std::cout << "9.  Сортировать по цене\n";
    std::cout << "10. Группировать по производителю\n";
    std::cout << "11. Группировать по типу\n";
    std::cout << "12. Экспорт в CSV (английский)\n";
    std::cout << "13. Импорт из CSV (английский)\n";
    std::cout << "14. Сохранить в бинарный файл\n";
    std::cout << "15. Загрузить из бинарного файла\n";
    std::cout << "16. Показать статистику\n";
    std::cout << "17. Запустить unit-тесты\n";
    std::cout << "0.  Выход\n";
    std::cout << "════════════════════════════════════════\n";
    std::cout << "Выберите действие: ";
}

int main() {
    Collection<Car> collection("Моя коллекция машинок");

    int choice;
    do {
        displayMenu();
        choice = inputInt("");

        switch (choice) {
        case 1:
            collection.displayAll();
            break;

        case 2:
            addCar(collection);
            break;

        case 3:
            removeCar(collection);
            break;

        case 4:
            editCar(collection);
            break;

        case 5: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            std::string manufacturer = inputString("Введите производителя: ");
            auto results = collection.findByManufacturer(manufacturer);
            std::cout << "Найдено " << results.size() << " машинок:\n";
            for (size_t i = 0; i < results.size(); ++i) {
                results[i]->displayInfo();
                std::cout << "\n";
            }
            break;
        }

        case 6: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            CarType type = selectCarType();
            auto results = collection.filterByType(type);
            std::cout << "Найдено " << results.size() << " машинок:\n";
            for (size_t i = 0; i < results.size(); ++i) {
                results[i]->displayInfo();
                std::cout << "\n";
            }
            break;
        }

        case 7: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            Condition condition = selectCondition();
            auto results = collection.filterByCondition(condition);
            std::cout << "Найдено " << results.size() << " машинок:\n";
            for (size_t i = 0; i < results.size(); ++i) {
                results[i]->displayInfo();
                std::cout << "\n";
            }
            break;
        }

        case 8: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            std::cout << "\nСортировка по году:\n";
            std::cout << "1 - по возрастанию\n";
            std::cout << "2 - по убыванию\n";
            int order = inputInt("Ваш выбор: ");
            collection.sortByYear(order == 1);
            std::cout << "Коллекция отсортирована!\n";
            break;
        }

        case 9: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            std::cout << "\nСортировка по цене:\n";
            std::cout << "1 - по возрастанию\n";
            std::cout << "2 - по убыванию\n";
            int order = inputInt("Ваш выбор: ");
            collection.sortByPrice(order == 1);
            std::cout << "Коллекция отсортирована!\n";
            break;
        }

        case 10: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            std::cout << "\nГруппировка по производителям:\n";
            auto groups = collection.groupByManufacturer();
            for (auto it = groups.begin(); it != groups.end(); ++it) {
                std::cout << "  " << it->first << ": " << it->second.size() << " машинок\n";
            }
            break;
        }

        case 11: {
            if (collection.empty()) {
                std::cout << "Коллекция пуста!\n";
                break;
            }
            std::cout << "\nГруппировка по типу:\n";
            auto groups = collection.groupByType();
            for (auto it = groups.begin(); it != groups.end(); ++it) {
                std::cout << "  " << EnumUtils::carTypeToStr(it->first) << ": "
                    << it->second.size() << " машинок\n";
            }
            break;
        }

        case 12:
            if (collection.empty()) {
                std::cout << "Коллекция пуста, нечего экспортировать!\n";
                break;
            }
            if (FileHandler::exportToCSV(collection, "collection.csv")) {
                std::cout << "Экспорт успешно завершен в файл collection.csv!\n";
                std::cout << "  (CSV файл содержит данные на английском языке)\n";
            }
            else {
                std::cout << "Ошибка экспорта!\n";
            }
            break;

        case 13: {
            std::string filename = inputString("Введите имя файла для импорта (по умолчанию: collection.csv): ");
            if (filename.empty()) {
                filename = "collection.csv";
            }
            if (FileHandler::importFromCSV(collection, filename)) {
                std::cout << "Импорт успешно завершен из файла " << filename << "!\n";
                std::cout << "  (CSV файл должен содержать данные на английском языке)\n";
            }
            else {
                std::cout << "Ошибка импорта!\n";
            }
            break;
        }

        case 14:
            if (collection.empty()) {
                std::cout << "Коллекция пуста, нечего сохранять!\n";
                break;
            }
            if (FileHandler::saveToBinary(collection, "collection.bin")) {
                std::cout << "Сохранение успешно завершено в файл collection.bin!\n";
            }
            else {
                std::cout << "Ошибка сохранения!\n";
            }
            break;

        case 15: {
            std::string filename = inputString("Введите имя файла для загрузки (по умолчанию: collection.bin): ");
            if (filename.empty()) {
                filename = "collection.bin";
            }
            if (FileHandler::loadFromBinary(collection, filename)) {
                std::cout << "Загрузка успешно завершена из файла " << filename << "!\n";
            }
            else {
                std::cout << "Ошибка загрузки!\n";
            }
            break;
        }

        case 16:
            std::cout << "\n=== Статистика коллекции ===\n";
            std::cout << "Количество машинок: " << collection.size() << "\n";
            std::cout << "Общая стоимость: " << std::fixed << std::setprecision(2) << collection.totalValue() << " руб.\n";
            break;

        case 17:
            runUnitTests();
            break;

        case 0:
            std::cout << "\nСпасибо за использование программы!\n";
            std::cout << "До свидания!\n";
            break;

        default:
            std::cout << "Неверный выбор! Пожалуйста, выберите действие от 0 до 17.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}