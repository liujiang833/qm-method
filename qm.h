#include <map>
#include <string>
#include <vector>
class MinTerm {
  public:
    enum BitSate { ONE, ZERO, DASH };
    int getOnesCount();
    int setPos(int pos, BitSate s);
    friend bool diffByOne(MinTerm &a, MinTerm &b);
    friend MinTerm combine(MinTerm &a, MinTerm &b);
    MinTerm(const int numBits);
    MinTerm(const int numBits, const std::vector<int> &vals);

  private:
    std::vector<bool> dashBits_;
    std::vector<bool> valBits_;
};

class QmProblem {
  public:
    std::string simplify(std::vector<MinTerm> minTerms);

  private:
};