#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <bitset>

#include <ctre_inc.h>

template<typename T> struct pt
{
    T x_;
    T y_;
    friend auto operator<=>(const pt<T>&, const pt<T>&) = default;
};

using pt_t = pt<int>;

auto get_input()
{
    std::vector<std::pair<pt_t, pt_t>> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto [m, x1, y1, x2, y2] = ctre::match<R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))">(ln);
        in.push_back({ { sv_to_t<int>(x1), sv_to_t<int>(y1) }, { sv_to_t<int>(x2), sv_to_t<int>(y2) } });
    }
    return in;
}

int manhattan_distance(pt_t const& f, pt_t const& t)
{
    return std::abs(f.x_ - t.x_) + std::abs(f.y_ - t.y_);
}

template<int R> auto pt1(auto const& in)
{
    // for each sensor/beacon, mark every position of closer or equal distance to the sensor as the beacon
    std::set<int> coverage;
    for (auto& sb : in)
    {
        auto s{ sb.first };
        auto d{ manhattan_distance(sb.first, sb.second) };
        for (auto y = 0; y <= d; ++y)
        {
            if (s.y_ - y == R || s.y_ + y == R)
            {
                for (auto x = y - d; x <= d - y; ++x)
                {
                    coverage.insert({ s.x_ + x});
                }
            }
        }
    }
    for (auto& sb : in)
    {
        if (sb.first.y_ == R && coverage.contains(sb.first.x_))
            coverage.erase(sb.first.x_);
        if (sb.second.y_ == R && coverage.contains(sb.second.x_))
            coverage.erase(sb.second.x_);
    }
    return coverage.size();
}

struct sq_t
{
    pt_t tl_;
    pt_t tr_;
    pt_t bl_;
    pt_t br_;
};

std::ostream& operator<<(std::ostream& os, pt_t& p )
{
    os << "(" << p.x_ << ", " << p.y_ << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, sq_t& sq )
{
    os << "[ " << sq.tl_ << ", " << sq.tr_ << " : " << sq.bl_ << ", " << sq.br_ << " ]";
    return os;
}

bool is_inside(auto const& pt, auto const& sq)
{
    return (pt.x_ >= sq.tl_.x_ && pt.x_ <= sq.tr_.x_ && pt.y_ >= sq.tl_.y_ && pt.y_ <= sq.br_.y_);
}

pt_t rotate(pt_t p)
{
    return { (p.x_ - p.y_), (p.x_ + p.y_)};
}

pt_t rotate_back(auto p)
{
    return { (p.x_ + p.y_ )/ 2, (-p.x_ + p.y_) / 2};
}

sq_t rotate(sq_t sq)
{
    return { rotate(sq.tl_), rotate(sq.tr_), rotate(sq.bl_), rotate(sq.br_)};
}

auto pt2(auto const& in)
{
    std::vector<sq_t> sqv;
    for (auto& sb : in)
    {
        auto s{ sb.first };
        auto d{ manhattan_distance(sb.first, sb.second) };
        sq_t sq{ { s.x_, s.y_ + d}, { s.x_ + d, s.y_ }, { s.x_ - d, s.y_}, { s.x_, s.y_ - d } };
        sqv.emplace_back(rotate(sq));
    }

    std::vector<int> xp;
    std::vector<int> yp;
    for (auto& sqa : sqv)
    {
        xp.push_back(sqa.tl_.x_);
        xp.push_back(sqa.br_.x_);
        yp.push_back(sqa.tl_.y_);
        yp.push_back(sqa.br_.y_);
    }
    std::ranges::sort(xp);
    std::ranges::sort(yp);
    xp.erase(std::unique(xp.begin(), xp.end()), xp.end());
    yp.erase(std::unique(yp.begin(), yp.end()), yp.end());
    std::vector<int> xpp;
    std::vector<int> ypp;
    auto itb{ xp.begin() };
    auto itn{ itb + 1 };
    while (itn != xp.end())
    {
        if (*itn - *itb < 3)
            xpp.push_back(*itb + 1);
        ++itb, ++itn;
    }
    itb =  yp.begin();
    itn =  itb + 1;
    while (itn != yp.end())
    {
        if (*itn - *itb < 3)
            ypp.push_back(*itb + 1);
        ++itb, ++itn;
    }
    for(auto x : xpp)
        for (auto y : ypp)
        {
            pt_t p{ x, y };
            bool seen{ false };
            for (auto& sq : sqv)
            {
                if (is_inside(p, sq))
                {
                    seen = true;
                    break;
                }
            }
            if (!seen)
            {
                auto pp{ rotate_back(p) };
                std::cout << pp << "\n";
                return int64_t(pp.x_) * 4000000LL + pp.y_;
            }
        }
    return -1LL;
}

int main()
{
    auto in {get_input()};
    std::cout << "v1\n";
//    std::cout << "pt1 = " << pt1<2000000>(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}