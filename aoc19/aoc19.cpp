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

struct bps_key
{
//    int time_ ;
    int n_o_r_ ; // number of ore robots
    int n_o_ ;   // amount of ore etc.
    int n_c_r_;
    int n_c_;
    int n_ob_r_;
    int n_ob_;
    int n_g_r_;
    bps_key(bp_state const& bps) :
//                                    time_{bps.time_},
                                    n_o_r_{bps.n_o_r_},
                                    n_o_{bps.n_o_},
                                    n_c_r_{bps.n_c_r_},
                                    n_c_{bps.n_c_}, 
                                    n_ob_r_{bps.n_ob_r_},
                                    n_ob_{bps.n_ob_},
                                    n_g_r_{bps.n_g_r_}
    {}
    friend auto operator<=>(bps_key const& l, bps_key const& r) = default;
};

std::ostream& operator<<(std::ostream& o, bp_state const& bps)
{
    o << "[" << bps.time_ << ", " << bps.n_o_r_ << ", " << bps.n_o_ << ", " << bps.n_c_r_ <<
        ", " << bps.n_c_ << ", " << bps.n_ob_r_ << ", " << bps.n_ob_ << ", " << bps.n_g_r_ << ", " << bps.n_g_ << "]";
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
        auto max_ore_r = std::max({bp.ore_, bp.clay_, bp.obs_ore_, bp.geode_ore_});
        auto max_clay_r = bp.obs_clay_;
        auto max_obs_r = bp.geode_obs_;

        bool build_ore { bps.n_o_r_ * bps.time_ + bps.n_o_ <= bps.time_ * max_ore_r};
        bool build_clay{ bps.n_c_r_ * bps.time_ + bps.n_c_ <= bps.time_ * max_clay_r};
        bool build_obs { bps.n_ob_r_ * bps.time_ + bps.n_ob_ <= bps.time_ * max_obs_r};

        bool do_nothing{ true };
        if (n_o_spend >= bp.geode_ore_ && n_ob_spend >= bp.geode_obs_)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_, bps.n_o_ + bps.n_o_r_ - bp.geode_ore_,
                                                bps.n_c_r_, bps.n_c_ + bps.n_c_r_,
                                                bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_ - bp.geode_obs_,
                                                bps.n_g_r_ + 1, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
            do_nothing = false;
        }
        if (n_o_spend >= bp.obs_ore_ && n_c_spend >= bp.obs_clay_ &&  build_obs)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_, bps.n_o_ + bps.n_o_r_ - bp.obs_ore_,
                                                bps.n_c_r_, bps.n_c_ + bps.n_c_r_ - bp.obs_clay_,
                                                bps.n_ob_r_ + 1 , bps.n_ob_ + bps.n_ob_r_,
                                                bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
            do_nothing = false;
        }
        if (n_o_spend >= bp.clay_ && build_clay)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_, bps.n_o_ + bps.n_o_r_ - bp.clay_,
                                                bps.n_c_r_ + 1, bps.n_c_ + bps.n_c_r_,
                                                bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_,
                                                bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
            do_nothing = false;
        }
        if (n_o_spend >= bp.ore_ && build_ore)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_ + 1, bps.n_o_ + bps.n_o_r_ - bp.ore_,
                                                bps.n_c_r_, bps.n_c_ + bps.n_c_r_,
                                                bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_,
                                                bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
            do_nothing = false;
        }
        if(do_nothing)
        {
            bp_state bps_n{ bps.time_ - 1, bps.n_o_r_ , bps.n_o_ + bps.n_o_r_,
                                        bps.n_c_r_, bps.n_c_ + bps.n_c_r_,
                                        bps.n_ob_r_ , bps.n_ob_ + bps.n_ob_r_,
                                        bps.n_g_r_, bps.n_g_ + bps.n_g_r_ };
            auto mx_this = step(bp, bps_n);
            if (mx_this > mx)
                mx = mx_this;
        }

        return mx;
    }
    return bps.n_g_;
}


int proc_for_geodes(bp_t const& bp)
{
    auto rv = step(bp, {});
    return rv;
}

// 1116 too low
// 1199 correct
auto pt1(auto const& in)
{
    int sm {0};
    for (int n{ 0 }; n < in.size(); ++n)
    {
        auto mx = proc_for_geodes(in[n]);
        std::cout << n << ", " << mx << "\n";
        sm += mx * (n + 1);
    }
    return sm;
}

int proc_for_geodes2(bp_t const& bp)
{
    auto rv = step(bp, {32});
    return rv;
}

// 2700 too low
// 3510 correct
auto pt2(auto const& in)
{
    int prd {1};
    for( int n = 0; n < std::min(in.size(), size_t(3)); ++n)
    {
        auto mx { proc_for_geodes2(in[n])};
        std::cout << n << ", " << mx << "\n";
        prd *= mx;
    }
    return prd;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}