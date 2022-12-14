#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <ctre_inc.h>
#include <experimental/mdspan>

namespace stdex = std::experimental;

auto get_input()
{
    std::vector<std::vector<std::pair<int, int>>> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        std::vector<std::pair<int, int>> vt;
        for (auto [m, x, y] : ctre::range<R"((\d+),(\d+))">(ln))
            vt.emplace_back(sv_to_t<int>(x), sv_to_t<int>(y));
        in.push_back(std::move(vt));
        vt.clear(); // necessary?
    }

    return in;
}

auto get_y_max(auto& in)
{
    int ymax = 0;
    for (auto& lv : in)
        for (auto& p : lv)
            if (ymax < p.second)
                ymax = p.second;
    return ymax ;
}

constexpr int max_x = 1024;

void write_wall(auto& md, auto& wl)
{
    auto from = wl.front();
    std::for_each(wl.begin() + 1, wl.end(), [&](auto pt)
        {
            // vert?
            if (from.first == pt.first)
            {
                int inc = pt.second > from.second ? 1 : -1;
                while (from.second != pt.second)
                {
                    md(from.first, from.second) = '#';
                    from.second += inc;
                }
            }
            else
            {
                int inc = pt.first > from.first ? 1 : -1;
                while (from.first != pt.first)
                {
                    md(from.first, from.second) = '#';
                    from.first += inc;
                }
            }
            md(from.first, from.second) = '#';
            from = pt;
        });
}

void print(auto& md, int grain = 0)
{
    if(grain > 0)
        std::cout << "\nafter grain " << grain << "\n";
    for (int row = 0; row < 25; ++row)
    {
        for (int col = 475; col < 525; ++col)
            std::cout << md(col, row);
        std::cout << "\n";
    }
}

auto drop_sand_wkr(auto& cave, int my)
{
    int x = 500;
    int y = 0;
    while (y < my)
    {
        if (cave(x, y + 1) == '.')
            ++y;
        else
        if (cave(x - 1, y + 1) == '.')
            --x, ++y;
        else
        if (cave(x + 1, y + 1) == '.')
            ++x, ++y;
        else
            break;
    }
    return std::pair{ x, y };
}

bool drop_sand(auto& cave, int my)
{
    auto [x, y] = drop_sand_wkr(cave, my);
    if (y < my)
    {
        cave(x, y) = 'o';
        return true;
    }
    return false;
}

bool drop_sand(auto& cave)
{
    auto [x, y] = drop_sand_wkr(cave, 1000);
    cave(x, y) = 'o';
    return y > 0;
}

auto pt1(auto const& in)
{
    auto ymax = get_y_max(in) + 2;
    std::vector<char> cave_data(max_x * ymax, '.');
    stdex::mdspan cave(cave_data.data(), max_x, ymax);
    for (auto& wall : in)
        write_wall(cave, wall);
    int sand{ 0 };
    while (drop_sand(cave, ymax))
        ++sand;
    return sand;
}

auto pt2(auto const& in)
{
    auto ymax = get_y_max(in);
    std::vector<char> cave_data(max_x * (ymax + 3), '.');
    stdex::mdspan cave(cave_data.data(), max_x, (ymax + 3));
    for (auto& wall : in)
        write_wall(cave, wall);
    std::vector<std::pair<int, int>> inf;
    inf.push_back({ 0, ymax + 2 });
    inf.push_back({ max_x, ymax + 2 });
    write_wall(cave, inf);
    int sand{ 1 };
    while (drop_sand(cave))
        ++sand;
    return sand;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}