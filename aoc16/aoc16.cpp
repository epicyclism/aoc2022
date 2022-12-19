#include <iostream>
#include <iomanip>

#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <ctre_inc.h>
#include "graph.h"

struct vertex_t
{
    std::vector<vertex_id_t> adj_;
    int flow_;
    // forward
    auto operator[](vertex_id_t v)
    {
        return adj_[v];
    }
    auto begin() const
    {
        return adj_.begin();
    }
    auto end() const
    {
        return adj_.end();
    }
};

using graph_t = std::vector<vertex_t>;

void add_vertex(graph_t& g, vertex_id_t v)
{
    if( g.size() < v + 1)
        g.resize(v + 1);
}

void set_flow(graph_t& g, vertex_id_t v, int flow)
{
    g[v].flow_ = flow;
}

void add_edge(graph_t& g, vertex_id_t f, vertex_id_t t)
{
    g[t].adj_.push_back(f);
}

vertex_id_t vertex_id_from_name(std::string_view nm)
{
	static std::map<std::string, vertex_id_t> nm_vertex;
    auto v = nm_vertex.try_emplace(std::string{ nm }, static_cast<vertex_id_t>( nm_vertex.size()));
	return (*v.first).second;
}

int64_t get_bit(int n)
{
    return 1LL << n;
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
            return in;
        }
        auto from_id { vertex_id_from_name(id)};
        auto flow_rate = sv_to_t<int>(rate);
        if(flow_rate > 0)
            ++cnt;
        add_vertex(in, from_id);
        set_flow(in, from_id, flow_rate);
        // find the connections
        std::string_view v { ln};
        v.remove_prefix(std::distance(ln.begin(), m.end())) ;
        for (auto i : ctre::range<R"(([A-Z]+))">(v))
        {
            auto to_id { vertex_id_from_name(i)};
            add_vertex(in, to_id);
            add_edge(in, from_id, to_id);
        }
    }
    return in;
}

std::vector<std::vector<int>> valve_valve_distances(auto const& g)
{
    std::vector<std::vector<int>> rv;
    for(vertex_id_t v = 0; v < g.size(); ++v)
    {
        auto ds {bfs(g, v) };
        rv.push_back(ds);
    }
    return rv;
}

auto pt1(auto const& g)
{
    auto dd { valve_valve_distances(g)};
    struct pth
    {
        vertex_id_t to_;
        int tm_;
        int flow_;
        int64_t seen_;
    };
    int mx{0};
    std::queue<pth> q;
    q.push({vertex_id_from_name("AA"), 30, 0, 0});
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        if(u.flow_ > mx)
            mx = u.flow_;
        for( vertex_id_t to = 0; to < g.size(); ++to)
        {
            if( to == u.to_)
                continue;
            auto ttv { dd[u.to_][to]};
            if(ttv < 1)
                continue;
            ++ttv;
            auto flw { g[to].flow_};
            if(flw && (ttv < u.tm_) && !(u.seen_ & get_bit(to)))
            {
                q.push({to, u.tm_ - ttv, u.flow_ + (u.tm_ - ttv) * flw, u.seen_ | get_bit(to) });
            }
        }
    }
    return mx;
}

auto pt2(auto const& g)
{
    auto dd { valve_valve_distances(g)};
    struct pth
    {
        vertex_id_t to_;
        int tm_;
        int flow_;
        int64_t seen_;
    };
    std::map<int64_t, int> mxs;
    std::queue<pth> q;
    q.push({vertex_id_from_name("AA"), 26, 0, 0});
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        if(u.flow_ > mxs[u.seen_])
            mxs[u.seen_] = u.flow_;
        for( vertex_id_t to = 0; to < g.size(); ++to)
        {
            if( to == u.to_)
                continue;
            auto ttv { dd[u.to_][to] + 1};
            auto flw { g[to].flow_};
            if(flw && (ttv < u.tm_) && !(u.seen_ & get_bit(to)))
            {
                q.push({to, u.tm_ - ttv, u.flow_ + (u.tm_ - ttv) * flw, u.seen_ | get_bit(to) });
            }
        }
    }
    std::vector<std::pair<int, int64_t>> cache;
    for(auto const& mm: mxs)
        cache.emplace_back(mm.second, mm.first);
    // now cross search
    int mx { 0 };
    for(int i = 0; i < cache.size() - 1; ++i)
        for(int j = i + 1; j < cache.size(); ++j)
        {
            if( !(cache[i].second & cache[j].second))
                if( cache[i].first + cache[j].first > mx)
                {
                    mx = cache[i].first + cache[j].first;
                }
        }
    return mx;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}