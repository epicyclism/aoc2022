#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>

using xvertex_id_t = std::pair<short, short>;
using graph_t = std::map<xvertex_id_t, std::vector<xvertex_id_t>>;

template <typename G, typename... ARGS> 
void add_edge(G& g, xvertex_id_t from, xvertex_id_t to, ARGS... args )
{
    g[from].emplace_back(to, args...);
}

template <typename G, typename... ARGS> 
void add_edge_undirected(G& g, xvertex_id_t from, xvertex_id_t to, ARGS... args )
{
    add_edge(g, from, to, args...);
    add_edge(g, to, from, args...);
}

xvertex_id_t adj(xvertex_id_t v, char d)
{
	switch(d)
	{
		case 'N':
			return {v.first + 1, v.second};
		case 'S':
			return {v.first - 1, v.second};
		case 'W':
			return {v.first, v.second - 1};
		case 'E':
			return {v.first, v.second + 1};
		default:
			// TILT
			std::cout << "Bad direction " << d << " in adj\n";
			return {32367, 32367};
	}
}

std::string_view parse_paren(std::string_view s, graph_t& g, xvertex_id_t v)
{
	auto b { s.begin()};
	auto e { s.end()};
	xvertex_id_t vu { v };
	while(b != e)
	{
//		std::cout << "\t" << *b << "\n";
		switch(*b)
		{
			case 'N':
			case 'S':
			case 'W':
			case 'E':
				{
					xvertex_id_t vt = adj(vu, *b);
					add_edge_undirected(g, vu, vt);
					vu = vt;
				}
				break;
			case '|':
				vu = v;
				break;
			case '(':
				b = parse_paren({b + 1, e}, g, vu).begin();
				break;
			case ')':
				return {b, e};
			default:
				std::cout << "CANT HAPPEN (" << *b << ")\n";
				break;
		}
		++b;
	}
	std::cout << "parse error, no matching close paren.\n";
	return {b, e};
}

graph_t parse_rx(std::string_view s)
{
	graph_t g;
	xvertex_id_t v { 0, 0};
	auto b { s.begin()};
	auto e { s.end()};
	while(b != e)
	{
//		std::cout << *b << "\n";
		switch(*b)
		{
			case 'N':
			case 'S':
			case 'W':
			case 'E':
				{
					xvertex_id_t vt = adj(v, *b);
					add_edge_undirected(g, v, vt);
					v = vt;
				}
				break;
			case '^':
				break;
			case '$':
				break;
			case '(':
				b = parse_paren({b + 1, e}, g, v).begin();
				break;
			default:
				std::cout << "CANT HAPPEN (" << *b << ")\n";
				break;
		}
		++b;
	}
	return g;
}

auto bfs(graph_t& g, xvertex_id_t id_from) 
{
	std::map<xvertex_id_t, int> distances;
	std::set<xvertex_id_t> visited;
    std::queue<xvertex_id_t> q;
    q.push(id_from);
    distances[id_from] = 0;
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		visited.insert(u);
        for (auto e : g[u])
        {
			if (!visited.contains(e))
			{
				distances[e] = distances[u] + 1;
				q.push(e);
			}
        }
    }
    return distances;
}

int main()
{
	std::string in;
	std::getline(std::cin, in);
	auto g = parse_rx(in);
	auto d = bfs(g, {0, 0});
	std::cout << g.size () << " vertices\n";
	std::cout << "pt1  " << (*std::max_element(d.begin(), d.end(), [](auto& l, auto& r) { return l.second < r.second;})).second << "\n";
	std::cout << "pt2  " << std::count_if(d.begin(), d.end(), [](auto v){ return v.second >= 1000;}) << "\n";
}