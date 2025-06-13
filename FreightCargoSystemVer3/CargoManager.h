#pragma once
#include "Cargo.h"
#include <vector>
#include <string>
using namespace std;

class CargoManager {
private:
    vector<Cargo> cargos;

public:
    void loadFromFile(const string& filePath);
    void saveToFile(const string& filePath) const;

    void add(const Cargo& cargo);
    void edit(const string& id);
    void remove(const string& id);
    void displayAll() const;

    const vector<Cargo>& getAll() const;
};
