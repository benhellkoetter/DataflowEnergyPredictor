#include "ProfileLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <llvm/Support/raw_ostream.h>

using json = nlohmann::json;

namespace dfep
{
    std::unordered_map<InstructionCategory, double>
    loadProfileMapFromJson(const std::string &jsonPath)
    {
        std::unordered_map<InstructionCategory, double> M;
        if (jsonPath.empty())
        {
            return M;
        }
        std::ifstream In(jsonPath);
        if (!In)
        {
            llvm::errs() << "Error: cannot open JSON file: " << jsonPath << "\n";
            return M;
        }
        json Profile;
        In >> Profile;
        if (Profile.contains("profile"))
        {
            for (auto &it : Profile["profile"].items())
            {
                auto key = it.key();
                double val = it.value().get<double>();
                InstructionCategory cat = EnergyModel::stringToCategory(key);
                M[cat] = val;
            }
        }
        return M;
    }
}
