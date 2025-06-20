#include "FreightManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

void FreightManager::loadFromFile(const string& filePath) {
    ifstream file(filePath);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, dest, time;
        getline(ss, id, ',');
        getline(ss, dest, ',');
        getline(ss, time, ',');
        bool valid = id.length() >= 2 && id[0] == 'F';
        if (valid) {
            for (size_t i = 1; i < id.length(); ++i) {
                if (!isdigit(id[i])) {
                    valid = false;
                    break;
                }
            }
        }
        bool exists = false;
        if (valid) {
            for (const auto& f : freights) {
                if (f.getId() == id) {
                    exists = true;
                    break;
                }
            }
        }
        if (valid && !exists) {
            freights.push_back(Freight(id, dest, time));
        }
        else if (!valid) {
            cout << "Skipping invalid Freight ID: " << id << " in " << filePath << "\n";
        }
        else {
            cout << "Skipping duplicate Freight ID: " << id << " in " << filePath << "\n";
        }
    }
    file.close();
}

void FreightManager::saveToFile(const string& filePath) const {
    ofstream fout(filePath);
    for (const auto& f : freights) {
        fout << f.toCSV() << "\n";
    }
    fout.close();
}

void FreightManager::add(const Freight& freight) {
    bool valid = freight.getId().length() >= 2 && freight.getId()[0] == 'F';
    if (valid) {
        for (size_t i = 1; i < freight.getId().length(); ++i) {
            if (!isdigit(freight.getId()[i])) {
                valid = false;
                break;
            }
        }
    }
    bool exists = false;
    if (valid) {
        for (const auto& f : freights) {
            if (f.getId() == freight.getId()) {
                exists = true;
                break;
            }
        }
    }
    if (valid && !exists) {
        freights.push_back(freight);
        cout << "Freight added successfully.\n";
    }
    else if (!valid) {
        cout << "\n----------------------------------------------\n";
        cout << "\nInvalid Freight ID: Must start with 'F' followed by digits (e.g., F01, F10, F100, F1000).\n";
    }
    else {
        cout << "\n----------------------------------------------\n";
        cout << "\nFreight ID already exists.\n";
    }
}

void FreightManager::edit(const string& id) {
    for (auto& f : freights) {
        if (f.getId() == id) {
            string dest, time;
            cout << "New destination (leave blank to keep \"" << f.getDestination() << "\"): ";
            getline(cin, dest);
            if (dest.empty()) dest = f.getDestination();

            cout << "New refuel time (leave blank to keep \"" << f.getTime() << "\"): ";
            getline(cin, time);
            if (!time.empty()) {
                bool validTime = time.length() == 4;
                if (validTime) {
                    for (size_t i = 0; i < time.length(); ++i) {
                        if (!isdigit(time[i])) {
                            validTime = false;
                            break;
                        }
                    }
                    if (validTime) {
                        int hour = stoi(time.substr(0, 2));
                        int minute = stoi(time.substr(2, 2));
                        validTime = (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59);
                    }
                }
                if (!validTime) {
                    cout << "\n----------------------------------------------\n";
                    cout << "\nInvalid Refuel Time: Must be 4 digits in 24-hour format (0000-2359).\n";
                    return;
                }
            }
            if (time.empty()) time = f.getTime();

            f = Freight(id, dest, time);
            cout << "Freight updated successfully.\n";
            return;
        }
    }
    cout << "Freight not found.\n";
}

void FreightManager::remove(const string& id) {
    for (auto it = freights.begin(); it != freights.end(); ++it) {
        if (it->getId() == id) {
            if (lastDeletedFreight) delete lastDeletedFreight;
            lastDeletedFreight = new Freight(*it);
            freights.erase(it);
            cout << "Freight " << id << " deleted successfully.\n";
            return;
        }
    }
    cout << "Freight ID not found.\n";
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


void FreightManager::displayAll() const {
    cout << left << setw(10) << "ID" << setw(15) << "Refuel Stop" << "Refuel Time\n";
    cout << "----------------------------------------\n";
    for (const auto& f : freights)
        f.display();
}

const vector<Freight>& FreightManager::getAll() const {
    return freights;
}