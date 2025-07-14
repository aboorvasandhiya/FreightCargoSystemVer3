#include "Freight.h"
#include <iostream>
using namespace std;

Freight::Freight() : Transport(), freightType(""), maxCapacity(0) {}

Freight::Freight(const string& id, const string& dest, const string& time, const string& type)
    : Transport(id, dest, time), freightType(type) {
    if (type == "MiniMover")
        maxCapacity = 2;
    else if (type == "CargoCruiser")
        maxCapacity = 6;
    else if (type == "MegaCarrier")
        maxCapacity = 12;
    else
        maxCapacity = 0; // fallback
}

string Freight::getFreightType() const {
    return freightType;
}

int Freight::getMaxCapacity() const {
    return maxCapacity;
}

void Freight::display() const {
    cout << "Freight ID: " << getId()
        << ", Destination: " << getDestination()
        << ", Time: " << getTime()
        << ", Type: " << freightType
        << ", Max Capacity: " << maxCapacity << endl;
}

string Freight::toCSV() const {
    return getId() + "," + getDestination() + "," + getTime() + "," + freightType;
}

bool Freight::matches(const Cargo& c) const {
    return getDestination() == c.getDestination() && getTime() >= c.getTime();
}