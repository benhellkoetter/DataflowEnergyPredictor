#include "EnergyOfValueAnalysis.h"
#include "phasar/PhasarLLVM/TypeHierarchy/DIBasedTypeHierarchy.h"

#include "phasar/DataFlow/IfdsIde/Solver/IDESolver.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DB/LLVMProjectIRDB.h"
#include "phasar/Utils/Logger.h"
#include "llvm/Support/CommandLine.h"
#include "ProfileLoader.h"
#include "EnergyModel.h"

#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"

#include <unordered_map>
#include <algorithm>
#include <string>

static llvm::cl::OptionCategory DataflowEnergyPredictorCategory("DataflowEnergyPredictor Options");

static llvm::cl::opt<std::string> llvmFilePath(llvm::cl::Positional, llvm::cl::desc("<LLVM-IR-File>"), llvm::cl::Required, llvm::cl::cat(DataflowEnergyPredictorCategory));

static llvm::cl::opt<std::string> jsonProfilePath("profile", llvm::cl::desc("Path to JSON energy profile"), llvm::cl::value_desc("profile.json"),
                                                  llvm::cl::Optional, llvm::cl::cat(DataflowEnergyPredictorCategory));

static llvm::cl::opt<int> loopIterations("iterations", llvm::cl::desc("Fixed number of iterations for all loops"), llvm::cl::value_desc("N"),
                                         llvm::cl::Optional, llvm::cl::init(50), llvm::cl::cat(DataflowEnergyPredictorCategory));

static llvm::cl::list<std::string> entryPoints("E", llvm::cl::desc("Entry points of the analysis"), llvm::cl::value_desc("function name"), llvm::cl::Required,
                                               llvm::cl::cat(DataflowEnergyPredictorCategory));

int main(int argc, char **argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "DataflowEnergyPredictor\n");

    auto profileMap = dfep::loadProfileMapFromJson(jsonProfilePath);
    dfep::EnergyModel energyModel(std::move(profileMap));

    psr::Logger::initializeStdoutLogger();
    psr::Logger::setLoggerFilterLevel(psr::SeverityLevel::INFO);

    psr::LLVMProjectIRDB projectIRDB(llvmFilePath);
    if (!projectIRDB.isValid())
    {
        llvm::errs() << "Error: loading LLVM-IR file " << llvmFilePath << "\n";
        return 1;
    }

    psr::DIBasedTypeHierarchy typeHierarchy(projectIRDB);
    psr::LLVMBasedICFG icfg(&projectIRDB, psr::CallGraphAnalysisType::CHA, entryPoints, &typeHierarchy);
    dfep::EnergyOfValueAnalysis energyAnalysisProblem(&projectIRDB, &icfg, entryPoints, &energyModel, loopIterations);
    psr::IDESolver<dfep::EnergyAnalysisDomain> solver(energyAnalysisProblem, &icfg);

    solver.solve();

    llvm::outs() << "Results of the EnergyOfValueAnalysis:\n";

    for (const llvm::Function *function : projectIRDB.getAllFunctions())
    {
        if (function->isDeclaration())
            continue;

        llvm::outs() << "\n--- Function: " << function->getName() << " ---\n";
        for (const llvm::BasicBlock &basicBlock : *function)
        {
            for (const llvm::Instruction &instruction : basicBlock)
            {
                auto results = solver.resultsAt(&instruction, true);
                if (!results.empty())
                {
                    instruction.print(llvm::outs());
                    llvm::outs() << "\n  Results:\n";
                    for (const auto &[fact, energy] : results)
                    {
                        llvm::outs() << "    - Fact: " << psr::DToString(fact) << ", Energy: " << energy << "\n";
                    }
                }
                else
                {
                    llvm::outs() << "No results for instruction: ";
                    instruction.print(llvm::outs());
                    llvm::outs() << "\n";
                }
            }
        }
    }
    return 0;
}