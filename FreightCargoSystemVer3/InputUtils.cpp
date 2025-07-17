#include "InputUtils.h" 

int getValidChoice(int min, int max) {
    int choice;
    while (true) {
        std::cout << "Enter choice: ";
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            return choice;
        }
    }
}

bool isValidTime(const std::string& time) {
    if (!std::regex_match(time, std::regex("[0-9]{4}"))) {
        return false; // Not 4 digits
    }
    int hh = std::stoi(time.substr(0, 2));
    int mm = std::stoi(time.substr(2, 2));
    return (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59);
}

bool isValidGroupSize(int groupSize) {
    return (groupSize >= 1 && groupSize <= 10);
}