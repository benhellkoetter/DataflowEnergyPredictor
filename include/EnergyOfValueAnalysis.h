#ifndef DATAFLOWENERGYPREDICTOR_ENERGYOFVALUEANALYSIS_H
#define DATAFLOWENERGYPREDICTOR_ENERGYOFVALUEANALYSIS_H

#include "EnergyFact.h"
#include "EnergyModel.h"

#include "phasar/DataFlow/IfdsIde/IFDSTabulationProblem.h"
#include "phasar/DataFlow/IfdsIde/InitialSeeds.h"
#include "phasar/Domain/BinaryDomain.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DB/LLVMProjectIRDB.h"
#include "phasar/PhasarLLVM/Domain/LLVMAnalysisDomain.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"

#include <string>
#include <vector>

namespace dfep {

struct EnergyAnalysisDomain : public psr::LLVMAnalysisDomainDefault {
    using d_t = dfep::EnergyFact;
    using l_t = psr::BinaryDomain;
};

class EnergyOfValueAnalysis
    : public psr::IFDSTabulationProblem<EnergyAnalysisDomain> {
private:
    dfep::EnergyModel model;
    psr::LLVMBasedICFG *ICFG;

public:
    using InitialSeeds_t = psr::InitialSeeds<n_t, d_t, l_t>;

    EnergyOfValueAnalysis(const psr::LLVMProjectIRDB *IRDB,
                          psr::LLVMBasedICFG *ICFG,
                          std::vector<std::string> EntryPoints = {"main"});

    psr::FlowFunctionPtrType<d_t>
    getNormalFlowFunction(n_t CurrentInst, n_t SuccessorInst) override;

    psr::FlowFunctionPtrType<d_t>
    getCallFlowFunction(n_t CallSite, f_t DestFun) override;

    psr::FlowFunctionPtrType<d_t>
    getRetFlowFunction(n_t CallSite, f_t CalleeFun, n_t ExitInst,
                       n_t RetSite) override;

    psr::FlowFunctionPtrType<d_t>
    getCallToRetFlowFunction(n_t CallSite, n_t RetSite,
                             llvm::ArrayRef<f_t> Callees) override;

    psr::FlowFunctionPtrType<d_t>
    getSummaryFlowFunction(n_t CallSite, f_t DestFun) override;

    InitialSeeds_t initialSeeds() override;
};

}

#endif