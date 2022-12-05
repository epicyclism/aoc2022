#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ctre_inc.h>

struct move
{
    int cnt_;
    int from_;
    int to_;
};

auto get_input()
{
    std::vector<std::vector<char>> stacks;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        if (ln.empty())
            break;
        for (auto m : ctre::range<R"(\[\H\])">(ln))
        {
            auto col = std::distance(ln.begin(), m.begin()) / 4;
            if (stacks.size() < col + 1)
                stacks.resize(col + 1);
            stacks[col].push_back(m.view()[1]);
        }
    }
    for (auto& s : stacks)
        std::ranges::reverse(s);
    std::vector<move> moves;
    while (std::getline(std::cin, ln))
    {
        auto [m, c, f, t] = ctre::match< R"(move (\d+) from (\d+) to (\d+))">(ln);
        moves.emplace_back(sv_to_t<int>(c), sv_to_t<int>(f) - 1, sv_to_t<int>(t) - 1);
    }
    return std::pair{ stacks, moves };
}

auto pt1(auto stacks, auto const& moves)
{
    for( auto& m : moves)
        for (auto n = 0; n < m.cnt_; ++n)
        {
            auto ch = stacks[m.from_].back();
            stacks[m.from_].pop_back();
            stacks[m.to_].push_back(ch);
        }
    std::string rs;
    for (auto& d : stacks)
        rs.push_back(d.back());
    return rs;
}

auto pt2(auto stacks, auto const& moves)
{
    for (auto& m : moves)
    {
        stacks[m.to_].insert(stacks[m.to_].end(), stacks[m.from_].end() - m.cnt_, stacks[m.from_].end());
        stacks[m.from_].erase(stacks[m.from_].end() - m.cnt_, stacks[m.from_].end());
    }
    std::string rs;
    for (auto& d : stacks)
        rs.push_back(d.back());
    return rs;
}

int main()
{
    auto[stacks, moves] {get_input()};

    std::cout << "pt1 = " << pt1(stacks, moves) << "\n";
    std::cout << "pt2 = " << pt2(stacks, moves) << "\n";
}