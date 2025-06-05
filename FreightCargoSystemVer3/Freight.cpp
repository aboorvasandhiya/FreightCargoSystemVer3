#include "Freight.h"
#include "Cargo.h"
#include <iostream>
#include <iomanip>

Freight::Freight(string id, string destination, string refuelTime)
    : Transport(id, destination, refuelTime) {}

bool Freight::matches(const Cargo& cargo) const {
    return destination == cargo.getDestination() && time == cargo.getTime();
}

void Freight::display() const {
    cout << left << setw(10) << id << setw(15) << destination << time << '\n';
}

string Freight::toCSV() const {
    return id + "," + destination + "," + time;
}

