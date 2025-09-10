#ifndef DATAFLOWENERGYPREDICTOR_ENERGYMODEL_H
#define DATAFLOWENERGYPREDICTOR_ENERGYMODEL_H

#include <unordered_map>
#include <string>
#include <llvm/IR/Instruction.h>

namespace dfep
{

    enum class InstructionCategory
    {
        MEMORY,
        PROGRAMFLOW,
        DIVISION,
        OTHER
    };

    class EnergyModel
    {
    public:
        EnergyModel(std::unordered_map<InstructionCategory, double> energyMap) : energyMap(std::move(energyMap)) {}
        double getInstructionEnergy(const llvm::Instruction *intruction) const;
        static InstructionCategory stringToCategory(const std::string &);

    private:
        std::unordered_map<InstructionCategory, double> energyMap;
        static double defaultEnergy(InstructionCategory category);
    };

}

#endif