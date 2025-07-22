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
        CALL,
        PROGRAMFLOW,
        DIVISION,
        OTHER
    };

    class EnergyModel
    {
    public:
        EnergyModel(std::unordered_map<InstructionCategory, double> map) : EnergyMap(std::move(map)) {}
        double getInstructionEnergy(const llvm::Instruction *Inst) const;
        static InstructionCategory stringToCategory(const std::string &);

    private:
        std::unordered_map<InstructionCategory, double> EnergyMap;
        static double defaultEnergy(InstructionCategory Cat);
    };

}

#endif