#include <EnergyModel.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>

namespace dfep {

    InstructionCategory
    EnergyModel::getInstructionCategory(const llvm::Instruction *Inst) const {
        
        if (llvm::isa<llvm::LoadInst>(Inst) || llvm::isa<llvm::StoreInst>(Inst) ||
            llvm::isa<llvm::AllocaInst>(Inst) ||
            llvm::isa<llvm::AtomicCmpXchgInst>(Inst) ||
            llvm::isa<llvm::AtomicRMWInst>(Inst) ||
            llvm::isa<llvm::GetElementPtrInst>(Inst)) {
            return InstructionCategory::MEMORY;
        }

        if (llvm::isa<llvm::CallInst>(Inst) || llvm::isa<llvm::InvokeInst>(Inst) ||
            llvm::isa<llvm::CallBrInst>(Inst)) {
            return InstructionCategory::CALL;
        }

        if (llvm::isa<llvm::ReturnInst>(Inst) ||
            llvm::isa<llvm::BranchInst>(Inst) ||
            llvm::isa<llvm::SwitchInst>(Inst) ||
            llvm::isa<llvm::IndirectBrInst>(Inst) ||
            llvm::isa<llvm::CatchReturnInst>(Inst) ||
            llvm::isa<llvm::CatchSwitchInst>(Inst)) {
            return InstructionCategory::PROGRAMFLOW;
        }

        if (llvm::isa<llvm::BinaryOperator>(Inst)) {
            unsigned OpCode = Inst->getOpcode();
            if (OpCode == llvm::Instruction::SDiv || OpCode == llvm::Instruction::UDiv ||
                OpCode == llvm::Instruction::FDiv || OpCode == llvm::Instruction::SRem ||
                OpCode == llvm::Instruction::URem || OpCode == llvm::Instruction::FRem) {
                return InstructionCategory::DIVISION;
            }
        }

        return InstructionCategory::OTHER;
    }

}
