#include "qm.h"
#include <unordered_set>

int countOnes(const MinTerm &minTerm) {
    int ones = 0;
    for (char c : minTerm) {
        ones += c == '1';
    }
    return ones;
}

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms) {}

std::vector<MinTerm> findImplicants(std::vector<MinTerm> &minTerms) {
    std::vector<MinTerm> implicants;
    std::map<int, std::vector<MinTerm>> groups = groupByOnes(minTerms);
    bool changed = true;
    while (changed) {
        changed = false;
        std::unordered_set<MinTerm> checked;
        std::map<int, std::vector<MinTerm>> newGroups;
        auto last = std::prev(groups.end());
        for (auto it = groups.begin(); it != last; it++) {
            auto next = std::next(it);
            std::unordered_set<MinTerm> gens;
            if (next->first - it->first != 1)
                continue;
            for (auto &currTerm : it->second) {
                for (auto &nextTerm : next->second) {
                    if (canCombine(currTerm, nextTerm)) {
                        MinTerm newTerm = combine(currTerm, nextTerm);
                        changed = true;
                        checked.insert(currTerm);
                        checked.insert(nextTerm);
                        if (gens.find(newTerm) == gens.end()) {
                            gens.insert(newTerm);
                            newGroups[it->first].push_back(newTerm);
                        }
                    }
                }
            }
            for (auto &currTerm : it->second)
                if (checked.find(currTerm) == checked.end())
                    implicants.push_back(currTerm);
        }
    }
    return implicants;
}
std::map<int, std::vector<MinTerm>>
groupByOnes(std::vector<MinTerm> &minTerms) {
    std::map<int, std::vector<MinTerm>> groups;
    for (auto &minTerm : minTerms)
        groups[countOnes(minTerm)].push_back(minTerm);
    return groups;
}

bool canCombine(const MinTerm &a, const MinTerm &b) {
    int numDiff = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] == b[i])
            continue;
        if (a[i] == '-' || b[i] == '-')
            return false;
        numDiff++;
    }
    return numDiff == 1;
}

MinTerm combine(const MinTerm &a, const MinTerm &b) {
    MinTerm newTerm = a;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            newTerm[i] = '-';
            break;
        }
    }
    return newTerm;
}