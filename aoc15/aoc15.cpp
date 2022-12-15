#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <bitset>
#include <numbers>

#include <ctre_inc.h>

using namespace std::numbers;

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

void print(auto const& in)
{
    for (auto& sb : in)
        std::cout << sb.first.x_ << ", " << sb.first.y_ << " : " << sb.second.x_ << ", " << sb.second.y_ << "\n";
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

auto pt2_20(auto const& in)
{
    std::bitset<21 * 21> bs;
    for (auto& sb : in)
    {
        auto s{ sb.first };
        auto d{ manhattan_distance(sb.first, sb.second) };
        for (auto y = 0; y <= d; ++y)
        {
            for (auto x = y - d; x <= d - y; ++x)
            {
                auto cx = s.x_ + x;
                auto cy = s.y_ - y;
                auto cy2 = s.y_ + y;
                if (cx >= 0 && cx <= 20 && cy >= 0 && cy <= 20)
                    bs.set(cy * 20 + cx);
                if (cx >= 0 && cx <= 20 && cy2 >= 0 && cy2 <= 20)
                    bs.set(cy2 * 20 + cx);
            }
        }
    }
    for (auto x = 0; x < 21; ++x)
        for (auto y = 0; y < 21; ++y)
            if (!bs.test(y * 20 + x))
                return int64_t(x) * 4000000LL + y;

    return 0LL;
}

template<typename T> struct sq
{
    pt<T> tl_;
    pt<T> tr_;
    pt<T> bl_;
    pt<T> br_;
};

using sq_t = sq<int>;

template<typename T>
std::ostream& operator<<(std::ostream& os, sq<T>& sq )
{
    os << "[ " << sq.tl_ << ", " << sq.tr_ << " : " << sq.bl_ << ", " << sq.br_ << " ]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, pt<T>& p )
{
    os << "(" << p.x_ << ", " << p.y_ << ")";
    return os;
}

bool is_inside(auto const& pt, auto const& sq)
{
    return (pt.x_ >= sq.tl_.x_ && pt.x_ <= sq.tr_.x_ && pt.y_ >= sq.tl_.y_ && pt.y_ <= sq.br_.y_);
}
template<typename T, typename U> pt<T> rotate(pt<U> p)
{
    return { (p.x_ - p.y_) / sqrt2, (p.x_ + p.y_) / sqrt2 };
}

pt_t rotate_back(auto p)
{
    return { int((p.x_ + p.y_ + 0.5)/ sqrt2), int(( - p.x_ + p.y_ + 0.5) / sqrt2)};
}

sq<double> rotate(sq_t sq)
{
    return { rotate<double>(sq.tl_), rotate<double>(sq.tr_), rotate<double>(sq.bl_), rotate<double>(sq.br_)};
}

auto pt2_4m(auto const& in)
{
    std::vector<sq<double>> sqv;
    for (auto& sb : in)
    {
        auto s{ sb.first };
        auto d{ manhattan_distance(sb.first, sb.second) };
        sq_t sq{ { s.x_, s.y_ + d}, { s.x_ + d, s.y_ }, { s.x_ - d, s.y_}, { s.x_, s.y_ - d } };
        sqv.emplace_back(rotate(sq));
    }

    std::vector<double> xp;
    std::vector<double> yp;
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
    std::vector<double> xpp;
    std::vector<double> ypp;
    auto itb{ xp.begin() };
    auto itn{ itb + 1 };
    while (itn != xp.end())
    {
        if (*itn - *itb < 1.42)
            xpp.push_back(*itb + sqrt2/2.0);
        ++itb, ++itn;
    }
    itb =  yp.begin();
    itn =  itb + 1;
    while (itn != yp.end())
    {
        if (*itn - *itb < 1.42)
            ypp.push_back(*itb + sqrt2/2.0);
        ++itb, ++itn;
    }
    for(auto x : xpp)
        for (auto y : ypp)
        {
            pt<double> p{ x, y };
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

    if (in.size() == 14)
    {
        std::cout << "pt1 = " << pt1<10>(in) << "\n";
        std::cout << "pt2 = " << pt2_20(in) << "\n";
    }
    else
    {
//        std::cout << "pt1 = " << pt1<2000000>(in) << "\n";
        std::cout << "pt2 = " << pt2_4m(in) << "\n";
    }
}