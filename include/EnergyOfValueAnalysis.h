#ifndef DATAFLOWENERGYPREDICTOR_ENERGYOFVALUEANALYSIS_H
#define DATAFLOWENERGYPREDICTOR_ENERGYOFVALUEANALYSIS_H

#include "EnergyFact.h"
#include "EnergyModel.h"

#include "phasar/DataFlow/IfdsIde/IDETabulationProblem.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DB/LLVMProjectIRDB.h"
#include "phasar/PhasarLLVM/Domain/LLVMAnalysisDomain.h"
#include "phasar/DataFlow/IfdsIde/Solver/IDESolver.h"

#include <string>
#include <vector>

#include <unordered_map>

namespace dfep
{

    struct EnergyAnalysisDomain : public psr::LLVMAnalysisDomainDefault
    {
        using d_t = dfep::EnergyFact;
        using n_t = const llvm::Instruction *;
        using l_t = double;
    };

    class EnergyOfValueAnalysis
        : public psr::IDETabulationProblem<EnergyAnalysisDomain>
    {
    private:
        const dfep::EnergyModel *model;
        int maxInstructionRepetitions;

        struct PairHash
        {
            size_t operator()(const std::pair<const llvm::Instruction *, const llvm::Value *> &p) const noexcept
            {
                auto h1 = std::hash<const void *>{}(reinterpret_cast<const void *>(p.first));
                auto h2 = std::hash<const void *>{}(reinterpret_cast<const void *>(p.second));
                return h1 ^ (h2 << 1);
            }
        };

        std::unordered_map<std::pair<const llvm::Instruction *, const llvm::Value *>, int, PairHash> instructionFactCounts;
        bool allowInstructionForInstAndFact(const llvm::Instruction *inst, const llvm::Value *factVal);

    public:
        using InitialSeeds_t = psr::InitialSeeds<n_t, d_t, l_t>;

        EnergyOfValueAnalysis(const psr::LLVMProjectIRDB *irdb, psr::LLVMBasedICFG *icfg, std::vector<std::string> entryPoints, const dfep::EnergyModel *model, int maxInstructionRepetitions);

        psr::FlowFunctionPtrType<d_t> getNormalFlowFunction(n_t currentInstruction, n_t successorInstruction) override;

        psr::FlowFunctionPtrType<d_t> getCallFlowFunction(n_t, f_t) override;
        psr::FlowFunctionPtrType<d_t> getRetFlowFunction(n_t, f_t, n_t, n_t) override;
        psr::FlowFunctionPtrType<d_t> getCallToRetFlowFunction(n_t, n_t, llvm::ArrayRef<f_t>) override;

        psr::EdgeFunction<l_t> getNormalEdgeFunction(n_t currentInstruction, d_t currentNode, n_t successorInstruction, d_t successorNode) override;

        psr::EdgeFunction<l_t> getCallEdgeFunction(n_t, d_t, f_t, d_t) override;
        psr::EdgeFunction<l_t> getReturnEdgeFunction(n_t, f_t, n_t, d_t, n_t, d_t) override;
        psr::EdgeFunction<l_t> getCallToRetEdgeFunction(n_t, d_t, n_t, d_t, llvm::ArrayRef<f_t>) override;

        l_t topElement() override;
        l_t bottomElement() override;
        l_t join(l_t leftHandSide, l_t rightHandSide) override;

        psr::EdgeFunction<l_t> allTopFunction() override;

        InitialSeeds_t initialSeeds() override;
    };

}

#endif