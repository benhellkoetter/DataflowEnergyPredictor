#ifndef DATAFLOWENERGYPREDICTOR_ENERGYFACT_H
#define DATAFLOWENERGYPREDICTOR_ENERGYFACT_H

#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <functional>

namespace dfep {

enum class EnergyBucket {
    BOTTOM, ZERO, LOW, MEDIUM, HIGH, CRITICAL, TOP
};

struct EnergyFact {
    const llvm::Value *V;
    EnergyBucket Bucket;

    EnergyFact(const llvm::Value *v, EnergyBucket b) : V(v), Bucket(b) {}

    bool operator==(const EnergyFact &O) const {
        return V == O.V && Bucket == O.Bucket;
    }

    bool operator<(const EnergyFact &O) const {
        if (V != O.V) {
            return std::less<const llvm::Value *>()(V, O.V);
        }
        return Bucket < O.Bucket;
    }
};

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const dfep::EnergyFact &Fact) {
    OS << "Fact(<";
    if (Fact.V) {
        Fact.V->printAsOperand(OS, false);
    } else {
        OS << "nullptr";
    }
    OS << ", ";
    switch (Fact.Bucket) {
        case dfep::EnergyBucket::BOTTOM: OS << "BOTTOM"; break;
        case dfep::EnergyBucket::ZERO:   OS << "ZERO";   break;
        case dfep::EnergyBucket::LOW:    OS << "LOW";    break;
        case dfep::EnergyBucket::MEDIUM: OS << "MEDIUM"; break;
        case dfep::EnergyBucket::HIGH:   OS << "HIGH";   break;
        case dfep::EnergyBucket::CRITICAL:OS << "CRITICAL";break;
        case dfep::EnergyBucket::TOP:    OS << "TOP";    break;
    }
    OS << ">)";
    return OS;
}

}

namespace std {
template <> struct hash<dfep::EnergyFact> {
    size_t operator()(const dfep::EnergyFact &f) const {
        return hash<const llvm::Value *>()(f.V) ^ hash<int>()(static_cast<int>(f.Bucket));
    }
};
}

namespace psr {
inline std::string DToString(const dfep::EnergyFact &Fact) {
    std::string Str;
    llvm::raw_string_ostream RSO(Str);
    RSO << Fact;
    return RSO.str();
}
}

#endif