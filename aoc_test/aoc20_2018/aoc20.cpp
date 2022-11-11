#include <iostream>
#include <string>
#include <vector>
#include <array>

struct room
{
	char c_;
	int nr_ = 0;
	std::array<room*, 4> doors_;
	room* new_room(char c)
	{
		room* r = new room(c);
		++nr_;
		return r;
	}
};

char const* parse(room* r, char const* pc)
{
	while (*pc != '$')
	{
		switch (*pc)
		{
		case '^':
			return pc;
		case 'N':
		case 'S':
		case 'E':
		case 'W':
			r = r->new_room(*pc);
			break;
		case '(':
			pc = parse(r->new_room(' '), pc + 1); 
			break;
		case '|':
			break;
		case ')':
			break;
		default:
			std::cout << "bad char " << *pc << "\n";
			break;
		}
		++pc;
	}
	return pc;
}

room parse(char const* pc)
{
	room rm;
	return parse(&rm, pc);
}

int pt1(std::string const& s)
{
	return s.size();
}

int main()
{
	std::string in;
	std::getline(std::cin, in);
	std::cout << "pt1  " << pt1(in) << "\n";
}