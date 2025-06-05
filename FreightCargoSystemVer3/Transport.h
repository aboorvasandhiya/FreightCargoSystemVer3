#pragma once
#include <string>
using namespace std;

class Transport {
protected:
    string id;
    string destination;
    string time;

public:
    Transport(string id, string destination, string time);
    virtual void display() const = 0;
    string getId() const { return id; }
    string getDestination() const { return destination; }
    string getTime() const { return time; }
    virtual ~Transport() = default;
};