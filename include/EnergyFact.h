#ifndef DATAFLOWENERGYPREDICTOR_ENERGYFACT_H
#define DATAFLOWENERGYPREDICTOR_ENERGYFACT_H

#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <functional>
#include <string>

namespace dfep
{
    struct EnergyFact
    {
        const llvm::Value *V;

        EnergyFact(const llvm::Value *v) : V(v) {}

        bool operator==(const EnergyFact &O) const { return V == O.V; }
        bool operator<(const EnergyFact &O) const
        {
            return std::less<const llvm::Value *>()(V, O.V);
        }
    };

}

namespace std
{
    template <>
    struct hash<dfep::EnergyFact>
    {
        size_t operator()(const dfep::EnergyFact &f) const
        {
            return hash<const llvm::Value *>()(f.V);
        }
    };
}

namespace psr
{
    inline std::string DToString(const dfep::EnergyFact &Fact)
    {
        std::string Str;
        llvm::raw_string_ostream RSO(Str);
        if (Fact.V)
        {
            Fact.V->printAsOperand(RSO, false);
        }
        else
        {
            RSO << "ZERO";
        }
        return RSO.str();
    }
}

#endif