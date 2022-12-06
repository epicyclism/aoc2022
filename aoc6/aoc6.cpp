#include <iostream>
#include <string>

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

int main()
{
    std::string in;
    std::getline(std::cin, in);
 
    std::cout << "pt1 = " << ptN<4>(in) << "\n";
    std::cout << "pt2 = " << ptN<14>(in) << "\n";
}