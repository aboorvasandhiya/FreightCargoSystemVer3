#pragma once
#include "Transport.h"
#include "Cargo.h"
#include <string>

class Freight : public Transport {
private:
    std::string freightType;
    int maxCapacity;

public:
    Freight();
    Freight(const std::string& id, const std::string& dest, const std::string& time, const std::string& type);

    std::string getFreightType() const;
    int getMaxCapacity() const;

    void display() const override;
    std::string toCSV() const override;

    bool matches(const Cargo& c) const;
};