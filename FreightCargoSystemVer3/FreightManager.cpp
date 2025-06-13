#include "FreightManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

void FreightManager::loadFromFile(const string& filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, dest, time;
        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, time, ',');
        freights.emplace_back(id, dest, time);
    }
    file.close();
}

void FreightManager::saveToFile(const string& filePath) const {
    ofstream file(filePath);
    for (const auto& f : freights)
        file << f.toCSV() << "\n";
    file.close();
}

void FreightManager::add(const Freight& freight) {
    freights.push_back(freight);
}

void FreightManager::edit(const string& id) {
    for (auto& f : freights) {
        if (f.getId() == id) {
            string dest, time;
            cout << "New destination (leave blank to keep \"" << f.getDestination() << "\"): ";
            getline(cin, dest);
            if (dest.empty()) dest = f.getDestination();

            cout << "New refuel time (leave blank to keep \"" << f.getTime() << "\"): ";
            getline(cin, time);
            if (time.empty()) time = f.getTime();

            f = Freight(id, dest, time);
            cout << "Freight updated successfully.\n";
            return;
        }
    }
    cout << "Freight not found.\n";
}

void FreightManager::remove(const string& id) {
    auto it = remove_if(freights.begin(), freights.end(), [&](const Freight& f) {
        return f.getId() == id;
        });
    if (it != freights.end()) {
        freights.erase(it, freights.end());
        cout << "Freight " << id << " removed successfully.\n";
    }
    else {
        cout << "Freight ID not found.\n";
    }
}

void FreightManager::displayAll() const {
    cout << left << setw(10) << "ID" << setw(15) << "Refuel Stop" << "Refuel Time\n";
    cout << "----------------------------------------\n";
    for (const auto& f : freights)
        f.display();
}

const vector<Freight>& FreightManager::getAll() const {
    return freights;
}
