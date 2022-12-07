#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

#include <ctre_inc.h>

// durectory to total size contained
using ds_t = std::map<std::string, uint64_t>;

uint64_t enumerate_dir(std::string const& dir, ds_t& d)
{
    std::string cwd { dir };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        if(auto[m, s, n] = ctre::match<R"((\d+) (\H+))">(ln); m) // file listing
        {
            d[dir] += sv_to_t<uint64_t>(s);
        }
        else
        if(auto[m2, n2] = ctre::match<R"(\$ cd (\H+))">(ln); m2) // change dir
        {
            auto arg {n2.view()};
            if(arg == "..")
                return d[dir];
            else
                cwd += arg;
        }
        else
        if(ctre::match<R"(\$ ls)">(ln)) // list
        {
            auto dd = enumerate_dir(cwd, d);
            d[dir] += dd;
        }
        else
        if(ctre::match<R"(dir (\H+))">(ln))
        {
        }
        else
            std::cout << "no match - " << ln << "\n";
    }
    return d[dir] ;
}

auto get_input()
{
    ds_t in;
    enumerate_dir("", in);
    return in;
}

auto pt1(auto const& in)
{
    return std::accumulate(in.begin(), in.end(), 0ULL, [](auto s, auto& r){if(r.second <= 100000) return s + r.second ; else return s;});
}

auto pt2(auto const& in)
{
    constexpr auto max      { 70000000ULL };
    constexpr auto free_min { 30000000ULL };
    auto cur {in.at("/")};
    auto max_min { free_min - (max - cur)};
    auto dir_sz{cur};
    for(auto& d : in)
        if( d.second > max_min && dir_sz > d.second)
            dir_sz = d.second;

    return dir_sz;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}