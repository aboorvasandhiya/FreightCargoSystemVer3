#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

Scheduler::Scheduler(FreightManager* fm, CargoManager* cm)
    : freightManager(fm), cargoManager(cm) {
}

void Scheduler::generateSchedule() {
    assignments.clear();
    const auto& freights = freightManager->getAll();
    const auto& cargos = cargoManager->getAll();

    for (const auto& f : freights) {
        for (const auto& c : cargos) {
            if (f.matches(c)) {
                assignments[f.getId()] = c.getId();
                break;
            }
        }
    }
}

void Scheduler::displaySchedule() const {
    cout << "\n--- FREIGHT TO CARGO SCHEDULE ---\n";
    cout << left << setw(12) << "Freight ID" << " -> " << "Cargo ID\n";
    cout << "------------------------------\n";
    for (const auto& pair : assignments)
        cout << setw(12) << pair.first << " -> " << pair.second << '\n';
}

void Scheduler::displayUnassigned() const {
    const auto& freights = freightManager->getAll();
    const auto& cargos = cargoManager->getAll();

    cout << "Unassigned Freights:\n";
    for (const auto& f : freights) {
        if (assignments.find(f.getId()) == assignments.end())
            cout << f.getId() << " (" << f.getDestination() << " at " << f.getTime() << ")\n";
    }

    cout << "\nUnassigned Cargos:\n";
    for (const auto& c : cargos) {
        bool assigned = false;
        for (const auto& pair : assignments) {
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
    ofstream fout(filePath);
    fout << "# Format: FreightID -> CargoID\n";
    for (const auto& pair : assignments)
        fout << pair.first << " -> " << pair.second << "\n";
    fout.close();
}
