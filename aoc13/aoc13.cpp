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

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

struct value : public std::variant<int, std::vector<value>>
{
	value(int n)
	{
		this->emplace<0>(n);
	}
	value(std::vector<value> const& vv)
	{
		this->emplace<1>(vv);
	}
};
#if 1
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
	return { t, fcr.ptr};
}

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
		}
		sv.remove_prefix(1);
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

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}