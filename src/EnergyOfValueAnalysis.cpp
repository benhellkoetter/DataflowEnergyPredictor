#include "EnergyOfValueAnalysis.h"
#include "EnergyEdgeFunctions.h"

#include "phasar/DataFlow/IfdsIde/EdgeFunctions.h"
#include "phasar/DataFlow/IfdsIde/FlowFunctions.h"
#include "phasar/DataFlow/IfdsIde/IDETabulationProblem.h"
#include "phasar/DataFlow/IfdsIde/InitialSeeds.h"
#include "phasar/PhasarLLVM/Utils/LLVMShorthands.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Instructions.h"

#include <limits>
#include <set>
#include <cmath>
#include <algorithm>

#include <unordered_map>

namespace dfep
{
    EnergyOfValueAnalysis::EnergyOfValueAnalysis(
        const psr::LLVMProjectIRDB *irdb,
        psr::LLVMBasedICFG *icfg,
        std::vector<std::string> entryPoints,
        const dfep::EnergyModel *model, int maxInstructionRepetitions)
        : IDETabulationProblem(irdb, std::move(entryPoints), EnergyFact(nullptr)), model(model), maxInstructionRepetitions(maxInstructionRepetitions) {}

    using l_t = EnergyOfValueAnalysis::l_t;

    double EnergyOfValueAnalysis::topElement() { return 0.0; }

    double EnergyOfValueAnalysis::bottomElement()
    {
        return std::numeric_limits<double>::infinity();
    }

    double EnergyOfValueAnalysis::join(double leftHandSide, double rightHandSide)
    {
        if (leftHandSide == topElement())
            return rightHandSide;

        if (rightHandSide == topElement())
            return leftHandSide;

        if (leftHandSide == bottomElement() || rightHandSide == bottomElement())
            return bottomElement();

        return std::max(leftHandSide, rightHandSide);
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::allTopFunction()
    {
        return psr::AllTop<l_t>{topElement()};
    }

    EnergyOfValueAnalysis::InitialSeeds_t EnergyOfValueAnalysis::initialSeeds()
    {
        InitialSeeds_t seeds;
        for (auto &entryPoints : EntryPoints)
        {
            if (auto *function = IRDB->getFunctionDefinition(entryPoints))
            {
                if (!function->isDeclaration())
                {
                    seeds.addSeed(&function->front().front(), this->getZeroValue(), topElement());
                }
            }
        }
        return seeds;
    }

    static std::string valueToString(const llvm::Value *value)
    {
        if (!value)
            return "nullptr";
        std::string str;
        llvm::raw_string_ostream rso(str);
        value->printAsOperand(rso, false);
        return rso.str();
    }

    static std::string factToString(const d_t &fact)
    {
        if (fact.mValue == nullptr)
            return "Zero";
        return valueToString(fact.mValue);
    }

    bool EnergyOfValueAnalysis::allowInstructionForInstAndFact(const llvm::Instruction *inst, const llvm::Value *factVal)
    {
        auto key = std::make_pair(inst, factVal);
        int &cnt = this->instructionFactCounts[key];
        ++cnt;
        llvm::errs() << "[COUNT] inst=" << valueToString(inst) << " fact=" << (factVal ? valueToString(factVal) : std::string("Zero")) << " cnt=" << cnt
                     << " limit=" << this->maxInstructionRepetitions << "\n";
        return cnt <= this->maxInstructionRepetitions;
    }

    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t> EnergyOfValueAnalysis::getNormalFlowFunction(n_t currentInstruction, n_t successorInstruction)
    {
        llvm::errs() << "[TRACE] getNormalFlowFunction called: inst=" << valueToString(currentInstruction) << " succ=" << valueToString(successorInstruction) << "\n";

        struct EnergyFlowFunction : psr::FlowFunction<d_t>
        {
            EnergyOfValueAnalysis *mAnalysis;
            const llvm::Instruction *mInstruction;
            const d_t mZero;
            EnergyFlowFunction(EnergyOfValueAnalysis *analysis, const llvm::Instruction *instruction, d_t zero) : mAnalysis(analysis), mInstruction(instruction), mZero(zero) {}

            std::set<d_t> computeTargets(d_t source) override
            {
                if (!mAnalysis->allowInstructionForInstAndFact(mInstruction, source.mValue))
                {
                    llvm::errs() << "[FLOW] skipping all targets for inst " << valueToString(mInstruction) << " (max repetitions reached)\n";
                    return {};
                }
                std::set<d_t> result;
                result.insert(source);

                llvm::errs() << "\n\n[FLOW] Inst: ";
                mInstruction->print(llvm::errs());
                llvm::errs() << "\n       sourceFact = " << factToString(source) << "\n";

                if (!mInstruction->getType()->isVoidTy() && source == mZero)
                {
                    llvm::errs() << "[FLOW] -> adding Zero -> result fact (" << valueToString(mInstruction) << ")\n";
                    result.insert(d_t(mInstruction));
                }

                for (auto &operand : mInstruction->operands())
                {
                    if (source.mValue == operand)
                    {
                        if (!mInstruction->getType()->isVoidTy())
                        {
                            llvm::errs() << "[FLOW] -> operand match: adding operand -> result (" << valueToString(mInstruction) << ")\n";
                            result.insert(d_t(mInstruction));
                        }
                        break;
                    }
                }

                if (auto *storeInstruction = llvm::dyn_cast<llvm::StoreInst>(mInstruction))
                {

                    if (source == mZero)
                    {
                        llvm::errs() << "[FLOW] -> (Zero case) adding Zero -> ptr (" << valueToString(storeInstruction->getPointerOperand()) << ")\n";
                        result.insert(d_t(storeInstruction->getPointerOperand()));
                    }

                    if (source.mValue == storeInstruction->getValueOperand())
                    {
                        llvm::errs() << "[FLOW] -> (Value case) adding value -> ptr (" << valueToString(storeInstruction->getPointerOperand()) << ")\n";
                        result.insert(d_t(storeInstruction->getPointerOperand()));
                    }
                }

                llvm::errs() << "[FLOW] Final targets for inst ";
                mInstruction->print(llvm::errs());
                llvm::errs() << " :";
                for (auto &targets : result)
                {
                    llvm::errs() << " [" << factToString(targets) << "]";
                }
                llvm::errs() << "\n";

                return result;
            }
        };

        return std::make_shared<EnergyFlowFunction>(this, currentInstruction, getZeroValue());
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::getNormalEdgeFunction(n_t currentInstruction, d_t currentFact, n_t successorInstruction, d_t successorFact)
    {
        auto printChoice = [&](const char *which, double val)
        {
            llvm::errs() << "[EDGE-NEW] Inst: ";
            currentInstruction->print(llvm::errs());
            llvm::errs() << "\n         Choice: " << which
                         << "  currFact=" << dfep::valueToString(currentFact.mValue)
                         << "  succFact=" << dfep::valueToString(successorFact.mValue)
                         << "  instrCost=" << val << "\n";
        };

        double instructionCost = model->getInstructionEnergy(currentInstruction);

        if (!currentInstruction->getType()->isVoidTy() && currentFact == getZeroValue() && successorFact.mValue == currentInstruction)
        {
            printChoice("Zero->Result (ENERGY)", instructionCost);
            return EnergyEdgeFunction{instructionCost, instructionCost, currentFact, successorFact, currentInstruction, successorInstruction};
        }

        if (successorFact.mValue == currentInstruction)
        {
            for (const llvm::Use &operand : currentInstruction->operands())
            {
                if (operand.get() == currentFact.mValue)
                {
                    printChoice("Operand->Result (IDENT)", 0.0);
                    return EnergyEdgeIdentityFunction{0, 0, currentFact, successorFact, currentInstruction, successorInstruction};
                }
            }
        }

        if (auto *storeInstruction = llvm::dyn_cast<llvm::StoreInst>(currentInstruction))
        {

            if (currentFact == getZeroValue() && successorFact.mValue == storeInstruction->getPointerOperand())
            {
                printChoice("Zero->Ptr (ENERGY)", instructionCost);
                return EnergyEdgeFunction{instructionCost, instructionCost, currentFact, successorFact, currentInstruction, successorInstruction};
            }

            if (currentFact.mValue == storeInstruction->getValueOperand() && successorFact.mValue == storeInstruction->getPointerOperand())
            {
                printChoice("Value->Ptr (IDENT)", 0.0);
                return EnergyEdgeIdentityFunction{0, 0, currentFact, successorFact, currentInstruction, successorInstruction};
            }
        }
        printChoice("Default (IDENT)", 0.0);
        return EnergyEdgeIdentityFunction{0, 0, currentFact, successorFact, currentInstruction, successorInstruction};
    }

    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t> EnergyOfValueAnalysis::getCallFlowFunction(n_t, f_t)
    {
        return identityFlow();
    }
    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t> EnergyOfValueAnalysis::getRetFlowFunction(n_t, f_t, n_t, n_t)
    {
        return identityFlow();
    }
    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t> EnergyOfValueAnalysis::getCallToRetFlowFunction(n_t, n_t, llvm::ArrayRef<f_t>)
    {
        return identityFlow();
    }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::getCallEdgeFunction(n_t, d_t, f_t, d_t)
    {
        return psr::EdgeIdentity<l_t>{};
    }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::getReturnEdgeFunction(n_t, f_t, n_t, d_t, n_t, d_t)
    {
        return psr::EdgeIdentity<l_t>{};
    }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::getCallToRetEdgeFunction(n_t, d_t, n_t, d_t, llvm::ArrayRef<f_t>)
    {
        return psr::EdgeIdentity<l_t>{};
    }
}