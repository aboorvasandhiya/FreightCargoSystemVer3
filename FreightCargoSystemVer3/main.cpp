#include <iostream>
#include <fstream>
#include "FreightManager.h"
#include "CargoManager.h"
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

bool isValidId(const string& id, char prefix) {
    if (id.length() < 2 || id[0] != prefix) return false;
    for (size_t i = 1; i < id.length(); ++i)
        if (!isdigit(id[i])) return false;
    return true;
}

bool isValidTime(const string& time) {
    if (time.length() != 4) return false;
    for (char c : time)
        if (!isdigit(c)) return false;
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(2, 2));
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

void showMenu() {
    cout << "\n===== International Freight Cargo Scheduler =====\n";
    cout << "1. Display All Freights\n";
    cout << "2. Display All Cargos\n";
    cout << "3. Generate and Display Schedule\n";
    cout << "4. Display Unassigned Records\n";
    cout << "5. Add Freight\n";
    cout << "6. Add Cargo\n";
    cout << "7. Edit Freight\n";
    cout << "8. Edit Cargo\n";
    cout << "9. Delete Freight\n";
    cout << "10. Delete Cargo\n";
    cout << "11. Save Freight and Cargo\n";
    cout << "12. Save Schedule\n";
    cout << "13. Exit\n";
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
        cin >> option;
        cin.ignore();

        string id, dest, time;

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
        case 5:
            cout << "Enter Freight ID (e.g., F01): ";
            getline(cin, id);
            cout << "Enter Refuel Stop: ";
            getline(cin, dest);
            cout << "Enter Refuel Time (HHMM): ";
            getline(cin, time);
            if (!isValidId(id, 'F'))
                cout << "❌ Invalid Freight ID.\n";
            else if (!isValidTime(time))
                cout << "❌ Invalid Time.\n";
            else
                freightManager.add(Freight(id, dest, time));
            break;
        case 6:
            cout << "Enter Cargo ID (e.g., C01): ";
            getline(cin, id);
            cout << "Enter Destination: ";
            getline(cin, dest);
            cout << "Enter Arrival Time (HHMM): ";
            getline(cin, time);
            if (!isValidId(id, 'C'))
                cout << "❌ Invalid Cargo ID.\n";
            else if (!isValidTime(time))
                cout << "❌ Invalid Time.\n";
            else
                cargoManager.add(Cargo(id, dest, time));
            break;
        case 7:
            cout << "Enter Freight ID to edit: ";
            getline(cin, id);
            freightManager.edit(id);
            break;
        case 8:
            cout << "Enter Cargo ID to edit: ";
            getline(cin, id);
            cargoManager.edit(id);
            break;
        case 9:
            cout << "Enter Freight ID to delete: ";
            getline(cin, id);
            freightManager.remove(id);
            break;
        case 10:
            cout << "Enter Cargo ID to delete: ";
            getline(cin, id);
            cargoManager.remove(id);
            break;
        case 11:
            freightManager.saveToFile("freights.txt");
            cargoManager.saveToFile("cargos.txt");
            cout << "Freights and Cargos saved.\n";
            break;
        case 12:
            scheduler.saveSchedule("schedule_output.txt");
            cout << "Schedule saved to schedule_output.txt\n";
            break;
        case 13:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid option.\n";
        }
    } while (option != 13);

    return 0;
}
