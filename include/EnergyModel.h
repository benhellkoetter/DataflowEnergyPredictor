#ifndef DATAFLOWENERGYPREDICTOR_ENERGYMODEL_H
#define DATAFLOWENERGYPREDICTOR_ENERGYMODEL_H

namespace llvm
{
    class Instruction;
}

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
        EnergyModel() = default;
        InstructionCategory getInstructionCategory(const llvm::Instruction *Inst) const;
        double getInstructionEnergy(const llvm::Instruction *Inst) const;
    };

}

#endif