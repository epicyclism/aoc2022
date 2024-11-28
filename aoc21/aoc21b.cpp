#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <variant>
#include <optional>

#include <ctre_inc.h>

struct monkey_t
{
    std::string nm_;
    std::variant<int64_t, std::pair<monkey_t, monkey_t>> val_;
};

auto get_input()
{
    std::vector<monkey_t> in;
    std::string ln;
    while (std::getline(std::cin, ln))
    {
        if (auto [m, nm, val] = ctre::match<R"((\w{4}): (\d+))">(ln); m)
            in.emplace_back(nm, sv_to_t<int64_t>(val));
        else if (auto [m, nm, l, op, r] = ctre::match<R"((\w{4}): (\w{4}) ([+-/*]) (\w{4}))">(ln); m)
            in.emplace_back(nm, l, r, op);
        else
            std::cout << "read error at " << ln << "\n";
    }

    return in;
}

int64_t proc(int64_t l, int64_t r, char op)
{
    switch (op)
    {
    case '+':
        return l + r;
    case '-':
        return l - r;
    case '*':
        return l * r;
    case '/':
        return l / r;
    }
    return -1;
}

int64_t pt1(std::vector<monkey_t> in)
{
    return 0;
}

int64_t pt2(std::vector<monkey_t> in)
{
    return 0;
}

int main()
{
    auto in{ get_input() };

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}