#include "EnergyOfValueAnalysis.h"

#include "phasar/DataFlow/IfdsIde/EdgeFunctions.h"
#include "phasar/DataFlow/IfdsIde/FlowFunctions.h"
#include "phasar/DataFlow/IfdsIde/IDETabulationProblem.h"
#include "phasar/DataFlow/IfdsIde/InitialSeeds.h"

#include "phasar/PhasarLLVM/Utils/LLVMShorthands.h"
#include "llvm/IR/Instructions.h"
#include <limits>
#include <set>

namespace dfep
{

    EnergyOfValueAnalysis::EnergyOfValueAnalysis(
        const psr::LLVMProjectIRDB *IRDB, psr::LLVMBasedICFG *, std::vector<std::string> EntryPoints)
        : IDETabulationProblem(IRDB, std::move(EntryPoints), dfep::EnergyFact(nullptr))
    {
    }

    double EnergyOfValueAnalysis::topElement() { return 0.0; }

    double EnergyOfValueAnalysis::bottomElement() { return std::numeric_limits<double>::infinity(); }

    double EnergyOfValueAnalysis::join(double Lhs, double Rhs)
    {
        if (Lhs == topElement())
            return Rhs;
        if (Rhs == topElement())
            return Lhs;
        if (Lhs == bottomElement() || Rhs == bottomElement())
            return bottomElement();

        return std::max(Lhs, Rhs);
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::allTopFunction()
    {
        return psr::AllTop<l_t>{topElement()};
    }

    EnergyOfValueAnalysis::InitialSeeds_t EnergyOfValueAnalysis::initialSeeds()
    {
        InitialSeeds_t Seeds;
        for (const auto &EntryPoint : this->EntryPoints)
        {
            const llvm::Function *F = this->IRDB->getFunctionDefinition(EntryPoint);
            if (F && !F->isDeclaration())
            {
                Seeds.addSeed(&F->front().front(), this->getZeroValue(), topElement());
            }
        }
        return Seeds;
    }

    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getNormalFlowFunction(n_t CurrentInst, n_t)
    {
        struct NormalFlow : psr::FlowFunction<d_t>
        {
            const llvm::Instruction *Inst;
            const d_t ZeroValue;

            NormalFlow(const llvm::Instruction *I, d_t ZV) : Inst(I), ZeroValue(ZV) {}

            std::set<d_t> computeTargets(d_t Source) override
            {
                std::set<d_t> Res;

                if (!Inst->getType()->isVoidTy())
                {
                    if (Source == ZeroValue)
                    {
                        Res.insert(d_t(Inst));
                    }

                    for (const auto &Operand : Inst->operands())
                    {
                        if (const auto *OpInst = llvm::dyn_cast<llvm::Instruction>(Operand))
                        {
                            if (Source.V == OpInst)
                            {
                                Res.insert(d_t(Inst));
                                break;
                            }
                        }
                    }
                }

                if (Source.V != Inst)
                {
                    Res.insert(Source);
                }

                return Res;
            }
        };
        return std::make_shared<NormalFlow>(CurrentInst, getZeroValue());
    }

    struct AddAllOperands
    {
        using l_t = double;
        const llvm::Instruction *Inst;
        const dfep::EnergyModel &Model;

        l_t computeTarget(l_t Source) const
        {
            double sum = 0.0;
            sum += Model.getInstructionEnergy(Inst);
            for (auto &Op : Inst->operands())
            {
                sum += Source;
            }
            return sum;
        }

        bool operator==(const AddAllOperands &Other) const
        {
            return Inst == Other.Inst;
        }

        static psr::EdgeFunction<l_t>
        compose(psr::EdgeFunctionRef<AddAllOperands> This,
                const psr::EdgeFunction<l_t> &SecondFunction)
        {
            if (SecondFunction.dyn_cast<psr::EdgeIdentity<l_t>>())
                return This;
            if (SecondFunction.dyn_cast<AddAllOperands>())
                return This;
            return psr::AllTop<l_t>{0.0};
        }

        static psr::EdgeFunction<l_t>
        join(psr::EdgeFunctionRef<AddAllOperands> This,
             const psr::EdgeFunction<l_t> &Other)
        {
            if (Other.dyn_cast<AddAllOperands>())
                return This;
            return This;
        }
    };

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getNormalEdgeFunction(n_t CurrentInst, d_t CurrentNode, n_t, d_t SuccessorNode)
    {
        struct AddConstant
        {
            using l_t = double;
            double EnergyToAdd;

            l_t computeTarget(l_t Source) const
            {
                if (Source == 0.0)
                    return EnergyToAdd;
                return Source + EnergyToAdd;
            }

            bool operator==(const AddConstant &Other) const
            {
                return std::abs(EnergyToAdd - Other.EnergyToAdd) < 1e-9;
            }

            static psr::EdgeFunction<l_t> compose(psr::EdgeFunctionRef<AddConstant> This, const psr::EdgeFunction<l_t> &SecondFunction)
            {
                if (SecondFunction.dyn_cast<psr::EdgeIdentity<l_t>>())
                {
                    return This;
                }
                if (const auto *Other = SecondFunction.dyn_cast<AddConstant>())
                {
                    return AddConstant{This->EnergyToAdd + Other->EnergyToAdd};
                }
                return psr::AllTop<l_t>{0.0};
            }

            static psr::EdgeFunction<l_t> join(psr::EdgeFunctionRef<AddConstant> This, const psr::EdgeFunction<l_t> &OtherFunction)
            {
                if (OtherFunction.dyn_cast<psr::EdgeIdentity<l_t>>())
                {
                    return This;
                }
                if (const auto *Other = OtherFunction.dyn_cast<AddConstant>())
                {
                    return AddConstant{std::max(This->EnergyToAdd, Other->EnergyToAdd)};
                }
                return psr::AllTop<l_t>{0.0};
            }
        };

        if (this->isZeroValue(CurrentNode) && SuccessorNode.V == CurrentInst)
        {
            double baseCost = model.getInstructionEnergy(CurrentInst);
            return AddConstant{baseCost};
        }

        if (CurrentNode.V != nullptr && SuccessorNode.V == CurrentInst)
        {
            for (const auto &Operand : CurrentInst->operands())
            {
                if (CurrentNode.V == Operand)
                {
                    if (auto *BinOp = llvm::dyn_cast<llvm::BinaryOperator>(CurrentInst))
                    {
                        double operationCost = model.getInstructionEnergy(BinOp);
                        return AddConstant{operationCost};
                    }
                    double operationCost = model.getInstructionEnergy(CurrentInst);
                    return AddConstant{operationCost};
                }
            }
        }

        if (auto *BinOp = llvm::dyn_cast<llvm::BinaryOperator>(CurrentInst))
        {
            if (BinOp->getOpcode() == llvm::Instruction::Add &&
                SuccessorNode.V == CurrentInst)
            {
                return psr::EdgeFunction<l_t>{dfep::AddAllOperands{CurrentInst, model}};
            }
        }

        return psr::EdgeIdentity<l_t>{};
    }

    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getCallFlowFunction(n_t, f_t) { return this->identityFlow(); }
    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getRetFlowFunction(n_t, f_t, n_t, n_t) { return this->identityFlow(); }
    psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
    EnergyOfValueAnalysis::getCallToRetFlowFunction(n_t, n_t, llvm::ArrayRef<f_t>) { return this->identityFlow(); }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getCallEdgeFunction(n_t, d_t, f_t, d_t) { return psr::EdgeIdentity<l_t>{}; }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getReturnEdgeFunction(n_t, f_t, n_t, d_t, n_t, d_t) { return psr::EdgeIdentity<l_t>{}; }
    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getCallToRetEdgeFunction(n_t, d_t, n_t, d_t, llvm::ArrayRef<f_t>) { return psr::EdgeIdentity<l_t>{}; }

}