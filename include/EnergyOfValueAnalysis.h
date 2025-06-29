#ifndef DATAFLOWENERGYPREDICTOR_ENERGYOFVALUEANALYSIS_H
#define DATAFLOWENERGYPREDICTOR_ENERGYOFVALUEANALYSIS_H

#include "EnergyFact.h"
#include "EnergyModel.h"

#include "phasar/DataFlow/IfdsIde/IDETabulationProblem.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DB/LLVMProjectIRDB.h"
#include "phasar/PhasarLLVM/Domain/LLVMAnalysisDomain.h"

#include <string>
#include <vector>

namespace dfep
{

    struct EnergyAnalysisDomain : public psr::LLVMAnalysisDomainDefault
    {
        using d_t = dfep::EnergyFact;
        using l_t = double;
    };

    class EnergyOfValueAnalysis
        : public psr::IDETabulationProblem<EnergyAnalysisDomain>
    {
    private:
        dfep::EnergyModel model;

    public:
        using InitialSeeds_t = psr::InitialSeeds<n_t, d_t, l_t>;

        EnergyOfValueAnalysis(const psr::LLVMProjectIRDB *IRDB,
                              psr::LLVMBasedICFG *ICFG,
                              std::vector<std::string> EntryPoints = {"main"});

        psr::FlowFunctionPtrType<d_t>
        getNormalFlowFunction(n_t CurrentInst, n_t SuccessorInst) override;

        psr::FlowFunctionPtrType<d_t> getCallFlowFunction(n_t, f_t) override;
        psr::FlowFunctionPtrType<d_t> getRetFlowFunction(n_t, f_t, n_t, n_t) override;
        psr::FlowFunctionPtrType<d_t> getCallToRetFlowFunction(n_t, n_t, llvm::ArrayRef<f_t>) override;

        psr::EdgeFunction<l_t>
        getNormalEdgeFunction(n_t CurrentInst, d_t CurrentNode, n_t SuccessorInst,
                              d_t SuccessorNode) override;

        psr::EdgeFunction<l_t> getCallEdgeFunction(n_t, d_t, f_t, d_t) override;
        psr::EdgeFunction<l_t> getReturnEdgeFunction(n_t, f_t, n_t, d_t, n_t, d_t) override;
        psr::EdgeFunction<l_t> getCallToRetEdgeFunction(n_t, d_t, n_t, d_t, llvm::ArrayRef<f_t>) override;

        l_t topElement() override;
        l_t bottomElement() override;
        l_t join(l_t Lhs, l_t Rhs) override;

        psr::EdgeFunction<l_t> allTopFunction() override;

        InitialSeeds_t initialSeeds() override;
    };

}

#endif