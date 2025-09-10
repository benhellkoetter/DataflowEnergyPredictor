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
        const llvm::Value *mValue;

        EnergyFact(const llvm::Value *value) : mValue(value) {}

        bool operator==(const EnergyFact &otherFact) const { return mValue == otherFact.mValue; }
        bool operator<(const EnergyFact &otherFact) const
        {
            return std::less<const llvm::Value *>()(mValue, otherFact.mValue);
        }
    };

}

namespace std
{
    template <>
    struct hash<dfep::EnergyFact>
    {
        size_t operator()(const dfep::EnergyFact &fact) const
        {
            return hash<const llvm::Value *>()(fact.mValue);
        }
    };
}

namespace psr
{
    inline std::string DToString(const dfep::EnergyFact &fact)
    {
        std::string str;
        llvm::raw_string_ostream stringStream(str);
        if (fact.mValue)
        {
            fact.mValue->printAsOperand(stringStream, false);
        }
        else
        {
            stringStream << "ZERO";
        }
        return stringStream.str();
    }
}

#endif