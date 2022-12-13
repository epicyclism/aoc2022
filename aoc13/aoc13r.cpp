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
            in.emplace_back(lnl, lnr);
        else
        if(lnl.empty())
            lnl.swap(ln);
        else
            lnr.swap(ln);
    }
    in.emplace_back(lnl, lnr);
    return in;
}

struct value
{
    int v_;
    std::vector<value> vv_;
    value() : v_ { 0}
    {}
    value(int v) : v_ {v}
    {}
    value(std::vector<value> const& vv) : v_{-1}, vv_ { vv}
    {}
};

bool operator< (value const& l, value const& r)
{
    if( l.v_ == -1 && r.v_ == -1)
        return l.vv_ < r.vv_;
    if(l.v_ == -1)
        return l.vv_[0] < r.v_;
    if(r.v_ == -1)
        return l.v_ < r.vv_[0];
    return l.v_ < r.v_;
}

#if 0
bool operator<(value const& lv, value const& rv)
{
#if 1
	return std::visit(overloaded{
				[](int l, int r) -> bool { return l < r; },
				[](int l, std::vector<value> const& r) -> bool { return l < r[0]; },
				[](std::vector<value> const& l, int r) -> bool { return l[0] < r; },
				[](std::vector<value> const& l, std::vector<value> const& r) -> bool { return l < r; }
		}, lv, rv);
#else
	return std::visit(lt_v_v, lv, rv);
#endif
}
#endif

template<typename T> std::pair<T, std::string_view> sv_to_t(std::string_view sv)
{
	T t{ 0 };
	auto fcr = std::from_chars(sv.data(), sv.data() + sv.size(), t);
    if( *fcr.ptr == ',')
        ++fcr.ptr;
	return { t, fcr.ptr};
}
#if 0
std::pair<value, std::string_view> parse(std::string_view sv)
{
	if (sv.front() == '[')
	{
		std::vector<value> vr;
		sv.remove_prefix(1);
		while (sv.front() != ']')
		{
			auto [v, sv2] = parse(sv);
			vr.push_back(v);
			sv = sv2;
            std::cout << "   " << sv << "\n";
		}
//		sv.remove_prefix(1);
        std::cout << sv << "\n";
		return { vr, sv };
	}
	else
		return sv_to_t<int>(sv);
}
bool compare(std::pair<std::string, std::string> const& ss)
{
	auto vl = parse(ss.first).first;
	auto vr = parse(ss.second).first;
	auto b{ vl < vr };
	return b;
}
#else
value parse(std::string_view& s)
{
	if (s[0] == '[')
	{
		std::vector<value> vr;
		if (s[1] != ']')
		{
			while (s[0] != ']')
			{
				s.remove_prefix(1);
				vr.push_back(parse(s));
			}
			s.remove_prefix(1);
		}
		else
		{
			s.remove_prefix(2);
		}
		return vr;
	}
	else
	{
		int v;
		auto p = std::from_chars(s.data(), s.data() + s.size(), v);
		s.remove_prefix(p.ptr - s.data());
		return v;
	}
}

value parse(std::string const& s)
{
    std::string_view sv { s};
    return parse(sv);
}

bool compare(std::pair<std::string, std::string> const& ss)
{
	auto vl = parse(ss.first);
	auto vr = parse(ss.second);
	auto b{ vl < vr };
	return b;
}
#endif
auto pt1(auto const& in)
{
    int rv {0};
    for(int n = 0; n < in.size(); ++n)
        if( compare(in[n]))
            rv += n + 1;
    return rv;
}
#if 0
auto pt2(auto const& in)
{
	constexpr std::string_view p1{ "[[6]]" };
	constexpr std::string_view p2{ "[[2]]" };
	std::vector<std::pair<value, int>> vv;
	vv.emplace_back(parse(p1).first, 0);
	vv.emplace_back(parse(p2).first, 1);
	for (auto& i : in)
	{
		vv.emplace_back(parse(i.first).first, -1);
		vv.emplace_back(parse(i.second).first, -1);
	}
	std::ranges::sort(vv, [](auto& l, auto& r) {return l.first < r.first; });
	auto pkt1 = std::ranges::find_if(vv, [](auto& p) { return p.second == 0; });
	auto pkt2 = std::ranges::find_if(vv, [](auto& p) { return p.second == 1; });
	return (std::distance(vv.begin(), pkt1) + 1) * (std::distance(vv.begin(), pkt2) + 1);
}
#else
auto pt2(auto const& in)
{
    std::string_view p1{ "[[6]]" };
	std::string_view p2{ "[[2]]" };
	std::vector<std::pair<value, int>> vv;
	vv.emplace_back(parse(p1), 0);
	vv.emplace_back(parse(p2), 1);
	for (auto& i : in)
	{
		vv.emplace_back(parse(i.first), -1);
		vv.emplace_back(parse(i.second), -1);
	}
	std::ranges::sort(vv, [](auto& l, auto& r) {return l.first < r.first; });
	auto pkt1 = std::ranges::find_if(vv, [](auto& p) { return p.second == 0; });
	auto pkt2 = std::ranges::find_if(vv, [](auto& p) { return p.second == 1; });
	return (std::distance(vv.begin(), pkt1) + 1) * (std::distance(vv.begin(), pkt2) + 1);
}
#endif
int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}