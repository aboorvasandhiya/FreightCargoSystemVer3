#pragma once
#include "Freight.h"
#include <vector>
#include <string>
using namespace std;

class FreightManager {
private:
    vector<Freight> freights;
    Freight* lastDeletedFreight = nullptr;

public:
    void loadFromFile(const string& filePath);
    void saveToFile(const string& filePath) const;

    void add(const Freight& freight);
    void edit(const string& id);
    void remove(const string& id);
    void displayAll() const;

    const vector<Freight>& getAll() const;

    void undoDelete();
};
