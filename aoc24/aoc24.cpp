#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

#include <experimental/mdspan>

namespace stdex = std::experimental;

constexpr int sz{128};

auto get_input()
{
    std::vector<short> arena(sz * sz, 0);
    stdex::mdspan md(arena.data(), sz, sz);

    int t {sz / 2 + 35};
    int cnt {0};
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        int l {sz / 2 - 35};
        for(auto c : ln)
        {
            if(c == '#')
            {
                ++cnt; 
                md(l, t) = cnt;
            }
            ++l;
        }
        --t;
    }
    std::cout << cnt << " elves\n";
    return arena;
}

void print(auto const& a)
{
    stdex::mdspan md(a.data(), sz, sz);
    for(auto y = sz - 1; y >= 0; --y)
    {
        for(auto x = 0; x < sz; ++x)
            std::cout << (md(x, y) ? '#' : '.');
        std::cout << "\n";
    }
}

struct pt_t
{
    int x_;
    int y_;
};
constexpr pt_t scan[] { {1, 0}, { 1, 1}, {1, -1},  // N NE NW
                        {-1, 0}, {-1, 1}, {-1, -1},// S SE SW
                        {0, -1}, {1, -1}, {-1, -1},// W, NW, SW
                        {0, 1}, {1, 1}, {-1, 1}    // E, NE, SE
                        };
auto pt1(auto const& in)
{
    auto arena {in};
    stdex::mdspan md(arena.data(), sz, sz);
    int scan_base { 0 };



    return in.size();
}

auto pt2(auto const& in)
{
    print (in);
    return in.size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}