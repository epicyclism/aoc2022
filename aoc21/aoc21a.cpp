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
    std::optional<int64_t> val_;
    std::variant<int64_t, std::string> l_;
    std::variant<int64_t, std::string> r_;
    char        op_;
    bool        done_;

    monkey_t(std::string_view nm, int64_t val) : nm_{ nm }, val_{ val }, l_{ val }, r_{ 0 }, op_{ '+' }, done_ { false }
    {
    }
    monkey_t(std::string_view nm, std::string_view l, std::string_view r, std::string_view op) : nm_{ nm }, l_{ std::string(l) }, r_{ std::string(r) }, op_{ op[0] }, done_{false}
    {
    }
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
    bool not_done{ true };
    while (not_done)
    {
        not_done = false;
        for (auto& m : in)
        {
            if (!m.done_)
            {
                if (m.l_.index() == 0 && m.r_.index() == 0)
                {
                    m.val_ = proc(std::get<0>(m.l_), std::get<0>(m.r_), m.op_);
//                    std::cout << "set " << m.nm_ << " to " << m.val_.value() << "\n";
                    for (auto& m2 : in)
                    {
                        if (m2.l_.index() == 1 && std::get<1>(m2.l_) == m.nm_)
                        {
                            m2.l_.emplace<0>(m.val_.value());
//                            std::cout << "    set " << m2.nm_ << ".l_ to " << m.val_.value() << "\n";
                        }
                        if (m2.r_.index() == 1 && std::get<1>(m2.r_) == m.nm_)
                        {
                            m2.r_.emplace<0>(m.val_.value());
//                            std::cout << "    set " << m2.nm_ << ".r_ to " << m.val_.value() << "\n";
                        }
                    }
                    m.done_ = true;
                }
            }
            if(!m.val_)
                not_done = true;
        }
    }
    for (auto& m : in)
    {
        if (m.nm_ == "root")
            return m.val_.value();
    }
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