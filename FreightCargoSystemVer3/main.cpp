#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include "FreightManager.h"
#include "CargoManager.h"
#include "Scheduler.h"
#include "ArrivalTimeStrategy.h"
#include "MinimizeFreightStrategy.h"
using namespace std;

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
        fout.close();
        cout << "Created freights.txt with sample data.\n";
    }

    if (!fileExists("cargos.txt")) {
        ofstream fout("cargos.txt");
        fout << "C05,London,0730,2\n";
        fout << "C07,Paris,1720,1\n";
        fout << "C09,New York,1400,3\n";
        fout.close();
        cout << "Created cargos.txt with sample data.\n";
    }
}

void showMenu() {
    cout << "\n===== International Freight Cargo Scheduler =====\n";
    cout << setw(2) << 1 << ". Display All Freights\n";
    cout << setw(2) << 2 << ". Display All Cargos\n";
    cout << setw(2) << 3 << ". Generate and Display Schedule\n";
    cout << setw(2) << 4 << ". Display Unassigned Records\n";
    cout << setw(2) << 5 << ". Choose Scheduling Strategy\n";
    cout << setw(2) << 6 << ". Add Freight\n";
    cout << setw(2) << 7 << ". Add Cargo\n";
    cout << setw(2) << 8 << ". Edit Freight by ID\n";
    cout << setw(2) << 9 << ". Edit Cargo by ID\n";
    cout << setw(2) << 10 << ". Save Schedule to File\n";
    cout << setw(2) << 11 << ". Save All Freights and Cargos to File\n";
    cout << setw(2) << 12 << ". Delete Freight by ID\n";
    cout << setw(2) << 13 << ". Delete Cargo by ID\n";
    cout << setw(2) << 14 << ". Undo Last Deleted Freight\n";
    cout << setw(2) << 15 << ". Undo Last Deleted Cargo\n";
    cout << setw(2) << 16 << ". Exit\n";
}

int getValidChoice() {
    int choice;
    while (!(cin >> choice)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

int main() {
    createSampleFiles();

    FreightManager freightManager;
    CargoManager cargoManager;
    freightManager.loadFromFile("freights.txt");
    cargoManager.loadFromFile("cargos.txt");

    Scheduler scheduler(&freightManager, &cargoManager);

    int option;
    do {
        showMenu();
        cout << "\nSelect an option: ";
        option = getValidChoice();

        switch (option) {
        case 1:
            freightManager.displayAll();
            break;
        case 2:
            cargoManager.displayAll();
            break;
        case 3:
            scheduler.generateSchedule();
            scheduler.displaySchedule();
            break;
        case 4:
            scheduler.displayUnassigned();
            break;
        case 5: {
            cout << "\nChoose scheduling strategy:\n";
            cout << "1. Arrival Time Priority\n";
            cout << "2. Minimize Freight Usage\n";
            cout << "Enter choice: ";
            int choice = getValidChoice();
            if (choice == 1) {
                scheduler.setStrategy(new ArrivalTimeStrategy());
                cout << "Strategy set to Arrival Time Priority.\n";
            }
            else if (choice == 2) {
                scheduler.setStrategy(new MinimizeFreightStrategy());
                cout << "Strategy set to Minimize Freight Usage.\n";
            }
            else {
                cout << "Invalid strategy choice.\n";
            }
            break;
        }
        case 6: {
            string id, dest, time, type;
            cout << "Enter Freight ID: ";
            getline(cin, id);
            cout << "Enter Destination: ";
            getline(cin, dest);
            cout << "Enter Refuel Time (HHMM): ";
            getline(cin, time);
            cout << "Enter Freight Type (MiniMover, CargoCruiser, MegaCarrier): ";
            getline(cin, type);
            freightManager.add(Freight(id, dest, time, type));
            break;
        }
        case 7: {
            string id, dest, time;
            int groupSize;
            cout << "Enter Cargo ID: ";
            getline(cin, id);
            cout << "Enter Destination: ";
            getline(cin, dest);
            cout << "Enter Arrival Time (HHMM): ";
            getline(cin, time);
            cout << "Enter Group Size (1-10): ";
            cin >> groupSize;
            cin.ignore();
            cargoManager.add(Cargo(id, dest, time, groupSize));
            break;
        }
        case 8: {
            string id;
            cout << "Enter Freight ID to edit: ";
            getline(cin, id);
            freightManager.edit(id);
            break;
        }
        case 9: {
            string id;
            cout << "Enter Cargo ID to edit: ";
            getline(cin, id);
            cargoManager.edit(id);
            break;
        }
        case 10:
            scheduler.saveSchedule("schedule_output.txt");
            break;
        case 11:
            freightManager.saveToFile("freights.txt");
            cargoManager.saveToFile("cargos.txt");
            break;
        case 12: {
            string id;
            cout << "Enter Freight ID to delete: ";
            getline(cin, id);
            freightManager.remove(id);
            break;
        }
        case 13: {
            string id;
            cout << "Enter Cargo ID to delete: ";
            getline(cin, id);
            cargoManager.remove(id);
            break;
        }
        case 14:
            freightManager.undoDelete();
            break;
        case 15:
            cargoManager.undoDelete();
            break;
        case 16:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (option != 16);

    return 0;
}