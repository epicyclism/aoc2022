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

int64_t get_bit(int n)
{
    return 1LL << n;
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

    std::cout << "AA at " << vertex_id_from_name("AA") << "\n";
    for(auto& vf : vflow)
        std::cout << vf.first << " = " << vf.second << "\n";
    graph_t gr(vflow.size() + 1);
    for(auto f = 0; f < vflow.size(); ++f)
        for(auto t = f + 1; t < vflow.size() + 1; ++t)
            boost::add_edge(f, t, gr);

    constexpr char name[]= {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    boost::print_vertices(gr, name);
    std::cout << "\n";
    boost::print_edges(gr, name);
    std::cout << "\n";
    return gr;
}

auto pt1(auto const& g)
{
    return 0;
}

auto pt2(auto const& g)
{
    return 0;
}

int main()
{
    auto g {get_input()};

    std::cout << "pt1 = " << pt1(g) << "\n";
    std::cout << "pt2 = " << pt2(g) << "\n";
}