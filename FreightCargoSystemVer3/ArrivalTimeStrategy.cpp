#include "ArrivalTimeStrategy.h"
#include <iostream>
using namespace std;

void ArrivalTimeStrategy::generate(
    FreightManager* freightManager,
    CargoManager* cargoManager,
    map<string, string>& assignments
) {
    assignments.clear();
    for (Cargo& c : cargoManager->getAll()) {
        bool assigned = false;
        for (Freight& f : freightManager->getAll()) {
            if (f.getDestination() == c.getDestination()) {
                int cTime = stoi(c.getTime());
                int fTime = stoi(f.getTime());
                if (fTime >= cTime && (fTime - cTime <= 15)) {
                    assignments[c.getId()] = f.getId();
                    assigned = true;
                    break;
                }
            }
        }
        if (!assigned) {
            assignments[c.getId()] = "UNASSIGNED";
        }
    }
}