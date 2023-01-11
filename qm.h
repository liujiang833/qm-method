#include <initializer_list>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
typedef std::string MinTerm;
typedef std::vector<std::vector<size_t>> Operand;
int countOnes(const MinTerm &minTerm);

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms);

auto findImplicants(std::vector<MinTerm> &minTerms)
    -> std::unordered_map<MinTerm, std::vector<MinTerm>>;

std::unordered_set<MinTerm> findEssentialImplicants(
    std::unordered_map<MinTerm, std::vector<MinTerm>> &implicantsAndCovers);

auto findRemaingImplicantsAndMinTerms(
    std::unordered_map<MinTerm, std::vector<MinTerm>> &implicantsAndCovers,
    std::vector<MinTerm> &minTerms,
    std::unordered_set<MinTerm> &essentialImplicants)
    -> std::pair<std::vector<MinTerm>, std::vector<MinTerm>>;

std::vector<MinTerm> petricksMethod(
    std::vector<MinTerm> &implicants,
    std::unordered_map<MinTerm, std::vector<MinTerm>> &implicantsAndCovers,
    std::vector<MinTerm> &minTerms);

Operand distribute(std::vector<Operand> &operands);
size_t calcNumVariables(std::vector<MinTerm> &implicants,
                        std::vector<size_t> &op);
std::vector<size_t> multiply(std::vector<size_t> &a, std::vector<size_t> &b);
std::map<int, std::vector<MinTerm>> groupByOnes(std::vector<MinTerm> &minTerms);
bool canCombine(const MinTerm &a, const MinTerm &b);
MinTerm combine(const MinTerm &a, const MinTerm &b);
std::unordered_set<MinTerm>
mergeSets(std::initializer_list<std::unordered_set<MinTerm>> sets);