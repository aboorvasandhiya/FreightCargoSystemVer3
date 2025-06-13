#include "CargoManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

void CargoManager::loadFromFile(const string& filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, dest, time;
        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, time, ',');
        cargos.emplace_back(id, dest, time);
    }
    file.close();
}

void CargoManager::saveToFile(const string& filePath) const {
    ofstream file(filePath);
    for (const auto& c : cargos)
        file << c.toCSV() << "\n";
    file.close();
}

void CargoManager::add(const Cargo& cargo) {
    cargos.push_back(cargo);
}

void CargoManager::edit(const string& id) {
    for (auto& c : cargos) {
        if (c.getId() == id) {
            string dest, time;
            cout << "New destination (leave blank to keep \"" << c.getDestination() << "\"): ";
            getline(cin, dest);
            if (dest.empty()) dest = c.getDestination();

            cout << "New arrival time (leave blank to keep \"" << c.getTime() << "\"): ";
            getline(cin, time);
            if (time.empty()) time = c.getTime();

            c = Cargo(id, dest, time);
            cout << "Cargo updated successfully.\n";
            return;
        }
    }
    cout << "Cargo not found.\n";
}

void CargoManager::remove(const string& id) {
    auto it = remove_if(cargos.begin(), cargos.end(), [&](const Cargo& c) {
        return c.getId() == id;
        });
    if (it != cargos.end()) {
        cargos.erase(it, cargos.end());
        cout << "Cargo " << id << " removed successfully.\n";
    }
    else {
        cout << "Cargo ID not found.\n";
    }
}

void CargoManager::displayAll() const {
    cout << left << setw(10) << "ID" << setw(15) << "Destination" << "Arrival Time\n";
    cout << "----------------------------------------\n";
    for (const auto& c : cargos)
        c.display();
}

const vector<Cargo>& CargoManager::getAll() const {
    return cargos;
}
