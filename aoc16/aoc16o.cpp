#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <bitset>

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

void print_g(graph_t const& in)
{
    for(vertex_id_t v = 0; v < in.size(); ++v)
    {
        std::cout << v << " : ";
        for(auto& t: in[v])
            std::cout << "( " << t.to_ << ", " << t.flow_ << " ) ";
        std::cout << "\n";  
    }
}

auto pt1(auto const& in)
{
    auto g { in.second};
    auto src = vertex_id_from_name("AA");
    struct pth
    {
        vertex_id_t to_;
        int tm_;
        int flow_;
        vertex_id_t prev_;
        int64_t seen_;
    };
    std::vector<int> mem(g.size(), -1);
    std::queue<pth> q; 
    q.push({src, 30, 0, src, 0});
    int mx{ 0 };
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        if (u.tm_ == 0)
        {
            if(u.flow_ > mx)
                mx = u.flow_;
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
                    if( v.flow_ > 0 && !(get_bit(v.to_) & u.seen_))
                    {
                        if( mem[u.to_] < u.flow_ + v.flow_ * u.tm_)
                        {
                            pth p { u };
                            p.to_ = v.to_;
                            p.flow_ += v.flow_ * p.tm_;
                            p.seen_ |= get_bit(v.to_);
                            mem[v.to_] = p.flow_;
                            p.prev_ = -1;
                            q.push(p);
                        }
                    }
                }
            }
        }
    }
    return mx;
}

auto pt2o(auto const& in)
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
    std::cout << cache.size() << "\n";
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
}

struct key0
{
    vertex_id_t v1_;
    vertex_id_t v2_;
    int64_t     seen1_;
    int64_t     seen2_;
    friend auto operator<=>(const key0&, const key0&) = default;
};

// 2043 too low
//
auto pt2p(auto const& in)
{
    auto s { in.first};
    auto g { in.second};

    auto src = vertex_id_from_name("AA");
    struct pth
    {
        vertex_id_t to_;
        vertex_id_t to2_;
        int tm_;
        int flow_;
        vertex_id_t prev_;
        vertex_id_t prev2_;
        int64_t seen1_;
        int64_t seen2_;
    };
    struct mem_t
    {
        std::map<key0, int> mem_;

        bool check_mem(vertex_id_t to, vertex_id_t to2, int64_t seen1, int64_t seen2, int flow)
        {
            auto itk = mem_.find({to, to2, seen1, seen2});
            return ( itk == mem_.end() || (*itk).second < flow);
        }
        void set_mem(vertex_id_t to, vertex_id_t to2, int64_t seen1, int64_t seen2, int flow)
        {
            mem_[{to, to2, seen1, seen2}] = flow;
        }
    };
    mem_t mem;
    std::queue<pth> q; 
    q.push({src, src, 26, 0, src, src, 0});
    int mx{0};
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        if (u.tm_ == 0 || ((u.seen1_ | u.seen2_) == s))
        {
            if ( u.flow_ > mx)
            {
                mx = u.flow_;
                std::cout << "mx = " << mx << "\n";
            }
        }
        else
        {
            --u.tm_;
            for (auto& v : g[u.to_])
            {   
                bool fin {false};
                pth p { u };
                auto seen = p.seen1_ | p.seen2_;
                if( v.to_ != u.prev_)
                {
                    fin = true;
                    if( v.flow_ < 1)
                    {
                        p.to_ = v.to_;
                        p.prev_ = u.to_;
                    }
                    else
                    if( !(seen & get_bit(v.to_)) )
                    {
                        p.to_ = v.to_;
                        p.flow_ += v.flow_ * p.tm_;
                        p.seen1_ |= get_bit(v.to_);
                        p.prev_ = -1;
                    }
                    else
                        fin = false;
                }
                if(fin)
                {
                    for (auto& v2 : g[u.to2_])
                    {
                        bool fin2 { false};
                        auto p2{p};
                        auto seen = p.seen1_ | p.seen2_;
                        if( fin && v2.to_ != u.prev2_)
                        {
                            fin2 = true;
                            if( v2.flow_ < 1)
                            {
                                p2.to2_ = v2.to_;
                                p2.prev2_ = u.to2_;
                            }
                            else
                            if( !(seen & get_bit(v2.to_)) )
                            {
                                p2.to2_ = v2.to_;
                                p2.flow_ += v2.flow_ * p2.tm_;
                                p2.seen2_ |= get_bit(v2.to_);
                                p2.prev2_ = -1;
                            }
                            else
                                fin2 = false;
                        }
                        if(( fin && fin2) && mem.check_mem(p2.to_, p2.to2_, p2.seen1_, p2.seen2_, p2.flow_))
                        {
                            mem.set_mem(p2.to_, p2.to2_, p2.seen1_, p2.seen2_, p2.flow_);
                            q.push(p2);
                        }
                    }
                }
            }
        }
    }
#if 1
    int mx2 { 0};
    for(auto& m : mem.mem_)
    {
        if( m.second > mx2)
            mx2 = m.second;
    }
#endif
    std::cout << mx2 << "\n";
    return mx;
}

struct key
{
    vertex_id_t v1_;
    vertex_id_t v2_;
    int64_t     seen_;
    friend auto operator<=>(const key&, const key&) = default;
};

auto pt2(auto const& in)
{
    auto s { in.first};
    auto g { in.second};

    auto src = vertex_id_from_name("AA");
    struct pth
    {
        vertex_id_t to_;
        vertex_id_t to2_;
        int tm_;
        int flow_;
        vertex_id_t prev_;
        vertex_id_t prev2_;
        int64_t seen_;
    };
    struct mem_t
    {
        std::map<key, int> mem_;

        bool check_mem(vertex_id_t to, vertex_id_t to2, int64_t seen, int flow)
        {
            auto itk = mem_.find({to, to2, seen});
            return ( itk == mem_.end() || (*itk).second < flow);
        }
        void set_mem(vertex_id_t to, vertex_id_t to2, int64_t seen, int flow)
        {
            mem_[{to, to2, seen}] = flow;
        }
    };
    mem_t mem;
    std::queue<pth> q; 
    q.push({src, src, 26, 0, src, src, 0});
    int mx{0};
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        if (u.tm_ == 0 || u.seen_ == s)
        {
            if ( u.flow_ > mx)
            {
                mx = u.flow_;
                std::cout << "mx = " << mx << "\n";
            }
        }
        else
        {
            --u.tm_;
            for (auto& v : g[u.to_])
            {   
                bool fin {false};
                pth p { u };
//                if( v.to_ != u.prev_)
                {
                    fin = true;
                    if( v.flow_ < 1)
                    {
                        p.to_ = v.to_;
                        p.prev_ = u.to_;
                    }
                    else
                    if( !(p.seen_ & get_bit(v.to_)) )
                    {
                        p.to_ = v.to_;
                        p.flow_ += v.flow_ * p.tm_;
                        p.seen_ |= get_bit(v.to_);
                        p.prev_ = -1;
                    }
                    else
                        fin = false;
                }
//                if(fin)
                {
                    for (auto& v2 : g[u.to2_])
                    {
                        bool fin2 { false};
                        auto p2{p};
//                        if( fin && v2.to_ != u.prev2_)
                        {
                            fin2 = true;
                            if( v2.flow_ < 1)
                            {
                                p2.to2_ = v2.to_;
                                p2.prev2_ = u.to2_;
                            }
                            else
                            if( !(p2.seen_ & get_bit(v2.to_)) )
                            {
                                p2.to2_ = v2.to_;
                                p2.flow_ += v2.flow_ * p2.tm_;
                                p2.seen_ |= get_bit(v2.to_);
                                p2.prev2_ = -1;
                            }
                            else
                                fin2 = false;
                        }
                        if(/*( fin && fin2) && */mem.check_mem(p2.to_, p2.to2_, p2.seen_, p2.flow_))
                        {
                            mem.set_mem(p2.to_, p2.to2_, p2.seen_, p2.flow_);
                            q.push(p2);
                        }
                    }
                }
            }
        }
    }
#if 1
    int mx2 { 0};
    for(auto& m : mem.mem_)
    {
        if( m.second > mx2)
            mx2 = m.second;
    }
#endif
    std::cout << mx2 << "\n";
    return mx;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}