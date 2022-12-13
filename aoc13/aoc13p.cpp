#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

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
    int list_depth{ 0 };
    char prev{ ' ' };
    for (auto c : s)
    {
        if (c == '[')
        {
            ++list_depth;
        }
        else
            if (c == ']')
            {
                if (prev == '[')
                    rv.push_back(list_depth << 8);
                --list_depth;
            }
            else
                if (::isdigit(c))
                {
                    if (::isdigit(prev))
                    {
                        // eek
                        auto v = rv.back();
                        v &= 0xf;
                        v -= 1;
                        v *= 10;
                        v += c - '0' + 1;
                        v |= (list_depth << 8);
                        rv.back() = v ;
                    }
                    else
                    {
                        rv.push_back(list_depth << 8 | (c - '0' + 1));
                    }
                }
        prev = c;
    }
    return rv;
}

bool compare(std::pair<std::string, std::string> const& ss)
{
    auto lv{ parse(ss.first) };
    auto rv{ parse(ss.second) };
#if 1
    auto rb = std::lexicographical_compare(lv.begin(), lv.end(), rv.begin(), rv.end(), [](auto l, auto r)
        {
            auto lo = l >> 8;
            auto ro = r >> 8;
            if (std::abs(lo - ro) == 1)
            {
                if (lo < ro)
                    l = (l & 0xff) | (lo + 1) << 8;
                else
                    r = (r & 0xff) | (ro + 1) << 8;
            }
            return l < r;
        });
#else
    auto rb = lv < rv;
#endif
    std::cout << ss.first << " < " << ss.second << " = " << rb << "\n";
    std::cout << "   ";
    for (auto i : lv)
        std::cout << i << " ";
    std::cout << "\n   ";
    for (auto i : rv)
        std::cout << i << " ";
    std::cout << "\n";
    return rb;
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