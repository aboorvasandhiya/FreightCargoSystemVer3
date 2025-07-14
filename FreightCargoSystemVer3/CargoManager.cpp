#include "CargoManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void CargoManager::loadFromFile(const string& filePath) {
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cout << "Could not open file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, dest, time, groupStr;
        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, time, ',');
        getline(ss, groupStr, ',');
        int groupSize = groupStr.empty() ? 1 : stoi(groupStr);
        cargos.emplace_back(id, dest, time, groupSize);
    }

    fin.close();
    cout << "Cargo data loaded.\n";
}

void CargoManager::saveToFile(const string& filePath) {
    ofstream fout(filePath);
    for (const Cargo& c : cargos) {
        fout << c.toCSV() << endl;
    }
    fout.close();
}

void CargoManager::add(const Cargo& c) {
    cargos.push_back(c);
    cout << "Cargo added.\n";
}

void CargoManager::edit(const string& id) {
    for (Cargo& c : cargos) {
        if (c.getId() == id) {
            string dest, time;
            int groupSize;
            cout << "Enter new destination: ";
            getline(cin, dest);
            cout << "Enter new time (HHMM): ";
            getline(cin, time);
            cout << "Enter group size (1-10): ";
            cin >> groupSize;
            cin.ignore();
            c = Cargo(id, dest, time, groupSize);
            cout << "Cargo updated.\n";
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
            cout << "Cargo deleted.\n";
            return;
        }
    }
    cout << "Cargo ID not found.\n";
}

void CargoManager::displayAll() const {
    for (const Cargo& c : cargos) {
        c.display();
    }
}

std::vector<Cargo>& CargoManager::getAll() {
    return cargos;
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