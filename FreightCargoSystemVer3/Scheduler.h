#pragma once
#include "FreightManager.h"
#include "CargoManager.h"
#include "SchedulingStrategy.h"
#include <map>
#include <string>

class Scheduler {
private:
    FreightManager* freightManager;
    CargoManager* cargoManager;
    SchedulingStrategy* strategy = nullptr;

    std::map<std::string, std::string> assignments;  // cargoID -> freightID

public:
    Scheduler(FreightManager* f, CargoManager* c);
    ~Scheduler();

    void setStrategy(SchedulingStrategy* s);
    void generateSchedule();
    void displaySchedule() const;
    void displayUnassigned() const;
    void displayUnderCapacityFreights() const; 
    void saveSchedule(const std::string& filename) const;
};