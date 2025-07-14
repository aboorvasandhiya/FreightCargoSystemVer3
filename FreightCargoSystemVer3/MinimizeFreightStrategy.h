#pragma once
#include "SchedulingStrategy.h"
#include <map>

class MinimizeFreightStrategy : public SchedulingStrategy {
public:
    void generate(
        FreightManager* freightManager,
        CargoManager* cargoManager,
        std::map<std::string, std::string>& assignments
    ) override;
};