#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <numeric>
#include <optional>
#include <compare>
#include <limits>

#include "ctre_inc.h"

struct region
{
    int64_t xf_;
    int64_t xt_;
    int64_t yf_;
    int64_t yt_;
    int64_t zf_;
    int64_t zt_;
    auto operator<=>(region const&)  const = default;
};

std::ostream& operator<<(std::ostream& o, region const& i)
{
    o << i.xf_ << ".." << i.xt_ << "," << i.yf_ << ".." << i.yt_ << "," << i.zf_ << ".." << i.zt_;
    return o;
}

auto get_input()
{
    std::vector<std::pair<bool, region>> vr;
    constexpr auto rx = ctll::fixed_string{ R"((on|off) x=(-?\d+)\.\.(-?\d+),y=(-?\d+)\.\.(-?\d+),z=(-?\d+)\.\.(-?\d+))" };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto[m, oo, xf, xt, yf, yt, zf, zt] = ctre::match<rx>(ln);
        vr.push_back({ oo.to_view() == "on", {sv_to_t<int64_t>(xf.to_view()), sv_to_t<int64_t>(xt.to_view()), sv_to_t<int64_t>(yf.to_view()), sv_to_t<int64_t>(yt.to_view()), sv_to_t<int64_t>(zf.to_view()), sv_to_t<int64_t>(zt.to_view())} });
    }
    return vr;
}

template<int64_t F, int64_t T> auto in_range(region const& i)
{
    return i.xf_ >= F && i.xf_ <= T && i.xt_ >= F && i.xt_ <= T &&
            i.yf_ >= F && i.yf_ <= T && i.yt_ >= F && i.yt_ <= T &&
            i.zf_ >= F && i.zf_ <= T && i.zt_ >= F && i.zt_ <= T ;
}

auto inner_regions(auto const& regions)
{
    std::vector<std::pair<bool, region>> vr;
    for (auto& r : regions)
        if (in_range<-50, 50>(r.second))
            vr.emplace_back(r);
    return vr;
}

template<int64_t F, int64_t T> auto to_off(int64_t x, int64_t y, int64_t z)
{
    return (x - F) + ((y - F) * (T - F + 1)) + ((z - F) * (T - F + 1) * (T - F + 1));
}

int pt1(auto const& in)
{
    std::bitset<101*101*101> bs;

    for(auto& i : in)
    {
        auto& r{ i.second };
        if( in_range<-50, 50>(r))
            for(auto x = r.xf_; x <= r.xt_; ++x)
                for(auto y = r.yf_; y <= r.yt_; ++y)
                    for(auto z = r.zf_; z <= r.zt_; ++z)
                            bs.set(to_off<-50, 50>(x, y, z), i.first);
    }
    return bs.count();
}

std::optional<region> intersect(region const& i1, region const& i2)
{
    region ic{  i1.xf_ < i2.xf_ ? i2.xf_ : i1.xf_,
                i1.xt_ < i2.xt_ ? i1.xt_ : i2.xt_,
                i1.yf_ < i2.yf_ ? i2.yf_ : i1.yf_,
                i1.yt_ < i2.yt_ ? i1.yt_ : i2.yt_,
                i1.zf_ < i2.zf_ ? i2.zf_ : i1.zf_,
                i1.zt_ < i2.zt_ ? i1.zt_ : i2.zt_};
    if ( ic.xt_ >= ic.xf_ && ic.yt_ >= ic.yf_ && ic.zt_ >= ic.zf_)
        return ic;
    return std::nullopt;
}

int64_t sz(region const& i)
{
    return (i.xt_ - i.xf_ + 1) * (i.yt_ - i.yf_ + 1) * (i.zt_ - i.zf_ + 1);
}

int64_t pt2(auto const& in)
{
    struct cube
    {
        region r_;
        std::vector<cube> h_;
        void cut(region const& c)
        {
            for (auto& h : h_)
                h.cut(c);
            if (r_ != c)
            {
                if(auto ii = intersect(r_, c); ii)
                    h_.push_back({ ii.value() });
            }
        }
        int64_t cnt() const
        {
            return sz(r_) - std::accumulate(h_.begin(), h_.end(), 0LL, [](auto s, auto& c) { return s + c.cnt(); });
        }
    };
    std::vector<cube> vc;
    for (auto& i : in)
    {
        for (auto& c : vc)
            c.cut(i.second);
        if (i.first)
            vc.push_back({ i.second });
    }
    return std::accumulate(vc.begin(), vc.end(), 0LL, [](auto s, auto& c) { return s + c.cnt(); });
}
#if 0
bool in_region_x(int64_t x, region const& r)
{
    return x >= r.xf_ && x <= r.xt_;
}

bool in_region_y(int64_t x, region const& r)
{
    return x >= r.yf_ && x <= r.yt_;
}

bool in_region_z(int64_t x, region const& r)
{
    return x >= r.zf_ && x <= r.zt_;
}

bool in_region(int64_t x, int64_t y, int64_t z, region const& r)
{
    return x >= r.xf_ && x <= r.xt_ &&
            y >= r.yf_ && y <= r.yt_ &&
            z >= r.zf_ && z <= r.zt_;
}

auto get_range(auto const& in)
{
   region r{ std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min(),
               std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min(), 
               std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min()};
   for (auto& c : in)
   {
       auto& ri{ c.second };
       if (ri.xf_ < r.xf_)
           r.xf_ = ri.xf_;
       if (ri.xt_ > r.xt_)
           r.xt_ = ri.xt_;
       if (ri.yf_ < r.yf_)
           r.yf_ = ri.yf_;
       if (ri.yt_ > r.yt_)
           r.yt_ = ri.yt_;
       if (ri.zf_ < r.zf_)
           r.zf_ = ri.zf_;
       if (ri.zt_ > r.zt_)
           r.zt_ = ri.zt_;
   }
   return r;
}

// not good for the full set...
int64_t pt2_2(auto const& in)
{
    auto rng = get_range(in);
    std::cout << rng << "\n";
    auto result{ 0LL };
    // for each x, y, z, find all cuboids containing it, add 1 to result if last cuboid is 'set'. 
    for (auto x = rng.xf_; x <= rng.xt_; ++x)
    {
        for (auto y = rng.yf_; y <= rng.yt_; ++y)
        {
            for (auto z = rng.zf_; z <= rng.zt_; ++z)
            {
                int s{ 0 };
                for (auto& c : in)
                {
                    if (in_region(x, y, z, c.second))
                        s = c.first ? 1 : 0;
                }
                result += s;
            }
        }
    }
    return result;
}

template <typename FF, typename FT> std::vector<std::pair<int64_t, int64_t>> rangesx(auto const& in, FF ff, FT ft)
{
    std::vector<std::pair<int64_t, int64_t>> vrt;
    for (auto& c : in)
        vrt.emplace_back(ff(c.second), ft(c.second));
    std::sort(vrt.begin(), vrt.end(), [](auto& l, auto& r) { return l.first < r.first; });
    std::vector<std::pair<int64_t, int64_t>> vr;
    auto li = vrt.begin();
    auto b = (*li).first;
    auto e = (*li).second;
    while (li != vrt.end())
    {
        if ((*li).first < e)
        {
            vr.emplace_back(b, e);
            b = (*li).first;
            e = (*li).second;
        }
        else
        if ((*li).second > e)
            e = (*li).second;
        ++li;
    }
    vr.emplace_back(b, e);
    return vr;
}

template <typename FF, typename FT> std::vector<std::pair<int64_t, int64_t>> ranges(auto const& in, FF ff, FT ft)
{
    std::vector<std::pair<int64_t, int64_t>> vrt;
    for (auto& c : in)
        vrt.emplace_back(ff(c.second), ft(c.second));
    std::sort(vrt.begin(), vrt.end(), [](auto& l, auto& r) { return l.first < r.first; });
    return vrt;
}

int64_t pt2_3(auto const& in)
{
    auto rx = ranges(in, [](auto& p) { return p.xf_; }, [](auto& p) { return p.xt_; });
    auto ry = ranges(in, [](auto& p) { return p.yf_; }, [](auto& p) { return p.yt_; });
    auto rz = ranges(in, [](auto& p) { return p.zf_; }, [](auto& p) { return p.zt_; });
    std::cout << "in size = " << in.size() << ", rx size = " << rx.size() << "\n";
    std::cout << "in size = " << in.size() << ", ry size = " << ry.size() << "\n";
    std::cout << "in size = " << in.size() << ", rz size = " << rz.size() << "\n";

    auto result{ 0LL };
    for (auto& rxx : rx)
    {
        for (auto x = rxx.first; x <= rxx.second; ++x)
        {
            for (auto ryy : ry)
            {
                for (auto y = ryy.first; y <= ryy.second; ++y)
                {
                    for (auto rzz : rz)
                    {
                        for (auto z = rzz.first; z <= rzz.second; ++z)
                        {
                            int s{ 0 };
                            for (auto& c : in)
                            {
                                if (in_region(x, y, z, c.second))
                                    s = c.first ? 1 : 0;
                            }
                            result += s;
                        }
                    }
                }
            }
        }
    }
    return result;
}
#endif
int main()
{
    auto in { get_input()};
    std::cout << "pt1  = " << pt1(in) << "\n";
    std::cout << "pt1a = " << pt2(inner_regions(in)) << "\n";
    std::cout << "pt2  = " << pt2(in) << "\n";
//    std::cout << "pt23 = " << pt2_3(in) << "\n";
}