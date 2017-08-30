#pragma GCC optimize "O3"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#define repeat(i, n) for (int i = 0; (i) < int(n); ++(i))
#define repeat_from(i, m, n) for (int i = (m); (i) < int(n); ++(i))
#define repeat_reverse(i, n) for (int i = (n)-1; (i) >= 0; --(i))
#define repeat_from_reverse(i, m, n) for (int i = (n)-1; (i) >= int(m); --(i))
#define whole(x) begin(x), end(x)
#define unittest_name_helper(counter) unittest_ ## counter
#define unittest_name(counter) unittest_name_helper(counter)
#define unittest __attribute__((constructor)) void unittest_name(__COUNTER__) ()
using ll = long long;
using namespace std;
template <class T> inline void setmax(T & a, T const & b) { a = max(a, b); }
template <class T> inline void setmin(T & a, T const & b) { a = min(a, b); }
const int dy[] = { -1, 1, 0, 0 };
const int dx[] = { 0, 0, 1, -1 };
bool is_on_field(int y, int x, int h, int w) { return 0 <= y and y < h and 0 <= x and x < w; }

double rdtsc() { // in seconds
    constexpr double ticks_per_sec = 2500000000;
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32 | lo) / ticks_per_sec;
}
constexpr int TLE = 10; // sec

default_random_engine gen;

const int knight_dy[] = { -1, -2, -2, -1, 1, 2, 2, 1 };
const int knight_dx[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
int calculate_score_at(int y, int x, vector<char> const & knight, int s, vector<char> const & board) {
    auto is_on_field = [&](int y, int x) { return 0 <= y and y < s and 0 <= x and x < s; };
    int attacked = 0;
    repeat (i, 8) {
        int ny = y + knight_dy[i];
        int nx = x + knight_dx[i];
        if (not is_on_field(ny, nx)) continue;
        attacked += bool(knight[ny * s + nx]);
    }
    return abs(attacked - board[y * s + x]);
}
int calculate_score(vector<char> const & knight, int s, vector<char> const & board) {
    int result = 0;
    repeat (y, s) {
        repeat (x, s) {
            result += calculate_score_at(y, x, knight, s, board);
        }
    }
    return result;
}
int calculate_score_around(int y, int x, vector<char> const & knight, int s, vector<char> const & board) {
    auto is_on_field = [&](int y, int x) { return 0 <= y and y < s and 0 <= x and x < s; };
    int result = 0;
    repeat (i, 8) {
        int ny = y + knight_dy[i];
        int nx = x + knight_dx[i];
        if (not is_on_field(ny, nx)) continue;
        result += calculate_score_at(ny, nx, knight, s, board);
    }
    return result;
}

vector<char> solve(int s, vector<char> const & board) {
    double clock_begin = rdtsc();
    int sq_s = s * s;
    vector<char> knight(sq_s, false);
    int current_score = calculate_score(knight, s, board);
    vector<char> result = knight;
    int best_score = current_score;
    for (int iteration = 0; ; ++ iteration) {
        if (iteration % 1000 == 0) {
            double clock_end = rdtsc();
            if (clock_end - clock_begin > TLE * 0.95) {
                break;
            }
        }
        int y = uniform_int_distribution<int>(0, s - 1)(gen);
        int x = uniform_int_distribution<int>(0, s - 1)(gen);
        int delta = 0;
        delta -= calculate_score_around(y, x, knight, s, board);
        knight[y * s + x] = not knight[y * s + x];
        delta += calculate_score_around(y, x, knight, s, board);
        if (delta <= 0) {
            current_score += delta;
            if (current_score < best_score) {
                result = knight;
                best_score = current_score;
            }
        } else {
            knight[y * s + x] = not knight[y * s + x];
        }
    }
    return knight;
}

class KnightsAttacks { public: vector<string> placeKnights(vector<string> board); };
vector<string> KnightsAttacks::placeKnights(vector<string> a_board) {
    int s = a_board.size();
    vector<char> board(s * s);
    repeat (y, s) repeat (x, s) board[y * s + x] = a_board[y][x] - '0';
    vector<char> result = solve(s, board);
    assert (result.size() == s * s);
    vector<string> str(s, string(s, '.'));
    repeat (y, s) repeat (x, s) if (result[y * s + x]) str[y][x] = 'K';
    return str;
}
