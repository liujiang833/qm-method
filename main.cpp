#include "qm.h"
#include <algorithm>
#include <iostream>

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

template <typename T> void assert_equal(T expect, T get) {
    if (expect == get) {
        std::cout << "succeeded\n";
        return;
    }
    std::cout << "failed. expect: " << expect << " get: " << get << '\n';
}

void testFindImplicant1() {
    std::vector<MinTerm> minterms = {"10", "11"};
    auto implicants = findImplicants(minterms).first;
    std::cout << "testFindImplicant1 ...\n";
    assert_equal(std::vector<MinTerm>{"1-"}, implicants);
}

void testFindImplicant2() {
    std::vector<MinTerm> minterms = {"0000", "0001", "0010", "1000", "0101",
                                     "0110", "1001", "1010", "0111", "1110"};
    std::vector<MinTerm> expect = {"0-01", "01-1", "011-",
                                   "-00-", "-0-0", "--10"};
    std::vector<std::vector<MinTerm>> expectedCover{
        {"0001", "0101"},
        {"0101", "0111"},
        {"0110", "0111"},
        {"0000", "0001", "1000", "1001"},
        {"0000", "0010", "1000", "1010"},
        {"0010", "0110", "1010", "1110"}};
    std::sort(expect.begin(), expect.end());
    auto [implicants, cover] = findImplicants(minterms);
    sort(implicants.begin(), implicants.end());
    for (size_t i = 0; i < cover.size(); i++) {
        std::sort(expectedCover[i].begin(), expectedCover[i].end());
        std::sort(cover[i].begin(), cover[i].end());
    }
    std::cout << "testFindImplicant1 ...\n";
    assert_equal(expect, implicants);
    assert_equal(expectedCover, cover);
}

int main() {
    testFindImplicant1();
    testFindImplicant2();
}