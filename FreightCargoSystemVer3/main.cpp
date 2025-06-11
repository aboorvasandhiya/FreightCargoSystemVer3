#include <iostream>
#include <fstream>
#include <iomanip>
#include "Scheduler.h"
using namespace std;


bool fileExists(const string& name) {
    ifstream f(name);
    return f.good();
}


void createSampleFiles() {
    if (!fileExists("freights.txt")) {
        ofstream fout("freights.txt");
        fout << "F01,London,0730\n";
        fout << "F02,Paris,1720\n";
        fout << "F03,Tokyo,1200\n";
        fout.close();
        cout << "Created freights.txt with sample data.\n";
    }

    if (!fileExists("cargos.txt")) {
        ofstream fout("cargos.txt");
        fout << "C05,London,0730\n";
        fout << "C07,Paris,1720\n";
        fout << "C09,New York,1400\n";
        fout.close();
        cout << "Created cargos.txt with sample data.\n";
    }
}


void showMenu() {
    cout << "\n===== International Freight Cargo Scheduler =====\n";
    cout << "1. Display All Freights\n";
    cout << "2. Display All Cargos\n";
    cout << "3. Generate and Display Schedule\n";
    cout << "4. Display Unassigned Records\n";
    cout << "5. Add Freight\n";
    cout << "6. Add Cargo\n";
    cout << "7. Save Schedule to File\n";
    cout << "8. Save All Freights and Cargos to File\n";
    cout << "9. Delete Freight by ID\n";
    cout << "10. Delete Cargo by ID\n";
    cout << "11. Edit Freight by ID\n";
    cout << "12. Edit Cargo by ID\n";
    cout << "13. Exit\n";
}

void printFreightHeader() {
    cout << "\n--- FREIGHT DATA FORMAT ---\n\n"
        << "Each freight requires to reach its specific airport stop for refuelling.\n\n"
        << ".Name of the freight, e.g. F01, F02, F03, etc.\n"
        << ".Specific refuel stops, e.g., London, Paris, New York, Bangkok, etc.\n"
        << ".Time for the refuelling, e.g. 0700, 0900, 1530, etc.\n\n";
}

void printCargoHeader() {
    cout << "\n--- CARGO DATA FORMAT ---\n\n"
        << "Each cargo requires to reach its destination by its required time.\n\n"
        << ".Name of the cargo, e.g. C01, C02, C03, etc.\n"
        << ".Cargo destinations, e.g., London, Paris, New York, Bangkok, etc.\n"
        << ".Cargo time to reach destination, e.g., 0700, 0900, 1500, etc.\n\n";
}

int main() {
    createSampleFiles(); // 🔧 Create freights.txt and cargos.txt if missing

    Scheduler scheduler;
    scheduler.loadFreights("freights.txt");
    scheduler.loadCargos("cargos.txt");
    

    int option;
    do {
        showMenu();
        cout << "\nSelect an option: ";
        cin >> option;
        cin.ignore();

        switch (option) {
        case 1:
            printFreightHeader();
            scheduler.displayAllFreights();
            break;
        case 2:
            printCargoHeader();
            scheduler.displayAllCargos();
            break;
        case 3:
            scheduler.generateSchedule();
            scheduler.displaySchedule();
            break;
        case 4:
            scheduler.displayUnassigned();
            break;
        case 5: {
            bool validInput = false;
            do{
                string id, dest, time;
                printFreightHeader();
                cout << "Enter Freight ID: ";
                getline(cin, id);
                cout << "Enter Refuel Stop: ";
                getline(cin, dest);
                cout << "Enter Refuel Time (HHMM): ";
                getline(cin, time);

                // check if any of the fields are empty
                if (id.empty() || dest.empty() || time.empty()) {
                    cout << "Invalid input: all fields must be filled.\n";
                    break;
                }
                else {
                    // check if the freight's ID is valid and exists already
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
                        for (const auto& f : scheduler.getFreights()) {
                            if (f.getId() == id) {
                                exists = true;
                                break;
                            }
                        }
                    }
                    // check if the time is in valid 24-hour format
                    bool validTime = time.length() == 4; // Must be exactly 4 digits
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

                    if (!valid) {
                        cout << "\n----------------------------------------------\n";
                        cout << "\nInvalid Freight ID: Must start with 'F' followed by digits (e.g., F01, F10, F100, F1000).\n";
                    }
                    else if (exists) {
                        cout << "\n----------------------------------------------\n";
                        cout << "\nFreight ID already exists.\n";
                    }
                    else if (!validTime) {
                        cout << "\n----------------------------------------------\n";
                        cout << "\nInvalid Arrival Time: Must be 4 digits in 24-hour format (0000-2359).\n";
                    }
                    else {
                        scheduler.addFreight(Freight(id, dest, time));
                        cout << "\nFreight added successfully.\n";
                        validInput = true;
                    }
                }
                if (!validInput) {
                    cout << "Would you like to:\n\n";
                    cout << "1. Re-enter details\n";
                    cout << "2. Return to main menu\n\n";
                    cout << "Enter choice (1 or 2): ";
                    int choice;
                    cin >> choice;
                    cin.ignore(); // Clear the newline character
                    if (choice != 1) {
                        break; // Return to main menu if not 1
                    }
                }
            } while (!validInput);
            break;
        }

            
           
        case 6: {
            bool validInput = false;
            do {
                string id, dest, time;
                printCargoHeader();
                cout << "Enter Cargo ID: ";
                getline(cin, id);
                cout << "Enter Destination: ";
                getline(cin, dest);
                cout << "Enter Arrival Time (HHMM): ";
                getline(cin, time);

                // check if any of the fields are empty
                if (id.empty() || dest.empty() || time.empty()) {
                    cout << "Invalid input: all fields must be filled.\n";
                }
                else {
                    // check if the cargo's ID is valid and exists already
                    bool valid = id.length() >= 2 && id[0] == 'C';
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
                        for (const auto& f : scheduler.getCargos()) {
                            if (f.getId() == id) {
                                exists = true;
                                break;
                            }
                        }
                    }

                    // check if the time is in valid 24-hour format
                    bool validTime = time.length() == 4; // Must be exactly 4 digits
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

                    if (!valid) {
                        cout << "\n----------------------------------------------\n";
                        cout << "\nInvalid Cargo ID: Must start with 'C' followed by digits (e.g., C01, C10, C100, C1000).\n";
                    }
                    else if (exists) {
                        cout << "\n----------------------------------------------\n";
                        cout << "\nCargo ID already exists.\n";
                    }
                    else if (!validTime) {
                        cout << "\n----------------------------------------------\n";
                        cout << "\nInvalid Refuel Time: Must be 4 digits in 24-hour format (0000-2359).\n";
                    }
                    else {
                        scheduler.addCargo(Cargo(id, dest, time));
                        cout << "Cargo added successfully.\n";
                        validInput = true;
                    }
                }

                if (!validInput) {
                    cout << "Would you like to:\n\n";
                    cout << "1. Re-enter details\n";
                    cout << "2. Return to main menu\n\n";
                    cout << "Enter choice (1 or 2): ";
                    int choice;
                    cin >> choice;
                    cin.ignore(); // Clear the newline character
                    if (choice != 1) {
                        break; // Return to main menu if not 1
                    }
                }
            } while (!validInput);
            break;
        }
        case 7:
            scheduler.saveSchedule("schedule_output.txt");
            cout << "Schedule saved to schedule_output.txt\n";
            break;

        case 8:
            scheduler.saveFreights("freights.txt");
            scheduler.saveCargos("cargos.txt");
            cout << "Freights and Cargos saved to file.\n";
            break;

        case 9: {
            string id;
            scheduler.displayAllFreights();
            cout << "Enter Freight ID to delete: ";
            getline(cin, id);
            scheduler.deleteFreightById(id);
            break;
        }
        case 10: {
            string id;
            scheduler.displayAllCargos();
            cout << "Enter Cargo ID to delete: ";
            getline(cin, id);
            scheduler.deleteCargoById(id);
            break;
        }
        case 11: {
            string id;
            scheduler.displayAllFreights();
            cout << "Enter Freight ID to edit: ";
            getline(cin, id);
            scheduler.editFreight(id);
            break;
        }
        case 12: {
            string id;
            scheduler.displayAllCargos();
            cout << "Enter Cargo ID to edit: ";
            getline(cin, id);
            scheduler.editCargo(id);
            break;
        }
        case 13:
            cout << "Exiting program.\n";
            break;


        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (option != 13);

    return 0;
}
