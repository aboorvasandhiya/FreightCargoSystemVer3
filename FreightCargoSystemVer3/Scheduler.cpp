#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

void Scheduler::loadFreights(const string& filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, dest, time;
        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, time, ',');
        bool valid = id.length() >= 2 && id[0] == 'F';
        if (valid) {
            for (size_t i = 1; i < id.length(); ++i) {
                if (!isdigit(id[i])) {
                    valid = false;
                    break;
                }
            }
        }
        bool exists = false;
        if (valid) {
            for (const auto& f : freights) {
                if (f.getId() == id) {
                    exists = true;
                    break;
                }
            }
        }
        if (valid && !exists) {
            freights.push_back(Freight(id, dest, time));
        }
        else if (!valid) {
            cout << "Skipping invalid Freight ID: " << id << " in " << filePath << "\n";
        }
        else {
            cout << "Skipping duplicate Freight ID: " << id << " in " << filePath << "\n";
        }
    }
    file.close();
}

void Scheduler::loadCargos(const string& filePath) {
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

void Scheduler::generateSchedule() {
    assignments.clear();
    for (auto& f : freights) {
        for (auto& c : cargos) {
            if (f.matches(c)) {
                assignments[f.getId()] = c.getId();
                break;
            }
        }
    }
}

void Scheduler::displayUnassigned() const {
    cout << "Unassigned Freights:\n";
    for (auto& f : freights) {
        if (assignments.find(f.getId()) == assignments.end())
            cout << f.getId() << " (" << f.getDestination() << " at " << f.getTime() << ")\n";
    }

    cout << "\nUnassigned Cargos:\n";
    for (auto& c : cargos) {
        bool assigned = false;
        for (auto& pair : assignments) {
            if (pair.second == c.getId()) {
                assigned = true;
                break;
            }
        }
        if (!assigned)
            cout << c.getId() << " (" << c.getDestination() << " at " << c.getTime() << ")\n";
    }
}

void Scheduler::saveSchedule(const string& filePath) const {
    ofstream file(filePath);
    file << "# Format: FreightID -> CargoID\n";
    for (auto& pair : assignments) {
        file << pair.first << " -> " << pair.second << "\n";
    }
    file.close();
}

void Scheduler::addFreight(Freight freight) {
    freights.push_back(freight);
}

void Scheduler::editFreight(const string& id) {
    for (auto& f : freights) {
        if (f.getId() == id) {
            string dest, time;
            cin.ignore(); // jus to make sure getline works if need

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

void Scheduler::deleteFreight(const string& id) {
    freights.erase(remove_if(freights.begin(), freights.end(),
        [&](Freight& f) { return f.getId() == id; }), freights.end());
}

void Scheduler::addCargo(Cargo cargo) {
    cargos.push_back(cargo);
}

void Scheduler::editCargo(const string& id) {
    for (auto& c : cargos) {
        if (c.getId() == id) {
            string dest, time;
            cin.ignore(); // jus to make sure getline works if need

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

void Scheduler::deleteCargo(const string& id) {
    cargos.erase(remove_if(cargos.begin(), cargos.end(),
        [&](Cargo& c) { return c.getId() == id; }), cargos.end());
}

void Scheduler::displayAllFreights() const {
    cout << left << setw(10) << "ID" << setw(15) << "Refuel Stop" << "Refuel Time\n";
    cout << "----------------------------------------\n";
    for (const auto& f : freights)
        f.display();
}

void Scheduler::displayAllCargos() const {
    cout << left << setw(10) << "ID" << setw(15) << "Destination" << "Arrival Time\n";
    cout << "----------------------------------------\n";
    for (const auto& c : cargos)
        c.display();
}

void Scheduler::displaySchedule() const {
    cout << "\n--- FREIGHT TO CARGO SCHEDULE ---\n";
    cout << left << setw(12) << "Freight ID" << " -> " << "Cargo ID\n";
    cout << "------------------------------\n";
    for (const auto& pair : assignments) {
        cout << setw(12) << pair.first << " -> " << pair.second << '\n';
    }
}

void Scheduler::saveFreights(const string& filePath) const {
    ofstream fout(filePath);
    for (const auto& f : freights) {
        fout << f.toCSV() << "\n";
    }
    fout.close();
}

void Scheduler::saveCargos(const string& filePath) const {
    ofstream fout(filePath);
    for (const auto& c : cargos) {
        fout << c.toCSV() << "\n";
    }
    fout.close();
}

void Scheduler::deleteFreightById(const string& id) {
    auto it = remove_if(freights.begin(), freights.end(),
        [&](const Freight& f) { return f.getId() == id; });
    if (it != freights.end()) {
        freights.erase(it, freights.end());
        cout << "Freight " << id << " deleted successfully.\n";
    }
    else {
        cout << "Freight ID not found.\n";
    }
}

void Scheduler::deleteCargoById(const string& id) {
    auto it = remove_if(cargos.begin(), cargos.end(),
        [&](const Cargo& c) { return c.getId() == id; });
    if (it != cargos.end()) {
        cargos.erase(it, cargos.end());
        cout << "Cargo " << id << " deleted successfully.\n";
    }
    else {
        cout << "Cargo ID not found.\n";
    }
}