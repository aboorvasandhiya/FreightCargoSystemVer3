#include "MinimizeFreightStrategy.h"
#include <map>
using namespace std;

void MinimizeFreightStrategy::generate(
    FreightManager* freightManager,
    CargoManager* cargoManager,
    map<string, string>& assignments
) {
    assignments.clear();
    map<string, int> freightLoad;  // freightID → current load

    for (Freight& f : freightManager->getAll()) {
        freightLoad[f.getId()] = 0;
    }

    for (Cargo& c : cargoManager->getAll()) {
        bool assigned = false;
        for (Freight& f : freightManager->getAll()) {
            if (f.getDestination() == c.getDestination()) {
                int& current = freightLoad[f.getId()];
                if (current + c.getGroupSize() <= f.getMaxCapacity()) {
                    current += c.getGroupSize();
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