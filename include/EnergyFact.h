#ifndef DATAFLOWENERGYPREDICTOR_ENERGYFACT_H
#define DATAFLOWENERGYPREDICTOR_ENERGYFACT_H

#include <llvm/IR/Value.h>
#include <functional>
#include <iostream>

struct EnergyFact {

    const llvm::Value *V;
    double Energy;

    EnergyFact(const llvm::Value *v, double e) : V(v), Energy(e) {}

    bool operator==(const EnergyFact &O) const {
        return V == O.V && Energy == O.Energy;
    }

    bool operator<(const EnergyFact &O) const {
        if (V != O.V) {
            return V < O.V;
        }
        return Energy < O.Energy;
    }
};

using d_t = EnergyFact;

#endif