#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ctre_inc.h>

struct ft_t
{
    int f1_;
    int t1_;
    int f2_;
    int t2_;
};

auto get_input()
{
    std::vector<ft_t> rv;
    constexpr auto rx = ctll::fixed_string{ R"((\d+)-(\d+),(\d+)-(\d+))" };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto[m, f1, t1, f2, t2] = ctre::match<rx>(ln);
        rv.emplace_back(sv_to_t<int>(f1.to_view()), sv_to_t<int>(t1.to_view()), sv_to_t<int>(f2.to_view()), sv_to_t<int>(t2.to_view()));
    }
    return rv;
}

bool contains(ft_t r)
{
    return r.f1_<= r.f2_ && r.t1_ >= r.t2_ ||
        r.f2_<= r.f1_ && r.t2_ >= r.t1_ ;
}

bool overlaps(ft_t r)
{
    return r.t1_>= r.t2_ && r.f1_ <= r.t2_ ||
        r.t2_ >= r.f1_ && r.f2_ <= r.t1_ ;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << std::ranges::count_if(in, [](auto& r){return contains(r);}) << "\n";
    std::cout << "pt2 = " << std::ranges::count_if(in, [](auto& r){return overlaps(r);}) << "\n";
}