#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

#include "graph.h"

auto get_input()
{
    std::string ln;
    std::vector<char> in;
    size_t stride{0};
    size_t S{0};
    size_t E{0};
    std::getline(std::cin, ln);
    stride = ln.size();
    do
    {
        std::copy(ln.begin(), ln.end(), std::back_inserter(in));
    } while (std::getline(std::cin, ln));
    for(auto n = 0; n < in.size(); ++n)
    {
        char c;
        switch(in[n])
        {
            case 'S':
                S = n;
                c = 0;
                break;
            case 'E':
                E = n;
                c = 25;
                break;
            default:
                c = in[n] - 'a';
                break;
        }
        in[n] = c;
    }
    return std::make_tuple(in, stride, S, E);
}

auto pt1(auto const& in)
{
    auto vp = [](char f, char t) -> bool { return t <= f + 1;};
    grid<char, decltype(vp)> g { std::get<0>(in), std::get<1>(in), vp};
    auto d { bfs(g, std::get<2>(in))};
    return d[std::get<3>(in)];
}

auto pt2(auto const& in)
{
    auto vp = [](char f, char t) -> bool { return t >= f - 1;};
    grid<char, decltype(vp)> g { std::get<0>(in), std::get<1>(in), vp};
    auto d { bfs(g, std::get<3>(in))};
    auto mp { g.size()};
    for(int n = 0; n < g.size(); ++n)
        if(std::get<0>(in)[n] == 0 && d[n] < mp)
            mp = d[n];
    return mp;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}