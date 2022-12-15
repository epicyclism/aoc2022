
#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <algorithm>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace nlohmann::literals;

auto get_input()
{
    std::vector<json> rv;
    std::string ln;
    while (std::getline(std::cin, ln))
        if(!ln.empty())
            rv.emplace_back(json::parse(ln));
    return rv;
}

bool operator<(json const& l, json const& r)
{
	if(l.is_array() && r.is_array())
		return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
	if(l.is_array())
		return l < json::array({r.get<int>()});
	else
	if(r.is_array())
		return json::array({l.get<int>()}) < r;
	return l.get<int>() < r.get<int>();
}

auto pt1(auto const& in)
{
    int rv {0};
    for(int n = 0; n < in.size() / 2; ++n)
        if(in[2 * n] < in[2 * n + 1])
            rv += n + 1;
    return rv;
}

auto pt2(auto in)
{
    std::sort(in.begin(), in.end());
    auto cl = std::distance(in.begin(), std::lower_bound(in.begin(), in.end(), json::parse("[[2]]")));
    auto cr = std::distance(in.begin(), std::lower_bound(in.begin(), in.end(), json::parse("[[6]]")));
	return (cl+1)*(cr + 2);
}

int main()
{
    auto in = get_input();

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}