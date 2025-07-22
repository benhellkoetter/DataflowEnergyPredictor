#include "EnergyOfValueAnalysis.h"
#include "phasar/PhasarLLVM/TypeHierarchy/DIBasedTypeHierarchy.h"

#include "phasar/DataFlow/IfdsIde/Solver/IDESolver.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DB/LLVMProjectIRDB.h"
#include "phasar/Utils/Logger.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>

#include "ProfileLoader.h"

static llvm::cl::OptionCategory
    DataflowEnergyPredictorCategory("DataflowEnergyPredictor Options");
static llvm::cl::opt<std::string>
    LLVMFilePath(llvm::cl::Positional, llvm::cl::desc("<LLVM-IR-File>"),
                 llvm::cl::Required, llvm::cl::cat(DataflowEnergyPredictorCategory));
static llvm::cl::opt<std::string> JSONProfilePath(
    "profile",
    llvm::cl::desc("Path to JSON energy profile"),
    llvm::cl::value_desc("profile.json"),
    llvm::cl::Optional,
    llvm::cl::cat(DataflowEnergyPredictorCategory));
static llvm::cl::list<std::string>
    EntryPoints("E", llvm::cl::desc("Entry points of the analysis"),
                llvm::cl::value_desc("function name"), llvm::cl::Required,
                llvm::cl::cat(DataflowEnergyPredictorCategory));

int main(int argc, char **argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "DataflowEnergyPredictor\n");

    auto profileMap = dfep::loadProfileMapFromJson(JSONProfilePath);
    dfep::EnergyModel Model(std::move(profileMap));

    psr::Logger::initializeStdoutLogger();
    psr::Logger::setLoggerFilterLevel(psr::SeverityLevel::INFO);

    psr::LLVMProjectIRDB IRDB(LLVMFilePath);
    if (!IRDB.isValid())
    {
        std::cerr << "Error: loading LLVM-IR file: " << LLVMFilePath << "\n";
        return 1;
    }

    psr::DIBasedTypeHierarchy TH(IRDB);
    psr::LLVMBasedICFG ICFG(&IRDB, psr::CallGraphAnalysisType::CHA, EntryPoints, &TH);

    dfep::EnergyOfValueAnalysis EnergyProblem(&IRDB, &ICFG, EntryPoints, &Model);

    psr::IDESolver<dfep::EnergyAnalysisDomain> Solver(EnergyProblem, &ICFG);
    Solver.solve();

    llvm::outs() << "Results of the EnergyOfValueAnalysis:\n";

    for (const llvm::Function *F : IRDB.getAllFunctions())
    {
        if (F->isDeclaration())
            continue;

        llvm::outs() << "\n--- Function: " << F->getName() << " ---\n";
        for (const llvm::BasicBlock &BB : *F)
        {
            for (const llvm::Instruction &Inst : BB)
            {
                auto Results = Solver.resultsAt(&Inst, true);
                if (!Results.empty())
                {
                    Inst.print(llvm::outs());
                    llvm::outs() << "\n  Results:\n";
                    for (const auto &[Fact, Energy] : Results)
                    {
                        llvm::outs() << "    - Fact: " << psr::DToString(Fact)
                                     << ", Energy: " << Energy << "\n";
                    }
                }
                else
                {
                    llvm::outs() << "No results for instruction: ";
                    Inst.print(llvm::outs());
                    llvm::outs() << "\n";
                }
            }
        }
    }
    return 0;
}