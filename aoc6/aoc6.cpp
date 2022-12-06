#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <chrono>

using namespace std::literals;

// brute force (pt1 15us, pt2 50us)
template<typename I> bool all_unique(I b, I e)
{
    for(;b != e; ++b)
    {
        for(auto bb {b + 1}; bb != e; ++bb)
            if ( *bb == *b)
                return false;
    }
    return true;
}

template<size_t N> auto ptN(auto const& in)
{
//    auto now = std::chrono::high_resolution_clock().now();
    auto it0 = in.begin();
    auto itN = in.begin() + N;
    while(itN != in.end())
    {
        if(all_unique(it0, itN))
            break;
        ++it0;
        ++itN;
    }
//    auto end = std::chrono::high_resolution_clock().now();
//    std::cout << "duration " << std::chrono::duration_cast<std::chrono::microseconds>(end - now).count() << "us\n";

    return itN - in.begin();
}

// smart (pt1 5us, pt2 8us)
template<size_t N> auto ptxN(auto const& in)
{
//    auto now = std::chrono::high_resolution_clock().now();
    std::array<unsigned, 256> seen{ 0 };
    unsigned unique{ 0 };
    std::for_each(in.begin(), in.begin() + N, [&](auto c)
        {
            ++seen[c];
            if (seen[c] == 1)
                ++unique;
        });
    auto it0 = in.begin();
    auto itN = in.begin() + N;
    if (unique != N)
    {
        for (; itN != in.end(); ++it0, ++itN)
        {
            --seen[*it0];
            if (seen[*it0] == 0)
                --unique;
            ++seen[*itN];
            if (seen[*itN] == 1)
                ++unique;
            if (unique == N)
                break;
        }
    }
//    auto end = std::chrono::high_resolution_clock().now();
//    std::cout << "duration " << std::chrono::duration_cast<std::chrono::microseconds>(end - now).count() << "us\n";

    return itN - in.begin() + 1;
}

int main()
{
    std::string in;
    std::getline(std::cin, in);
 
    std::cout << "pt1  = " << ptN<4>(in) << "\n";
    std::cout << "pt2  = " << ptN<14>(in) << "\n";
    std::cout << "ptx1 = " << ptxN<4>(in) << "\n";
    std::cout << "ptx2 = " << ptxN<14>(in) << "\n";
}