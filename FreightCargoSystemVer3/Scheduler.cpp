#include "Scheduler.h"
#include <iostream>
#include <fstream>
using namespace std;

Scheduler::Scheduler(FreightManager* f, CargoManager* c)
    : freightManager(f), cargoManager(c) {}

Scheduler::~Scheduler() {
    delete strategy;
}

void Scheduler::setStrategy(SchedulingStrategy* s) {
    if (strategy) delete strategy;
    strategy = s;
}

void Scheduler::generateSchedule() {
    if (!strategy) {
        cout << "No scheduling strategy set.\n";
        return;
    }
    strategy->generate(freightManager, cargoManager, assignments);
    cout << "Schedule generated using selected strategy.\n";
}

void Scheduler::displaySchedule() const {
    cout << "\n--- SCHEDULED ASSIGNMENTS ---\n";
    for (const auto& pair : assignments) {
        cout << "Cargo " << pair.first << " → " << pair.second << endl;
    }
}

void Scheduler::displayUnassigned() const {
    cout << "\n--- UNASSIGNED CARGOS ---\n";
    for (const auto& pair : assignments) {
        if (pair.second == "UNASSIGNED")
            cout << "Cargo " << pair.first << " is unassigned.\n";
    }
}

void Scheduler::saveSchedule(const string& filename) const {
    ofstream fout(filename);
    for (const auto& pair : assignments) {
        fout << pair.first << "," << pair.second << endl;
    }
    fout.close();
    cout << "Schedule saved to " << filename << endl;
}
