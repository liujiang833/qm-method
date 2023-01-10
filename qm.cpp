#include "qm.h"
#include <unordered_map>
#include <unordered_set>

int countOnes(const MinTerm &minTerm) {
    int ones = 0;
    for (char c : minTerm) {
        ones += c == '1';
    }
    return ones;
}

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms) {}

std::pair<std::vector<MinTerm>, std::vector<std::vector<MinTerm>>>
findImplicants(std::vector<MinTerm> &minTerms) {
    std::vector<MinTerm> implicants;
    std::unordered_map<std::string, std::unordered_set<MinTerm>> cover;
    for (auto &minTerm : minTerms)
        cover[minTerm].insert(minTerm);
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
                        cover[newTerm] =
                            mergeSets({cover[currTerm], cover[nextTerm]});
                        gens.insert(newTerm);
                        newGroups[it->first].push_back(newTerm);
                    }
                }
            }
        }
        for (auto &item : groups) {
            for (auto &minTerm : item.second) {
                if (checked.find(minTerm) == checked.end())
                    implicants.push_back(minTerm);
            }
        }
        groups = newGroups;
    }
    std::vector<std::vector<MinTerm>> coverVec;
    for (auto &implicant : implicants) {
        auto &implCover = cover[implicant];
        coverVec.push_back(
            std::vector<MinTerm>(implCover.begin(), implCover.end()));
    }
    return {implicants, coverVec};
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

std::unordered_set<MinTerm>
mergeSets(std::initializer_list<std::unordered_set<MinTerm>> sets) {
    std::unordered_set<MinTerm> res;
    for (auto it = sets.begin(); it != sets.end(); it++) {
        for (auto ele : *it)
            res.insert(ele);
    }
    return res;
}