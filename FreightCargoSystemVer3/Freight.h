#pragma once
#include "Transport.h"
#include "SaveToFile.h"
class Cargo; // forward declaration

class Freight : public Transport, public SavableToFile {
public:
    Freight(string id, string destination, string refuelTime);
    bool matches(const Cargo& cargo) const;
    void display() const override;
    string toCSV() const override;
};