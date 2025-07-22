#include "EnergyModel.h"
#include <llvm/IR/Instructions.h>

namespace dfep
{

    InstructionCategory EnergyModel::stringToCategory(const std::string &S)
    {
        if (S == "DIVISION")
            return InstructionCategory::DIVISION;
        if (S == "MEMORY")
            return InstructionCategory::MEMORY;
        if (S == "CALL")
            return InstructionCategory::CALL;
        if (S == "PROGRAMFLOW")
            return InstructionCategory::PROGRAMFLOW;
        return InstructionCategory::OTHER;
    }

    double EnergyModel::defaultEnergy(InstructionCategory Cat)
    {
        switch (Cat)
        {
        case InstructionCategory::DIVISION:
            return 20.0;
        case InstructionCategory::MEMORY:
            return 5.0;
        case InstructionCategory::CALL:
            return 2.0;
        case InstructionCategory::PROGRAMFLOW:
            return 1.0;
        case InstructionCategory::OTHER:
            return 1.0;
        }
        return 0.0;
    }

    double EnergyModel::getInstructionEnergy(const llvm::Instruction *Inst) const
    {
        InstructionCategory Cat;

        if (llvm::isa<llvm::LoadInst>(Inst) ||
            llvm::isa<llvm::StoreInst>(Inst) ||
            llvm::isa<llvm::AllocaInst>(Inst) ||
            llvm::isa<llvm::AtomicCmpXchgInst>(Inst) ||
            llvm::isa<llvm::AtomicRMWInst>(Inst) ||
            llvm::isa<llvm::GetElementPtrInst>(Inst))
        {
            Cat = InstructionCategory::MEMORY;
        }
        else if (llvm::isa<llvm::CallInst>(Inst) ||
                 llvm::isa<llvm::InvokeInst>(Inst) ||
                 llvm::isa<llvm::CallBrInst>(Inst))
        {
            Cat = InstructionCategory::CALL;
        }
        else if (llvm::isa<llvm::ReturnInst>(Inst) ||
                 llvm::isa<llvm::BranchInst>(Inst) ||
                 llvm::isa<llvm::SwitchInst>(Inst) ||
                 llvm::isa<llvm::IndirectBrInst>(Inst) ||
                 llvm::isa<llvm::CatchReturnInst>(Inst) ||
                 llvm::isa<llvm::CatchSwitchInst>(Inst))
        {
            Cat = InstructionCategory::PROGRAMFLOW;
        }
        else if (auto *Bin = llvm::dyn_cast<llvm::BinaryOperator>(Inst))
        {
            unsigned Op = Bin->getOpcode();
            if (Op == llvm::Instruction::SDiv || Op == llvm::Instruction::UDiv ||
                Op == llvm::Instruction::FDiv || Op == llvm::Instruction::SRem ||
                Op == llvm::Instruction::URem || Op == llvm::Instruction::FRem)
            {
                Cat = InstructionCategory::DIVISION;
            }
            else
            {
                Cat = InstructionCategory::OTHER;
            }
        }
        else
        {
            Cat = InstructionCategory::OTHER;
        }

        // Lookup in der Map, sonst Default
        auto It = EnergyMap.find(Cat);
        if (It != EnergyMap.end())
        {
            return It->second;
        }
        return defaultEnergy(Cat);
    }

}
