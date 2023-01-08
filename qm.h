#include <iostream>
#include <map>
#include <string>
#include <vector>
class MinTerm {
  public:
    enum BitSate { ONE, ZERO, DASH };
    MinTerm(const int numBits);
    MinTerm(const std::vector<bool> &vals);
    int getOnesCount();
    int setPos(int pos, BitSate s);
    friend bool diffByOne(MinTerm &a, MinTerm &b);
    friend MinTerm combine(MinTerm &a, MinTerm &b);
    friend std::ostream &operator<<(std::ostream &os, const MinTerm &m);

  private:
    std::vector<bool> dashBits_;
    std::vector<bool> valBits_;
};

std::vector<MinTerm> simplify(std::vector<MinTerm> &minTerms);