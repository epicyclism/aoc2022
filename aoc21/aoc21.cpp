#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

#include <ctre_inc.h>

auto get_input()
{
    std::vector<std::string> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        in.emplace_back(ln);
    }

    return in;
}

auto pt1(auto const& in)
{
    return in.size();
}

auto pt2(auto const& in)
{
    return in.size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}