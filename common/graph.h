//
// 'generic' graph structures and algorithms
//
  
#include <vector>
#include <queue>
#include <tuple>
#include <concepts>
#include <type_traits>

using vertex_id_t = std::size_t;

template<typename GraphContainer> 
concept graph_container = requires (vertex_id_t v, GraphContainer G)
{
    G[v].begin();
    G[v].end();
    G.size();
    { *(G[v].begin())} -> std::convertible_to<vertex_id_t>;
};

template<typename D> struct result_recorder_distance
{
    using return_type = std::vector<D>;
    return_type rv;
    result_recorder_distance(size_t sz) : rv (sz, -1)
    {}
    void set_distance(vertex_id_t v, vertex_id_t u)
    {
        rv[v] = rv[u] + 1;
    }
    auto get_distance(vertex_id_t v) const
    {
        return rv[v];
    }
    void set_previous(vertex_id_t v, vertex_id_t u)
    {}
};

template<typename D> struct result_recorder_distance_previous
{
    using return_type = std::pair<std::vector<D>, std::vector<vertex_id_t>>;
    return_type rv;
    result_recorder_distance_previous(size_t sz) : rv ( std::piecewise_construct, std::forward_as_tuple( sz, -1), std::forward_as_tuple(sz, -1))
    {}    
    void set_distance(vertex_id_t v, vertex_id_t u)
    {
        rv.first[v] = rv.first[u] + 1;
    }
    auto get_distance(vertex_id_t v) const
    {
        return rv.first[v];
    }
    void set_previous(vertex_id_t v, vertex_id_t u)
    {
        rv.second[v] = u;
    }
};

template<typename G, bool previous = false> 
requires graph_container<G> 
auto bfs(G const& g, vertex_id_t id_from) 
{
    // abstract the differences in return data required
    using recorder_t = typename std::conditional<previous, result_recorder_distance_previous<int>, result_recorder_distance<int>>::type;

    recorder_t              recorder(g.size());
    std::vector<bool>       visited(g.size());
    std::queue<vertex_id_t> q;
    q.push(id_from);
    recorder.set_distance(id_from, id_from);
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		visited[u] = true;
        for (auto e : g[u])
        {
			if (!visited[e])
			{
                recorder.set_distance(e, u);
                recorder.set_previous(e, u);
				q.push(e);
			}
        }
    }
    return recorder.rv;
}  

#if 0
template<typename G, typename W, bool previous = false> 
requires graph_container<G> 
auto dijkstra(G const& g, vertex_id_t from)
{
    using weight_t  = W;
    // abstract the differences in return data required
    using recorder_t = typename std::conditional<previous, result_recorder_distance_previous<weight_t>, result_recorder_distance<weight_t>>::type;

    recorder_t recorder(g.size());
    using pq_t = std::pair<vertex_id_t, weight_t>;
    auto pq_t_cmp = [](auto& l, auto& r){ return l.second > r.second;};
    std::priority_queue<pq_t, std::vector<pq_t>, decltype(pq_t_cmp)> q(pq_t_cmp);
    q.push({from, 0});
    recorder.set_distance(from, from);
    while (!q.empty())
    {
        auto p = q.top(); q.pop();

        for( auto e : g[p.first])  
        {
            auto ev = g::vertex(e);
            if (recorder.get_distance(ev) == -1 || recorder.get_distance(ev) > recorder.get_distance(p.first) + g::weight(e))
            {
                recorder.set_distance(ev, recorder.get_distance(p.first) + g::weight(e));
                recorder.set_previous(ev, p.first);
                q.push({ev, recorder.get_distance(ev)});
            }
        }
    }

    return recorder.rv;
}
#endif
// helpers for adjacency lists
template<typename AdjacencyList, typename... ARGS> 
concept adjacency_list = requires (AdjacencyList g, ARGS... args)
{
    g.size();
    g.resize(size_t());
    g[0].emplace_back(0, args...);
};

template <typename G, typename... ARGS> 
requires adjacency_list<G, ARGS...>
void add_edge(G& g, vertex_id_t from, vertex_id_t to, ARGS... args )
{
    if (g.size() < from + 1)
        g.resize(from + 1);
    g[from].emplace_back(to, args...);
}

template <typename G, typename... ARGS> 
requires adjacency_list<G, ARGS...>
void add_edge_undirected(G& g, vertex_id_t from, vertex_id_t to, ARGS... args )
{
    add_edge(g, from, to, args...);
    add_edge(g, to, from, args...);
}
#if 0
// helper for grid of weights stored as a vector with a stride
//
template<typename W>
class weighted_grid
{
private:
    const std::vector<W> const& data_;
    const size_t stride_;
public:
    using edge_t = std::pair<vertex_id_t, W>;
    static vertex_id_t vertex(edge_t const& e) const
    {
        return e.first;
    }
    static W weight(edge_t const& e) const
    {
        return e.second;
    }   
    graph_t(std::vector<W> const& d, size_t s) : data_{ d }, stride_{s)}
    {}
    std::vector<edge_t> operator[](vertex_id_t v) const
    {
        std::vector<edge_t> rv;
        // left
        if (v % stride_ != 0)
            rv.emplace_back(v - 1, data_[v - 1]);
        // right
        if (v % stride_ != stride_ - 1)
            rv.emplace_back(v + 1, data_[v + 1]);
        // up
        if (v > stride_)
            rv.emplace_back(v - stride_, data_[v - stride_]);
        // down
        if (v < data_.size() - stride_)
            rv.emplace_back(v + stride_, data_[v + stride_]);
        return rv;
    }
    size_t size() const
    {
        return data_.size();
    }
};
#endif