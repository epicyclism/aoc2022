#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <future>

#include <ctre_inc.h>
#include "graph.h"

struct edge_t
{
    vertex_id_t to_;
    int flow_;
};

using graph_t = std::vector<std::vector<edge_t>>;

vertex_id_t vertex_id_from_name(std::string_view nm)
{
	static std::map<std::string, vertex_id_t> nm_vertex;
    auto v = nm_vertex.try_emplace(std::string{ nm }, static_cast<vertex_id_t>( nm_vertex.size()));
	return (*v.first).second;
}

int64_t get_bit(int n)
{
    return 1 << n;
}

auto get_input()
{
    int cnt{0};
    graph_t in;
    std::string ln;
    int64_t seen { 0};
    while(std::getline(std::cin, ln))
    {
        auto[m, id, rate, tu] = ctre::search<R"(Valve ([A-Z]+) has flow rate=(\d+); (tunnel leads to valve|tunnels lead to valves) )">(ln);
        if (!m)
        {
            std::cout << "failed to match " << ln << "\n";
            return std::pair(seen, in);
        }
        auto from_id { vertex_id_from_name(id)};
        auto flow_rate = sv_to_t<int>(rate);
        // add the loop
        if(flow_rate > 0)
        {
            ++cnt;
            seen |= get_bit(from_id);
            add_edge(in, from_id, from_id, flow_rate);
        }
        // find the connections
        std::string_view v { ln};
        v.remove_prefix(std::distance(ln.begin(), m.end())) ;
        for (auto i : ctre::range<R"(([A-Z]+))">(v))
        {
            add_edge(in, from_id, vertex_id_from_name(i), 0);
        }
    }
    std::cout << cnt << " active valves.\n";
    return std::pair(seen, in);
}

auto pt2(auto const& in)
{
    auto g { in.second};
    std::vector<std::pair<int, int64_t>> cache;
    auto src = vertex_id_from_name("AA");
    struct pth
    {
        vertex_id_t to_;
        int tm_;
        int flow_;
        vertex_id_t prev_;
        int64_t seen_;
    };
    std::queue<pth> q; 
    q.push({src, 26, 0, src, 0});
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        if (u.tm_ == 0)
        {
           cache.emplace_back(u.flow_, u.seen_);
        }
        else
        {
            --u.tm_;
            for (auto& v : g[u.to_])
            {   
                if( v.to_ != u.prev_)
                {
                    if( v.flow_ < 1)
                    {
                        pth p { u };
                        p.to_ = v.to_;
                        p.prev_ = u.to_;
                        q.push(p);
                    }
                    else
                    if( v.flow_ > 0 && !(u.seen_ & get_bit(v.to_)))
                    {
                            pth p { u };
                            p.to_ = v.to_;
                            p.flow_ += v.flow_ * p.tm_;
                            p.seen_ |= get_bit(v.to_);
                            p.prev_ = -1;
                            q.push(p);
                    }
                }
            }
        }
    }
    std::cout << cache.size() << " paths.\n";
#if 1
    auto thrds { std::thread::hardware_concurrency ()};
    std::cout << "Using " << thrds << " threads.\n";

    auto wkr = [&](size_t f, size_t t)
    {
        int mx { 1700 };
        for(size_t i = 0; i < cache.size() - 1; ++i)
            for(size_t j {f}; j < t; ++j)
            {
                if( !(cache[i].second & cache[j].second))
                    if( cache[i].first + cache[j].first > mx)
                    {
                        mx = cache[i].first + cache[j].first;
                        std::cout << mx << " (" << cache[i].first << ", " << cache[j].first << ")\n";
                    }
            }
        std::cout << "Max for range " << f << " to " << t << " = " << mx << "\n";
    };
    std::vector<std::thread> thds;
    size_t f { 0 };
    size_t inc { cache.size() / thrds};
    for( unsigned n = 0; n < thrds; ++n)
    {
        std::thread th {wkr, f, std::min(cache.size(), f + inc)};
        thds.push_back(std::move(th));
        f += inc;
    }
    std::for_each(thds.begin(), thds.end(), [](auto& t){ return t.join();});

    return 0;
#else
    int mx { 1700 };
    for(int i = 0; i < cache.size() - 1; ++i)
        for(int j {i+ 1}; j < cache.size(); ++j)
        {
            if( !(cache[i].second & cache[j].second))
                if( cache[i].first + cache[j].first > mx)
                {
                    mx = cache[i].first + cache[j].first;
                    std::cout << mx << " (" << cache[i].first << ", " << cache[j].first << ")\n";
                }
        }
    return mx;
#endif
}

int main()
{
    auto in {get_input()};

    std::cout << "pt2 = " << pt2(in) << "\n";
}