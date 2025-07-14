#pragma once
#include "SchedulingStrategy.h"

class ArrivalTimeStrategy : public SchedulingStrategy {
public:
    void generate(
        FreightManager* freightManager,
        CargoManager* cargoManager,
        std::map<std::string, std::string>& assignments
    ) override;
};