#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ctre_inc.h>

struct monkey_t
{
    int id_;
    std::vector<int64_t> items_;
    char op_;
    int param_;
    int test_;
    int true_;
    int false_;
    int inspected_;
};

auto get_input()
{
    std::vector<monkey_t> in;
    std::string ln;
    monkey_t mk;
    mk.inspected_ = 0;
    while(std::getline(std::cin, ln))
    {
        if( ln.empty() && !mk.items_.empty())
        {
            mk.id_ = in.size();
            in.emplace_back(std::move(mk));
            mk.items_.clear();
            continue;
        }
        if(auto m = ctre::match<R"(Monkey \d+:)">(ln); m)
            continue;
        if(auto m = ctre::search<R"(  Starting items: )">(ln); m)
            for (auto i : ctre::range<R"((\d+))">(ln))
                mk.items_.push_back(sv_to_t<int64_t>(i));
        else
        if(auto[m, o, p] = ctre::match<R"(  Operation: new = old (\H) (\d+))">(ln); m)
        {
            mk.op_ = o.view()[0];
            mk.param_ = sv_to_t<int>(p);
        }
        else
        if(auto[m, o] = ctre::match<R"(  Operation: new = old (\H) old)">(ln); m)
        {
            mk.op_ = o.view()[0];
            mk.param_ = -1;
        }
        else
        if(auto[m, t] = ctre::match<R"(  Test: divisible by (\d+))">(ln); m)
            mk.test_ = sv_to_t<int>(t);
        else
        if(auto[m, v] = ctre::match<R"(    If true: throw to monkey (\d+))">(ln); m)
            mk.true_ = sv_to_t<int>(v);
        else
        if(auto[m, v] = ctre::match<R"(    If false: throw to monkey (\d+))">(ln); m)
            mk.false_ = sv_to_t<int>(v);
    }
    mk.id_ = in.size();
    if( !mk.items_.empty())
        in.emplace_back(std::move(mk));

    return in;
}

std::ostream& operator<<(std::ostream& ostr, monkey_t const& mk)
{
    ostr << "Monkey " << mk.id_ << ":\n";
    ostr << "  Starting items: ";
    for(auto i : mk.items_)
        std::cout << i << " ";
    ostr << "\n";
    if(mk.param_ == -1)
        ostr << "  Operation: new = old * old\n";
    else
        ostr << "  Operation: new = old " << mk.op_ << " " << mk.param_ << "\n";
    ostr << "  Test: divisible by " << mk.test_ << "\n";
    ostr << "    If true: throw to monkey " << mk.true_ << "\n";
    ostr << "    If false: throw to monkey " << mk.false_ << "\n";
    ostr << "Inspected: " << mk.inspected_ << "\n";
    return ostr;
}

void print(auto const& in)
{
    for(auto& m : in)
        std::cout << m << "\n";
}

template<typename F> void round(auto& in, F fn)
{
    for(auto& m : in)
    {
        for(auto itm: m.items_)
        {
            auto f {0};
            if(m.param_ == -1)
                f = itm;
            else
                f = m.param_;
            if( m.op_ == '*')
                itm *= f;
            else
                itm += f;
            itm = fn(itm);
            if(itm % m.test_ == 0LL)
                in[m.true_].items_.push_back(itm);
            else
                in[m.false_].items_.push_back(itm);
           ++m.inspected_;
        }
        m.items_.clear();
    }
}

auto get_m(auto const& in)
{
    int64_t m { 1 };
    for(auto& mk : in)
        m *= mk.test_;
    return m;
}

auto pt1(auto in)
{
    for(int r = 0; r < 20; ++r)
        round(in, [](auto i){ return i /3;});
    std::ranges::sort(in, [](auto& l, auto& r){ return l.inspected_ > r.inspected_;});

    return in[0].inspected_ * in[1].inspected_;
}

auto pt2(auto in)
{
    for(int r = 0; r < 10000; ++r)
        round(in, [m = get_m(in)](auto i){ return i % m;});
    std::ranges::sort(in, [](auto& l, auto& r){ return l.inspected_ > r.inspected_;});

    return int64_t(in[0].inspected_) * in[1].inspected_;
}


int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}