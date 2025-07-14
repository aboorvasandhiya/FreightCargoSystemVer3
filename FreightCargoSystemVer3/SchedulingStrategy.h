#pragma once
#include "FreightManager.h"
#include "CargoManager.h"
#include <map>

class SchedulingStrategy {
public:
    virtual void generate(
        FreightManager* freightManager,
        CargoManager* cargoManager,
        std::map<std::string, std::string>& assignments
    ) = 0;

    virtual ~SchedulingStrategy() = default;
};