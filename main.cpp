#include "qm.h"
#include <algorithm>
#include <iostream>
template <typename myMap>
std::vector<typename myMap::key_type> keys(const myMap &m) {
    std::vector<typename myMap::key_type> r;
    r.reserve(m.size());
    for (const auto &kvp : m) {
        r.push_back(kvp.first);
    }
    return r;
}
std::ostream &operator<<(std::ostream &os,
                         const std::vector<MinTerm> &minTerms) {
    std::cout << "{ ";
    for (auto &term : minTerms)
        std::cout << term << " ";
    std::cout << "}";
    return os;
}
std::ostream &operator<<(std::ostream &os,
                         const std::vector<std::vector<MinTerm>> &cover) {
    std::cout << "{ ";
    for (auto &c : cover)
        std::cout << "[ " << c << "]"
                  << " ";
    std::cout << "}";
    return os;
}

std::ostream &
operator<<(std::ostream &os,
           const std::unordered_map<MinTerm, std::vector<MinTerm>> &cover) {
    auto ks = keys(cover);
    std::sort(ks.begin(), ks.end());
    os << "{ ";
    for (auto &k : ks) {
        os << k << " -> ";
        for (const auto &c : cover.at(k)) {
            os << c;
        }
        os << ",";
    }
    os << " }";
    return os;
}

template <typename T> void assert_equal(T expect, T get) {
    if (expect == get) {
        std::cout << "succeeded\n";
        return;
    }
    std::cout << "failed. expect: " << expect << "\nget: " << get << '\n';
}

void testFindImplicant1() {
    std::vector<MinTerm> minterms = {"10", "11"};
    auto implicants = keys(findImplicants(minterms));
    std::cout << "testFindImplicant1 ...\n";
    assert_equal(std::vector<MinTerm>{"1-"}, implicants);
}

void testFindImplicant2() {
    std::vector<MinTerm> minterms = {"0000", "0001", "0010", "1000", "0101",
                                     "0110", "1001", "1010", "0111", "1110"};
    std::unordered_map<MinTerm, std::vector<MinTerm>> expect = {
        {"0-01", {"0001", "0101"}},
        {"01-1", {"0101", "0111"}},
        {"011-", {"0110", "0111"}},
        {"-00-", {"0000", "0001", "1000", "1001"}},
        {"-0-0", {"0000", "0010", "1000", "1010"}},
        {"--10", {"0010", "0110", "1010", "1110"}}};
    auto implicantsAndCover = findImplicants(minterms);
    for (auto &[k, v] : expect)
        std::sort(v.begin(), v.end());
    for (auto &[k, v] : implicantsAndCover)
        std::sort(v.begin(), v.end());
    assert_equal(expect, implicantsAndCover);
}

void testSimplify() {
    std::vector<MinTerm> minterms = {"0000", "0001", "0010", "1000", "0101",
                                     "0110", "1001", "1010", "0111", "1110"};
    std::cout << simplify(minterms);
}
int main() {
    testFindImplicant1();
    testFindImplicant2();
    testSimplify();
}