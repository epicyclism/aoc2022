#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>

#include <ctre_inc.h>

auto get_input()
{
    std::vector<std::pair<char, char>> rv;
    constexpr auto rx = ctll::fixed_string{ R"(([ABC]) ([XYZ]))" };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto[m, a, x] = ctre::match<rx>(ln);
        rv.emplace_back(a.to_view()[0], x.to_view()[0]);
    }
    return rv;
}

auto pt(auto const& in, auto& gt)
{
    int score {0};
    for(auto& g: in)
        score += gt[g.first-'A' + (g.second - 'X') * 3];
    return score;
}

int main()
{
    constexpr std::array gt1{ 
                        //    R  P  S
                              4, 1, 7, // R: D L W
                              8, 5, 2, // P: W D L
                              3, 9, 6};// S: L W D
    constexpr std::array gt2{ 3, 1, 2, // L: S R P
                              4, 5, 6, // D: R P S
                              8, 9, 7};// W: P S R

    auto in { get_input()};
    std::cout << "pt1 = " << pt(in, gt1) << "\n";
    std::cout << "pt2 = " << pt(in, gt2) << "\n";
}