#include "Transport.h"
#include <iostream>
using namespace std;

Transport::Transport() : id(""), destination(""), time("") {}

Transport::Transport(const std::string& id, const std::string& destination, const std::string& time)
    : id(id), destination(destination), time(time) {}

std::string Transport::getId() const {
    return id;
}

std::string Transport::getDestination() const {
    return destination;
}

std::string Transport::getTime() const {
    return time;
}

void Transport::display() const {
    cout << "ID: " << id
        << ", Destination: " << destination
        << ", Time: " << time << endl;
}

std::string Transport::toCSV() const {
    return id + "," + destination + "," + time;
}
