#include "FIRfilter.hpp"
#include <cmath>
#include <algorithm>
#include <omp.h>

namespace 
{
    inline int startIndexForPartition(int partitionNumberStartingFromZero,int NumPartitions,int ArraySize) 
    {
        return partitionNumberStartingFromZero * (ArraySize/NumPartitions);
    }
    inline int endIndexForPartition(int partitionNumberStartingFromZero,int NumPartitions,int ArraySize) 
    {
        return (partitionNumberStartingFromZero == NumPartitions-1)? ArraySize-1 : startIndexForPartition(partitionNumberStartingFromZero+1,NumPartitions,ArraySize) - 1;
    }
}

template <class NumericT> class openMPForkJoinFilter : public FIRfilter<NumericT>
{
    public:
        template<class Container> openMPForkJoinFilter(const Container& numericCoefficients) : FIRfilter<NumericT>(numericCoefficients)
        {        
        }

        virtual void apply(const std::vector<NumericT>& data,std::vector<NumericT>& result) 
        {

            size_t N = data.size();

            #pragma omp parallel
            {
                int tid = omp_get_thread_num();
                auto nthread = omp_get_num_threads();
                int startIndex = startIndexForPartition(tid,nthread,N);
                int endIndex = endIndexForPartition(tid,nthread,N);
                int n;
                for (n = endIndex; n >= startIndex; n--)  //  position in output
                {
                    int i,j;
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
        }

};