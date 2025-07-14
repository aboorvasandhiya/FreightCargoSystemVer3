#pragma once
#include <string>

class Transport {
protected:
    std::string id;
    std::string destination;
    std::string time;

public:
    Transport();
    Transport(const std::string& id, const std::string& destination, const std::string& time);

    std::string getId() const;
    std::string getDestination() const;
    std::string getTime() const;

    virtual void display() const;
    virtual std::string toCSV() const;
};
