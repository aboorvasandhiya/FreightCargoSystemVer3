#pragma once
#include "Cargo.h"
#include <vector>
#include <string>

class CargoManager {
private:
    std::vector<Cargo> cargos;
    Cargo* lastDeletedCargo = nullptr;

public:
    void loadFromFile(const std::string& filePath);
    void saveToFile(const std::string& filePath);
    void add(const Cargo& c);
    void edit(const std::string& id);
    void remove(const std::string& id);
    void displayAll() const;
    std::vector<Cargo>& getAll();
    void undoDelete();
};