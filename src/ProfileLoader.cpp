#include "ProfileLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace dfep
{
    std::unordered_map<InstructionCategory, double> loadProfileMapFromJson(const std::string &jsonPath)
    {
        std::unordered_map<InstructionCategory, double> map;
        if (jsonPath.empty())
        {
            return map;
        }
        std::ifstream input(jsonPath);
        if (!input)
        {
            llvm::errs() << "Error: cannot open JSON file " << jsonPath << "\n";
            return map;
        }
        nlohmann::json profile;
        input >> profile;
        if (profile.contains("profile"))
        {
            for (auto &iterator : profile["profile"].items())
            {
                auto key = iterator.key();
                double value = iterator.value().get<double>();
                InstructionCategory category = EnergyModel::stringToCategory(key);
                map[category] = value;
            }
        }
        return map;
    }
}
