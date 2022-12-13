#define BOOST_JSON_HEADER_ONLY
#define BOOST_JSON_STATIC_LINK
#include <boost/json.hpp>

#include <utility>
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <algorithm>
#include <charconv>

auto get_input()
{
    std::string ln;
    std::string lnl;
    std::string lnr;
    std::vector<std::pair<std::string, std::string>> in;
    while (std::getline(std::cin, ln))
    {
        if(ln.empty())
		{
            in.emplace_back(lnl, lnr);
			lnl.clear();
		}
		else
        if(lnl.empty())
            lnl = ln;
        else
            lnr = ln;
    }
    in.emplace_back(lnl, lnr);
    return in;
}

bool less_v_v(boost::json::value const& l, boost::json::value const& r)
{
	if(l.is_array() && r.is_array())
		return std::lexicographical_compare(l.as_array().begin(), l.as_array().end(), r.as_array().begin(), r.as_array().end(), less_v_v);
	if(l.is_array())
		return less_v_v(l.as_array(), boost::json::value{r.as_int64()});
	else
	if(r.is_array())
		return less_v_v(boost::json::value{l.as_int64()}, r);
	return l.as_int64 () < r.as_int64();
}

bool compare(std::pair<std::string, std::string> const& ss)
{
#if 1
	auto vl = boost::json::parse(ss.first);
	auto vr = boost::json::parse(ss.second);
	auto b{ less_v_v(vl, vr )};
	return b;
#else
	return false;
#endif
}

auto pt1(auto const& in)
{
    int rv {0};
    for(int n = 0; n < in.size(); ++n)
        if( compare(in[n]))
            rv += n + 1;
    return rv;
}

auto pt2(auto const& in)
{
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
}

int main()
{
	std::cout << "v2\n";
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}