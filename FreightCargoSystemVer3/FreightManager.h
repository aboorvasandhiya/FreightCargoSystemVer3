#pragma once
#include "Freight.h"
#include <vector>
#include <string>

class FreightManager {
private:
    std::vector<Freight> freights;
    Freight* lastDeletedFreight = nullptr;

public:
    void loadFromFile(const std::string& filePath);
    void saveToFile(const std::string& filePath);
    void add(const Freight& f);
    void edit(const std::string& id);
    void remove(const std::string& id);
    void displayAll() const;
    std::vector<Freight>& getAll();
    void undoDelete();
};