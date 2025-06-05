#include "Cargo.h"
#include <iostream>
#include <iomanip>

Cargo::Cargo(string id, string destination, string arrivalTime)
    : Transport(id, destination, arrivalTime) {}

void Cargo::display() const {
    cout << left << setw(10) << id << setw(15) << destination << time << '\n';
}

string Cargo::toCSV() const {
    return id + "," + destination + "," + time;
}
