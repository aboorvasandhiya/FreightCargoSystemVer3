#pragma once
#include "FreightManager.h"
#include "CargoManager.h"
#include <map>
using namespace std;

class Scheduler {
private:
    FreightManager* freightManager;
    CargoManager* cargoManager;
    map<string, string> assignments; // Freight ID -> Cargo ID

public:
    Scheduler(FreightManager* fm, CargoManager* cm);

    void generateSchedule();
    void displaySchedule() const;
    void displayUnassigned() const;
    void saveSchedule(const string& filePath) const;
};
