#pragma once
#include <string>
using namespace std;

class SavableToFile {
public:
    virtual string toCSV() const = 0;
    virtual ~SavableToFile() = default;
};