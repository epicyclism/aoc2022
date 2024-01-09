#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <ctre_inc.h>

#include "boost/graph/graph_utility.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/floyd_warshall_shortest.hpp"

using vertex_id_t = unsigned long;

namespace boost
{
    enum vertex_flow_t { vertex_flow };
    BOOST_INSTALL_PROPERTY(vertex, flow);
}

vertex_id_t vertex_id_from_name(std::string_view nm)
{
	static std::map<std::string, vertex_id_t> nm_vertex;
    auto v = nm_vertex.try_emplace(std::string{ nm }, static_cast<vertex_id_t>( nm_vertex.size()));
	return (*v.first).second;
}

using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::property<boost::vertex_flow_t, int>, boost::property<boost::edge_weight_t, int>>;

auto get_input()
{
    constexpr auto MAX_V { 64 };
    int D[MAX_V][MAX_V];
    using input_graph_t = boost::adjacency_list < boost::vecS, boost::vecS, boost::directedS> ;
    input_graph_t g;
    std::vector<std::pair<vertex_id_t, int>> vflow; // list of vertices that have flow, by id and flow rate
    std::string ln;

    while(std::getline(std::cin, ln))
    {
        auto[m, id, rate, tu] = ctre::search<R"(Valve ([A-Z]+) has flow rate=(\d+); (tunnel leads to valve|tunnels lead to valves) )">(ln);
        auto from_id { vertex_id_from_name(id)};
        if(auto flow_rate = sv_to_t<int>(rate); flow_rate)
            vflow.emplace_back(from_id, flow_rate);

        std::string_view v { m.end(), ln.end()};
        for (auto i : ctre::range<R"(([A-Z]+))">(v))
        {
            auto to_id { vertex_id_from_name(i)};
            auto ef = boost::add_edge(from_id, to_id, g);
        }
    }

    auto wm {boost::make_static_property_map<typename input_graph_t::edge_descriptor, int>(1)};
    boost::floyd_warshall_all_pairs_shortest_paths(g, D, boost::weight_map(wm));

    vflow.insert(vflow.begin(), { vertex_id_from_name("AA") , 0 });
    graph_t gr(vflow.size());
    for(auto f = 0; f < vflow.size() - 1; ++f)
        for(auto t = f + 1; t < vflow.size(); ++t)
        {
            auto d = D[vflow[f].first][vflow[t].first];
            auto ed = boost::add_edge(f, t, d, gr);
        }
    auto pm_v = boost::get(boost::vertex_flow_t(),gr);
    boost::put(pm_v, 0, 0);
    for(auto f = 0; f < vflow.size(); ++f)
        boost::put(pm_v, f, vflow[f].second);

    return gr;
}

bool is_visited(int visited, vertex_id_t v)
{
    return visited & ( 1 << v);
}

int mark_visited(int visited, vertex_id_t v)
{
    return visited | (1 << v);
}

template<int N, typename F> void dfs_visit(graph_t const& g, vertex_id_t v, int time, int visited, int flow, F f)
{
    flow += boost::get(boost::vertex_flow_t(), g, v) * (N - time);
    for(auto[e, ee] = boost::out_edges(v, g); e != ee; ++e)
    {
        auto d = boost::get(boost::edge_weight_t(), g, *e);
        auto vt = boost::target(*e, g);
        if( time + d <= N)
        {
            if(!is_visited(visited, vt))
                dfs_visit<N>(g, vt, time + d + 1, mark_visited(visited, vt), flow, f);
        }
        else
            f(flow, visited);
    }
}

auto pt1(auto const& g)
{
    int mx { 0 };
    dfs_visit<30>(g, 0, 0, 0, 0, [&](int f, int){ if( f > mx) mx = f;});
    return mx;
}

auto pt2(auto const& g)
{
    std::map<int, int> mxs;
    dfs_visit<26>(g, 0, 0, 0, 0, [&](int f, int v){ if(mxs[v] < f) mxs[v] = f;});
    std::vector<std::pair<int, int>> cache;
    for(auto const& mm: mxs)
        cache.emplace_back(mm.second, mm.first);
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

void print(graph_t& g)
{
    for (auto [v, vv] = boost::vertices(g); v != vv; ++v)
    {
        std::cout << *v << " (" << boost::get(boost::vertex_flow_t(), g, *v) << ") : ";
        for (auto [e, ee] = boost::out_edges(*v, g); e != ee; ++e)
        {
            auto d = boost::get(boost::edge_weight_t(), g, *e);
            auto vt = boost::target(*e, g);
            std::cout << vt << " (" << d << "), ";
        }
        std::cout << "\n";
    }
}

int main()
{
    auto g {get_input()};
    print(g);
    std::cout << "pt1 = " << pt1(g) << "\n";
    std::cout << "pt2 = " << pt2(g) << "\n";
}