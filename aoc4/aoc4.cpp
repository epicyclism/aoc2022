#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

auto get_input()
{
    std::vector<std::string> rv;
    std::string ln;
    while(std::getline(std::cin, ln))
        rv.emplace_back(ln);
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