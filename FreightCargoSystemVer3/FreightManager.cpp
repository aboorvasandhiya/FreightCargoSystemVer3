#include "FreightManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <vector> 
#include "InputUtils.h"


using namespace std;

void FreightManager::loadFromFile(const string& filePath) {
    freights.clear(); 
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        cout << "Could not open freight file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string id, dest, time, type;
        string segment;

        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, time, ',');
        getline(ss, type);

        if (!id.empty() && !dest.empty() && !time.empty() && !type.empty()) {
            freights.push_back(Freight(id, dest, time, type));
        }
    }
    inFile.close();
    cout << "Loaded " << freights.size() << " freights from " << filePath << endl;
}

void FreightManager::saveToFile(const string& filePath) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        cout << "Could not open freight file for saving: " << filePath << endl;
        return;
    }
    for (const auto& f : freights) {
        outFile << f.getId() << "," << f.getDestination() << ","
            << f.getTime() << "," << f.getFreightType() << "\n";
    }
    outFile.close();
    cout << "Saved " << freights.size() << " freights to " << filePath << endl;
}

void FreightManager::add(const Freight& f) {
    freights.push_back(f);
    cout << "Freight added successfully.\n";
}

void FreightManager::edit(const string& id) {
    for (Freight& f : freights) {
        if (f.getId() == id) {
            string dest, time, type;
            bool inputValid = false;
            int retryChoice;

            cout << "Enter new destination (current: " << f.getDestination() << "): ";
            getline(cin, dest);

            do {
                cout << "Enter new time (HHMM, 0000-2359) (current: " << f.getTime() << "): ";
                getline(cin, time);
                if (isValidTime(time)) { 
                    inputValid = true;
                }
                else {
                    cout << "\nInvalid Time format. Must be HHMM between 0000 and 2359.\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n"; 
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) { 
                        return; 
                    }
                }
            } while (!inputValid);
            inputValid = false; 

            const vector<string> freightTypes = { "MiniMover", "CargoCruiser", "MegaCarrier" };
            cout << "Select new freight type (current: " << f.getFreightType() << "):\n";
            for (size_t i = 0; i < freightTypes.size(); ++i) {
                cout << "  " << (i + 1) << ". " << freightTypes[i] << "\n";
            }
            int typeChoice = getValidChoice(1, static_cast<int>(freightTypes.size())); 
            type = freightTypes[typeChoice - 1];

            f = Freight(id, dest, time, type); 
            cout << "Freight updated successfully.\n";
            return;
        }
    }
    cout << "Freight with ID '" << id << "' not found.\n";
}

void FreightManager::remove(const string& id) {
    for (auto it = freights.begin(); it != freights.end(); ++it) {
        if (it->getId() == id) {
            if (lastDeletedFreight) { 
                delete lastDeletedFreight;
            }
            lastDeletedFreight = new Freight(*it); 
            freights.erase(it);
            cout << "Freight deleted.\n";
            return;
        }
    }
    cout << "Freight ID not found.\n";
}

void FreightManager::displayAll() const {
    if (freights.empty()) {
        cout << "No freights to display.\n";
        return;
    }
    // Table Header
    cout << "\n---------------------------------------------------------------------------------\n";
    cout << left << setw(12) << "Freight ID"
        << left << setw(15) << "Destination"
        << left << setw(8) << "Time"
        << left << setw(15) << "Type"
        << left << setw(15) << "Max Capacity" << endl;
    cout << "---------------------------------------------------------------------------------\n";

    // Table Rows
    for (const Freight& f : freights) {
        cout << left << setw(12) << f.getId()
            << left << setw(15) << f.getDestination()
            << left << setw(8) << f.getTime()
            << left << setw(15) << f.getFreightType()
            << left << setw(15) << f.getMaxCapacity() << endl;
    }
    cout << "---------------------------------------------------------------------------------\n";
}

std::vector<Freight>& FreightManager::getAll() {
    return freights;
}

void FreightManager::undoDelete() {
    if (lastDeletedFreight) {
        freights.push_back(*lastDeletedFreight);
        delete lastDeletedFreight;
        lastDeletedFreight = nullptr;
        cout << "Last deleted freight restored.\n";
    }
    else {
        cout << "No freight to undo.\n";
    }
}