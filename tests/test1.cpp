#include "../src/qm.h"
#include <iostream>

int test1() {
    std::vector<MinTerm> minTerms = {"10", "11"};
    auto implicants = findImplicants(minTerms);
    if (implicants == std::vector<MinTerm>{"1"})
        return 0;
    return 1;
}

int main() {
    int res;
    res = test1();
    if (res)
        return res;
    return 0;
}