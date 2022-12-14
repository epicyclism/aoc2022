#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <experimental/mdspan>

namespace stdex = std::experimental;

auto get_input()
{
    std::vector<char> data;
    size_t stride { 0 };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        stride = ln.size();
        std::ranges::transform(ln, std::back_inserter(data), [](auto c){return c - '0';});
    }
    return std::pair(data, stride);
}

template<typename F> void proc_1(auto const& mds, F f)
{
    char max_so_far { -1 };
    for (int i = 0; i < mds.extent(0); ++i)
    {
        if (max_so_far < mds[i])
            f(i);
        if (mds[i] > max_so_far)
            max_so_far = mds[i];
    }
    max_so_far = -1;
    for (int i = mds.extent(0) - 1; i >= 0; --i)
    {
        if (max_so_far < mds[i])
            f(i);
        if (mds[i] > max_so_far)
            max_so_far = mds[i];
    }
}

template<typename F> void proc_2(auto const& mds, F f)
{
    for (int t = 1; t < mds.extent(0); ++t)
    {
        int v{ 0 };
        auto h{ mds[t] };
        for (auto c = t - 1; c >= 0; --c)
        {
            ++v;
            if (mds[c] >= h)
                break;
        }
        f(t, v);
        v = 0;
        for (auto c = t + 1; c < mds.extent(0); ++c)
        {
            ++v;
            if (mds[c] >= h)
                break;
        }
        f(t, v);
    }
}

auto pt12(auto const& in)
{
    stdex::mdspan md(in.first.data(), in.second, in.second);
    std::vector<bool> ss(in.second * in.second); // record out for part 1!
    std::vector<int> out(in.first.size(), 1);    // record out for part 2!
    stdex::mdspan mdo(out.data(), in.second, in.second);
    for (int r = 0; r < md.extent(0); ++r)
    {
        auto mds = stdex::submdspan(md, r, stdex::full_extent);
        proc_1(mds, [&](auto c) {ss[r*in.second+c] = true; });
        proc_2(mds, [&mdo, r](auto c, auto v) {mdo(c, r) *= v; });
    }
    for (int c = 0; c < md.extent(1); ++c)
    {
        auto mds = stdex::submdspan(md, stdex::full_extent, c);
        proc_1(mds, [&](auto r) {ss[r * in.second + c] = true; });
        proc_2(mds, [&mdo, c](auto r, auto v) {mdo(c, r) *= v; });
    }
    return std::pair(std::ranges::count(ss, true), *std::ranges::max_element(out));
}

int main()
{
    auto in {get_input()};
    auto p12{ pt12(in) };
    std::cout << "pt1 = " << p12.first << "\n";
    std::cout << "pt2 = " << p12.second << "\n";
}