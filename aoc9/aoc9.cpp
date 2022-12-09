#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <ctre_inc.h>

using pt_t = std::pair<short, short>;

auto get_input()
{
    std::vector<pt_t> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto[m, c, s] = ctre::match<R"(([DLRU]) (\d+))">(ln);
        auto d { sv_to_t<short>(s)};
        switch(c.view()[0])
        {
            case 'D':
                in.emplace_back(0, -d);
                break;
            case 'L':
                in.emplace_back(-d, 0);
                break;
            case 'R':
                in.emplace_back(d, 0);
                break;
            case 'U':
                in.emplace_back(0, d);
                break;
            default:
                break; // NOT REACHED
        }
        ;
    }
    return in;
}

int distance(pt_t H, pt_t T)
{
    if( !(H.first == T.first || H.second == T.second))
            return std::abs(H.first - T.first) + std::abs(H.second - T.second) - 1;
    return std::abs(H.first - T.first) + std::abs(H.second - T.second);
}

pt_t simple_follow(pt_t H, pt_t T, auto& sT, bool record)
{
    if(H.first == T.first)
    {
        if(H.second > T.second)
        {
            while(T.second != H.second - 1)
            {
                ++T.second;
                if(record)
                    sT.insert(T);
            }
        }
        else
        {
            while(T.second != H.second + 1)
            {
                --T.second;
                if(record)
                    sT.insert(T);
            }
        }
    }
    else
    if(H.second == T.second)
    {
        if(H.first > T.first)
        {
            while(T.first != H.first - 1)
            {
                ++T.first;
                if(record)
                    sT.insert(T);
            }
        }
        else
        {
            while(T.first != H.first + 1)
            {
                --T.first;
                if(record)
                    sT.insert(T);
            }
        }
    }
    else
        std::cout << "Simple follow bug\n";
    return T;
}

pt_t follow(pt_t H, pt_t T, auto& sT, bool record)
{
    // tail has to move?
    auto d = distance(H, T);
//    std::cout << "d = " << d << "\n";
    if(d < 2)
        return T;
    if( !(H.first == T.first || H.second == T.second))
    {
        if(std::abs(H.first - T.first) == 1)
        {
            T.first = H.first;
            if(H.second > T.second)
                ++T.second;
            else
                --T.second;
        }
        else
        {
            T.second = H.second;
            if(H.first > T.first)
                ++T.first;
            else
                --T.first;
        }
        if(record)
            sT.insert(T);
    }
    return simple_follow(H, T, sT, record);
}

auto pt1(auto const& in)
{
    std::set<pt_t> sT;
    pt_t H{0, 0};
    pt_t T{0, 0};
    sT.insert(T);
    for(auto mv : in)
    {
        H.first += mv.first;
        H.second += mv.second;
        T = follow(H, T, sT, true);
    }
    std::cout << "H (" << H.first << ", " << H.second << ")\n";
    std::cout << "T (" << T.first << ", " << T.second << ")\n";
    return sT.size();
}

auto pt2(auto const& in)
{
    std::set<pt_t> sT;
    std::array<pt_t, 10> H2T {};
    for(auto& p : H2T)
        std::cout << p.first << ", " << p.second << "\n";
    sT.insert(H2T[9]);
    for(auto mv : in)
    {
        H2T[0].first += mv.first;
        H2T[0].second += mv.second;
        for(int n = 1; n < H2T.size(); ++n)
            H2T[n] = follow(H2T[n - 1], H2T[n], sT, n == 9);
        for(auto& p : H2T)
            std::cout << "( " << p.first << ", " << p.second << "),";
        std::cout << "\n";
//        std::cout << H2T[9].first << ", " << H2T[9].second << "\n";
    }
    return sT.size();

}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}