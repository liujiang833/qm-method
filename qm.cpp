#include "qm.h"

int MinTerm::getOnesCount() {
    int count = 0;
    for (size_t i = 0; i < dashBits_.size(); i++) {
        if (dashBits_[i])
            continue;
        count += valBits_[i];
        return count;
    }
}