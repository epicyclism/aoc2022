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

int val(char c)
{
    return (c >= 'a') ? c - ('a' - 1) : c - ('A' - 1) + 26;
}

auto pt1(auto const& in)
{
    int sum { 0 };
    std::string tmp ;
    for(auto& s: in)
    {
        auto fs { s.substr(0, s.size()/2) };
        auto ss { s.substr(s.size()/2) };
        std::ranges::sort(fs);
        std::ranges::sort(ss);
        tmp.clear();
        std::ranges::set_intersection(fs, ss, std::back_inserter(tmp));
        sum += val(tmp.front());
    }
    return sum;
}

auto pt2(auto const& in)
{
    std::string tmp;
    std::string tmp2;
    int sum{0};
    auto it = in.begin();
    while(it != in.end())
    {
        auto s1 {*it++};
        auto s2 {*it++};
        auto s3{*it++};
        std::ranges::sort(s1);
        std::ranges::sort(s2);
        std::ranges::sort(s3);
        tmp.clear();
        tmp2.clear();
        std::ranges::set_intersection(s1, s2, std::back_inserter(tmp));
        std::ranges::set_intersection(tmp, s3, std::back_inserter(tmp2));
        sum += val(tmp2.front());
    }
    return sum;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}