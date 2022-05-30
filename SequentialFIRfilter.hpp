#pragma once
#include "FIRfilter.hpp"

template <class NumericT> class SequentialFIRfilter : public FIRfilter<NumericT>
{
    public:
        template<class Container> SequentialFIRfilter(const Container& numericCoefficients) : FIRfilter<NumericT>(numericCoefficients)
        {        
        }

        virtual void apply(const std::vector<NumericT>& data,std::vector<NumericT>& result) 
        {

            int i, j, n;
            NumericT tmp;
            size_t N = data.size();
            for (n = N-1; n >= 0; n--)  //  position in output
            {
                tmp = 0;
                for (i = 0; i < this->fsize; i++)  //  position in coefficients array
                {
                    j = n - i;  //  position in input
                    if (j >= 0)  //  bounds check for input buffer
                    {
                        tmp += this->bCoefficients[i] * data[j];
                    }
                }
                result[n] = tmp;
            }
        }

};