#include "EnergyOfValueAnalysis.h"

#include "phasar/DataFlow/IfdsIde/EdgeFunctions.h"
#include "phasar/DataFlow/IfdsIde/FlowFunctions.h"
#include "phasar/DataFlow/IfdsIde/IDETabulationProblem.h"
#include "phasar/DataFlow/IfdsIde/InitialSeeds.h"

#include "phasar/PhasarLLVM/Utils/LLVMShorthands.h"
#include "llvm/IR/Instructions.h"
#include <limits>

namespace dfep
{

    EnergyOfValueAnalysis::EnergyOfValueAnalysis(
        const psr::LLVMProjectIRDB *IRDB, psr::LLVMBasedICFG *, std::vector<std::string> EntryPoints)
        : IDETabulationProblem(IRDB, std::move(EntryPoints), dfep::EnergyFact(nullptr))
    {
    }

    double EnergyOfValueAnalysis::topElement() { return std::numeric_limits<double>::infinity(); }
    double EnergyOfValueAnalysis::bottomElement() { return 0.0; }
    double EnergyOfValueAnalysis::join(double Lhs, double Rhs) { return std::max(Lhs, Rhs); }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t> EnergyOfValueAnalysis::allTopFunction()
    {
        return psr::AllTop<l_t>{};
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
                if (Source.V == Inst)
                {
                    return {};
                }

                if (Source == ZeroValue && !Inst->getType()->isVoidTy())
                {
                    return {ZeroValue, d_t(Inst)};
                }

                return {Source};
            }
        };
        return std::make_shared<NormalFlow>(CurrentInst, this->getZeroValue());
    }

    psr::EdgeFunction<EnergyOfValueAnalysis::l_t>
    EnergyOfValueAnalysis::getNormalEdgeFunction(n_t CurrentInst, d_t CurrentNode, n_t, d_t SuccessorNode)
    {
        if (this->isZeroValue(CurrentNode) && SuccessorNode.V != nullptr)
        {
            struct GenEdge
            {
                using l_t = double;
                double EnergyToAssign;
                l_t computeTarget(l_t /*Source*/) const { return EnergyToAssign; }
                bool operator==(const GenEdge &Other) const { return EnergyToAssign == Other.EnergyToAssign; }
                static psr::EdgeFunction<l_t> compose(psr::EdgeFunctionRef<GenEdge> This, const psr::EdgeFunction<l_t> &SecondFunction)
                {
                    if (llvm::isa<psr::EdgeIdentity<l_t>>(SecondFunction))
                    {
                        return This;
                    }
                    return SecondFunction;
                }
                static psr::EdgeFunction<l_t> join(psr::EdgeFunctionRef<GenEdge> This, const psr::EdgeFunction<l_t> &OtherFunction)
                {
                    if (const auto *Other = OtherFunction.dyn_cast<GenEdge>())
                    {
                        if (This->EnergyToAssign == Other->EnergyToAssign)
                        {
                            return GenEdge{This->EnergyToAssign};
                        }
                    }
                    return psr::AllBottom<l_t>{};
                }
            };

            return GenEdge{model.getInstructionEnergy(CurrentInst)};
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