#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include <ctre_inc.h>

auto get_input()
{
    std::vector<int> in;
    std::string ln;
    while(std::getline(std::cin, ln))
    {
       in.emplace_back(sv_to_t<int>(ln));
    }

    return in;
}

void advance(auto& l, auto& i, auto n)
{
    for(int cnt = 0; cnt < n; ++cnt)
    {
        ++i;
        if( i == l.end())
            i = l.begin();
    }    
}
#if 0
void retard(auto& l, auto& i, auto n)
{
    for(int cnt = 0; cnt < n; ++cnt)
    {
        if(i == l.begin())
            i = l.end();
        --i;
    }    
}
#endif
template<typename L, typename I> void swap_left(L& l, I& i)
{
    auto to_the_left { i};
    if( to_the_left == l.begin())
        to_the_left = l.end();
    --to_the_left;
    if( to_the_left == l.begin())
        to_the_left = l.end();
    L tmp;
    tmp.splice(tmp.begin(), l, i);
    l.splice(to_the_left, tmp);
 }

template<typename L, typename I>  void swap_right(L& l, I& i)
{
    auto to_the_right {i};
    ++to_the_right;
    if( to_the_right == l.end())
        to_the_right = l.begin();
    ++to_the_right;
    L tmp;
    tmp.splice(tmp.begin(), l, i);
    l.splice(to_the_right, tmp);
}

auto pt1(auto const& in)
{
    std::list<int> inl;
    std::vector<std::list<int>::iterator> inlp;
    auto itz { inl.begin()};
    for(auto i: in)
    {
        inl.push_back(i);
        auto ite = inl.end();
        --ite;
        inlp.push_back(ite);
        if(i == 0)
            itz = ite;
    }
    for(auto it: inlp)
    {
        if(*it == 0)
        {
            continue;
        }
        int cnt = std::abs(*it);
        if(*it < 0)
            for(int n = 0; n < cnt; ++n)
                swap_left(inl, it);
        else
            for(int n = 0; n < cnt; ++n)
                swap_right(inl, it);
    }
    int r { 0 };
    for(int n = 0; n < 3; ++n)
    {
        advance(inl, itz, 1000);
        r += *itz;
    }
   return r;
}

auto pt2(auto  in)
{
    std::list<int64_t> inl;
    std::vector<std::list<int64_t>::iterator> inlp;
    auto itz { inl.begin()};
    for(auto i: in)
    {
        inl.push_back(i * 811589153LL);
        auto ite = inl.end();
        --ite;
        inlp.push_back(ite);
        if(i == 0)
            itz = ite;
    } 
    for(int n = 0; n < 10; ++n)
    {
        for(auto it: inlp)
        {
            if(*it == 0)
                continue;
            int64_t cnt = std::abs(*it);
            if(*it < 0)
                for(int64_t n = 0; n < cnt % (inl.size() - 1); ++n)
                    swap_left(inl, it);
            else
                for(int64_t n = 0; n < cnt % (inl.size() - 1); ++n)
                    swap_right(inl, it);
        }
    }
    int64_t r { 0 };
    for(int n = 0; n < 3; ++n)
    {
        advance(inl, itz, 1000);
        r += *itz;
    }
    return r;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}