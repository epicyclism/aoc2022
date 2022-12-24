#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#include <experimental/mdspan>

namespace stdex = std::experimental;

struct blizzard_t
{
    int x_;
    int y_;
    char d_;
};

struct arena_t
{
    std::vector<blizzard_t> b_;
    size_t w_;
    size_t h_;
    size_t s_;
    size_t e_;
    void step()
    {
        for(auto& b: b_)
        {
            switch(b.d_)
            {
                case '>':
                    ++b.x_;
                    if(b.x_ == w_ - 1)
                        b.x_ = 1;
                    break;
                case '<':
                    --b.x_;
                    if(b.x_ == 0)
                        b.x_ = w_ - 2;
                   break;
                case '^':
                    --b.y_;
                    if( b.y_ == 0)
                        b.y_ = h_ - 2;
                    break;
                case 'v':
                    ++b.y_;
                    if(b.y_ == h_ - 1)
                        b.y_ = 1;
                    break;
            }
        }
    }
    std::vector<char> render() const
    {
        std::vector<char> a(w_ * h_, '.');
        std::fill_n(a.begin(), w_, '#');
        std::fill_n(a.end() - w_, w_, '#');
        a[s_] = '.';
        a[e_] = '.';
        for(int l = 1; l < h_ - 1; ++l)
        {
            a[l * w_] = '#';
            a[l * w_ + w_- 1] = '#';
        }
        for(auto const& b: b_)
            a[b.x_ + w_ * b.y_] = b.d_;
        return a;
    }
};

auto get_input()
{
    arena_t a;
    a.s_ = 1;
    std::string ln;
    a.h_ = 0;
    while (std::getline(std::cin, ln))
    {
        a.w_ = ln.size();
        for(int n = 1; n < a.w_ - 1; ++n)
            if( ln[n] == '<' || ln[n] == '>' || ln[n] == '^' || ln[n] == 'v')
                a.b_.emplace_back(n, a.h_, ln[n]);
        ++a.h_;
    }
    a.s_ = 1;
    a.e_ = a.w_ * a.h_ - 2;

    return a;
}

void dump(auto const& a)
{
    std::cout << "w = " << a.w_ << ", h = " << a.h_ << "\n";
    std::cout << "s = " << a.s_ << ", e = " << a.e_ << "\n";
    std::cout << a.b_.size() << " blizzards\n";
    for(auto& b: a.b_)
        std::cout << b.x_ << ", " << b.y_ << " : " << b.d_ << "\n";
}

std::ostream& operator<<( std::ostream& os, arena_t const& a)
{
    auto ar { a.render()};
    int cnt {0};
    for(auto c: ar)
    {
        if(cnt == a.w_)
        {
            os << "\n";
            cnt = 0;
        }
        os << c ;
        ++cnt;
    }
    return os;
}

auto pt0(auto const& in)
{
    auto a {in};
    for(int n = 0; n < 19; ++n)
    {
        std::cout << n << "\n" << a << "\n";
        a.step();
    }
    return 0;
}

auto pt1(auto const& in)
{
    auto a {in};
    std::queue<std::pair<int, int>> q; // pos, time
    int time {0};
    std::vector<char> ar;
    q.push({a.s_, 0});
    while(!q.empty())
    {
        auto u = q.front();
        if(u.second == time)
        {
            a.step();
            ar = a.render();
            std::cout << time << "\n" << a << "\n";
            ++time;
            if( time > 19)
                break;
        }
        std::cout << u.first << ", " << u.second << "\n";
        q.pop();
        auto current = u.first;
        if(current == a.e_)
            break;
        if(ar[current] != '.')
            continue;
        q.push({current, time}); // stay
        if(ar[current - 1] == '.')
            q.push({current - 1, time}); // left
        if(ar[current + 1] == '.')
            q.push({current + 1, time}); // right
        if(ar[current - a.w_] == '.')
            q.push({current - a.w_, time}); // up
        if(ar[current + a.w_] == '.')
            q.push({current + a.w_, time}); // down
    }
    return time;
}

auto pt2(auto const& in)
{
    return 0;
}

int main()
{
    auto in {get_input()};
//    dump(in);
//   pt0(in);
    std::cout << "pt1 = " << pt1(in)<< "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}