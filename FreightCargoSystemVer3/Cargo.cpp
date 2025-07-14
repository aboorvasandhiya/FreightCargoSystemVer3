#include "Cargo.h"
#include <iostream>
using namespace std;

Cargo::Cargo() : Transport(), groupSize(1) {}

Cargo::Cargo(const string& id, const string& dest, const string& time, int groupSize)
    : Transport(id, dest, time), groupSize(groupSize) {}

int Cargo::getGroupSize() const {
    return groupSize;
}

void Cargo::setGroupSize(int size) {
    groupSize = size;
}

void Cargo::display() const {
    cout << "Cargo ID: " << getId()
        << ", Destination: " << getDestination()
        << ", Time: " << getTime()
        << ", Group Size: " << groupSize << endl;
}

string Cargo::toCSV() const {
    return getId() + "," + getDestination() + "," + getTime() + "," + to_string(groupSize);
}