#pragma once
#include "Transport.h"
#include <string>

class Cargo : public Transport {
private:
    int groupSize;

public:
    Cargo();
    Cargo(const std::string& id, const std::string& dest, const std::string& time, int groupSize = 1);

    int getGroupSize() const;
    void setGroupSize(int size);

    void display() const override;
    std::string toCSV() const override;
};