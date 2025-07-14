#include "FreightManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void FreightManager::loadFromFile(const string& filePath) {
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cout << "Could not open file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, destination, time, type;
        getline(ss, id, ',');
        getline(ss, destination, ',');
        getline(ss, time, ',');
        getline(ss, type, ',');
        Freight f(id, destination, time, type);
        freights.push_back(f);
    }

    fin.close();
    cout << "Freight data loaded.\n";
}

void FreightManager::saveToFile(const string& filePath) {
    ofstream fout(filePath);
    for (const Freight& f : freights) {
        fout << f.toCSV() << endl;
    }
    fout.close();
}

void FreightManager::add(const Freight& f) {
    freights.push_back(f);
    cout << "Freight added.\n";
}

void FreightManager::edit(const string& id) {
    for (Freight& f : freights) {
        if (f.getId() == id) {
            string dest, time, type;
            cout << "Enter new destination: ";
            getline(cin, dest);
            cout << "Enter new time (HHMM): ";
            getline(cin, time);
            cout << "Enter new freight type: ";
            getline(cin, type);
            f = Freight(id, dest, time, type);
            cout << "Freight updated.\n";
            return;
        }
    }
    cout << "Freight not found.\n";
}

void FreightManager::remove(const string& id) {
    for (auto it = freights.begin(); it != freights.end(); ++it) {
        if (it->getId() == id) {
            if (lastDeletedFreight) delete lastDeletedFreight;
            lastDeletedFreight = new Freight(*it);
            freights.erase(it);
            cout << "Freight deleted.\n";
            return;
        }
    }
    cout << "Freight ID not found.\n";
}

void FreightManager::displayAll() const {
    for (const Freight& f : freights) {
        f.display();
    }
}

std::vector<Freight>& FreightManager::getAll() {
    return freights;
}

void FreightManager::undoDelete() {
    if (lastDeletedFreight) {
        freights.push_back(*lastDeletedFreight);
        delete lastDeletedFreight;
        lastDeletedFreight = nullptr;
        cout << "Last deleted freight restored.\n";
    }
    else {
        cout << "No freight to undo.\n";
    }
}