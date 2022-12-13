#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include <ctre_inc.h>

auto get_input()
{
    std::vector<int> in;
    std::string ln;
    in.emplace_back(1);
    while(std::getline(std::cin, ln))
    {
        in.emplace_back(0);
        if(ln[0] == 'a')
            in.emplace_back(sv_to_t<int>(std::string_view(ln.c_str() + 5)));
    }
    std::partial_sum(in.begin(), in.end(), in.begin());
    return in;
}

auto pt1(auto const& in)
{
    constexpr std::array tgts{20, 60, 100, 140, 180, 220}; // the nth cycle is at offset n-1...
    return std::accumulate(tgts.begin(), tgts.end(), 0, [&](auto a, auto v){return a + v * in[v-1];});
}

auto pt2(auto const& in)
{
    constexpr int sw { 40 };
    std::string screen ((sw + 1) * 6, '\n');
    for(auto iti { in.begin()}, its{screen.begin()}; iti != in.end(); ++iti, ++its)
    {
        auto col = std::distance(screen.begin(), its) % 41;
        if(col == 40) // flyback, takes no cycles...
        {
            col = 0;
            ++its;
        }
        *its = (std::abs(col - *iti) < 2) ? '#' : '.';
        ++col;
    }
    return screen ;
}

int main()
{
    auto in {get_input()};
    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 =\n"  << pt2(in);
}