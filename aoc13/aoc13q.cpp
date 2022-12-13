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
            in.emplace_back(std::move(lnl), std::move(lnr));
        else
        if(lnl.empty())
            lnl.swap(ln);
        else
            lnr.swap(ln);
    }
    in.emplace_back(std::move(lnl), std::move(lnr));
    return in;
}

struct Value : public std::variant<int, std::vector<Value>>
{
	Value() { this->emplace<0>(0); }
	Value(int v) { this->emplace<0>(v); }
	Value(const std::vector<Value>& v) { this->emplace<1>(v); }
	bool operator < (const Value& rhs) const;
};


struct CompareVisitor
{
	bool operator ()(int l, int r) const
	{
		return l < r;
	}
	bool operator ()(const std::vector<Value>& l, const std::vector<Value>& r) const
	{
		return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end());
	}
	bool operator ()(int l, const std::vector<Value>& r) const
	{
		return operator()(std::vector<Value>{l}, r);
	}
	bool operator ()(const std::vector<Value>& l, int r) const
	{
		return operator()(l, std::vector<Value>{r});
	}
} compareVisitor;

bool Value::operator < (const Value& rhs) const
{
	return std::visit(compareVisitor, *this, rhs);
}

struct PrintVisitor
{
	std::ostream& out;
	void operator () (int v) const
	{
		out << v;
	}
	void operator () (const std::vector<Value>& v) const
	{
		bool first = true;
		for (auto& child : v)
		{
			out << (first ? "[" : ",");
			std::visit(*this, child);
			first = false;
		}
		if (first) out << "[";
		out << "]";
	}
};

std::ostream& operator << (std::ostream& out, const Value& v)
{
	visit(PrintVisitor{ out }, v);
	return out;
}

Value parse(std::string_view& s)
{
	if (s[0] == '[')
	{
		std::vector<Value> vr;
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

bool compare(std::pair<std::string, std::string> const& ss)
{
	std::string_view sv = ss.first;
	auto vl = parse(sv);
	sv = ss.second;
	auto vr = parse(sv);
	auto b{ vl < vr };
	//	std::cout << vl << " < " << vr << " = " << b << "\n";
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
	std::string_view p1{ "[[6]]" };
	std::string_view p2{ "[[2]]" };
	std::vector<std::pair<Value, int>> vv;
	vv.emplace_back(parse(p1), 0);
	vv.emplace_back(parse(p2), 1);
	for (auto& i : in)
	{
		std::string_view sv = i.first;
		vv.emplace_back(parse(sv), -1);
		sv = i.second;
		vv.emplace_back(parse(sv), -1);
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