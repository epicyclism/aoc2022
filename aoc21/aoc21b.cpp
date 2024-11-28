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
    monkey_op_t(std::string_view l, std::string_view r, char op) : l_{l}, r_{r}, op_{op}
    {}
};

struct monkey_t
{
    std::string nm_;
    std::variant<int64_t, monkey_op_t> val_;
    
    monkey_t(std::string_view nm, int64_t val) : nm_{ nm }, val_{ val }
    {
    }
    monkey_t(std::string_view nm, std::string_view l, std::string_view r, std::string_view op) : nm_{ nm }, val_{monkey_op_t{std::string(l), std::string(r), op[0]} }
    {
    }
};

struct monkey_node_t
{
    int m_;
    std::variant<int64_t,std::vector<monkey_node_t>> c_;
};

int lookup(std::vector<monkey_t> const& v, std::string_view k)
{
    return std::find_if(v.begin(), v.end(), [k](auto& m){ return m.nm_ == k;}) - v.begin();
}

monkey_node_t build_c_(std::vector<monkey_t> const& vm, int nd)
{
    if(vm[nd].val_.index() == 1)
    {
        std::vector<monkey_node_t> v(2);
        v.emplace_back(build_c_(vm, lookup(vm, std::get<1>(vm[nd].val_).l_)));
        v.emplace_back(build_c_(vm, lookup(vm, std::get<1>(vm[nd].val_).r_)));
        return monkey_node_t{nd, v};
    }
    return monkey_node_t{nd, std::get<0>(vm[nd].val_)};
}

monkey_node_t make_tree(std::vector<monkey_t> const& vm)
{
    int rt { lookup(vm, "root")};
    return build_c_(vm, rt);
}

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

int64_t eval1(std::vector<monkey_t> const& vm, monkey_node_t nd)
{
    if(nd.c_.index() == 1)
    {
        return proc(eval1(vm, std::get<1>(nd.c_)[0]), eval1(vm, std::get<1>(nd.c_)[1]), std::get<1>(vm[nd.m_].val_).op_);
    }
    return std::get<0>(vm[std::get<0>(nd.c_)].val_);
}

int64_t pt1(std::vector<monkey_t> in)
{
    auto tree { make_tree(in)};
    return eval1(in, tree);
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