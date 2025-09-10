#include "EnergyModel.h"
#include <llvm/IR/Instructions.h>

namespace dfep
{

    InstructionCategory EnergyModel::stringToCategory(const std::string &string)
    {
        std::string upper = string;
        std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char chr)
                       { return static_cast<char>(std::toupper(chr)); });

        if (upper == "DIVISION")
            return InstructionCategory::DIVISION;
        if (upper == "MEMORY")
            return InstructionCategory::MEMORY;
        if (upper == "PROGRAMFLOW")
            return InstructionCategory::PROGRAMFLOW;
        return InstructionCategory::OTHER;
    }

    double EnergyModel::defaultEnergy(InstructionCategory category)
    {
        switch (category)
        {
        case InstructionCategory::DIVISION:
            return 10.0;
        case InstructionCategory::MEMORY:
            return 15.0;
        case InstructionCategory::PROGRAMFLOW:
            return 5.0;
        case InstructionCategory::OTHER:
            return 20.0;
        }
        return 0.0;
    }

    double EnergyModel::getInstructionEnergy(const llvm::Instruction *instruction) const
    {
        InstructionCategory category;

        if (llvm::isa<llvm::LoadInst>(instruction) ||
            llvm::isa<llvm::StoreInst>(instruction) ||
            llvm::isa<llvm::AllocaInst>(instruction) ||
            llvm::isa<llvm::AtomicCmpXchgInst>(instruction) ||
            llvm::isa<llvm::AtomicRMWInst>(instruction) ||
            llvm::isa<llvm::GetElementPtrInst>(instruction))
        {
            category = InstructionCategory::MEMORY;
        }
        else if (llvm::isa<llvm::ReturnInst>(instruction) ||
                 llvm::isa<llvm::BranchInst>(instruction) ||
                 llvm::isa<llvm::SwitchInst>(instruction) ||
                 llvm::isa<llvm::IndirectBrInst>(instruction) ||
                 llvm::isa<llvm::CatchReturnInst>(instruction) ||
                 llvm::isa<llvm::CatchSwitchInst>(instruction))
        {
            category = InstructionCategory::PROGRAMFLOW;
        }
        else if (auto *binaryOp = llvm::dyn_cast<llvm::BinaryOperator>(instruction))
        {
            unsigned opCode = binaryOp->getOpcode();
            if (opCode == llvm::Instruction::SDiv || opCode == llvm::Instruction::UDiv ||
                opCode == llvm::Instruction::FDiv || opCode == llvm::Instruction::SRem ||
                opCode == llvm::Instruction::URem || opCode == llvm::Instruction::FRem)
            {
                category = InstructionCategory::DIVISION;
            }
            else
            {
                category = InstructionCategory::OTHER;
            }
        }
        else
        {
            category = InstructionCategory::OTHER;
        }

        auto iterator = energyMap.find(category);
        if (iterator != energyMap.end())
        {
            return iterator->second;
        }
        return defaultEnergy(category);
    }

}
