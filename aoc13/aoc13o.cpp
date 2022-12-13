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

std::vector<int64_t> parse(std::string_view s)
{
    std::vector<int64_t> rv;
    int64_t factor { 1 };
    for(auto c: s)
    {
        if(c == '[')
        {
            factor *= 16;
        }
        else
        if(c == ']')
        {
            factor /= 16;
        }
        else
        if (::isdigit(c))
        {
            rv.push_back(factor * (c - '0' + 2));
        }
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

bool compare0(std::pair<std::string, std::string> const& ss)
{
#if 1
    auto lv = parse(ss.first);
    auto rv = parse(ss.second);
    auto rb = lv < rv;
    auto rb2 = std::lexicographical_compare(lv.begin(), lv.end(), rv.begin(), rv.end(), [](auto l, auto r)
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

    std::cout << ss.first << " < " << ss.second << " = " << rb << ", " << rb2 << "\n";
    std::cout << "   " ;
    for(auto i: lv)
        std::cout << i << " ";
    std::cout << "\n   ";
    for(auto i: rv)
        std::cout << i << " ";
    std::cout << "\n";
    return rb2;
#else
    return parse(ss.first) < parse(ss.second);
#endif
}

bool compare1(std::string const& l, std::string const& r, bool& final)
{
    auto dl = ::isdigit(l[0]);
    auto dr = ::isdigit(r[0]);
    if (dl && dr)
    {
        if (l[0] == r[0]) // continue
            return true;
        final = l[0] < r[0];
        return false; // stop
    }
    if (dl)
    {
        std::string sl{ '[', l[0], ']' };
        return compare1(sl, r, final);
    }
    if (dr)
    {
        std::string sr{ '[', r[0], ']' };
        return compare1(l, sr, final);
    }
    auto sl = l;
    auto sr = r;
    do
    {
        if (!(sl[0] == ']' && sr[0] == ']'))
        {
            if (sl[0] == ']')
            {
                final = true;
                return false;
            }
            if (sr[0] == ']')
            {
                final = false;
                return false;
            }
        }
        sl = sl.substr(1);
        if (sl[0] == ',')
            sl = sl.substr(1);
        sr = sr.substr(1);
        if (sr[0] == ',')
            sr = sr.substr(1);
    }
    while(compare1(sl, sr, final));
    return false;
}

auto find_close(auto b, auto e)
{
    int nb{ 1 };
    while (b != e)
    {
        switch (*b)
        {
        case '[':
            ++nb;
            break;
        case ']':
            --nb;
            if (nb == 0)
                return b;
            break;
        default:
            break;
        }
        ++b;
    }
    return b;
}

std::string_view get_next(std::string_view whole)
{
    auto itb = whole.begin();
    auto ite = itb + 1;
    while (ite != whole.end())
    {
        switch (*itb)
        {
        case '[':
            return { itb+1, find_close(ite, whole.end()) };
        case ']':
            std::cout << "tilt\n";
        case ',':
            ++itb, ++ite;
            break;
        default: // digit
            return { itb, ite };
        }
    }
    return { itb, ite };
}

void dump(std::string_view s)
{
//    auto lst = get_next(s);
//    auto off{ lst.size() + 1};
    auto off{ 0 };
    while (off < s.size())
    {
        auto lst = get_next({ s.begin() + off, s.end()});
        off += lst.size() + 1;
        if (lst.size() == 1)
        {
            // digit
            std::cout << lst << "\n";
        }
        else
            dump(lst);
    }
}

bool compare(std::string const& l, std::string const& r)
{
    dump(l);
    std::cout << "\n";
    dump(r);

    return false;
}

bool compare(std::pair<std::string, std::string> const& ss)
{
//    return compare(ss.first, ss.second);
    return ss.first > ss.second;
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