#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <ctre_inc.h>

using pt_t = std::pair<int32_t, int32_t>;
using set_t = std::set<pt_t>;

auto get_input()
{
    std::vector<pt_t> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto[m, c, s] = ctre::match<R"(([DLRU]) (\d+))">(ln);
        auto d { sv_to_t<short>(s)};
        pt_t step;
        switch(c.view()[0])
        {
            case 'D':
                step = { 0, -1 };
                break;
            case 'L':
                step = { -1, 0 };
                break;
            case 'R':
                step = { 1, 0 };
                break;
            case 'U':
                step = { 0, 1 };
                break;
            default:
                break; // NOT REACHED
        }
        for (int n = 0; n < d; ++n)
            in.push_back(step);
    }
    return in;
}

pt_t follow(pt_t H, pt_t T, auto& sT, bool record)
{
    auto ax = std::abs(H.first - T.first);
    auto ay = std::abs(H.second - T.second);
    if (ax == 2)
    {
        if (H.first > T.first)
            ++T.first;
        else
            --T.first;
        if (ay == 1)
            T.second = H.second;
    }
    if (ay == 2)
    {
        if (H.second > T.second)
            ++T.second;
        else
            --T.second;
        if (ax == 1)
            T.first = H.first;
    }
    if (record)
        sT.insert(T);
    return T;
}

void print(set_t const& sT)
{
    short xf{ 0 }, xt{ 0 };
    short yf{ 0 }, yt{ 0 };
    for (auto& p : sT)
    {
        if (p.first < xf)
            xf = p.first;
        if (p.first > xt)
            xt = p.first;
        if (p.second < yf)
            yf = p.second;
        if (p.second > yt)
            yt = p.second;
    }
    auto cols = xt - xf + 1;
    auto rows = yt - yf + 1;

    std::vector<char> printa(rows * cols, '.');
    for (auto& p : sT)
    {
        printa[p.first - xf + (p.second - yf) * cols] = '*';
    }
    printa[-xf + -yf * cols] = 's';
    std::cout << "\n";
    auto it = printa.begin();
    for (int n = 0; n < rows; ++n)
    {
        std::copy(it, it + cols, std::ostream_iterator<char>(std::cout, ""));
        std::cout << "\n";
        it += cols;
    }
}

auto pt1(auto const& in)
{
    set_t sT;
    pt_t H{0, 0};
    pt_t T{0, 0};
    sT.insert(T);
    for(auto mv : in)
    {
        H.first += mv.first;
        H.second += mv.second;
        T = follow(H, T, sT, true);
    }
    return sT.size();
}

auto pt2(auto const& in)
{
    set_t sT;
    std::array<pt_t, 10> H2T{};
    sT.insert(H2T[9]);
    for (auto mv : in)
    {
        H2T[0].first += mv.first;
        H2T[0].second += mv.second;
        for (int n = 1; n < H2T.size(); ++n)
            H2T[n] = follow(H2T[n - 1], H2T[n], sT, n == 9);
    }
    return sT.size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}