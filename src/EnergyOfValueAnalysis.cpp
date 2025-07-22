#include "EnergyOfValueAnalysis.h"

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

namespace dfep
{
    EnergyOfValueAnalysis::EnergyOfValueAnalysis(
        const psr::LLVMProjectIRDB *IRDB,
        psr::LLVMBasedICFG *,
        std::vector<std::string> EntryPoints,
        const dfep::EnergyModel *Model)
        : IDETabulationProblem(IRDB, std::move(EntryPoints), EnergyFact(nullptr)), model(Model) {}

    double EnergyOfValueAnalysis::topElement() { return 0.0; }
    double EnergyOfValueAnalysis::bottomElement()
    {
        return std::numeric_limits<double>::infinity();
    }

    double EnergyOfValueAnalysis::join(double Lhs, double Rhs)
    {
        printf("[JOIN] Lhs: %f, Rhs: %f\n", Lhs, Rhs);
        if (Lhs == topElement())
            return Rhs;
        if (Rhs == topElement())
            return Lhs;
        if (Lhs == bottomElement() || Rhs == bottomElement())
            return bottomElement();
        printf("[JOIN] Lhs: %f, Rhs: %f | min: %f | max: %f | sum: %f\n", Lhs, Rhs, std::min(Lhs, Rhs), std::max(Lhs, Rhs), (Lhs + Rhs));
        return std::max(Lhs, Rhs);
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::allTopFunction()
    {
        return psr::AllTop<l_t>{topElement()};
    }

    EnergyOfValueAnalysis::InitialSeeds_t EnergyOfValueAnalysis::initialSeeds()
    {
        InitialSeeds_t Seeds;
        for (auto &EP : EntryPoints)
        {
            if (auto *F = IRDB->getFunctionDefinition(EP))
            {
                if (!F->isDeclaration())
                {
                    Seeds.addSeed(&F->front().front(), this->getZeroValue(), topElement());
                }
            }
        }
        return Seeds;
    }

    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t> EnergyOfValueAnalysis::getNormalFlowFunction(n_t CurrInst, n_t SuccInst)
    {
        struct EnergyFlowFunction : psr::FlowFunction<d_t>
        {
            const llvm::Instruction *Inst;
            const d_t Zero;
            EnergyFlowFunction(const llvm::Instruction *I, d_t Z) : Inst(I), Zero(Z) {}
            std::set<d_t> computeTargets(d_t Source) override
            {
                std::set<d_t> Res;

                Res.insert(Source);

                if (!Inst->getType()->isVoidTy() && Source == Zero)
                {
                    Res.insert(d_t(Inst));
                }

                for (auto &Op : Inst->operands())
                {
                    if (Source.V == Op)
                    {
                        if (!Inst->getType()->isVoidTy())
                        {
                            Res.insert(d_t(Inst));
                        }
                        break;
                    }
                }
                return Res;
            }
        };
        return std::make_shared<EnergyFlowFunction>(CurrInst, getZeroValue());
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::getNormalEdgeFunction(n_t CurrInst, d_t CurrFact,
                                                                                               n_t SuccInst, d_t SuccFact)
    {
        struct AddConstant
        {
            using l_t = double;
            double EnergyToAdd;

            l_t computeTarget(l_t Source) const noexcept
            {
                return Source + EnergyToAdd;
            }

            bool operator==(AddConstant const &O) const noexcept
            {
                return EnergyToAdd == O.EnergyToAdd;
            }

            static psr::EdgeFunction<l_t> compose(psr::EdgeFunctionRef<AddConstant> A, psr::EdgeFunction<l_t> const &B)
            {
                printf("Function compose executed");
                if (auto *O = B.dyn_cast<AddConstant>())
                    return AddConstant{A->EnergyToAdd + O->EnergyToAdd};
                if (B.dyn_cast<psr::EdgeIdentity<l_t>>())
                    return A;
                return psr::AllTop<l_t>{0.0};
            }

            static psr::EdgeFunction<l_t> join(psr::EdgeFunctionRef<AddConstant> A, const psr::EdgeFunction<l_t> &B)
            {
                printf("Function join executed");
                if (auto *O = B.dyn_cast<AddConstant>())
                {
                    double merged = A->EnergyToAdd + O->EnergyToAdd;
                    printf("[JOIN] Merging AddConstant: %f + %f = %f\n",
                           A->EnergyToAdd, O->EnergyToAdd, merged);
                    return AddConstant{merged};
                }
                return A;
            }
        };

        double instructionCost = model->getInstructionEnergy(CurrInst);

        if (CurrFact == getZeroValue() && SuccFact.V == CurrInst)
        {
            return AddConstant{instructionCost};
        }

        if (auto *SI = llvm::dyn_cast<llvm::StoreInst>(CurrInst))
        {
            if (CurrFact.V == SI->getPointerOperand() &&
                SuccFact.V == SI->getPointerOperand())
            {
                return AddConstant{instructionCost};
            }
        }

        return psr::EdgeIdentity<l_t>{};
    }

    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getCallFlowFunction(n_t, f_t)
    {
        return identityFlow();
    }
    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getRetFlowFunction(n_t, f_t, n_t, n_t)
    {
        return identityFlow();
    }
    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getCallToRetFlowFunction(n_t, n_t,
                                                    llvm::ArrayRef<f_t>)
    {
        return identityFlow();
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getCallEdgeFunction(n_t, d_t, f_t, d_t)
    {
        return psr::EdgeIdentity<l_t>{};
    }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getReturnEdgeFunction(n_t, f_t, n_t, d_t, n_t, d_t)
    {
        return psr::EdgeIdentity<l_t>{};
    }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getCallToRetEdgeFunction(n_t, d_t, n_t, d_t,
                                                    llvm::ArrayRef<f_t>)
    {
        return psr::EdgeIdentity<l_t>{};
    }

}