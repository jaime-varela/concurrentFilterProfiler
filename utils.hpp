#pragma once
#include <random>
#include <iostream>


template<class NumericT>
std::vector<NumericT> generateGaussianDistributedRandomVector(size_t N, NumericT mean, NumericT standardDeviation) {
    std::vector<NumericT> result(N);
    std::default_random_engine generator;
    std::normal_distribution<NumericT> distribution(mean,standardDeviation);
    for (size_t i = 0; i < N; i++)
    {
        result[i] = distribution(generator);
    }
    return result;
}

template<class IterationT>
void printInterable(const IterationT& anIterable) {
    for (auto&& val : anIterable)
    {
        std::cout << val << std::endl;
    }    
}

template<class IterationT>
bool areIterablesEqual(const IterationT& anIterable1, const IterationT& anIterable2) {
    if(anIterable1.size() != anIterable2.size()) {
        return false;
    }
    for (size_t i = 0; i < anIterable1.size(); i++)
    {
        if(anIterable1[i] != anIterable2[i]) {
            return false;
        }
    }
    return true;
    

}


template<class IterationT,class NumericT>
bool areIterablesEqualWithinThreshold(const IterationT& anIterable1, const IterationT& anIterable2,NumericT thresh) {
    if(anIterable1.size() != anIterable2.size()) {
        return false;
    }
    for (size_t i = 0; i < anIterable1.size(); i++)
    {
        auto diff = (anIterable1[i] - anIterable2[i] > 0)? anIterable1[i] - anIterable2[i] : anIterable2[i] - anIterable1[i]; 
        if(diff > thresh) {
            std::cout << "index: "<< i<< " , val1: "<< anIterable1[i] << " , val2: " << anIterable2[i] << std::endl;
            return false;
        }
    }
    return true;
    

}