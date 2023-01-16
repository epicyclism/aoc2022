#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct pt_t
{
    int x_;
    int y_;
    friend auto operator<=>(pt_t const&, pt_t const&) = default;
};

auto get_input()
{
    std::map<pt_t, int> arena; // map of positions to ids

    int t {0};
    int id {0};
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        int l {0};
        for(auto c : ln)
        {
            if(c == '#')
            {
                ++id;
                arena[{l, t}] = id;
            }
            ++l;
        }
        --t;
    }
    return arena;
}

constexpr pt_t scans[] { {0, 1}, {1, 1}, {-1, 1},  // N NE NW
                        {0, -1}, {1, -1}, {-1, -1},// S SE SW
                        {-1, 0}, {-1, 1}, {-1, -1},// W, NW, SW
                        {1, 0}, {1, 1}, {1, -1}    // E, NE, SE
                        };

struct proposal
{
    int xf_;
    int yf_;
    int xt_;
    int yt_;
    bool set_;
    bool accepted_;
};

int calc_p1(auto const& in)
{
    int max_y{ -1000 };
    int min_y{ 1000 };
    int max_x{ -1000 };
    int min_x{ 1000 };
    for (auto& e : in)
    {
        auto pos{ e.first };
        if (pos.x_ > max_x)
            max_x = pos.x_;
        if (pos.x_ < min_x)
            min_x = pos.x_;
        if (pos.y_ > max_y)
            max_y = pos.y_;
        if (pos.y_ < min_y)
            min_y = pos.y_;
    }
    return (std::abs(max_x - min_x) + 1) * (std::abs(max_y - min_y) + 1) - in.size();
}

auto pt12(auto const& in)
{
    auto arena{ in };
    int scan_base{ 0 };
    int p1{ 0 };
    std::map<int, proposal> proposals;
    int round{ 0 };
    while(1)
    {
        proposals.clear();
        for (auto& e : arena)
        {
            auto pos{ e.first };
            int id = e.second;
            int neighbour_cnt{ 0 };
            int seg_cnt{ 0 };
            proposal prp{ -1, -1, false, false };
            for (int s = 0; s < 12; ++s)
            {
                int scan = (s + scan_base) % 12;
                if (arena.contains({ pos.x_ + scans[scan].x_, pos.y_ + scans[scan].y_ }))
                {
                    ++neighbour_cnt;
                    ++seg_cnt;
                }
                if ((scan % 3) == 2) // end of set
                {
                    if (!prp.set_ && seg_cnt == 0)
                    {
                        auto mv{ scans[scan - 2] };
                        prp.xf_ = pos.x_;
                        prp.yf_ = pos.y_;
                        prp.xt_ = pos.x_ + mv.x_;
                        prp.yt_ = pos.y_ + mv.y_;
                        prp.set_ = true;
                        prp.accepted_ = true;
                    }
                    seg_cnt = 0;
                }
            }
            // make the proposal.
            if (neighbour_cnt && prp.set_)
                proposals.insert({ id, prp });
        }
        // apply the proposals
        for (auto& e : arena)
        {
            // if this elf made a proposal
            if (proposals.contains(e.second))
            {
                auto& po = *proposals.find(e.second);
                // is that proposal unique?
                for (auto& p : proposals)
                {
                    if (p.first != po.first && p.second.xt_ == po.second.xt_ && p.second.yt_ == po.second.yt_)
                        po.second.accepted_ = false;
                }
            }
        }
        for (auto& p : proposals)
        {
            if (p.second.accepted_)
            {
                arena.erase(pt_t{ p.second.xf_, p.second.yf_ });
                arena[pt_t{ p.second.xt_, p.second.yt_ }] = p.first;
            }
        }
        scan_base += 3;
        scan_base %= 12;
        ++round;
        if (round == 10)
            p1 = calc_p1(arena);
        if (proposals.empty())
            break;
    }
    return std::pair{ p1, round };
}

int main()
{
    auto in {get_input()};

    auto p12 = pt12(in);
    std::cout << "pt1 = " << p12.first << "\n";
    std::cout << "pt2 = " << p12.second << "\n";
}