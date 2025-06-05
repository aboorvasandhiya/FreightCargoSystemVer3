#pragma once
#include "Transport.h"
#include "SaveToFile.h"

class Cargo : public Transport, public SavableToFile {
public:
    Cargo(string id, string destination, string arrivalTime);
    void display() const override;
    string toCSV() const override;
};