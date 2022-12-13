#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ctre_inc.h>

auto get_input()
{
    std::string ln;
    std::string lnl;
    std::string lnr;
    std::vector<std::pair<std::string, std::string>> in;
    while (std::getline(std::cin, ln))
    {
        if(ln.empty())
            in.emplace_back(std::move(lnl), std::move(lnr));
        else
        if(lnl.empty())
            lnl.swap(ln);
        else
            lnr.swap(ln);
    }
    in.emplace_back(std::move(lnl), std::move(lnr));
    return in;
}

std::vector<int64_t> parse1(std::string_view s)
{
    std::vector<int64_t> rv;
    int64_t factor { 1 };
    int cnt { 0 };
    for(auto c: s)
    {
        if(c == '[')
        {
            factor *= 10;
            if(cnt == 1)
                rv.back() *= factor;
            cnt = 0;
        }
        else
        if(c == ']')
        {
            if(cnt == 1)
                rv.back() *= 10;
            factor /= 10;
            cnt = 0;
        }
        else
        if (::isdigit(c))
        {
            rv.push_back(factor * (c - '0'));
            ++cnt;
        }
    }
    return rv;
}

std::vector<int64_t> parse(std::string_view s)
{
    std::vector<int64_t> rv;
    int64_t factor { 1 };
    char lc {' '};
    for(auto c: s)
    {
        if(c == '[')
        {
            factor *= 16;
        }
        else
        if(c == ']')
        {
            if(lc == '[')
                rv.push_back(factor);
            factor /= 16;
        }
        else
        if (::isdigit(c))
        {
            rv.push_back(factor * (c - '0' + 2));
        }
        lc = c;
    }
    return rv;
}

template<int N> int power_of_N(int n)
{
    int power{0};
    n /= N;
    while(n)
    {
        ++power;
        n /= N;
    }
    return power;
}
bool compare(std::pair<std::string, std::string> const& ss)
{
#if 1
    auto lv = parse(ss.first);
    auto rv = parse(ss.second);
//    auto rb = lv < rv;
    auto rb = std::lexicographical_compare(lv.begin(), lv.end(), rv.begin(), rv.end(), [](auto l, auto r)
    {
        auto lo = power_of_N<16>(l);
        auto ro = power_of_N<16>(r);
        if(std::abs(lo - ro) > 1)
            std::cout << l << " ! " << r << "\n";
        if( lo > ro)
            r *= 16;
        else
        if( ro > lo)
            l *= 16;
        return l < r;
    });

    std::cout << ss.first << " < " << ss.second << " = " << rb << "\n";
    std::cout << "   " ;
    for(auto i: lv)
        std::cout << i << " ";
    std::cout << "\n   ";
    for(auto i: rv)
        std::cout << i << " ";
    std::cout << "\n";
    return rb;
#else
    return parse(ss.first) < parse(ss.second);
#endif
}

// 5497 too high
// 5160 too low
// 5241 too low
auto pt1(auto const& in)
{
    int rv {0};
    for(int n = 0; n < in.size(); ++n)
        if( compare(in[n]))
            rv += n + 1;
    return rv;
}

auto pt2(auto const& in)
{
//    for(auto&& ss: in)
//        std::cout << ss.first << " " << ss.second << "\n";
    return in.size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}