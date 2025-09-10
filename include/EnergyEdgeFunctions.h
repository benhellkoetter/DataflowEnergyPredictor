#pragma once

#include <algorithm>
#include <cmath>
#include <string>

#include "phasar/DataFlow/IfdsIde/EdgeFunctions.h"
#include "phasar/DataFlow/IfdsIde/EdgeFunctionUtils.h"
#include "EnergyOfValueAnalysis.h"

namespace llvm
{
    class Instruction;
    class Value;
}

namespace dfep
{
    using d_t = EnergyOfValueAnalysis::d_t;

    struct EnergyEdgeFunction;
    struct EnergyEdgeIdentityFunction;
    struct ResetEdgeFunction;

    static std::string valueToString(const llvm::Value *v);
    static std::string instToString(const llvm::Instruction *I);
    static std::string factToString(const d_t &f);

    struct EnergyEdgeFunction
    {
        using l_t = double;

        double energy;
        double initialAddedEnergy = 0;
        d_t currentFact;
        d_t successorFact;
        const llvm::Instruction *currentInstruction;
        const llvm::Instruction *successorInstruction;
        bool doubled = false;

        l_t computeTarget(l_t source) const noexcept;
        bool operator==(EnergyEdgeFunction const &other) const noexcept;

        static psr::EdgeFunction<l_t> compose(psr::EdgeFunctionRef<EnergyEdgeFunction> functionA, const psr::EdgeFunction<l_t> &functionB);

        static psr::EdgeFunction<l_t> join(psr::EdgeFunctionRef<EnergyEdgeFunction> functionA, const psr::EdgeFunction<l_t> &functionB);
    };

    struct EnergyEdgeIdentityFunction
    {
        using l_t = double;

        double energy;
        double initialAddedEnergy = 0;
        d_t currentFact;
        d_t successorFact;
        const llvm::Instruction *currentInstruction;
        const llvm::Instruction *successorInstruction;
        bool doubled = false;

        l_t computeTarget(l_t source) const noexcept;
        bool operator==(EnergyEdgeIdentityFunction const &other) const noexcept;

        static psr::EdgeFunction<l_t> compose(psr::EdgeFunctionRef<EnergyEdgeIdentityFunction> functionA, const psr::EdgeFunction<l_t> &functionB);

        static psr::EdgeFunction<l_t> join(psr::EdgeFunctionRef<EnergyEdgeIdentityFunction> functionA, const psr::EdgeFunction<l_t> &functionB);
    };

}
