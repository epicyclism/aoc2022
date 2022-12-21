#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <optional>

#include <ctre_inc.h>

struct monkey_t
{
    std::string nm_;
    std::string l_;
    std::string r_;
    char       op_;

    std::optional<int64_t> val_;
    std::optional<int64_t> lval_;
    std::optional<int64_t> rval_;

    bool done_;

    monkey_t(std::string_view nm, int64_t val) : nm_{nm}, val_{val}, done_{false}
    {}
    monkey_t(std::string_view nm, std::string_view l, std::string_view r, std::string_view op): nm_{nm}, l_{l}, r_{r}, op_{op[0]}, done_{false}
    {}
    friend auto operator<(monkey_t const& l, monkey_t const& r)
    {
        return l.nm_ < r.nm_;
    }
    friend auto operator<(monkey_t const& l, std::string_view r)
    {
        return l.nm_ < r;
    }
    friend auto operator<(std::string_view l, monkey_t const& r)
    {
        return l < r.nm_;
    }
};

auto get_input()
{
    std::vector<monkey_t> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        if(auto[ m, nm, val] = ctre::match<R"((\w{4}): (\d+))">(ln); m)
            in.emplace_back(nm, sv_to_t<int64_t>(val));
        else if(auto[ m, nm, l, op, r] = ctre::match<R"((\w{4}): (\w{4}) ([+-/*]) (\w{4}))">(ln); m)
            in.emplace_back(nm, l, r, op);
        else
            std::cout << "read error at " << ln << "\n";
    }
    std::ranges::sort(in, [](auto& l, auto& r){return l.nm_ < r.nm_;});
//    std::ranges::sort(in);

    return in;
}

int64_t proc(int64_t l, int64_t r, char op)
{
    switch(op)
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

auto pt1(auto in)
{
    std::map<std::string, size_t> names;
    std::multimap<std::string, size_t> left;
    std::multimap<std::string, size_t> right;
    for(size_t i = 0; i < in.size(); ++i)
    {
        names[in[i].nm_] = i;
        if(!in[i].l_.empty())
            left.emplace(in[i].l_, i);
        if(!in[i].r_.empty())
            right.emplace(in[i].r_, i);
    }

    auto ir = names["root"];
    while(!in[ir].val_)
    {
        for(size_t i { 0 }; i < in.size(); ++i)
        {
            auto& ri = in[i];
            if(!ri.done_ && ri.val_)
            {
                auto rngl { left.equal_range(ri.nm_)};
                for(auto i {rngl.first}; i != rngl.second; ++i )
                {
                    auto& rl = in[(*i).second];
                    rl.lval_ = ri.val_.value();
                    if(rl.lval_ && rl.rval_)
                    {
                        rl.val_ = proc(rl.lval_.value(), rl.rval_.value(), rl.op_);
                    }
                }
                auto rngr { right.equal_range(ri.nm_)};
                for(auto i {rngr.first}; i != rngr.second; ++i )
                {
                    auto& rr = in[(*i).second];
                    rr.rval_ = ri.val_.value();
                    if(rr.lval_ && rr.rval_)
                    {
                        rr.val_ = proc(rr.lval_.value(), rr.rval_.value(), rr.op_);
                    }
                }
                ri.done_ = true;
            }
        }
    }
    return in[ir].val_.value();
}

auto pt2(auto const& inc)
{
    std::map<std::string, size_t> names;
    std::multimap<std::string, size_t> left;
    std::multimap<std::string, size_t> right;
    for(size_t i = 0; i < inc.size(); ++i)
    {
        names[inc[i].nm_] = i;
        if(!inc[i].l_.empty())
            left.emplace(inc[i].l_, i);
        if(!inc[i].r_.empty())
            right.emplace(inc[i].r_, i);
    }

    auto ir = names["root"];
    std::vector<monkey_t> in ;
//    int64_t start {inc[names["humn"]].val_.value()};
    int64_t start {3712643961852};
    int64_t end { start};
    do
    {
        in.clear();
        in.insert(in.begin(), inc.begin(), inc.end() );
        in[names["humn"]].val_ = start;

        while(!in[ir].val_)
        {
            for(size_t i { 0 }; i < in.size(); ++i)
            {
                auto& ri = in[i];
                if(!ri.done_ && ri.val_)
                {
                    auto rngl { left.equal_range(ri.nm_)};
                    for(auto i {rngl.first}; i != rngl.second; ++i )
                    {
                        auto& rl = in[(*i).second];
                        rl.lval_ = ri.val_.value();
                        if(rl.lval_ && rl.rval_)
                        {
                            rl.val_ = proc(rl.lval_.value(), rl.rval_.value(), rl.op_);
                        }
                    }
                    auto rngr { right.equal_range(ri.nm_)};
                    for(auto i {rngr.first}; i != rngr.second; ++i )
                    {
                        auto& rr = in[(*i).second];
                        rr.rval_ = ri.val_.value();
                        if(rr.lval_ && rr.rval_)
                        {
                            rr.val_ = proc(rr.lval_.value(), rr.rval_.value(), rr.op_);
                        }
                    }
                    ri.done_ = true;
                }
            }
        }
        auto rootl { in[ir].lval_.value()};
        auto rootr { in[ir].rval_.value()};
        std::cout << "start = " << start << ", root got " << rootl << " and " << rootr << "\n";
//        if(rootl > rootr)
//            start /= 2;
//        else
            start +=1;
    }
    while((in[ir].lval_.value() > in[ir].rval_.value()));

    return in[ir].val_.value();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}