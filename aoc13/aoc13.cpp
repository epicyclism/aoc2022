
#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <algorithm>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
#if 0
    std::string_view p1{ "[[6]]" };
	std::string_view p2{ "[[2]]" };

	std::vector<std::pair<boost::json::value, int>> vv;
	vv.emplace_back(boost::json::parse(p1), 0);
	vv.emplace_back(boost::json::parse(p2), 1);
	for (auto& i : in)
	{
		vv.emplace_back(boost::json::parse(i.first), -1);
		vv.emplace_back(boost::json::parse(i.second), -1);
	}
	std::sort(vv.begin(), vv.end(), [](auto& l, auto& r) {return less_v_v(l.first, r.first); });
	auto pkt1 = std::find_if(vv.begin(), vv.end(), [](auto& p) { return p.second == 0; });
	auto pkt2 = std::find_if(vv.begin(), vv.end(), [](auto& p) { return p.second == 1; });
	return (std::distance(vv.begin(), pkt1) + 1) * (std::distance(vv.begin(), pkt2) + 1);
#else
    auto jl = json("[[\"2\"]]");
    auto jr = json("[[\"6\"]]");
    int cl { 0};
    int cr { 0};
    for(auto& v : in)
    {
        if( jl < v)
            ++cl;
        if(jr < v)
            ++cr;
    }
	return cl*(cr + 1);
#endif
}

int main()
{
    auto in = get_input();

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}