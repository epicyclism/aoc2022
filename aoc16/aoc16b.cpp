#include <iostream>
#include <iomanip>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <tuple>

#include <ctre_inc.h>

#include "boost/graph/graph_utility.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/adjacency_matrix.hpp"
#include "boost/graph/floyd_warshall_shortest.hpp"

constexpr auto MAX_V { 64 };
using vertex_id_t = unsigned long;
using graph1_t = boost::adjacency_list < boost::vecS, boost::vecS, boost::directedS>;
namespace boost
{
    enum vertex_flow_t { vertex_flow };
    BOOST_INSTALL_PROPERTY(vertex, flow);
}
using graph_t = boost::adjacency_matrix<boost::undirectedS,
    boost::property<boost::vertex_flow_t, int>, boost::property<boost::edge_weight_t, int>>;

vertex_id_t vertex_id_from_name(std::string_view nm)
{
	static std::map<std::string, vertex_id_t> nm_vertex;
    auto v = nm_vertex.try_emplace(std::string{ nm }, static_cast<vertex_id_t>( nm_vertex.size()));
	return (*v.first).second;
}

int get_bit(int n)
{
    return 1 << n;
}

auto get_input()
{
    int D[MAX_V][MAX_V];
    graph1_t g;
    std::vector<std::pair<vertex_id_t, int>> vflow; // list of vertices that have flow, by id and flow rate
    std::string ln;

    while(std::getline(std::cin, ln))
    {
        auto[m, id, rate, tu] = ctre::search<R"(Valve ([A-Z]+) has flow rate=(\d+); (tunnel leads to valve|tunnels lead to valves) )">(ln);
        auto from_id { vertex_id_from_name(id)};
        if(auto flow_rate = sv_to_t<int>(rate); flow_rate)
            vflow.emplace_back(from_id, flow_rate);
        std::string_view v { ln };
        v.remove_prefix(std::distance(ln.begin(), m.end())) ;
        for (auto i : ctre::range<R"(([A-Z]+))">(v))
        {
            auto to_id { vertex_id_from_name(i)};
            auto ef = boost::add_edge(from_id, to_id, g);
        }
    }

    auto wm {boost::make_static_property_map<typename graph1_t::edge_descriptor, int>(1)};
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

template<int N, typename F> void dfs_visit(graph_t const& g, vertex_id_t v, int time, int visited, int flow, F f)
{
    flow += boost::get(boost::vertex_flow_t(), g, v) * (N - time - 1);
    for(auto[e, ee] = boost::out_edges(v, g); e != ee; ++e)
    {
        auto d = boost::get(boost::edge_weight_t(), g, *e);
        auto vt = boost::target(*e, g);
        if( time + d < N)
        {
            if(!(visited & get_bit(vt)))
                dfs_visit<N>(g, vt, time + d + 1, visited | get_bit(vt), flow, f);
        }
        else
            f(flow, visited);
    }
}

auto pt1(auto const& g)
{
    int mx { 0 };
    dfs_visit<31>(g, 0, 0, 0, 0, [&](int f, int){ if( f > mx) mx = f;});
    return mx;
}

auto pt2(auto const& g)
{
    std::map<int64_t, int> mxs;
    dfs_visit<27>(g, 0, 0, 0, 0, [&](int f, int v){ if(mxs[v] < f) mxs[v] = f;});
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
    return mx;}

int main()
{
    auto g {get_input()};

    std::cout << "pt1 = " << pt1(g) << "\n";
    std::cout << "pt2 = " << pt2(g) << "\n";
}