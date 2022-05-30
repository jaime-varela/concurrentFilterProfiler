#include "SequentialFIRfilter.hpp"
#include "openMPSimpleFIRfilter.hpp"
#include "openMPForkJoinFIRfilter.hpp"
#include "utils.hpp"

#include <iostream>
#include <chrono>

const unsigned int SIGNAL_SIZE = 100'000'000;

int main(int argc, char const *argv[])
{
    std::vector<float> vec({1,1,2,3});
    SequentialFIRfilter<float> basicFilter(vec);    
    float threshold = 0.00005;

    auto randomSignal = generateGaussianDistributedRandomVector(SIGNAL_SIZE,0.0f,1.0f);
    decltype(randomSignal) filteredSignalBuffer(randomSignal.size());
    decltype(randomSignal) ompFilteredSignalBuffer(randomSignal.size());
    // printInterable(randomSignal);
    std::cout << "Size: " << randomSignal.size() << std::endl;

    auto totalDuration = 0;
    auto t1 = std::chrono::high_resolution_clock::now();

    basicFilter.apply(randomSignal,filteredSignalBuffer);
    // printInterable(filteredSignal);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    totalDuration += duration;    
    std::cout << "Simple Filter time(ms): "<< totalDuration << std::endl;


    openMPSimpleFIRfilter<float> ompSimpleFilter(vec);
    auto ompDuration = 0;
    t1 = std::chrono::high_resolution_clock::now();

    ompSimpleFilter.apply(randomSignal,ompFilteredSignalBuffer);
    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    ompDuration += duration;    
    std::cout << "OMP Simple Filter time(ms): "<< ompDuration << std::endl;
    assert(areIterablesEqualWithinThreshold(filteredSignalBuffer,ompFilteredSignalBuffer,threshold));


    std::cout << "Simple/OMP: " << (1.0 * totalDuration) / ompDuration << std::endl; 

    openMPForkJoinFilter<float> forkJoinFilter(vec);
    auto otherOmpDuration = 0;
    t1 = std::chrono::high_resolution_clock::now();

    forkJoinFilter.apply(randomSignal,ompFilteredSignalBuffer);
    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
    otherOmpDuration += duration;    
    std::cout << "Another OMP Filter time(ms): "<< otherOmpDuration << std::endl;
    
    std::cout << "Simple/ForkJoin: " << (1.0 * totalDuration) / otherOmpDuration << std::endl; 
    assert(areIterablesEqualWithinThreshold(filteredSignalBuffer,ompFilteredSignalBuffer,threshold));
    return 0;

}
