#include "ArrivalTimeStrategy.h"
#include <iostream>
#include <algorithm> 
#include <vector>    
#include <map>       
using namespace std;

void ArrivalTimeStrategy::generate(
    FreightManager* freightManager,
    CargoManager* cargoManager,
    map<string, string>& assignments
) {
    assignments.clear();
    map<string, int> freightCurrentLoad; 

    
    for (Freight& f : freightManager->getAll()) {
        freightCurrentLoad[f.getId()] = 0;
    }

    
    vector<Cargo*> sortedCargos;
    for (Cargo& c : cargoManager->getAll()) {
        sortedCargos.push_back(&c);
    }
    // Sort cargos by their arrival time (ascending)
    sort(sortedCargos.begin(), sortedCargos.end(), [](const Cargo* a, const Cargo* b) {
        return stoi(a->getTime()) < stoi(b->getTime());
        });

    for (Cargo* c_ptr : sortedCargos) { // Iterate through sorted cargos
        Cargo& c = *c_ptr; 
        bool assigned = false;

        // Iterate through freights to find a suitable one
        for (Freight& f : freightManager->getAll()) {
            if (f.getDestination() == c.getDestination()) {
                int cTime = stoi(c.getTime());
                int fTime = stoi(f.getTime());

                // Time constraint: Freight must depart >= Cargo arrival time AND <= 15 minutes after
                if (fTime >= cTime && (fTime - cTime <= 15)) {
                    // Capacity constraint: Check if freight has enough capacity
                    int& currentLoad = freightCurrentLoad[f.getId()];
                    if (currentLoad + c.getGroupSize() <= f.getMaxCapacity()) {
                        currentLoad += c.getGroupSize(); // Update freight load
                        assignments[c.getId()] = f.getId();
                        assigned = true;
                        break; // Assign cargo and move to the next cargo
                    }
                }
            }
        }
        if (!assigned) {
            assignments[c.getId()] = "UNASSIGNED";
        }
    }
}