#include "MinimizeFreightStrategy.h"
#include <map>
#include <algorithm> 
#include <vector>    
#include <iostream>  
using namespace std;

void MinimizeFreightStrategy::generate(
    FreightManager* freightManager,
    CargoManager* cargoManager,
    map<string, string>& assignments
) {
    assignments.clear();
    map<string, int> freightLoad;  

    for (Freight& f : freightManager->getAll()) {
        freightLoad[f.getId()] = 0;
    }

    // Sort cargos by group size (descending) to try to fill larger freights first
    vector<Cargo*> sortedCargos;
    for (Cargo& c : cargoManager->getAll()) {
        sortedCargos.push_back(&c);
    }
    sort(sortedCargos.begin(), sortedCargos.end(), [](const Cargo* a, const Cargo* b) {
        return a->getGroupSize() > b->getGroupSize(); // Sort by group size, largest first
        });

    for (Cargo* c_ptr : sortedCargos) {
        Cargo& c = *c_ptr;
        bool assigned = false;

        vector<Freight*> sortedFreights;
        for (Freight& f : freightManager->getAll()) {
            sortedFreights.push_back(&f);
        }

        // Sort freights by their available capacity (ascending). This tries to fill up freights
        // that are already somewhat full or are smaller (MiniMover, CargoCruiser) before
        // starting new, larger freights (MegaCarrier), which helps minimize freight usage.
        sort(sortedFreights.begin(), sortedFreights.end(), [&](const Freight* a, const Freight* b) {
            int remaining_a = a->getMaxCapacity() - freightLoad[a->getId()];
            int remaining_b = b->getMaxCapacity() - freightLoad[b->getId()];

            if (remaining_a != remaining_b) {
                return remaining_a < remaining_b; // Prefer freights with less remaining space (i.e., more full)
            }
            return a->getMaxCapacity() < b->getMaxCapacity(); // Then prefer smaller freight types
            });


        for (Freight* f_ptr : sortedFreights) {
            Freight& f = *f_ptr; 
            if (f.getDestination() == c.getDestination()) {
                int cTime = stoi(c.getTime());
                int fTime = stoi(f.getTime());

                // Time constraint: Freight must depart >= Cargo arrival time AND <= 15 minutes after
                if (fTime >= cTime && (fTime - cTime <= 15)) {
                    int& currentLoad = freightLoad[f.getId()];
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