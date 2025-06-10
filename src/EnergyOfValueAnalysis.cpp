#include "EnergyOfValueAnalysis.h"

#include "EnergyFact.h"
#include "phasar/DataFlow/IfdsIde/FlowFunctions.h"
#include "phasar/PhasarLLVM/Utils/LLVMShorthands.h"
#include "phasar/Utils/Logger.h"
#include "llvm/IR/Instructions.h"

namespace dfep {

struct IdentityFlow : public psr::FlowFunction<EnergyOfValueAnalysis::d_t> {
    std::set<EnergyOfValueAnalysis::d_t>
    computeTargets(EnergyOfValueAnalysis::d_t Source) override {
        return {Source};
    }
};

EnergyOfValueAnalysis::EnergyOfValueAnalysis(
    const psr::LLVMProjectIRDB *IRDB, psr::LLVMBasedICFG *ICFG,
    std::vector<std::string> EntryPoints)
    : IFDSTabulationProblem(IRDB, std::move(EntryPoints),
                            dfep::EnergyFact(nullptr, dfep::EnergyBucket::BOTTOM)) {
    this->ICFG = ICFG;
}

psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
EnergyOfValueAnalysis::getNormalFlowFunction(
    n_t CurrentInst, n_t) {

    struct EnergyFlowFunction : public psr::FlowFunction<d_t> {
        const llvm::Instruction* Inst;
        EnergyFlowFunction(const llvm::Instruction* I) : Inst(I) {}

        std::set<d_t> computeTargets(d_t Source) override {
            std::set<d_t> Res;
            Res.insert(Source);
            if (llvm::isa<llvm::AllocaInst>(Inst)) {
                Res.insert(d_t(Inst, dfep::EnergyBucket::ZERO));
            }
            return Res;
        }
    };
    return std::make_shared<EnergyFlowFunction>(CurrentInst);
}

psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
EnergyOfValueAnalysis::getCallFlowFunction(
    n_t, f_t) {
  return std::make_shared<IdentityFlow>();
}

psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
EnergyOfValueAnalysis::getRetFlowFunction(
    n_t , f_t,
    n_t, n_t) {
  return std::make_shared<IdentityFlow>();
}

psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
EnergyOfValueAnalysis::getCallToRetFlowFunction(
    n_t, n_t,
    llvm::ArrayRef<f_t>) {
  return std::make_shared<IdentityFlow>();
}

psr::FlowFunctionPtrType<EnergyOfValueAnalysis::d_t>
EnergyOfValueAnalysis::getSummaryFlowFunction(
    n_t, f_t ) {
  return nullptr;
}

EnergyOfValueAnalysis::InitialSeeds_t EnergyOfValueAnalysis::initialSeeds() {
    InitialSeeds_t Seeds;
    for (const auto &EntryPoint : this->EntryPoints) {
        const llvm::Function *F = this->ICFG->getFunction(EntryPoint);
        if (F && !F->isDeclaration()) {
            Seeds.addSeed(&F->front().front(), *this->ZeroValue);
        } else {
            PHASAR_LOG_LEVEL(WARNING, "Entry point '" << EntryPoint << "' not found or is a declaration.");
        }
    }
    return Seeds;
}

}