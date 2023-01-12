#include "../src/qm.h"
#include <iostream>
#include <sstream>
#include <string>

std::vector<std::string> split(std::string &s) {
    std::stringstream ss(s);
    std::vector<std::string> strs;
    std::string temp;
    while (ss) {
        ss >> temp;
        strs.push_back(temp);
    }
    return strs;
}

std::vector<char> getVocab(std::string &minTermStr) {
    std::vector<char> vocab;
    for (char c : minTermStr)
        if (c != '~')
            vocab.push_back(c);
    return vocab;
}

MinTerm toMinTerm(std::string &minTermStr) {
    char prev = ' ';
    MinTerm minTerm;
    for (char c : minTermStr) {
        if (c != '~')
            minTerm.push_back(prev == '~' ? '0' : '1');
        prev = c;
    }
    return minTerm;
}

std::vector<MinTerm> toMinTerms(std::vector<std::string> &minTermStrs) {
    auto vocab = getVocab(minTermStrs[0]);
    for (size_t i = 1; i < minTermStrs.size(); i++)
        if (vocab != getVocab(minTermStrs[i]))
            throw "wrong vocab";
    std::vector<MinTerm> res;
    for (auto &minTermStr : minTermStrs)
        res.push_back(toMinTerm(minTermStr));
    return res;
}

std::vector<std::string> toSymbols(const std::vector<char> &vocab,
                                   const std::vector<MinTerm> minTerms) {
    std::vector<std::string> symbols;
    for (auto &minTerm : minTerms) {
        std::string symbol;
        char prev = ' ';
        for (size_t i = 0; i < minTerm.size(); i++) {
            if (minTerm[i] == '-')
                continue;
            if (minTerm[i] == '0')
                symbol.push_back('~');
            symbol.push_back(vocab[i]);
        }
        symbols.push_back(symbol);
    }
    return symbols;
}

int main() {
    std::cout << "Enter the min terms to simplify:\n";
    std::string symbolStr;
    std::getline(std::cin, symbolStr);
    std::vector<std::string> symbols = split(symbolStr);
    auto minterms = toMinTerms(symbols);
    auto simplified = simplify(minterms);
    auto simplifiedSymbols = toSymbols(getVocab(symbols[0]), simplified);
    std::cout << "Simplifed experssion is:\n";
    for (auto &symbol : simplifiedSymbols)
        std::cout << symbol << " ";
    std::cout << "\n";
}
