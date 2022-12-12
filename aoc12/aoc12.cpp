#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

#include "graph.h"

auto get_input()
{
    std::string ln;
    std::vector<char> in;
    size_t stride{0};
    size_t S{0};
    size_t E{0};
    while(std::getline(std::cin, ln))
    {
        std::copy(ln.begin(), ln.end(), std::back_inserter(in));
        stride = ln.size();
    }
    for(auto n = 0; n < in.size(); ++n)
    {
        char c;
        switch(in[n])
        {
            case 'S':
                S = n;
                c = 0;
                break;
            case 'E':
                E = n;
                c = 25;
                break;
            default:
                c = in[n] - 'a';
                break;
        }
        in[n] = c;
    }
    return std::make_tuple(in, stride, S, E);
}

// helper for grid
//
class grid
{
private:
    const std::vector<char>& data_;
    const size_t stride_;
public:
    grid(std::vector<char> const& d, size_t s) : data_{ d }, stride_{s}
    {}
    std::vector<vertex_id_t> operator[](vertex_id_t v) const
    {
        auto this_ht { data_[v]};
        ++this_ht;
        std::vector<vertex_id_t> rv;
        // left
        if (v % stride_ != 0 && (data_[v - 1] <= this_ht))
            rv.emplace_back(v - 1);
        // right
        if ((v % stride_) != (stride_ - 1) && (data_[v + 1] <= this_ht))
            rv.emplace_back(v + 1);
        // up
        if (v > stride_ && (data_[v - stride_] <= this_ht))
            rv.emplace_back(v - stride_);
        // down
        if (v < data_.size() - stride_ && (data_[v + stride_] <= this_ht))
            rv.emplace_back(v + stride_);
        return rv;
    }
    size_t size() const
    {
        return data_.size();
    }
};

auto pt1(auto const& in)
{
    grid g { std::get<0>(in), std::get<1>(in)};
    auto d { bfs(g, std::get<2>(in))};
    return d[std::get<3>(in)];
}

auto pt2(auto const& in)
{
    std::cout << std::get<1>(in) << ", " << std::get<2>(in) << ", " << std::get<3>(in) << "\n";
    return std::get<0>(in).size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}