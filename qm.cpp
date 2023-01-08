#include "qm.h"

MinTerm::MinTerm(const int numBits) {
    dashBits_ = std::vector<bool>(numBits);
    valBits_ = std::vector<bool>(numBits);
}

MinTerm::MinTerm(const std::vector<bool> &vals) { valBits_ = vals; }

int MinTerm::getOnesCount() {
    int count = 0;
    for (size_t i = 0; i < dashBits_.size(); i++) {
        if (dashBits_[i])
            continue;
        count += valBits_[i];
        return count;
    }
}

int MinTerm::setPos(int pos, BitSate s) {
    switch (s) {
    case DASH:
        dashBits_[pos] = 1;
        valBits_[pos] = 0;
        break;
    case ZERO:
        dashBits_[pos] = 0;
        valBits_[pos] = 0;
    case ONE:
        dashBits_[pos] = 0;
        valBits_[pos] = 1;
    default:
        break;
    }
}

bool diffByOne(MinTerm &a, MinTerm &b) {
    size_t len = a.dashBits_.size();
    int count = 0;
    for (size_t i = 0; i < len; i++) {
        if (a.dashBits_[i] != b.dashBits_[i])
            return false;
        if (a.valBits_[i] != b.valBits_[i])
            count++;
    }
    return count == 1;
}

MinTerm combine(MinTerm &a, MinTerm &b) {
    MinTerm res = a;
    size_t len = a.dashBits_.size();
    for (size_t i = 0; i < len; i++) {
        if (a.valBits_[i] != b.valBits_[i]) {
            res.dashBits_[i] = 1;
            res.valBits_[i] = 0;
            return res;
        }
    }
}

std::ostream &operator<<(std::ostream &os, const MinTerm &m) {
    size_t len = m.dashBits_.size();
    for (int i = 0; i < len; i++) {
        if (m.dashBits_[i] == 1)
            os << '-';
        else if (m.valBits_[i] == 0)
            os << '0';
        else
            os << '1';
    }
}

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms) {}
