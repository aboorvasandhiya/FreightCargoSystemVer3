#include <iostream>
#include <fstream>
#include <iomanip> 
#include <limits>  
#include <vector>
#include <string>
#include <regex>

#include "FreightManager.h"
#include "CargoManager.h"
#include "Scheduler.h"
#include "ArrivalTimeStrategy.h"
#include "MinimizeFreightStrategy.h"
#include "InputUtils.h" 

#include "Freight.h"
#include "Cargo.h"
#include "Transport.h"
#include "SchedulingStrategy.h"


using namespace std;

void showMainMenu();
void manageFreightsMenu(FreightManager& freightManager);
void manageCargosMenu(CargoManager& cargoManager);
void scheduleOperationsMenu(Scheduler& scheduler);
void chooseSchedulingStrategyMenu(Scheduler& scheduler);

bool isValidFreightId(const string& id) {
    return regex_match(id, regex("F[0-9]+"));
}

bool isValidCargoId(const string& id) {
    return regex_match(id, regex("C[0-9]+"));
}


// --- File Management Functions ---
bool fileExists(const string& name) {
    ifstream f(name);
    return f.good();
}

void createSampleFiles() {
    if (!fileExists("freights.txt")) {
        ofstream fout("freights.txt");
        fout << "F01,London,0730,MegaCarrier\n";      
        fout << "F02,Paris,1720,CargoCruiser\n";    
        fout << "F03,Tokyo,1200,MiniMover\n";        
        fout << "F04,Berlin,0900,CargoCruiser\n";   
        fout << "F05,Rome,1030,MegaCarrier\n";       
        fout << "F06,Madrid,1500,MiniMover\n";       
        fout << "F07,Beijing,2000,MegaCarrier\n";     
        fout << "F08,Sydney,0600,CargoCruiser\n";     
        fout << "F09,Cairo,1145,MiniMover\n";       
        fout << "F10,New York,1830,MegaCarrier\n";    
        fout.close();
        cout << "Created freights.txt with comprehensive sample data.\n";
    }

    if (!fileExists("cargos.txt")) {
        ofstream fout("cargos.txt");
        fout << "C01,London,0720,2\n";    
        fout << "C02,Paris,1715,1\n";     
        fout << "C03,Tokyo,1150,2\n";    
        fout << "C04,Berlin,0850,5\n";     
        fout << "C05,Rome,1020,3\n";      
        fout << "C06,Madrid,1450,1\n";     
        fout << "C07,Beijing,1950,7\n";  
        fout << "C08,Sydney,0550,4\n";     
        fout << "C09,Cairo,1130,2\n";    
        fout << "C10,New York,1820,6\n";  
        fout.close();
        cout << "Created cargos.txt with matching sample data.\n";
    }
}


// --- Menu Display Functions ---
void showMainMenu() {
    cout << "\n===== International Freight Cargo Scheduler =====\n";
    cout << setw(2) << 1 << ". Manage Freights\n";
    cout << setw(2) << 2 << ". Manage Cargos\n";
    cout << setw(2) << 3 << ". Schedule Operations\n";
    cout << setw(2) << 4 << ". Save All Data\n";
    cout << setw(2) << 5 << ". Exit\n";
}

void showFreightMenu() {
    cout << "\n--- Freight Management ---\n";
    cout << setw(2) << 1 << ". Display All Freights\n";
    cout << setw(2) << 2 << ". Add New Freight\n";
    cout << setw(2) << 3 << ". Edit Freight\n";
    cout << setw(2) << 4 << ". Delete Freight\n";
    cout << setw(2) << 5 << ". Undo Delete Freight\n";
    cout << setw(2) << 6 << ". Back to Main Menu\n";
}

void showCargoMenu() {
    cout << "\n--- Cargo Management ---\n";
    cout << setw(2) << 1 << ". Display All Cargos\n";
    cout << setw(2) << 2 << ". Add New Cargo\n";
    cout << setw(2) << 3 << ". Edit Cargo\n";
    cout << setw(2) << 4 << ". Delete Cargo\n";
    cout << setw(2) << 5 << ". Undo Delete Cargo\n";
    cout << setw(2) << 6 << ". Back to Main Menu\n";
}

void showScheduleMenu() {
    cout << "\n--- Schedule Operations ---\n";
    cout << setw(2) << 1 << ". Choose Scheduling Strategy\n"; 
    cout << setw(2) << 2 << ". Generate and Display Schedule\n"; 
    cout << setw(2) << 3 << ". Display Freights Not at Full Capacity\n";
    cout << setw(2) << 4 << ". Display Unassigned Records\n"; 
    cout << setw(2) << 5 << ". Save Schedule to File\n";
    cout << setw(2) << 6 << ". Back to Main Menu\n";
}

// --- Menu Logic Functions ---

void chooseSchedulingStrategyMenu(Scheduler& scheduler) {
    int choice;
    do {
        cout << "\n--- Choose Scheduling Strategy ---\n";
        cout << "1. Arrival Time Priority\n";
        cout << "2. Minimize Freight Usage\n";
        cout << "3. Back to Schedule Operations Menu\n";
        cout << "\n";
        choice = getValidChoice(1, 3); 

        switch (choice) {
        case 1:
            scheduler.setStrategy(new ArrivalTimeStrategy());
            cout << "Strategy set to Arrival Time Priority.\n";
            break;
        case 2:
            scheduler.setStrategy(new MinimizeFreightStrategy());
            cout << "Strategy set to Minimize Freight Usage.\n";
            break;
        case 3:
            // Exit this sub-menu
            break;
        default:
            cout << "Invalid strategy choice. Try again.\n";
        }
    } while (choice != 3);
}

void scheduleOperationsMenu(Scheduler& scheduler) {
    int option;
    do {
        showScheduleMenu();
        cout << "\n";
        option = getValidChoice(1, 6); 

        switch (option) {
        case 1: 
            chooseSchedulingStrategyMenu(scheduler);
            break;
        case 2: 
            scheduler.generateSchedule();
            scheduler.displaySchedule();
            break;
        case 3: 
            scheduler.displayUnderCapacityFreights();
            break;
        case 4: 
            scheduler.displayUnassigned();
            break;
        case 5: { 
            cout << "\n";
            string filename;
            cout << "Enter filename to save schedule (e.g., my_schedule.txt): ";
            getline(cin, filename);
            scheduler.saveSchedule(filename);
            break;
        }
        case 6: 
            // Back to Main Menu
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (option != 6);
}

void manageFreightsMenu(FreightManager& freightManager) {
    int option;
    const vector<string> freightTypes = { "MiniMover", "CargoCruiser", "MegaCarrier" };

    do {
        showFreightMenu();
        cout << "\n";
        option = getValidChoice(1, 6);

        switch (option) {
        case 1:
            freightManager.displayAll();
            break;
        case 2: { // Add Freight with validation and duplicate ID check
            cout << "\n";
            string id, dest, time, type;
            bool inputValid = false;
            int retryChoice;

            // Validate Freight ID and check for duplicates
            do {
                cout << "Enter Freight ID (e.g., F01, F100): ";
                getline(cin, id);
                if (isValidFreightId(id)) { 
                    // Check for duplicate ID
                    bool idExists = false;
                    for (const auto& f : freightManager.getAll()) { 
                        if (f.getId() == id) {
                            idExists = true;
                            break;
                        }
                    }
                    if (idExists) {
                        cout << "\nError: Freight with ID '" << id << "' already exists. Please use a unique ID.\n\n";
                        cout << "Do you want to\n";
                        cout << "(1) Re-enter ID\n";
                        cout << "(2) Go back to menu?\n\n";
                        retryChoice = getValidChoice(1, 2); 
                        if (retryChoice == 2) return;
                        inputValid = false; // Stay in loop
                    }
                    else {
                        inputValid = true; // ID is valid and unique
                    }
                }
                else {
                    cout << "\nInvalid Freight ID format. Must start with 'F' followed by numbers (e.g., F01).\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);
            inputValid = false; // Reset for next validation

            cout << "Enter Destination: ";
            getline(cin, dest);

            // Validate Time
            do {
                cout << "Enter Refuel Time (HHMM, 0000-2359): ";
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
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);
            inputValid = false;

            // Select Freight Type from list
            cout << "Select Freight Type:\n";
            for (size_t i = 0; i < freightTypes.size(); ++i) {
                cout << "  " << (i + 1) << ". " << freightTypes[i] << "\n";
            }
            int typeChoice = getValidChoice(1, static_cast<int>(freightTypes.size())); 
            type = freightTypes[typeChoice - 1];

            freightManager.add(Freight(id, dest, time, type));
            break;
        }
        case 3: { // Edit Freight with validation
            cout << "\n";
            string id;
            bool inputValid = false;
            int retryChoice;

            do {
                cout << "Enter Freight ID to edit (e.g., F01, F100): ";
                getline(cin, id);
                if (isValidFreightId(id)) {
                    inputValid = true;
                }
                else {
                    cout << "\nInvalid Freight ID format. Must start with 'F' followed by numbers (e.g., F01).\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);

            freightManager.edit(id); // FreightManager::edit handles validation internally
            break;
        }
        case 4: { // Delete Freight
            cout << "\n";
            string id;
            bool inputValid = false;
            int retryChoice;

            do {
                cout << "Enter Freight ID to delete (e.g., F01, F100): ";
                getline(cin, id);
                if (isValidFreightId(id)) {
                    inputValid = true;
                }
                else {
                    cout << "\nInvalid Freight ID format. Must start with 'F' followed by numbers (e.g., F01).\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);

            freightManager.remove(id);
            break;
        }
        case 5:
            freightManager.undoDelete();
            break;
        case 6:
            // Back to Main Menu
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (option != 6);
}

void manageCargosMenu(CargoManager& cargoManager) {
    int option;
    do {
        showCargoMenu();
        cout << "\n";
        option = getValidChoice(1, 6);

        switch (option) {
        case 1:
            cargoManager.displayAll();
            break;
        case 2: { // Add Cargo with validation and duplicate ID Check
            cout << "\n";
            string id, dest, time;
            int groupSize;
            bool inputValid = false;
            int retryChoice;

            // Validate Cargo ID and check for duplicates
            do {
                cout << "Enter Cargo ID (e.g., C01, C100): ";
                getline(cin, id);
                if (isValidCargoId(id)) {
                    // Check for duplicate ID
                    bool idExists = false;
                    for (const auto& c : cargoManager.getAll()) { 
                        if (c.getId() == id) {
                            idExists = true;
                            break;
                        }
                    }
                    if (idExists) {
                        cout << "\nError: Cargo with ID '" << id << "' already exists. Please use a unique ID.\n\n";
                        cout << "Do you want to\n";
                        cout << "(1) Re-enter ID\n";
                        cout << "(2) Go back to menu?\n\n";
                        retryChoice = getValidChoice(1, 2); 
                        if (retryChoice == 2) return;
                        inputValid = false; 
                    }
                    else {
                        inputValid = true; // ID is valid and unique
                    }
                }
                else {
                    cout << "\nInvalid Cargo ID format. Must start with 'C' followed by numbers (e.g., C01).\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);
            inputValid = false;

            cout << "Enter Destination: ";
            getline(cin, dest);

            // Validate Time
            do {
                cout << "Enter Arrival Time (HHMM, 0000-2359): ";
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
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);
            inputValid = false;

            // Validate Group Size
            do {
                cout << "Enter Group Size (1-10): ";
                cin >> groupSize;
                if (cin.fail() || !isValidGroupSize(groupSize)) { 
                    cout << "\nInvalid Group Size. Must be an integer between 1 and 10.\n\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
                else {
                    inputValid = true;
                }
            } while (!inputValid);
            cin.ignore();

            cargoManager.add(Cargo(id, dest, time, groupSize));
            break;
        }
        case 3: { // Edit Cargo with validation
            cout << "\n";
            string id;
            bool inputValid = false;
            int retryChoice;

            do {
                cout << "Enter Cargo ID to edit (e.g., C01, C100): ";
                getline(cin, id);
                if (isValidCargoId(id)) { 
                    inputValid = true;
                }
                else {
                    cout << "\nInvalid Cargo ID format. Must start with 'C' followed by numbers (e.g., C01).\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);

            cargoManager.edit(id); 
            break;
        }
        case 4: { // Delete Cargo
            cout << "\n";
            string id;
            bool inputValid = false;
            int retryChoice;

            do {
                cout << "Enter Cargo ID to delete (e.g., C01, C100): ";
                getline(cin, id);
                if (isValidCargoId(id)) { 
                    inputValid = true;
                }
                else {
                    cout << "\nInvalid Cargo ID format. Must start with 'C' followed by numbers (e.g., C01).\n\n";
                    cout << "Do you want to\n";
                    cout << "(1) Re-enter\n";
                    cout << "(2) Go back to menu?\n\n";
                    retryChoice = getValidChoice(1, 2); 
                    if (retryChoice == 2) return;
                }
            } while (!inputValid);

            cargoManager.remove(id);
            break;
        }
        case 5:
            cargoManager.undoDelete();
            break;
        case 6:
            // Back to Main Menu
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (option != 6);
}


int main() {
    createSampleFiles(); // Create sample files if they don't exist

    FreightManager freightManager;
    CargoManager cargoManager;
    Scheduler scheduler(&freightManager, &cargoManager);

    // Load initial data
    freightManager.loadFromFile("freights.txt");
    cargoManager.loadFromFile("cargos.txt");

    // Set default scheduling strategy
    scheduler.setStrategy(new ArrivalTimeStrategy()); // Default to Arrival Time Priority

    int mainChoice;
    do {
        showMainMenu();
        cout << "\n";
        mainChoice = getValidChoice(1, 5); 

        switch (mainChoice) {
        case 1:
            manageFreightsMenu(freightManager);
            break;
        case 2:
            manageCargosMenu(cargoManager);
            break;
        case 3:
            scheduleOperationsMenu(scheduler);
            break;
        case 4: { // Save All Data
            cout << "\n";
            string freightFile, cargoFile;
            cout << "Enter filename for freights (leave blank for freights.txt): ";
            getline(cin, freightFile);
            if (freightFile.empty()) freightFile = "freights.txt";

            cout << "Enter filename for cargos (leave blank for cargos.txt): ";
            getline(cin, cargoFile);
            if (cargoFile.empty()) cargoFile = "cargos.txt";

            freightManager.saveToFile(freightFile);
            cargoManager.saveToFile(cargoFile);
            cout << "All data saved.\n";
            break;
        }
        case 5: // Exit
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (mainChoice != 5);

    return 0;
}