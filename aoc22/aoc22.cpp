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

auto get_c_for_dir(int d)
{
    constexpr char ad[]{">v<^"};
    return ad[d];
}

state move(auto& mz, int n, state const& st)
{
    auto str { st};
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
    return {st.row_, st.col_, d};
}

// 150154 too high
// 127226 too high
// 13566 OK
//
auto pt1(auto const& in)
{
    auto mz { in.first};
    auto& ds { in.second};
    state st { 0, static_cast<int>(mz[0].from_), 0};
    for(auto& c: in.second)
        std::visit(overloaded{[&](int n){ st = move(mz, n, st);},
                                [&](char t){st = turn(st, t);}}, c); 
//    report_maze(mz);
    return 1000 * (st.row_ + 1) + 4 * (st.col_ + 1) + st.d_;
}

struct fold_t
{
    std::pair<int, int> row_from_;
    std::pair<int, int> col_from_;
    std::pair<int, int> row_col_orig_;
    std::pair<int, int> row_adj_;
    std::pair<int, int> col_adj_;
    int turn_to_;

    constexpr fold_t (int rf1, int rf2, int cf1, int cf2, int rcr, int rcc, int ra1, int ra2, int ca1, int ca2, int t) :
        row_from_{rf1, rf2}, col_from_{cf1, cf2}, row_col_orig_{rcr, rcc}, row_adj_{ra1, ra2},  col_adj_{ca1, ca2}, turn_to_{t}
    {}
};

constexpr fold_t folds[] = { {   0,   1,  50,  99, 150,   0,  0,  1,  0,  0, 0,}, // A->L
                             {   0,   1, 100, 149, 199,   0,  0,  0,  0,  1, 3,}, // B->N
                             {   0,  49, 149, 150, 149,  50, -1,  0,  0,  0, 2,}, // D->J
                             {  49,  50, 100, 149,  50,  99,  0,  1,  0,  0, 2,}, // E->G
                             {  50,  99,  50,  51,  50,   0,  0,  0,  1,  0, 1,}, // F->I
                             {   0,  49,  50,  51, 149,   0, -1,  0,  0,  0, 0,}, // C->H
                             { 150, 199,   0,   1,   0,  50,  0,  0,  1,  0, 1,}, // L->A
                             { 199, 200,   0,  49,   0, 100,  0,  0,  0,  1, 1,}, // N->B
                             { 100, 149,  49,  50,  49, 149, -1,  0,  0,  0, 2,}, // J->D
                             {  50,  99,  99, 100,  49, 100,  0,  0,  1,  0, 3,}, // G->E
                             { 100, 101,   0,  49,  50,  50,  0,  1,  0,  0, 0,}, // I->F
                             { 100, 149,   0,   1,  49,  50, -1,  0,  0,  0, 0,}, // H->C
};

constexpr fold_t foldsx[] = { {   0,   1,  10,  14,   5,   4,  0,  0,  0, -1, 1,}, // A->D
                             {   0,   4,  10,  11,   5,   5,  0,  0,  1,  0, 1,}, // B->E
                             {   0,   4,  14,  15,  10,  19, -1,  0,  0,  0, 2,}, // C->L
                             {   5,   9,   0,   1,  14,  19,  0,  0, -1,  0, 3,}, // F->N
                             {   5,   9,  14,  15,  10,  19,  0,  0, -1,  0, 1,}, // G->K
                             {   9,  10,   0,   4,  14,  14, -1,  0,  0,  0, 3,}, // H->M
                             {  10,  14,  10,  11,   9,   9,  0,  0,  0, -1, 3,}, // J->I
                             {   5,   6,   0,   4,   0,  14,  0,  0,  0, -1, 1,}, // D->A
                             {   5,   6,   5,   9,   0,  10,  0,  1,  0,  0, 0,}, // E->B
                             {  10,  14,  19,  20,   4,  14,  0,  0,  0, -1, 2,}, // L->C
                             {  14,  15,  15,  19,   9,   0,  0, -1,  0,  0, 0,}, // N->F
                             {   9,  10,  15,  19,   9,  14, -1,  0,  0,  0, 2,}, // K->G
                             {  14,  15,  10,  14,   9,   4, -1,  0,  0,  0, 3,}, // M->H
                             {   9,  10,   5,   9,  10,  14,  0, -1,  0,  0, 0,}, // I->J
};

std::pair<int, int> fold(int row, int col, fold_t const& f)
{
    int rf = f.row_col_orig_.first  + f.row_adj_.first * (row - f.row_from_.first) + f.row_adj_.second * (col - f.col_from_.first);
    int cf = f.row_col_orig_.second + f.col_adj_.first * (row - f.row_from_.first) + f.col_adj_.second * (col - f.col_from_.first);
    return { rf, cf };
}

void test_fold()
{
    // A->L
    auto fAL{ fold(0, 65, folds[0]) };
    std::cout << "A->L from 0, 65 to " << fAL.first << ", " << fAL.second << "\n";
    // B->N
    auto fBN{ fold(0, 127, folds[1]) };
    std::cout << "B->N from 0, 127 to " << fBN.first << ", " << fBN.second << "\n";
    // D->J
    auto fDJ{ fold(13, 149, folds[2]) };
    std::cout << "D->J from 13, 149 to " << fDJ.first << ", " << fDJ.second << "\n";
    // E->G
    auto fEG{ fold(49, 103, folds[3]) };
    std::cout << "E->G from 49, 103 to " << fEG.first << ", " << fEG.second << "\n";
    // F->I
    auto fFI{ fold(53, 50, folds[4]) };
    std::cout << "F->I from 53, 50 to " << fFI.first << ", " << fFI.second << "\n";
    // C->H
    auto fCH{ fold(37, 50, folds[5]) };
    std::cout << "C->H from 37, 50 to " << fCH.first << ", " << fCH.second << "\n";
    // L->A
    auto fLA{ fold(fAL.first, fAL.second, folds[6]) };
    std::cout << "L->A from " << fAL.first << ", " << fAL.second << " to " << fLA.first << ", " << fLA.second << "\n";
    // N->B
    auto fNB{ fold(fBN.first, fBN.second, folds[7]) };
    std::cout << "N->B from " << fBN.first << ", " << fBN.second << " to " << fNB.first << ", " << fNB.second << "\n";
    // J->D
    auto fJD{ fold(fDJ.first, fDJ.second, folds[8]) };
    std::cout << "J->D from " << fDJ.first << ", " << fDJ.second << " to " << fJD.first << ", " << fJD.second << "\n";
    // G->E
    auto fGE{ fold(fEG.first, fEG.second, folds[9]) };
    std::cout << "G->E from " << fEG.first << ", " << fEG.second << " to " << fGE.first << ", " << fGE.second << "\n";
    // I->F
    auto fIF{ fold(fFI.first, fFI.second, folds[10]) };
    std::cout << "I->F from " << fFI.first << ", " << fFI.second << " to " << fIF.first << ", " << fIF.second << "\n";
    // H->C
    auto fHC{ fold(fCH.first, fCH.second, folds[11]) };
    std::cout << "H->C from " << fCH.first << ", " << fCH.second << " to " << fHC.first << ", " << fHC.second << "\n";
}

state fold_state(int row, int col)
{
    for (auto& f : folds)
    {
        if (row >= f.row_from_.first && row < f.row_from_.second && col >= f.col_from_.first && col < f.col_from_.second)
        {
            auto [r, c] = fold(row, col, f);
            return { r, c, f.turn_to_ };
        }
    }
    std::cout << "bad fold\n";
    return { 0, 0, 0 };
}

std::pair<state, bool> move_up2(auto& mz, state const& st)
{
    auto rs{ st };
    int row = st.row_;
    --row;
    if (!valid_row_for_col(mz, row, st.col_))
    {
        rs = fold_state(rs.row_, rs.col_);
    }
    else
        rs.row_ = row;
    if (valid(mz, rs.row_, rs.col_))
        return { rs, true };
    return { st, false };
}

std::pair<state, bool>  move_down2(auto& mz, state const& st)
{
    auto rs{ st };
    int row = st.row_;
    ++row;
    if (!valid_row_for_col(mz, row, st.col_))
    {
        rs = fold_state(rs.row_, rs.col_);
    }
    else
        rs.row_ = row;
    if (valid(mz, rs.row_, rs.col_))
        return { rs, true };
    return { st, false };
}

std::pair<state, bool>  move_left2(auto& mz, state const& st)
{
    auto rs{ st };
    if (rs.col_ == mz[st.row_].from_)
        rs = fold_state(st.row_, st.col_);
    else
        --rs.col_;
    if (valid(mz, rs.row_, rs.col_))
        return { rs, true };
    return { st, false };
}

std::pair<state, bool>  move_right2(auto& mz, state const& st)
{
    auto rs{ st };
    ++rs.col_;
    if (rs.col_ == mz[st.row_].to_)
        rs = fold_state(st.row_, st.col_);
    if (valid(mz, rs.row_, rs.col_))
        return { rs, true };
    return { st, false };
}

state move2(auto& mz, int n, state const& st)
{
    auto str{ st };
    char c = get_c_for_dir(st.d_);
    mz[st.row_].tiles_[st.col_] = c;
    for (auto cnt = 0; cnt < n; ++cnt)
    {
        std::pair<state, bool> rsb;
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
        if (!rsb.second)
            break;
        str = rsb.first;
        mz[str.row_].tiles_[str.col_] = c;
    }
    return str;
}

// 151007 too high
// 151024...
auto pt2(auto const& in)
{
    auto mz{ in.first };
    auto& ds{ in.second };
    state st{ 0, static_cast<int>(mz[0].from_), 0 };
    for (auto& c : in.second)
        std::visit(overloaded{ [&](int n) { st = move2(mz, n, st); },
                                [&](char t) {st = turn(st, t); } }, c);
    report_maze(mz);
    return 1000 * (st.row_ + 1) + 4 * (st.col_ + 1) + st.d_;
}

int main()
{
    auto in {get_input()};

    std::cout << "pt1 = " << pt1(in) << "\n";
    std::cout << "pt2 = " << pt2(in) << "\n";
//    test_fold();
}