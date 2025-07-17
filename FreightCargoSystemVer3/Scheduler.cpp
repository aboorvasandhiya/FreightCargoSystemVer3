#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <iomanip>   
#include <algorithm> 

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
        std::cout << "No scheduling strategy set.\n";
        return;
    }
    strategy->generate(freightManager, cargoManager, assignments);
    std::cout << "Schedule generated using selected strategy.\n";
}

void Scheduler::displaySchedule() const {
    std::cout << "\n--- SCHEDULED ASSIGNMENTS ---\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------\n";

    // Define column widths for better alignment
    const int CARGO_ID_WIDTH = 10;    
    const int C_DEST_WIDTH = 15;
    const int C_TIME_WIDTH = 10;
    const int C_QUANTITY_WIDTH = 12;  
    const int ARROW_WIDTH = 10;        
    const int FREIGHT_ID_WIDTH = 12;
    const int F_DEST_WIDTH = 15;
    const int F_TIME_WIDTH = 10;
    const int F_TYPE_WIDTH = 15;

    // Print headers with proper spacing
    std::cout << std::left << std::setw(CARGO_ID_WIDTH) << "Cargo ID"
        << std::setw(C_DEST_WIDTH) << "C. Dest"
        << std::setw(C_TIME_WIDTH) << "C. Time"
        << std::setw(C_QUANTITY_WIDTH) << "C. Quantity" 
        << std::setw(ARROW_WIDTH) << "" // Placeholder for the arrow in the header
        << std::setw(FREIGHT_ID_WIDTH) << "Freight ID"
        << std::setw(F_DEST_WIDTH) << "F. Dest"
        << std::setw(F_TIME_WIDTH) << "F. Time"
        << std::setw(F_TYPE_WIDTH) << "F. Type"
        << "\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------\n";

    for (const auto& pair : assignments) {
        // Find the Cargo object
        const Cargo* cargo = nullptr;
        for (const auto& c : cargoManager->getAll()) {
            if (c.getId() == pair.first) {
                cargo = &c;
                break;
            }
        }

        if (!cargo) {
            std::cout << "Error: Cargo " << pair.first << " not found.\n";
            continue;
        }

        // Print Cargo details
        std::cout << std::left << std::setw(CARGO_ID_WIDTH) << cargo->getId()
            << std::setw(C_DEST_WIDTH) << cargo->getDestination()
            << std::setw(C_TIME_WIDTH) << cargo->getTime()
            << std::setw(C_QUANTITY_WIDTH) << std::to_string(cargo->getGroupSize()); 

        // Add the arrow separator
        std::cout << std::setw(ARROW_WIDTH) << " -> ";

        // Check if assigned to a freight or UNASSIGNED
        if (pair.second == "UNASSIGNED") {
            std::cout << std::left << std::setw(FREIGHT_ID_WIDTH) << "N/A"
                << std::setw(F_DEST_WIDTH) << "N/A"
                << std::setw(F_TIME_WIDTH) << "N/A"
                << std::setw(F_TYPE_WIDTH) << "N/A"
                << "\n";
        }
        else {
            // Find the Freight object
            const Freight* freight = nullptr;
            for (const auto& f : freightManager->getAll()) {
                if (f.getId() == pair.second) {
                    freight = &f;
                    break;
                }
            }

            if (freight) {
                // Print Freight details
                std::cout << std::left << std::setw(FREIGHT_ID_WIDTH) << freight->getId()
                    << std::setw(F_DEST_WIDTH) << freight->getDestination()
                    << std::setw(F_TIME_WIDTH) << freight->getTime()
                    << std::setw(F_TYPE_WIDTH) << freight->getFreightType()
                    << "\n";
            }
            else {
                std::cout << std::left << std::setw(FREIGHT_ID_WIDTH) << "ERR" // Indicate error for freight
                    << std::setw(F_DEST_WIDTH) << "ERR"
                    << std::setw(F_TIME_WIDTH) << "ERR"
                    << std::setw(F_TYPE_WIDTH) << "ERR"
                    << "\n";
            }
        }
    }
    std::cout << "------------------------------------------------------------------------------------------------------------------\n";
}

void Scheduler::displayUnassigned() const {
    std::cout << "\n--- UNASSIGNED CARGOS ---\n";
    for (const auto& pair : assignments) {
        if (pair.second == "UNASSIGNED")
            std::cout << "Cargo " << pair.first << " is unassigned.\n";
    }
}

void Scheduler::displayUnderCapacityFreights() const {
    std::cout << "\n--- FREIGHTS UNDER CAPACITY ---\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------\n";

    // Column widths (can be adjusted)
    const int ID_WIDTH = 8;
    const int DEST_WIDTH = 15;
    const int TIME_WIDTH = 10;
    const int TYPE_WIDTH = 15;
    const int CURRENT_LOAD_WIDTH = 15;
    const int MAX_CAPACITY_WIDTH = 15;
    const int REMAINING_CAPACITY_WIDTH = 20;


    // Print headers
    std::cout << std::left << std::setw(ID_WIDTH) << "ID"
        << std::setw(DEST_WIDTH) << "Destination"
        << std::setw(TIME_WIDTH) << "Time"
        << std::setw(TYPE_WIDTH) << "Type"
        << std::setw(CURRENT_LOAD_WIDTH) << "Current Load"
        << std::setw(MAX_CAPACITY_WIDTH) << "Max Capacity"
        << std::setw(REMAINING_CAPACITY_WIDTH) << "Remaining Capacity"
        << "\n";
    std::cout << "------------------------------------------------------------------------------------------------------------------\n";


    // Step 1: Calculate current load for each freight
    std::map<std::string, int> currentFreightLoads;
    for (const auto& f : freightManager->getAll()) {
        currentFreightLoads[f.getId()] = 0; // Initialize all freights to 0 load
    }

    for (const auto& pair : assignments) {
        if (pair.second != "UNASSIGNED") {
            const Cargo* cargo = nullptr;
            for (const auto& c : cargoManager->getAll()) {
                if (c.getId() == pair.first) {
                    cargo = &c;
                    break;
                }
            }
            if (cargo) {
                // Add cargo group size to the assigned freight's load
                currentFreightLoads[pair.second] += cargo->getGroupSize();
            }
        }
    }

    // Step 2 & 3: Iterate through freights and display if under capacity
    bool foundUnderCapacity = false;
    for (const auto& f : freightManager->getAll()) {
        int currentLoad = currentFreightLoads[f.getId()];
        int maxCapacity = f.getMaxCapacity();

        if (currentLoad < maxCapacity) {
            foundUnderCapacity = true;
            std::cout << std::left << std::setw(ID_WIDTH) << f.getId()
                << std::setw(DEST_WIDTH) << f.getDestination()
                << std::setw(TIME_WIDTH) << f.getTime()
                << std::setw(TYPE_WIDTH) << f.getFreightType()
                << std::setw(CURRENT_LOAD_WIDTH) << std::to_string(currentLoad)
                << std::setw(MAX_CAPACITY_WIDTH) << std::to_string(maxCapacity)
                << std::setw(REMAINING_CAPACITY_WIDTH) << std::to_string(maxCapacity - currentLoad)
                << "\n";
        }
    }

    if (!foundUnderCapacity) {
        std::cout << "All freights are either fully utilized or no freights found.\n";
    }

    std::cout << "------------------------------------------------------------------------------------------------------------------\n";
}

void Scheduler::saveSchedule(const std::string& filename) const {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return;
    }

    // Write header
    fout << "Cargo ID,C. Dest,C. Time,Group,Freight ID,F. Dest,F. Time,F. Type\n";

    for (const auto& pair : assignments) {
        const Cargo* cargo = nullptr;
        for (const auto& c : cargoManager->getAll()) {
            if (c.getId() == pair.first) {
                cargo = &c;
                break;
            }
        }

        if (!cargo) {
            fout << pair.first << ",Error: Cargo Not Found\n"; // Log error
            continue;
        }

        fout << cargo->getId() << "," << cargo->getDestination() << "," << cargo->getTime() << "," << cargo->getGroupSize();

        if (pair.second == "UNASSIGNED") {
            fout << ",N/A,N/A,N/A,N/A\n";
        }
        else {
            const Freight* freight = nullptr;
            for (const auto& f : freightManager->getAll()) {
                if (f.getId() == pair.second) {
                    freight = &f;
                    break;
                }
            }

            if (freight) {
                fout << "," << freight->getId() << "," << freight->getDestination() << "," << freight->getTime() << "," << freight->getFreightType() << "\n";
            }
            else {
                fout << "," << pair.second << ",Error: Freight Not Found,ERR,ERR\n"; // Log error
            }
        }
    }
    fout.close();
    std::cout << "Schedule saved to " << filename << std::endl;
}