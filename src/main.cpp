#include "EnergyOfValueAnalysis.h"

#include "phasar/DataFlow/IfdsIde/Solver/IFDSSolver.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DB/LLVMProjectIRDB.h"
#include "phasar/Utils/Logger.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <string>
#include <set>

static llvm::cl::OptionCategory
    DataflowEnergyPredictorCategory("DataflowEnergyPredictor Options");
static llvm::cl::opt<std::string>
    LLVMFilePath(llvm::cl::Positional, llvm::cl::desc("<LLVM-IR-File>"),
                 llvm::cl::Required, llvm::cl::cat(DataflowEnergyPredictorCategory));
static llvm::cl::list<std::string>
    EntryPoints("E", llvm::cl::desc("Entry points of the analysis"),
                llvm::cl::value_desc("function name"), llvm::cl::Required,
                llvm::cl::cat(DataflowEnergyPredictorCategory));

int main(int argc, char **argv) {
    llvm::cl::ParseCommandLineOptions(argc, argv, "DataflowEnergyPredictor\n");

    psr::Logger::initializeStdoutLogger();
    psr::Logger::setLoggerFilterLevel(psr::SeverityLevel::INFO);

    psr::LLVMProjectIRDB IRDB(LLVMFilePath);
    if (!IRDB.isValid()) {
        std::cerr << "Error: Could not load LLVM IR file: " << LLVMFilePath
                  << std::endl;
        return 1;
    }
    
    psr::LLVMBasedICFG ICFG(&IRDB, psr::CallGraphAnalysisType::CHA, EntryPoints);

    dfep::EnergyOfValueAnalysis EnergyProblem(&IRDB, &ICFG, EntryPoints);

    psr::IFDSSolver<dfep::EnergyAnalysisDomain> Solver(EnergyProblem, &ICFG);
    Solver.solve();

    llvm::outs() << "Results of the EnergyOfValueAnalysis:\n";
    
    for (const llvm::Function *F : ICFG.getAllVertexFunctions()) {
        if (F->isDeclaration()) continue;

        llvm::outs() << "\n--- Function: " << F->getName() << " ---\n";
        for (const llvm::BasicBlock &BB : *F) {
            for (const llvm::Instruction &Inst : BB) {
                std::set<dfep::EnergyFact> Facts = Solver.ifdsResultsAt(&Inst);
                
                if (!Facts.empty()) {
                    Inst.print(llvm::outs());
                    llvm::outs() << "\n  Facts:\n";
                    for (const dfep::EnergyFact &Fact : Facts) {
                        llvm::outs() << "    - " << Fact << "\n";
                    }
                }
            }
        }
    }

    return 0;
}