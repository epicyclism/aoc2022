#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

#include <ctre_inc.h>

struct bp_t
{
    int no_;
    int ore_;
    int clay_;
    int obs_ore_;
    int obs_clay_;
    int geode_ore_;
    int geode_obs_;
};

auto get_input()
{
    std::vector<bp_t> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
        auto [m, n, o, c, oo, oc, gor, gob] = ctre::match<R"(Blueprint (\d+): Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.)">(ln);
        in.emplace_back(sv_to_t<int>(n), sv_to_t<int>(o), sv_to_t<int>(c), sv_to_t<int>(oo), sv_to_t<int>(oc), sv_to_t<int>(gor), sv_to_t<int>(gob));
    }

    return in;
}

struct bp_state
{
    int time_ = 24;
    int n_o_r_ = 1; // number of ore robots
    int n_o_ = 0;   // amount of ore etc.
    int n_c_r_ = 0;
    int n_c_ = 0;
    int n_ob_r_ = 0;
    int n_ob_ = 0;
    int n_g_r_ = 0;
    int n_g_ = 0;
    friend auto operator<=>(bp_state const& l, bp_state const& r) = default;
};

std::ostream& operator<<(std::ostream& o, bp_state const& bps)
{
    o << "[" << bps.time_ << ", " << bps.n_o_r_ << ", " << bps.n_o_ << ", " << bps.n_c_r_ <<
        ", " << bps.n_c_ << ", " << bps.n_ob_r_ << ", " << bps.n_ob_ << ", " << bps.n_g_r_ << ", " << bps.n_g_;
    return o;
}

int step(bp_t const& bp, bp_state bps)
{
    if (bps.time_)
    {
        int mx{ 0 };
        auto n_o_spend = bps.n_o_;
        auto n_c_spend = bps.n_c_;
        auto n_ob_spend = bps.n_ob_;

        bp_state bps_n{ bps.time_ - 1, bps.n_o_r_ , bps.n_o_ + bps.n_o_r_,
                                     bps.n_c_r_, bps.n_c_ + bps.n_c_r_,
                                     bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_,
                                     bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
        auto mx_this = step(bp, bps_n);
        if (mx_this > mx)
            mx = mx_this;

        if (n_o_spend > bp.ore_)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_ + 1, bps.n_o_ + bps.n_o_r_ - bp.ore_,
                                                 bps.n_c_r_, bps.n_c_ + bps.n_c_r_,
                                                 bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_,
                                                 bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
        }
        if (n_o_spend > bp.clay_)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_, bps.n_o_ + bps.n_o_r_ - bp.clay_,
                                                 bps.n_c_r_ + 1, bps.n_c_ + bps.n_c_r_,
                                                 bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_,
                                                 bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
        }
        if (n_o_spend > bp.obs_ore_ && n_c_spend > bp.obs_clay_)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_, bps.n_o_ + bps.n_o_r_ - bp.obs_ore_,
                                                bps.n_c_r_, bps.n_c_ + bps.n_c_r_ - bp.obs_clay_,
                                                bps.n_ob_r_ + 1 , bps.n_ob_ + bps.n_ob_r_,
                                                bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
        }
        if (n_o_spend > bp.geode_ore_ && n_ob_spend > bp.geode_obs_)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_, bps.n_o_ + bps.n_o_r_ - bp.geode_ore_,
                                                bps.n_c_r_, bps.n_c_ + bps.n_c_r_,
                                                bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_ - bp.geode_obs_,
                                                bps.n_g_r_ + 1, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
        }
        return mx;
    }
    if (bps.n_g_ > 0)
        std::cout << "got " << bps.n_g_ << " geodes\n";
    return bps.n_g_;
}


int proc_for_geodes(bp_t const& bp)
{
    auto rv = step(bp, {});
    return rv;
}

auto pt1(auto const& in)
{
    int best_bp{ -1 };
    int max_geodes{ -1 };
    for (int n{ 0 }; n < in.size(); ++n)
    {
        std::cout << "bp " << n << "\n";
        auto mx = proc_for_geodes(in[n]);
        if (mx > max_geodes)
        {
            best_bp = n + 1;
            max_geodes = mx;
        }
    }
    return best_bp * max_geodes;
}

auto pt2(auto const& in)
{
    return in.size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}