#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>

#include <ctre_inc.h>

struct maze_line
{
    size_t from_;
    size_t to_;
    std::string tiles_;
};

using cmd = std::variant<int, char>;
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

auto get_input()
{
    std::vector<maze_line> mz;
    std::vector<cmd> pwd;

    std::string ln;
    while(std::getline(std::cin, ln))
    {
        if(ln.empty())
            break;
        auto from = ln.find_first_not_of(" ");
        auto to = ln.size();
        mz.emplace_back(from, to, ln);
    }
    std::getline(std::cin, ln);
    for(auto [m, cnt, dir]: ctre::range<R"((\d+)|([DLRU]))">(ln))
        if(cnt)
            pwd.emplace_back(sv_to_t<int>(cnt));
        else
            pwd.emplace_back(dir.view()[0]);

    return std::pair{mz, pwd};
}

void report_maze(auto const& in)
{
    for(auto& mr : in)
        std::cout << mr.tiles_ << "\n";
    std::cout << "\n";
}

void report_input(auto const& in)
{
    for(auto& mr : in.first)
        std::cout << mr.from_ << " - " << mr.to_ << " " << mr.tiles_ << "\n";
    std::cout << "\n";
    for(auto& c: in.second)
        std::visit(overloaded{[](auto a){ std::cout << a << " " << "\n";}}, c); 
}

struct state
{
    int row_;
    int col_;
    int d_;
};

bool valid(auto& mz, int row, int col)
{
    bool rb { mz[row].tiles_[col] != '#' && mz[row].tiles_[col] != ' '};
//    std::cout << row << ", " << col << " = " << mz[row].tiles_[col] << (rb ? " valid\n":" invalid\n");
    return rb;
}

bool valid_row_for_col(auto& mz, int row, int col)
{
    return row >= 0 && row < mz.size() && col >= mz[row].from_ && col < mz[row].to_ ;
}

std::pair<state, bool> move_up(auto& mz, state const& st)
{
    int row = st.row_;
    --row;
    if(!valid_row_for_col(mz, row, st.col_))
    {
        row = mz.size() - 1;
        while( !valid_row_for_col(mz, row, st.col_)) 
            --row;  
    }
    if(valid(mz, row, st.col_))
        return {{row, st.col_, st.d_}, true};
    return {st, false};
}

std::pair<state, bool>  move_down(auto& mz, state const& st)
{
    int row = st.row_;
    ++row;
    if(!valid_row_for_col(mz, row, st.col_))
    {
        row = 0;
        while( !valid_row_for_col(mz, row, st.col_)) 
            ++row;  
    }
    if(valid(mz, row, st.col_))
        return {{row, st.col_, st.d_}, true};
    return {st, false};
}

std::pair<state, bool>  move_left(auto& mz, state const& st)
{
    int col = st.col_;
    if( col == mz[st.row_].from_)
        col =  mz[st.row_].to_;
    else
        --col;
    if(valid(mz, st.row_, col))
        return {{st.row_, col, st.d_}, true};
    return {st, false};
}

std::pair<state, bool>  move_right(auto& mz, state const& st)
{
    int col = st.col_;
    ++col;
    if( col == mz[st.row_].to_ )
        col = mz[st.row_].from_;
    if(valid(mz, st.row_, col))
        return {{st.row_, col, st.d_}, true};
    return {st, false};
}

auto get_c_for_dir(int d)
{
    constexpr char ad[]{">v<^"};
    return ad[d];
}

state move(auto& mz, int n, state const& st)
{
    auto str { st};
    std::cout << "move " << n << " from " << st.row_ << ", " << st.col_ << "\n";
    char c = get_c_for_dir(st.d_);
    mz[st.row_].tiles_[st.col_] = c;
    for(auto cnt = 0; cnt < n; ++cnt)
    {
        std::pair<state, bool> rsb;
        switch(st.d_)
        {
            case 0:
                rsb = move_right(mz, str);
                break;
            case 1:
                rsb = move_down(mz, str);
                break; 
             case 2:
                rsb = move_left(mz, str);
                break;
             case 3:
                rsb = move_up(mz, str);
                break;
        }
        if(!rsb.second)
            break;
        str = rsb.first;
        mz[str.row_].tiles_[str.col_] = c;
    }
    std::cout << "move to " << str.row_ << ", " << str.col_ << "\n";
    return str;
}

state turn(state const& st, char t)
{
    int d { st.d_};
    switch(t)
    {
        case 'L':
            --d;
            if(d < 0)
                d = 3;
            break;
        case 'R':
            ++d;
            if( d > 3)
                d = 0;
            break;
        default:
            break;
    }
    std::cout << "turn " << get_c_for_dir(d) << "\n";
    return {st.row_, st.col_, d};
}

// 150154 too high
// 127226 too high
auto pt1(auto const& in)
{
    auto mz { in.first};
    auto& ds { in.second};
    state st { 0, static_cast<int>(mz[0].from_), 0};
    for(auto& c: in.second)
        std::visit(overloaded{[&](int n){ st = move(mz, n, st);},
                                [&](char t){st = turn(st, t);}}, c); 
    report_maze(mz);
    return 1000 * (st.row_ + 1) + 4 * (st.col_ + 1) + st.d_;
}

auto pt2(auto const& in)
{
//    report_input(in);
    return in.first.size();
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}