#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

#include <ctre_inc.h>

struct pt_t
{
    int x_;
    int y_;
    int z_;
};

auto get_input()
{
    std::vector<pt_t> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto [m, x, y, z] = ctre::match<R"((\d+),(\d+),(\d+))">(ln);
        in.emplace_back(sv_to_t<int>(x), sv_to_t<int>(y), sv_to_t<int>(z));
    }

    return in;
}

constexpr int bound {32};

int pt_to_off(pt_t p, int shift = 0)
{
    return (p.z_ + shift) * bound * bound + (p.y_ + shift) * bound + (p.x_ + shift);
}

auto adjacents(pt_t const& p)
{
    std::vector<pt_t> vr;
    if(p.x_ > 0)
        vr.emplace_back(p.x_ - 1, p.y_, p.z_);
    if(p.y_ > 0)
        vr.emplace_back(p.x_, p.y_ - 1, p.z_);
    if(p.z_ > 0)
        vr.emplace_back(p.x_, p.y_, p.z_ - 1);
    if(p.x_ < bound - 1)
        vr.emplace_back(p.x_ + 1, p.y_, p.z_);
    if(p.y_ < bound - 1)
        vr.emplace_back(p.x_, p.y_ + 1, p.z_);
    if(p.z_ < bound - 1)
        vr.emplace_back(p.x_, p.y_, p.z_ + 1);
    return vr;
}

auto pt1(auto const& in)
{
    std::bitset<bound*bound*bound> bs;
    int cnt{0};
    for(auto& p : in)
    {
        cnt += 6;
        auto vs {adjacents(p)};
        for(auto i: vs)
            cnt -= 2 * bs.test(pt_to_off(i));
        bs.set(pt_to_off(p));
    }

    return cnt;
}

// returns count of the faces it cannot visit
int dfs_visit(auto const& bsg, pt_t p, auto& bsv)
{
    int rv { 0 };
    auto al { adjacents(p)};
    bsv.set(pt_to_off(p));
    for(auto a: al)
    {
        auto ao { pt_to_off(a)};
        if(bsg.test(ao))
            ++rv;
        else
        if( !bsv.test(ao))
            rv += dfs_visit(bsg, a, bsv);
    }
    return rv;
}

auto pt2(auto const& in)
{
    std::bitset<bound*bound*bound> bs; // set for cube here
    for(auto& p : in)
       bs.set(pt_to_off(p, 1)); // move cubes so none touch axes -> clear connection right round
    std::bitset<bound*bound*bound> bsv; // set for visited
    return dfs_visit(bs, {0, 0, 0}, bsv); // we know 0,0,0 is outside object
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}