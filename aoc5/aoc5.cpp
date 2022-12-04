#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ctre_inc.h>

auto get_input()
{
    std::vector<std::string> rv;
#if 0
    constexpr auto rx = ctll::fixed_string{ R"((\d+)-(\d+),(\d+)-(\d+))" };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        rv.push_back(ln);
    }
#endif
    return rv;
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

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}