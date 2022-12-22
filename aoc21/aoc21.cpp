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
//    std::ranges::sort(in, [](auto& l, auto& r){return l.nm_ < r.nm_;});
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

struct keys
{
    std::map<std::string, size_t> names;
    std::multimap<std::string, size_t> left;
    std::multimap<std::string, size_t> right; 
};

keys pre_process(auto const& in)
{
    keys k;
    for(size_t i = 0; i < in.size(); ++i)
    {
        k.names[in[i].nm_] = i;
        if(!in[i].l_.empty())
            k.left.emplace(in[i].l_, i);
        if(!in[i].r_.empty())
            k.right.emplace(in[i].r_, i);
    }
    return k;
}

void evaluate(keys const& k, auto& in)
{
    auto ir = (*k.names.find("root")).second;
    while(!in[ir].val_)
    {
        for(size_t i { 0 }; i < in.size(); ++i)
        {
            auto& ri = in[i];
            if(!ri.done_ && ri.val_)
            {
                auto rngl { k.left.equal_range(ri.nm_)};
                for(auto i {rngl.first}; i != rngl.second; ++i )
                {
                    auto& rl = in[(*i).second];
                    rl.lval_ = ri.val_.value();
                    if(rl.lval_ && rl.rval_)
                    {
                        rl.val_ = proc(rl.lval_.value(), rl.rval_.value(), rl.op_);
                    }
                }
                auto rngr { k.right.equal_range(ri.nm_)};
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
}

auto pt1(keys const& k, auto const& in)
{
    auto in_m{in};
    auto ir = (*k.names.find("root")).second;
    evaluate(k, in_m);
    return in_m[ir].val_.value();
}
#if 0
auto pt2(keys const& k, auto const& in)
{
    auto ir = names["root"];

    int64_t start {0};
    int64_t inc   {1000000000000};

    while(in[ir].lval_.value() != in[ir].rval_.value())
    {

    }
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
#endif
int main()
{
    auto in {get_input()};
    auto keys {pre_process(in)};

    std::cout << "pt1 = " << pt1(keys, in)<< "\n";
//    std::cout << "pt2 = " << pt2(in) << "\n";
}