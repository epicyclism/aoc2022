#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

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

int step0(bp_t const& bp, bp_state bps, std::map<bp_state, int>& cache)
{
    // monitor production
    bps.n_o_ += bps.n_o_r_;
    bps.n_c_ += bps.n_c_r_;
    bps.n_ob_ += bps.n_ob_r_;
    bps.n_g_ += bps.n_g_r_;
    // produce robots and recurse
    if (bps.time_)
    {
        int mx{ 0 };
        for (int n_o_spend = 0; n_o_spend <= bps.n_o_ - bps.n_o_r_; ++n_o_spend)
        {
            for (int n_c_spend = 0; n_c_spend <= bps.n_c_ - bps.n_c_r_; ++n_c_spend)
            {
                for (int n_ob_spend = 0; n_ob_spend <= bps.n_ob_ - bps.n_ob_r_; ++n_ob_spend)
                {
                    for (int n_o_to_buy = 0; n_o_to_buy <= n_o_spend / bp.ore_; ++n_o_to_buy)
                    {
                        auto n_o_spend2 = n_o_spend - n_o_to_buy * bp.ore_;
                        for (int n_c_to_buy = 0; n_c_to_buy <= n_o_spend2 / bp.clay_; ++n_c_to_buy)
                        {
                            auto n_o_spend3 = n_o_spend2 - n_c_to_buy * bp.clay_;
                            auto max_n_ob_to_buy = std::min(n_o_spend3 / bp.obs_ore_, n_c_spend / bp.obs_clay_);
                            for (int n_ob_to_buy = 0; n_ob_to_buy <= max_n_ob_to_buy; ++n_ob_to_buy)
                            {
                                auto n_o_spend4 = n_o_spend3 - n_ob_to_buy * bp.obs_ore_;
                                auto n_c_spend2 = n_c_spend - n_ob_to_buy * bp.obs_clay_;
                                auto max_n_geo_to_buy = std::min(n_o_spend4 / bp.geode_ore_, n_ob_spend / bp.geode_obs_);
                                for (int n_geo_to_buy = 0; n_geo_to_buy <= max_n_geo_to_buy; ++n_geo_to_buy)
                                {
                                    bp_state bp_s{ bps.time_ -1, bps.n_o_r_ + n_o_to_buy, bps.n_o_ - n_o_spend4 - n_geo_to_buy * bp.geode_ore_,
                                                                        bps.n_c_r_ + n_c_to_buy, bps.n_c_ - n_c_spend2,
                                                                        bps.n_ob_r_ + n_ob_to_buy, bps.n_ob_ - n_geo_to_buy * bp.geode_obs_,
                                                                        bps.n_g_r_ + n_geo_to_buy, bps.n_g_};
#if 0
                                    if (cache.contains(bp_s))
                                    {
                                        int mxthis = cache[bp_s];
                                        if (mxthis < mx)
                                            mx = mxthis;
                                    }
                                    else
                                    {
#endif
                                        int mxthis = step0( bp, bp_s, cache);
                                        if (mxthis > mx)
                                            mx = mxthis;
//                                        cache[bp_s] = mxthis;
//                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
//        if (mx == 0)
//            mx = step(time - 1, bp, bps, cache);
        return mx;
    }
    if (bps.n_g_ > 0)
        std::cout << "got " << bps.n_g_ << " geodes.\n";
    return bps.n_g_;
}

int step(int time, bp_t const& bp, bp_state bps, std::map<bp_state, int>& cache)
{
//    std::cout << bps << "\n";
    // produce robots and recurse
    if (time)
    {
        int mx{ 0 };
        auto n_o_spend = bps.n_o_;
        auto n_c_spend = bps.n_c_;
        auto n_ob_spend = bps.n_ob_;
        for (int n_o_to_buy = 0; n_o_to_buy <= n_o_spend / bp.ore_; ++n_o_to_buy)
        {
            auto n_o_spent = n_o_to_buy * bp.ore_;
            for (int n_c_to_buy = 0; n_c_to_buy <= (n_o_spend - n_o_spent) / bp.clay_; ++n_c_to_buy)
            {
                n_o_spent += n_c_to_buy * bp.clay_;
                auto max_n_ob_to_buy = std::min((n_o_spend - n_o_spent) / bp.obs_ore_, n_c_spend / bp.obs_clay_);
                for (int n_ob_to_buy = 0; n_ob_to_buy <= max_n_ob_to_buy; ++n_ob_to_buy)
                {
                    n_o_spent += n_ob_to_buy * bp.obs_ore_;
                    auto n_c_spent = n_ob_to_buy * bp.obs_clay_;
                    auto max_n_geo_to_buy = std::min((n_o_spend - n_o_spent) / bp.geode_ore_, n_ob_spend / bp.geode_obs_);
                    for (int n_geo_to_buy = 0; n_geo_to_buy <= max_n_geo_to_buy; ++n_geo_to_buy)
                    {
                        n_o_spent += n_geo_to_buy * bp.geode_ore_;
                        auto n_ob_spent = n_geo_to_buy * bp.geode_obs_;
                        bp_state bp_s{ 24, bps.n_o_r_ + n_o_to_buy, bps.n_o_ + bps.n_o_r_ - n_o_spent,
                                                            bps.n_c_r_ + n_c_to_buy, bps.n_c_ + bps.n_c_r_ - n_c_spent,
                                                            bps.n_ob_r_ + n_ob_to_buy, bps.n_ob_ + bps.n_ob_r_ - n_ob_spent,
                                                            bps.n_g_r_ + n_geo_to_buy, bps.n_g_ + bps.n_g_r_ };
#if 0
                        if (cache.contains(bp_s))
                        {
                            int mxthis = cache[bp_s];
                            if (mxthis < mx)
                                mx = mxthis;
                        }
                        else
#endif
                        {
                            int mxthis = step(time - 1, bp, bp_s, cache);
                            if (mxthis > mx)
                                mx = mxthis;
//                            cache[bp_s] = mxthis;
                        }
                    }
                }
            }
        }
        return mx;
    }
//    if (bps.n_g_ > 0)
//        std::cout << "got " << bps.n_g_ << " geodes, " << bps.n_g_r_ << " geode miners\n";
    return bps.n_g_ ;
}

int proc_for_geodes0(bp_t const& bp)
{
    std::map<bp_state, int> cache;
    return step0(bp, {}, cache);
}

int proc_for_geodes(bp_t const& bp)
{
    std::map<bp_state, int> cache;
    auto rv = step(24, bp, {}, cache);
    auto rv2 = *std::max_element(cache.begin(), cache.end(), [](auto& l, auto& r) { return l.first.n_g_ < r.first.n_g_; });
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