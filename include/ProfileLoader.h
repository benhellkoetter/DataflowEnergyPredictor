#pragma once
#include <string>
#include <unordered_map>
#include "EnergyModel.h"

namespace dfep
{
    std::unordered_map<InstructionCategory, double>
    loadProfileMapFromJson(const std::string &jsonPath);
}