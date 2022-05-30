#include "FIRfilter.hpp"
#include <iostream>
#include <omp.h> 

template <class NumericT> class openMPSimpleFIRfilter : public FIRfilter<NumericT>
{
    public:
        template<class Container> openMPSimpleFIRfilter(const Container& numericCoefficients) : FIRfilter<NumericT>(numericCoefficients)
        {        
        }

        virtual void apply(const std::vector<NumericT>& data,std::vector<NumericT>& result) 
        {
            int n;
            size_t N = data.size();
            int cpus = omp_get_num_procs();
            auto nthread = omp_get_num_threads();
            #pragma omp parallel for
            for (n = N-1; n >= 0; n--)  //  position in output
            {
                int i, j;
                NumericT tmp = 0;
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