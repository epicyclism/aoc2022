#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

auto get_input()
{
    std::vector<char> data;
    size_t stride { 0 };
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        if(stride == 0)
            stride = ln.size();
        std::transform(ln.begin(), ln.end(), std::back_inserter(data), [](auto c){return c - '0';});
    }
    return std::pair(data, stride);
}

using uc = unsigned char;
using seen_set = std::set<uint16_t>;

bool seen(uc r, uc c, seen_set& ss)
{
    uint16_t v { r << 8 | c};
    if(ss.contains(v))
        return true;
    ss.insert(v);

    return false;
}

auto pt1(auto const& in)
{
    auto stride { in.second};
    auto& grid { in.first};
    seen_set ss;
    int cnt { 0 };
    // rows
    for(uc r = 0; r < stride; ++r)
    {
        char max_so_far { -1 };
        for(uc c = 0; c < stride; ++c)
        {
            auto val {grid[r*stride + c]};
            if(max_so_far < val && !seen(r, c, ss))
                ++cnt;
            if(val > max_so_far)
                max_so_far = val;
        }
    }
    // rows reverse
    for(uc r = 0; r < stride; ++r)
    {
        char max_so_far { -1 };    
        for(uc c = 0; c < stride; ++c)
        {
            uc col = stride - c - 1;
            auto val {grid[r*stride + col]};
            if(max_so_far < val  && !seen(r, col, ss))
                ++cnt;
            if(val > max_so_far)
                max_so_far = val;
        }
    }
    // cols
    for(uc c = 0; c < stride; ++c)
    {
        char max_so_far { -1 };
        for(uc r = 0; r < stride; ++r)
        {
            auto val {grid[r*stride + c]};
            if(max_so_far < val && !seen(r, c, ss))
                ++cnt;
            if(val > max_so_far)
                max_so_far = val;
        }
    }
    // cols reverse
    for(uc c = 0; c < stride; ++c)
    {
        char max_so_far { -1 };
        for(uc r = 0; r < stride; ++r)
        {
            uc row = stride - r - 1;
            auto val {grid[row*stride + c]};
            if(max_so_far < val  && !seen(row, c, ss))
                ++cnt;
            if(val > max_so_far)
                max_so_far = val;
        }
    }
    return cnt;
}

int view_dist(auto& grid, auto stride, auto rr, auto cc)
{
    auto t { grid[rr*stride + cc]};
    // left
    int l {0};
    for(auto c = cc; c != 0; --c)
    {
        auto cm = c - 1;
        ++l;
        if(grid[rr*stride + cm] >= t)
            break;

    }
    if(l == 0)
        l = 1;
    // right
    int r {0};
    for(auto c = cc + 1; c < stride; ++c)
    {
        ++r;        
        if(grid[rr*stride + c] >= t)
            break;
    }
    if(r == 0)
        r = 1;
    // up
    int u{0};
    for(auto r = rr; r != 0; --r)
    {
        ++u;
        auto rm = r - 1;
        if(grid[rm*stride + cc] >= t)
            break;
    }
    if(u == 0)
        u = 1;
    // down
    int d{0};
    for(auto rt = rr + 1; rt < stride; ++rt)
    {
        ++d;
        if(grid[rt*stride + cc] >= t)
            break;
    }
    if(d == 0)
        d = 1;

    return u*d*l*r;
}

auto pt2(auto const& in)
{
    auto stride { in.second};
    auto& grid { in.first};

    int max_vd {0};
    for(uc c = 1; c < stride - 1; ++c)
        for(uc r = 1; r < stride - 1; ++r)
        {
            auto vd {view_dist(grid, stride, r, c)};
            if(vd > max_vd)
                max_vd = vd;
        }
    return max_vd;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}