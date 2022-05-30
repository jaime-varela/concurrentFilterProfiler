#pragma once
#include <vector>
#include <assert.h>     /* assert */


template <class NumericT>
class FIRfilter {
  public:
    template<class Container> FIRfilter(const Container& numericCoefficients)
    {
        for (auto val : numericCoefficients)
        {
            bCoefficients.push_back(val);
        }        
        fsize = bCoefficients.size();
    }
    /// gives the b_{index} coefficient where b_{index} multiplies x[n-index] in the time domain
    inline NumericT bCoefficient(size_t index) {
        assert(index < fsize && index >= 0);
        return bCoefficients[index];
    }

    size_t filterSize() {
        return fsize;
    }
    // passing in references as time allocating data can be done outside.
    virtual void apply(const std::vector<NumericT>& data,std::vector<NumericT>& out) = 0;

   protected:
       std::vector<NumericT> bCoefficients;
       size_t fsize;
};
