#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <charconv>

#include <algorithm>

int digit(char c)
{
    switch(c)
    {
        case '2':
            return 2;
        case '1':
            return 1;
        case '0':
            return 0;
        case '-':
            return -1;
        case '=':
            return -2;
    }
    return -100;
}

int64_t from_snafu(std::string_view s)
{
    int64_t rv {0};
    for(auto c: s)
    {
        rv *= 5;
        rv += digit(c);
    }
    return rv;
}

auto to_b5(int64_t n)
{
    std::array<char, 32> out;
    auto [ptr, ec] = std::to_chars(out.data(), out.data() + out.size(), n, 5);
    std::string s{out.data(), ptr};
    return s;
}

auto to_snafu(int64_t n)
{
    auto s { to_b5(n)};
    std::reverse(s.begin(), s.end());
    int carry{0};
    for(auto it = s.begin(); it != s.end(); ++it)
    {
        switch(*it + carry)
        {
            case '5':
                *it = '0';
                carry = 1;
                break;
            case '4':
                *it = '-';
                carry = 1;
                break;
            case '3':
                *it = '=';
                carry = 1;
                break;
            case '2':
            case '1':
            case '0':
                *it = *it + carry;
                carry = 0;
                break;
            default:
                std::cout << "eek " << *it << ", " << carry << "\n";
                break;
        }
    }
    if(carry)
        s.push_back('1');
    std::reverse(s.begin(), s.end());
    return s;
}

int main()
{
    std::string ln;
    int64_t sm {0};
    while(std::getline(std::cin, ln))
    {
        auto val { from_snafu(ln)};
        sm += val;
    }
    std::cout << "pt1 = " << sm << " = b5 " << to_b5(sm) << " = snafu " << to_snafu(sm) << "\n";
}