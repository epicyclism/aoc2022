#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <array>

#include <ctre_inc.h>

using uc_t = std::pair<unsigned, int>;

auto get_input()
{
    std::vector<uc_t> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        if(ln[0] == 'n')
            in.emplace_back(1, 0);
        else
            in.emplace_back(2, sv_to_t<int>(std::string_view(ln.c_str() + 5)));
    }
    return in;
}

auto pt1(auto const& in)
{
    int rv { 0};
    constexpr std::array tgts{20, 60, 100, 140, 180, 220};
    int cycle {0};
    int X {1};
    int Xn{1};
    auto ins { in.begin()};
    auto tgt { tgts.begin()};
    while(tgt != tgts.end())
    {
        while(cycle < *tgt)
        {
            X = Xn;
            cycle += (*ins).first;
            Xn = X + (*ins).second;
            ++ins;
        }
        rv += (*tgt * X);
        ++tgt;
    }
    return rv;
}

auto pt2(auto const& in)
{
    constexpr int sw { 40 };
    std::array <char, sw * 6> screen;
    screen.fill('.');
    int cycle {0};
    int X {1};
    for(auto ins : in)
    {
        for( int uc = 0; uc < ins.first; ++uc)
        {
            auto hp = cycle % 40;
            if(hp >= X - 1 && hp <= X + 1)
                screen[cycle] = '#';
            ++cycle;
        }
        X += ins.second;
    }
    auto oi = screen.begin();
    while( oi != screen.end())
    {
        std::copy(oi, oi + sw, std::ostream_iterator<char>(std::cout, ""));
        std::cout << "\n";
        oi += sw;
    }
}

int main()
{
    auto in {get_input()};
    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 =\n" ;
    pt2(in);
}