#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <variant>
#include <optional>

#include <ctre_inc.h>

struct monkey_op_t
{
    std::string l_;
    std::string r_;
    char op_;
    monkey_op_t(std::string_view l, std::string_view r, std::string_view op) : l_{l}, r_{r}, op_{op[0]}
    {}
};

auto get_input()
{
    std::map<std::string, std::variant<int64_t, monkey_op_t>> in;
    std::string ln;
    while (std::getline(std::cin, ln))
    {
        if (auto [m, nm, val] = ctre::match<R"((\w{4}): (\d+))">(ln); m)
            in.emplace(nm, sv_to_t<int64_t>(val));
        else if (auto [m, nm, l, op, r] = ctre::match<R"((\w{4}): (\w{4}) ([+-/*]) (\w{4}))">(ln); m)
            in.emplace(nm, monkey_op_t{ l.view(), r.view(), op.view()});
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

int64_t eval1(std::map<std::string, std::variant<int64_t, monkey_op_t>> const& m, std::string const& nm)
{
    auto e{m.find(nm)};
    if((*e).second.index() == 0)
        return std::get<0>((*e).second);
    auto& mot{std::get<1>((*e).second)};
    return proc(eval1(m, mot.l_), eval1( m, mot.r_), mot.op_);
}

int64_t pt1(std::map<std::string, std::variant<int64_t, monkey_op_t>> const& in)
{
    return eval1(in, "root");
}

bool find(std::map<std::string, std::variant<int64_t, monkey_op_t>> const& m, std::string const& nm)
{
    if(nm == "humn")
        return true;
    auto e{m.find(nm)};
    if((*e).second.index() == 0)
        return false;
    auto& mot{std::get<1>((*e).second)};
    return find(m, mot.l_) || find( m, mot.r_);
}

void trace(std::map<std::string, std::variant<int64_t, monkey_op_t>> const& m, std::string const& nm)
{
    if(nm == "humn")
        return ;
    std::cout << nm << "\n";
    auto e{m.find(nm)};
    if((*e).second.index() == 0)
    {
        std::cout << " " << std::get<0>((*e).second) << "\n";
        return;
    }
    auto& mot{std::get<1>((*e).second)};
    trace(m, mot.l_) ;
    std::cout << mot.op_ << "\n";
    trace( m, mot.r_);
}

int64_t pt2(std::map<std::string, std::variant<int64_t, monkey_op_t>> const& in)
{
    auto e{in.find("root")};
     auto& mot{std::get<1>((*e).second)};

    if(!find(in, mot.l_))
    {
        std::cout << "\nl = " << eval1(in, mot.l_) << "\n";
        trace(in, mot.r_);
    }
    if(!find(in, mot.r_))
    {
        std::cout << "\nr = " << eval1(in, mot.r_) << "\n";
        trace(in, mot.l_);
    }
    return 0;
}

int main()
{
    auto in{ get_input() };

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}