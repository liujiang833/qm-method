#include <initializer_list>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>
typedef std::string MinTerm;

int countOnes(const MinTerm &minTerm);

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms);
std::pair<std::vector<MinTerm>, std::vector<std::vector<MinTerm>>>
findImplicants(std::vector<MinTerm> &minTerms);
std::map<int, std::vector<MinTerm>> groupByOnes(std::vector<MinTerm> &minTerms);
bool canCombine(const MinTerm &a, const MinTerm &b);
MinTerm combine(const MinTerm &a, const MinTerm &b);
std::unordered_set<MinTerm>
mergeSets(std::initializer_list<std::unordered_set<MinTerm>> sets);