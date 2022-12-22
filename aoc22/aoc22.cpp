#include <iostream>
#include <string>
#include <array>
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

struct state
{
    int row_;
    int col_;
    int d_;
    auto  operator<=>(const state&) const = default;

};

bool valid(auto& mz, int row, int col)
{
    bool rb { mz[row].tiles_[col] == '.'};
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
        col =  mz[st.row_].to_ - 1;
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

state move(auto& mz, int n, state const& st)
{
    auto str { st};
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
    }
    return str;
}

state turn(state const& st, char t)
{
    int d { st.d_};
    switch(t)
    {
        case 'L':
            d += 3;
            break;
        case 'R':
            ++d;
            break;
        default:
            break;
    }
    if( d > 3)
        d -= 4;
    return {st.row_, st.col_, d};
}

auto pt1(auto const& in)
{
    auto mz { in.first};
    auto& ds { in.second};
    state st { 0, static_cast<int>(mz[0].from_), 0};
    for(auto& c: in.second)
        std::visit(overloaded{[&](int n){ st = move(mz, n, st);},
                                [&](char t){st = turn(st, t);}}, c); 
    return 1000 * (st.row_ + 1) + 4 * (st.col_ + 1) + st.d_;
}

struct fold_t
{
    std::pair<int, int> row_from_;
    std::pair<int, int> col_from_;
    std::pair<int, int> row_col_orig_;
    std::pair<int, int> row_adj_;
    std::pair<int, int> col_adj_;
    int dir_in_;
    int turn_to_;

    constexpr fold_t (int rf1, int rf2, int cf1, int cf2, int rcr, int rcc, int ra1, int ra2, int ca1, int ca2, int tf, int tt) :
        row_from_{rf1, rf2}, col_from_{cf1, cf2}, row_col_orig_{rcr, rcc}, row_adj_{ra1, ra2},  col_adj_{ca1, ca2}, dir_in_{tf}, turn_to_{tt}
    {}
};

constexpr fold_t folds[] =  {{   0,   1,  50, 100, 150,   0,  0,  1,  0,  0, 3, 0,}, // A->L
                             {   0,   1, 100, 150, 199,   0,  0,  0,  0,  1, 3, 3,}, // B->N
                             {   0,  50, 149, 150, 149,  99, -1,  0,  0,  0, 0, 2,}, // D->J
                             {  49,  50, 100, 150,  50,  99,  0,  1,  0,  0, 1, 2,}, // E->G
                             {  50, 100,  50,  51, 100,   0,  0,  0,  1,  0, 2, 1,}, // F->I
                             {   0,  50,  50,  51, 149,   0, -1,  0,  0,  0, 2, 0,}, // C->H
                             { 150, 200,  49,  50, 149,  50,  0,  0,  1,  0, 0, 3,}, // M->K
                             { 150, 200,   0,   1,   0,  50,  0,  0,  1,  0, 2, 1,}, // L->A
                             { 199, 200,   0,  50,   0, 100,  0,  0,  0,  1, 1, 1,}, // N->B
                             { 100, 150,  99, 100,  49, 149, -1,  0,  0,  0, 0, 2,}, // J->D
                             {  50, 100,  99, 100,  49, 100,  0,  0,  1,  0, 0, 3,}, // G->E
                             { 100, 101,   0,  50,  50,  50,  0,  1,  0,  0, 3, 0,}, // I->F
                             { 100, 150,   0,   1,  49,  50, -1,  0,  0,  0, 2, 0,}, // H->C
                             { 149, 150,  50, 100, 150,  49,  0,  1,  0,  0, 1, 2,}, // K->M
};

constexpr fold_t foldsx[] = { {   0,   1,   8,  12,   4,   3,  0,  0,  0, -1, 0, 1}, // A->D
                             {   0,   4,   8,   9,   4,   4,  0,  0,  1,  0, 2, 1}, // B->E
                             {   0,   4,  11,  12,   8,  15, -1,  0,  0,  0, 0, 2}, // C->L
                             {   4,   8,   0,   1,  11,  15,  0,  0, -1,  0, 2, 3}, // F->N
                             {   4,   8,  11,  12,   8,  15,  0,  0, -1,  0, 0, 1}, // G->K
                             {   7,   8,   0,   4,  11,  11, -1,  0,  0,  0, 1, 3}, // H->M
                             {   8,  12,   8,   9,   7,   7,  0,  0,  0, -1, 2, 3}, // J->I
                             {   4,   5,   0,   4,   0,  11,  0,  0,  0, -1, 3, 1}, // D->A
                             {   4,   5,   4,   8,   0,   8,  0,  1,  0,  0, 3, 0}, // E->B
                             {   8,  12,  15,  16,   3,  11,  0,  0,  0, -1, 0, 2}, // L->C
                             {  11,  12,  12,  16,   7,   0,  0, -1,  0,  0, 1, 0}, // N->F
                             {   8,   9,  12,  16,   7,  11, -1,  0,  0,  0, 3, 2}, // K->G
                             {  11,  12,   8,  12,   7,   3,  0,  0,  0, -1, 1, 3}, // M->H
                             {   7,   8,   4,   8,   8,  11,  0, -1,  0,  0, 1, 0}, // I->J
};

std::pair<int, int> fold(int row, int col, fold_t const& f)
{
    int rf = f.row_col_orig_.first  + f.row_adj_.first * (row - f.row_from_.first) + f.row_adj_.second * (col - f.col_from_.first);
    int cf = f.row_col_orig_.second + f.col_adj_.first * (row - f.row_from_.first) + f.col_adj_.second * (col - f.col_from_.first);
    return { rf, cf };
}

state fold_state(state const& st)
{
    for (auto& f : folds)
    {
        if (st.d_ == f.dir_in_ && st.row_ >= f.row_from_.first && st.row_ < f.row_from_.second && st.col_ >= f.col_from_.first && st.col_ < f.col_from_.second)
        {
            auto [r, c] = fold(st.row_, st.col_, f);
            return { r, c, f.turn_to_ };
        }
    }
    return {st};
}

state move_up2(auto& mz, state const& st)
{
    auto rs = fold_state(st);
    if( rs == st)
        --rs.row_;
    if (valid(mz, rs.row_, rs.col_))
        return rs ;
    return st;
}

state move_down2(auto& mz, state const& st)
{
    auto rs = fold_state(st);
    if( rs == st)
        ++rs.row_;
    if (valid(mz, rs.row_, rs.col_))
        return rs ;
    return st;
}

state move_left2(auto& mz, state const& st)
{
    auto rs = fold_state(st);
    if( rs == st)
       --rs.col_;
    if (valid(mz, rs.row_, rs.col_))
        return rs;
    return st;
}

state move_right2(auto& mz, state const& st)
{
    auto rs = fold_state(st);
    if( rs == st)
        ++rs.col_;
    if (valid(mz, rs.row_, rs.col_))
        return rs;
    return st;
}

state move2(auto& mz, int n, state const& st)
{
    auto str{ st };
    for (auto cnt = 0; cnt < n; ++cnt)
    {
        state rsb;
        switch (str.d_)
        {
        case 0:
            rsb = move_right2(mz, str);
            break;
        case 1:
            rsb = move_down2(mz, str);
            break;
        case 2:
            rsb = move_left2(mz, str);
            break;
        case 3:
            rsb = move_up2(mz, str);
            break;
        }
        if( rsb == str)
           break;
        str = rsb;
    }
    return str;
}

auto pt2(auto const& in)
{
    auto mz{ in.first };
    auto& ds{ in.second };
    state st{ 0, static_cast<int>(mz[0].from_), 0 };
    for (auto& c : in.second)
    {
        std::visit(overloaded{ [&](int n) { st = move2(mz, n, st); },
                            [&](char t) {st = turn(st, t); }}, c);
    }
    return 1000 * (st.row_ + 1) + 4 * (st.col_ + 1) + st.d_;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
}