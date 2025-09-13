#include "EnergyEdgeFunctions.h"
#include <llvm/Support/raw_ostream.h>
#include <string>

namespace dfep
{
    std::string valueToString(const llvm::Value *value)
    {
        if (!value)
            return "nullptr";
        std::string str;
        llvm::raw_string_ostream rso(str);
        value->printAsOperand(rso, false);
        return rso.str();
    }

    std::string instToString(const llvm::Instruction *instruction)
    {
        if (!instruction)
            return "nullptr";
        std::string str;
        llvm::raw_string_ostream rso(str);
        instruction->print(rso);
        return rso.str();
    }

    std::string factToString(const d_t &fact)
    {
        if (fact.mValue == nullptr)
            return "Zero";
        return valueToString(fact.mValue);
    }

    // EnergyEdgeFunction
    EnergyEdgeFunction::l_t EnergyEdgeFunction::computeTarget(l_t source) const noexcept
    {
        llvm::errs() << "[DEFAULT::computeTarget] Inst=" << instToString(currentInstruction)
                     << " currentFact=" << factToString(currentFact)
                     << " successorFact=" << factToString(successorFact)
                     << " energyAdded=" << energy << "\n";
        return energy;
    }

    bool EnergyEdgeFunction::operator==(EnergyEdgeFunction const &other) const noexcept
    {
        bool equal = currentInstruction == other.currentInstruction && currentFact.mValue == other.currentFact.mValue && successorFact.mValue == other.successorFact.mValue && energy == other.energy;
        llvm::errs() << "[DEFAULT::operator==] Inst=" << instToString(currentInstruction)
                     << " currentFact=" << factToString(currentFact)
                     << " successorFact=" << factToString(successorFact)
                     << " equal=" << equal << "\n";
        return equal;
    }

    psr::EdgeFunction<EnergyEdgeFunction::l_t> EnergyEdgeFunction::compose(psr::EdgeFunctionRef<EnergyEdgeFunction> functionA, const psr::EdgeFunction<l_t> &functionB)
    {
        llvm::errs() << "[DEFAULT::compose] functionA(energy=" << functionA->energy << ", src=" << factToString(functionA->currentFact) << ") with ";

        if (auto *other = functionB.dyn_cast<EnergyEdgeIdentityFunction>())
        {
            llvm::errs() << "EdgeIdentity\n";
            return EnergyEdgeIdentityFunction{functionA->energy, functionA->energy, other->currentFact, other->successorFact, functionA->successorInstruction, other->successorInstruction, false};
        }
        llvm::errs() << "EnergyEdgeFunction\n";
        return psr::AllTop<l_t>{0.0};
    }

    psr::EdgeFunction<EnergyEdgeFunction::l_t> EnergyEdgeFunction::join(psr::EdgeFunctionRef<EnergyEdgeFunction> functionA, const psr::EdgeFunction<l_t> &functionB)
    {
        auto factsEqual = [](const d_t &a, const d_t &b)
        {
            return a.mValue == b.mValue;
        };

        if (auto *other = functionB.dyn_cast<EnergyEdgeIdentityFunction>())
        {
            if (factsEqual(functionA->currentFact, other->currentFact) && factsEqual(functionA->successorFact, other->successorFact))
            {
                double select;
                if (functionA->energy == other->energy)
                    select = other->energy + functionA->initialAddedEnergy;
                else
                    select = other->energy;
                llvm::errs() << "[DEFAULT::join] Matching facts (Energy+&dentity) -> " << select << "\n";
                return EnergyEdgeIdentityFunction{select, functionA->initialAddedEnergy, functionA->currentFact, functionA->successorFact, functionA->currentInstruction, functionA->successorInstruction, false};
            }
            if (factsEqual(functionA->currentFact, nullptr))
            {
                double sum = functionA->energy + other->energy;
                llvm::errs() << "[ZERO::join] Matching facts (Energy+Id):" << functionA->energy << " + " << other->energy << " =" << sum << "\n";
                return EnergyEdgeIdentityFunction{sum, functionA->initialAddedEnergy, functionA->currentFact, functionA->successorFact, functionA->currentInstruction, functionA->successorInstruction, false};
            }
        }

        llvm::errs() << "[DEFAULT::join] joining with OtherEdgeFunction -> keep A\n";
        return functionA;
    }

    // EnergyEdgeIdentityFunction
    EnergyEdgeIdentityFunction::l_t EnergyEdgeIdentityFunction::computeTarget(l_t source) const noexcept
    {
        llvm::errs() << "[IDENT::computeTarget] Inst=" << instToString(currentInstruction)
                     << " currentFact=" << factToString(currentFact)
                     << " successorFact=" << factToString(successorFact)
                     << " source=" << source
                     << " energyAdded=" << energy << "\n";
        return energy;
    }

    bool EnergyEdgeIdentityFunction::operator==(EnergyEdgeIdentityFunction const &other) const noexcept
    {
        bool equal = currentInstruction == other.currentInstruction && currentFact.mValue == other.currentFact.mValue && successorFact.mValue == other.successorFact.mValue && energy == other.energy;
        llvm::errs() << "[IDENT::operator==] Inst=" << instToString(currentInstruction)
                     << " currentFact=" << factToString(currentFact)
                     << " successorFact=" << factToString(successorFact)
                     << " equal=" << equal << "\n";
        return equal;
    }

    psr::EdgeFunction<EnergyEdgeIdentityFunction::l_t> EnergyEdgeIdentityFunction::compose(psr::EdgeFunctionRef<EnergyEdgeIdentityFunction> functionA, const psr::EdgeFunction<l_t> &functionB)
    {
        llvm::errs() << "[IDENT::compose] Ident(src=" << factToString(functionA->currentFact) << ") with ";
        if (auto *otherE = functionB.dyn_cast<EnergyEdgeFunction>())
        {
            llvm::errs() << "EnergyEdgeFunction(energy=" << otherE->energy << ")\n";
            return EnergyEdgeFunction{otherE->energy, otherE->initialAddedEnergy, functionA->successorFact, otherE->successorFact, functionA->successorInstruction, otherE->successorInstruction, false};
        }
        else if (auto *otherId = functionB.dyn_cast<EnergyEdgeIdentityFunction>())
        {
            llvm::errs() << "EnergyEdgeIdentityFunction\n";
            return EnergyEdgeIdentityFunction{functionA->energy, functionA->initialAddedEnergy, functionA->successorFact, otherId->successorFact, functionA->successorInstruction, otherId->successorInstruction, false};
        }

        llvm::errs() << "Fallback\n";
        return psr::AllTop<l_t>{0.0};
    }

    psr::EdgeFunction<EnergyEdgeIdentityFunction::l_t> EnergyEdgeIdentityFunction::join(psr::EdgeFunctionRef<EnergyEdgeIdentityFunction> functionA, const psr::EdgeFunction<l_t> &functionB)
    {
        auto factsEqual = [](const d_t &a, const d_t &b)
        {
            return a.mValue == b.mValue;
        };

        if (auto *other = functionB.dyn_cast<EnergyEdgeFunction>())
        {
            if (factsEqual(functionA->currentFact, other->currentFact) && factsEqual(functionA->successorFact, other->successorFact))
            {
                double select;
                if (functionA->energy == other->energy)
                    select = other->energy + functionA->initialAddedEnergy;
                else
                    select = other->energy;
                llvm::errs() << "[IDENT::join] Matching facts (Energy&Energy) -> " << select << "\n";
                return EnergyEdgeIdentityFunction{select, functionA->initialAddedEnergy, functionA->currentFact, functionA->successorFact, functionA->currentInstruction, functionA->successorInstruction, false};
            }

            if (factsEqual(other->currentFact, nullptr))
            {
                double sum = functionA->energy + other->energy;
                llvm::errs() << "[ZERO::join] Matching facts (Identity + Energy) -> " << functionA->energy << " + " << other->energy << " = " << sum << "\n";
                return EnergyEdgeIdentityFunction{sum, other->initialAddedEnergy, functionA->currentFact, functionA->successorFact, functionA->currentInstruction, functionA->successorInstruction, false};
            }
        }

        if (auto *other = functionB.dyn_cast<EnergyEdgeIdentityFunction>())
        {
            if (factsEqual(functionA->currentFact, other->currentFact) && factsEqual(functionA->successorFact, other->successorFact))
            {
                double select;
                if (functionA->energy == other->energy)
                    select = other->energy + functionA->initialAddedEnergy;
                else
                    select = other->energy;
                llvm::errs() << "[IDENT::join] Matching facts (Identity+Identity) -> " << select << "\n";
                return EnergyEdgeIdentityFunction{select, functionA->initialAddedEnergy, functionA->currentFact, functionA->successorFact, functionA->currentInstruction, functionA->successorInstruction, false};
            }
            double max;
            if (functionA->doubled && other->doubled)
            {
                llvm::errs() << "[IDENT] Both doubled!\n";
                max = std::max(functionA->energy / 2, other->energy / 2);
            }
            else if (functionA->doubled)
            {
                llvm::errs() << "[IDENT] A doubled!\n";
                max = std::max(functionA->energy / 2, other->energy);
            }
            else if (other->doubled)
            {
                llvm::errs() << "[IDENT] B doubled!\n";
                max = std::max(functionA->energy, other->energy / 2);
            }
            else
            {
                llvm::errs() << "[IDENT] Nothing doubled!\n";
                max = std::max(functionA->energy, other->energy);
            }

            llvm::errs() << "[IDENT::join] A.energy=" << functionA->energy
                         << " A.doubled=" << (functionA->doubled ? "true" : "false")
                         << " B.energy=" << other->energy
                         << " B.doubled=" << (other->doubled ? "true" : "false")
                         << " -> merged_adj=" << max
                         << " returned=" << (2 * max) << "\n";
            return EnergyEdgeIdentityFunction{2 * max, functionA->initialAddedEnergy, functionA->currentFact, functionA->successorFact, functionA->currentInstruction, functionA->successorInstruction, true};
        }

        llvm::errs() << "[IDENT::join] joining with OtherEdgeFunction -> keep A\n";
        return functionA;
    }

}
