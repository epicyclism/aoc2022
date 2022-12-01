#include <iostream>
#include <string>
#include <charconv>
#include <vector>
#include <algorithm>
#include <numeric>

auto get_input()
{
    std::vector<int> rv;
    std::string ln;
    int cal { 0 };
    while(std::getline(std::cin, ln))
    {
        if(ln.empty())
        {
            rv.push_back(cal);
            cal = 0;
        }
        else
        {
            int item{ 0 };
            std::from_chars(ln.c_str(), ln.c_str() + ln.size(), item);
            cal += item;
        }
    }
    rv.push_back(cal);

    return rv;
}

int main()
{
    auto in {get_input()};

    std::partial_sort(in.begin(), in.end(), in.begin() + 3, std::greater<>());
    std::cout << "pt1 = " << in.front() << "\n";
    std::cout << "pt2 = " << std::accumulate(in.begin(), in.begin() + 3, 0) << "\n";
}