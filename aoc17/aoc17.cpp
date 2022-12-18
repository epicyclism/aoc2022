#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

#include <experimental/mdspan>

namespace stdex = std::experimental;

auto get_input()
{
    std::string ln;
    std::getline(std::cin, ln);
    return ln;
}

enum item_t { minus, plus, ell, pipe, square};

struct pt_t
{
    int x_;
    int y_;
};

constexpr pt_t pts_minus[] = {{ 0, 0}, {1, 0}, {2, 0}, {3, 0}};
constexpr pt_t pts_plus []= {{ 1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};
constexpr pt_t pts_ell []{{ 0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
constexpr pt_t pts_pipe []{{ 0, 0}, {0, 1}, {0, 2}, {0, 3}};
constexpr pt_t pts_square []{{ 0, 0}, {1, 0}, {0, 1}, {1, 1}};

item_t next_item(item_t i)
{
    if( i == square)
        return minus;
    return item_t((int)i + 1);
}

struct item_pos_t
{
    int pos_;
    item_t itm_;
};

constexpr int width {7};
constexpr int height{ 2022 * 4};

int item_height(item_t i)
{
    switch(i)
    {
        case minus:
            return 1;
        case plus:
            return 3;
        case ell:
            return 3;
        case pipe:
            return 4;
        case square:
            return 2;
    }
    std::cout << "Bug item_height(" << i << ")\n";
    return 0;
}

int item_width(item_t i)
{
    switch(i)
    {
        case minus:
            return 4;
        case plus:
            return 3;
        case ell:
            return 3;
        case pipe:
            return 1;
        case square:
            return 2;
    }
    std::cout << "Bug item_width(" << i << ")\n";
    return 0;
}

item_pos_t shift_item(char c, item_pos_t i)
{
    if( c == '<')
    {
         if( i.pos_ > 0)
            --i.pos_;
    }
    else
    {
        if( i.pos_ + item_width(i.itm_) < width)
            ++i.pos_;
    }
    return i;
}

struct blower_t
{
    std::string const& ln_;
    int pos_ ;
    blower_t() = delete;
    blower_t(std::string const& s) : ln_{s}, pos_ {0}
    {}
    char get_blow()
    {
        char rc { ln_[pos_]};
        ++pos_;
        if( pos_ == ln_.size())
            pos_ = 0;
        return rc;
    }
    char peek_blow() const
    {
        return ln_[pos_];
    }
};

//using chamber_t_base = std::array<char, height * width>;
using chamber_t_base = std::vector<char>;

bool can_fit(int x, int y, item_t i, auto& tower)
{
    if( y < 0)
        return false;
    bool rv { true};
    switch(i)
    {
        case minus:
            std::for_each(std::begin(pts_minus), std::end(pts_minus), [&](auto p){if ( '.' != tower(x + p.x_, y + p.y_ )) rv = false ;});
            break;
        case plus:
            std::for_each(std::begin(pts_plus), std::end(pts_plus), [&](auto p){if(tower(x + p.x_, y + p.y_ ) != '.') rv = false;});
            break;
        case ell:
            std::for_each(std::begin(pts_ell), std::end(pts_ell), [&](auto p){if(tower(x + p.x_, y + p.y_ ) != '.') rv = false;});
            break;
        case pipe:
            std::for_each(std::begin(pts_pipe), std::end(pts_pipe), [&](auto p){if(tower(x + p.x_, y + p.y_ ) != '.') rv = false;});
            break;
        case square:
            std::for_each(std::begin(pts_square), std::end(pts_square), [&](auto p){if(tower(x + p.x_, y + p.y_ ) != '.') rv = false;});
            break;
    }
    return rv;
}

void draw_item(int x, int y, item_t i, auto& tower)
{
    switch(i)
    {
        case minus:
            std::for_each(std::begin(pts_minus), std::end(pts_minus), [&](auto p){tower(x + p.x_, y + p.y_ ) = '#';});
            break;
        case plus:
            std::for_each(std::begin(pts_plus), std::end(pts_plus), [&](auto p){tower(x + p.x_, y + p.y_ ) = '#';});
            break;
        case ell:
            std::for_each(std::begin(pts_ell), std::end(pts_ell), [&](auto p){tower(x + p.x_, y + p.y_ ) = '#';});
            break;
        case pipe:
            std::for_each(std::begin(pts_pipe), std::end(pts_pipe), [&](auto p){tower(x + p.x_, y + p.y_ ) = '#';});
            break;
        case square:
            std::for_each(std::begin(pts_square), std::end(pts_square), [&](auto p){tower(x + p.x_, y + p.y_ ) = '#';});
            break;
        default:
            std::cout << "BUG draw item, item type (" << i << ")\n";
            break;
    }
}

void print(int ht, auto& chm)
{
    for(int r = ht; r >= 0; --r)
    {
        std::cout << "|";
        for( int c = 0; c < width; ++c)
            std::cout << chm(c, r) ;
        std::cout << "|\n";
    }
    std::cout << "---------\n";
}

item_pos_t get_next_item(item_t i)
{
    return item_pos_t(2, next_item(i));
}

auto pt0(auto const& in)
{
    chamber_t_base chm(height * width, '.');
    stdex::mdspan chamber(chm.data(), width, height);
    blower_t blower{in};
    item_pos_t item{2, minus};
    item = shift_item(blower.get_blow(), item);
    draw_item(item.pos_, 0, item.itm_, chamber);
    print(2, chamber);
    return 0;
}

auto pt1(auto const& in)
{
    chamber_t_base chm(height * width, '.');
    stdex::mdspan chamber(chm.data(), width, height);
    blower_t blower{in};
    item_pos_t item{2, minus};
    int ht { 0 };
    int draw { 0};
    for(int rock_count{0}; rock_count < 2022; ++rock_count)
    {
        for(int n = 0; n < 3; ++n)
            item = shift_item(blower.get_blow(), item);
        draw = ht;
        if( ht > 0)
        {
            while(1)
            {
                auto item2 = shift_item(blower.peek_blow(), item);
                if( can_fit(item2.pos_, draw, item2.itm_, chamber))
                    item = item2;
                blower.get_blow();
                if( can_fit(item.pos_, draw - 1, item.itm_, chamber))
                    --draw;
                else
                    break;
           }
        }
        else
            item = shift_item(blower.get_blow(), item);
        draw_item(item.pos_, draw, item.itm_, chamber);
        if( draw + item_height(item.itm_) > ht)
            ht = draw + item_height(item.itm_);
        item = get_next_item(item.itm_);
    }

    return ht;
}

// 1531919532262 too low
// 1532183908048 just right...
auto pt2(auto const& in)
{
    auto cnk {std::lcm(in.size(), 5)};
    auto part { 1000000000000LL % (348 * cnk)};
    auto cnt { 1000000000000LL / (348 * cnk)};
    int64_t part_result { 0 };
    std::cout << cnk << ", " << cnt << ", " << part << "\n"; 

    auto height2 {cnk * 4000};
    chamber_t_base chm(height2 * width, '.');
    stdex::mdspan chamber(chm.data(), width, height2);
    blower_t blower{in};
    item_pos_t item{2, minus};
    int64_t ht { 0 };
    int64_t last_ht { 0};
    int64_t draw { 0 };
    std::vector<int64_t> history;
    for(int rock_count{0}; rock_count < cnk * 2000; ++rock_count)
    {
        if (rock_count == part)
            part_result = ht;
        for(int n = 0; n < 3; ++n)
            item = shift_item(blower.get_blow(), item);
        draw = ht;
        if( ht > 0)
        {
            while(1)
            {
                auto item2 = shift_item(blower.peek_blow(), item);
                if( can_fit(item2.pos_, draw, item2.itm_, chamber))
                    item = item2;
                blower.get_blow();
                if( can_fit(item.pos_, draw - 1, item.itm_, chamber))
                    --draw;
                else
                    break;
           }
        }
        else
            item = shift_item(blower.get_blow(), item);
        draw_item(item.pos_, draw, item.itm_, chamber);
        if( draw + item_height(item.itm_) > ht)
            ht = draw + item_height(item.itm_);
        item = get_next_item(item.itm_);
        if(rock_count % cnk == 0)
        {
            history.push_back(ht - last_ht);
            last_ht = ht;
        }
    }
#if 1
    for( int n = 1; n < history.size() / 3; ++n)
    {
        auto it = history.begin() + 1;
        std::cout << n << " : ";
        int64_t lst { 0 };
        while( std::distance(it, history.end()) > n)
        {
            auto itN = it + n ;
            auto sm = std::accumulate(it, itN, 0LL) ;
            std::cout << sm  << " ";
            if( lst == sm ) 
                std::cout << " ! ";
            lst = sm ;
            it += n;
        }
        std::cout << "\n";
    }
#endif
//    return cnt * 424 + part_result; // test
    return cnt * 26902606LL + part_result;
}

// 12613750     
// 1000000000000
// 1514285714288

int main()
{
    auto in {get_input()};
//    std::cout << "pt0 = " << pt0(in) << "\n";
    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}