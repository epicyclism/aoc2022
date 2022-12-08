#include <iostream>
#include <string>
#include <array>
#include <algorithm>

// brute force
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
    auto it0 = in.begin();
    auto itN = in.begin() + N;
    while(itN != in.end())
    {
        if(all_unique(it0, itN))
            break;
        ++it0;
        ++itN;
    }

    return itN - in.begin();
}

// standard
auto pt12(auto const& in)
{
    int rv1{ 0 };
    int rv2{ 0 };
    std::array<int, 256> last{ -1 };
    int left = 0;
    for (int right = 0; right < in.size(); ++right)
    {
        left = (std::max)(left, last[in[right]] + 1);
        last[in[right]] = right;
        if (rv1 == 0 && right - left + 1 == 4)
            rv1 = right + 1;
        if (rv2 == 0 && right - left + 1 == 14)
        {
            rv2 = right + 1;
            break;
        }
    }

    return std::pair{ rv1, rv2 };
}

int main()
{
    std::string in;
    std::getline(std::cin, in);
 
    std::cout << "pt1   = " << ptN<4>(in) << "\n";
    std::cout << "pt2   = " << ptN<14>(in) << "\n";
    auto p12{ pt12(in) };
    std::cout << "pt121 = " << p12.first << "\n";
    std::cout << "pt122 = " << p12.second << "\n";
}