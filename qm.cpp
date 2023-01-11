#include "qm.h"
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
int countOnes(const MinTerm &minTerm) {
    int ones = 0;
    for (char c : minTerm) {
        ones += c == '1';
    }
    return ones;
}

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms) {
    auto implicantsAndCovers = findImplicants(minTerms);
    auto essentialImplicants = findEssentialImplicants(implicantsAndCovers);
    auto [remainingImlicants, remainingTerms] =
        findRemaingImplicantsAndMinTerms(implicantsAndCovers, minTerms,
                                         essentialImplicants);
    std::vector<MinTerm> res;
    res.insert(res.end(), essentialImplicants.begin(),
               essentialImplicants.end());
    if (remainingTerms.size() != 0) {
        auto nonEssentialUsed = petricksMethod(
            remainingImlicants, implicantsAndCovers, remainingTerms);
        res.insert(res.end(), nonEssentialUsed.begin(), nonEssentialUsed.end());
    }
    return res;
}

std::vector<MinTerm> petricksMethod(
    std::vector<MinTerm> &implicants,
    std::unordered_map<MinTerm, std::vector<MinTerm>> &implicantsAndCovers,
    std::vector<MinTerm> &minTerms) {
    std::unordered_map<MinTerm, std::vector<std::vector<size_t>>> operands;
    for (auto &minTerm : minTerms)
        operands.insert({minTerm, {}});
    for (size_t i = 0; i < implicants.size(); i++) {
        for (auto &minTerm : implicantsAndCovers.at(implicants[i])) {
            if (operands.find(minTerm) != operands.end())
                operands[minTerm].push_back({i});
        }
    }
    std::vector<Operand> operandVec;
    for (auto &[minTerm, ops] : operands)
        operandVec.push_back(ops);
    auto sop = distribute(operandVec);
    std::vector<size_t> numVariables;
    for (auto &op : sop)
        numVariables.push_back(calcNumVariables(implicants, op));
    auto cmp = [&sop, &numVariables](const size_t a, const size_t b) {
        if (sop[a].size() != sop[b].size())
            return sop[a].size() < sop[b].size();
        return numVariables[a] < numVariables[b];
    };
    std::vector<size_t> idx(sop.size());
    std::iota(idx.begin(), idx.end(), 0);
    std::nth_element(idx.begin(), idx.begin(), idx.end(), cmp);
    std::vector<MinTerm> res;
    for (size_t pos : sop[idx[0]])
        res.push_back(implicants[pos]);
    return res;
}
size_t calcNumVariables(std::vector<MinTerm> &implicants,
                        std::vector<size_t> &op) {
    std::unordered_set<size_t> vars;
    for (size_t pos : op) {
        auto &implicant = implicants[pos];
        for (size_t i = 0; i < implicant.size(); i++)
            if (implicant[i] != '-')
                vars.insert(i);
    }
    return vars.size();
}

Operand distribute(std::vector<Operand> &operands) {
    Operand curr = operands.back();
    operands.pop_back();
    while (!operands.empty()) {
        auto op = operands.back();
        operands.pop_back();
        Operand temp;
        for (auto &t1 : curr)
            for (auto &t2 : op)
                temp.push_back(multiply(t1, t2));
        curr = temp;
    }
    return curr;
}

std::vector<size_t> multiply(std::vector<size_t> &a, std::vector<size_t> &b) {
    std::unordered_set<size_t> s;
    for (size_t var : a)
        s.insert(var);
    for (size_t var : b)
        s.insert(var);
    std::vector<size_t> res(s.begin(), s.end());
    std::sort(res.begin(), res.end());
    return res;
}

auto findImplicants(std::vector<MinTerm> &minTerms)
    -> std::unordered_map<MinTerm, std::vector<MinTerm>> {
    std::unordered_map<MinTerm, std::vector<MinTerm>> implicantsAndCovers;
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
                        if (gens.find(newTerm) == gens.end()) {
                            gens.insert(newTerm);
                            newGroups[it->first].push_back(newTerm);
                            cover[newTerm] =
                                mergeSets({cover[currTerm], cover[nextTerm]});
                        }
                    }
                }
            }
        }
        for (auto &item : groups) {
            for (auto &minTerm : item.second) {
                if (checked.find(minTerm) == checked.end()) {
                    implicantsAndCovers[minTerm] = std::vector<MinTerm>(
                        cover[minTerm].begin(), cover[minTerm].end());
                }
            }
        }
        groups = newGroups;
    }
    return implicantsAndCovers;
}

std::unordered_set<MinTerm> findEssentialImplicants(
    std::unordered_map<MinTerm, std::vector<MinTerm>> &implicantsAndCovers) {
    std::unordered_map<MinTerm, std::vector<MinTerm>> coveredBy;
    for (auto &[implicant, cover] : implicantsAndCovers)
        for (auto &minTerm : cover)
            coveredBy[minTerm].push_back(implicant);

    std::unordered_set<MinTerm> essentialImplicants;
    for (auto &[minTerm, covers] : coveredBy) {
        if (covers.size() == 1)
            essentialImplicants.insert(covers[0]);
    }
    return essentialImplicants;
}

auto findRemaingImplicantsAndMinTerms(
    std::unordered_map<MinTerm, std::vector<MinTerm>> &implicantsAndCovers,
    std::vector<MinTerm> &minTerms,
    std::unordered_set<MinTerm> &essentialImplicants)
    -> std::pair<std::vector<MinTerm>, std::vector<MinTerm>> {
    std::unordered_set<MinTerm> covered;
    for (auto &ei : essentialImplicants) {
        for (auto &minTerm : implicantsAndCovers[ei])
            covered.insert(minTerm);
    }
    std::vector<MinTerm> remainingImplicants;
    std::vector<MinTerm> remainingMinTerms;
    for (auto &minTerm : minTerms)
        if (covered.find(minTerm) == covered.end())
            remainingMinTerms.push_back(minTerm);
    for (auto &[implicants, cover] : implicantsAndCovers)
        if (essentialImplicants.find(implicants) == essentialImplicants.end())
            remainingImplicants.push_back(implicants);
    return {remainingImplicants, remainingMinTerms};
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