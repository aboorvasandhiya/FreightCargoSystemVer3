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
        cargos.push_back(Cargo(id, dest, time));
    }
    file.close();
}

void CargoManager::saveToFile(const string& filePath) const {
    ofstream fout(filePath);
    for (const auto& c : cargos) {
        fout << c.toCSV() << "\n";
    }
    fout.close();
}

void CargoManager::add(const Cargo& cargo) {
    bool valid = cargo.getId().length() >= 2 && cargo.getId()[0] == 'C';
    if (valid) {
        for (size_t i = 1; i < cargo.getId().length(); ++i) {
            if (!isdigit(cargo.getId()[i])) {
                valid = false;
                break;
            }
        }
    }
    bool exists = false;
    if (valid) {
        for (const auto& c : cargos) {
            if (c.getId() == cargo.getId()) {
                exists = true;
                break;
            }
        }
    }
    if (valid && !exists) {
        cargos.push_back(cargo);
        cout << "Cargo added successfully.\n";
    }
    else if (!valid) {
        cout << "\n----------------------------------------------\n";
        cout << "\nInvalid Cargo ID: Must start with 'C' followed by digits (e.g., C01, C10, C100, C1000).\n";
    }
    else {
        cout << "\n----------------------------------------------\n";
        cout << "\nCargo ID already exists.\n";
    }
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
            if (!time.empty()) {
                bool validTime = time.length() == 4;
                if (validTime) {
                    for (size_t i = 0; i < time.length(); ++i) {
                        if (!isdigit(time[i])) {
                            validTime = false;
                            break;
                        }
                    }
                    if (validTime) {
                        int hour = stoi(time.substr(0, 2));
                        int minute = stoi(time.substr(2, 2));
                        validTime = (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59);
                    }
                }
                if (!validTime) {
                    cout << "\n----------------------------------------------\n";
                    cout << "\nInvalid Arrival Time: Must be 4 digits in 24-hour format (0000-2359).\n";
                    return;
                }
            }
            if (time.empty()) time = c.getTime();

            c = Cargo(id, dest, time);
            cout << "Cargo updated successfully.\n";
            return;
        }
    }
    cout << "Cargo not found.\n";
}

void CargoManager::remove(const string& id) {
    for (auto it = cargos.begin(); it != cargos.end(); ++it) {
        if (it->getId() == id) {
            if (lastDeletedCargo) delete lastDeletedCargo;
            lastDeletedCargo = new Cargo(*it);
            cargos.erase(it);
            cout << "Cargo " << id << " deleted successfully.\n";
            return;
        }
    }
    cout << "Cargo ID not found.\n";
}

void CargoManager::undoDelete() {
    if (lastDeletedCargo) {
        cargos.push_back(*lastDeletedCargo);
        delete lastDeletedCargo;
        lastDeletedCargo = nullptr;
        cout << "Last deleted cargo restored.\n";
    }
    else {
        cout << "No cargo to undo.\n";
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