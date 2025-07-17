#include "CargoManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> 
#include <limits>  
#include "InputUtils.h" 


using namespace std;

void CargoManager::loadFromFile(const string& filePath) {
    cargos.clear();
    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        cout << "Could not open cargo file: " << filePath << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string id, dest, timeStr, groupSizeStr;
        int groupSize;

        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, timeStr, ',');
        getline(ss, groupSizeStr); 

        try {
            groupSize = stoi(groupSizeStr);
        }
        catch (const invalid_argument& e) {
            cerr << "Error converting group size for cargo " << id << ": " << e.what() << endl;
            continue; 
        }
        catch (const out_of_range& e) {
            cerr << "Group size out of range for cargo " << id << ": " << e.what() << endl;
            continue; 
        }

        if (!id.empty() && !dest.empty() && !timeStr.empty() && groupSize > 0) {
            cargos.push_back(Cargo(id, dest, timeStr, groupSize));
        }
    }
    inFile.close();
    cout << "Loaded " << cargos.size() << " cargos from " << filePath << endl;
}

void CargoManager::saveToFile(const string& filePath) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        cout << "Could not open cargo file for saving: " << filePath << endl;
        return;
    }
    for (const auto& c : cargos) {
        outFile << c.getId() << "," << c.getDestination() << ","
            << c.getTime() << "," << c.getGroupSize() << "\n";
    }
    outFile.close();
    cout << "Saved " << cargos.size() << " cargos to " << filePath << endl;
}

void CargoManager::add(const Cargo& c) {
    cargos.push_back(c);
    cout << "Cargo added successfully.\n";
}

void CargoManager::edit(const string& id) {
    for (Cargo& c : cargos) {
        if (c.getId() == id) {
            string dest, time;
            int groupSize;
            bool inputValid = false;
            int retryChoice;

            cout << "Enter new destination (current: " << c.getDestination() << "): ";
            getline(cin, dest);

            // Validate new time input using isValidTime from InputUtils.h/cpp
            do {
                cout << "Enter new time (HHMM, 0000-2359) (current: " << c.getTime() << "): ";
                getline(cin, time);
                if (isValidTime(time)) { // Using utility function
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

            
            do {
                cout << "Enter new group size (1-10) (current: " << c.getGroupSize() << "): ";
                cin >> groupSize;
                if (cin.fail() || !isValidGroupSize(groupSize)) { // Using utility function
                    cout << "\nInvalid Group Size. Must be an integer between 1 and 10.\n\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n"; 
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) { 
                        return; 
                    }
                }
                else {
                    inputValid = true;
                }
            } while (!inputValid);
            cin.ignore(); 

            c = Cargo(id, dest, time, groupSize); 
            cout << "Cargo updated successfully.\n";
            return;
        }
    }
    cout << "Cargo with ID '" << id << "' not found.\n";
}

void CargoManager::remove(const string& id) {
    for (auto it = cargos.begin(); it != cargos.end(); ++it) {
        if (it->getId() == id) {
            if (lastDeletedCargo) { 
                delete lastDeletedCargo;
            }
            lastDeletedCargo = new Cargo(*it); 
            cargos.erase(it);
            cout << "Cargo deleted.\n";
            return;
        }
    }
    cout << "Cargo ID not found.\n";
}

void CargoManager::displayAll() const {
    if (cargos.empty()) {
        cout << "No cargos to display.\n";
        return;
    }

    // Table Header
    cout << "\n---------------------------------------------------------\n";
    cout << left << setw(10) << "Cargo ID"
        << left << setw(15) << "Destination"
        << left << setw(8) << "Time"
        << left << setw(12) << "Group Size" << endl;
    cout << "---------------------------------------------------------\n";

    // Table Rows
    for (const Cargo& c : cargos) {
        cout << left << setw(10) << c.getId()
            << left << setw(15) << c.getDestination()
            << left << setw(8) << c.getTime()
            << left << setw(12) << c.getGroupSize() << endl;
    }
    cout << "---------------------------------------------------------\n";
}

std::vector<Cargo>& CargoManager::getAll() {
    return cargos;
}

void CargoManager::undoDelete() {
    if (lastDeletedCargo) {
        cargos.push_back(*lastDeletedCargo);
        delete lastDeletedCargo;
        lastDeletedCargo = nullptr;
        cout << "Last deleted cargo restored.\n";
    }
    else {
        cout << "No cargo to undo.\n";
    }
}