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

constexpr double ticks_per_sec = 2800000000;
constexpr double ticks_per_sec_inv = 1.0 / ticks_per_sec;
inline double rdtsc() { // in seconds
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo) * ticks_per_sec_inv;
}
constexpr int TLE = 10; // sec

default_random_engine gen;

const int knight_dy[] = { -1, -2, -2, -1, 1, 2, 2, 1 };
const int knight_dx[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
constexpr int MAX_S = 500;
int s;
char board[MAX_S * MAX_S];
bool is_on_field(int y, int x) { return 0 <= y and y < s and 0 <= x and x < s; }
char knight[MAX_S * MAX_S];
char attacked[MAX_S * MAX_S];
int current_score;
void flip(int y, int x) {
    repeat (i, 8) {
        int ny = y + knight_dy[i];
        int nx = x + knight_dx[i];
        if (not is_on_field(ny, nx)) continue;
        current_score -= abs(board[ny * s + nx] - attacked[ny * s + nx]);
        attacked[ny * s + nx] += (knight[y * s + x] ? -1 : +1);
        current_score += abs(board[ny * s + nx] - attacked[ny * s + nx]);
    }
    knight[y * s + x] = not knight[y * s + x];
}
int get_delta(int y, int x) {
    int delta = 0;
    repeat (i, 8) {
        int ny = y + knight_dy[i];
        int nx = x + knight_dx[i];
        if (not is_on_field(ny, nx)) continue;
        delta -= abs(board[ny * s + nx] - attacked[ny * s + nx]);
        delta += abs(board[ny * s + nx] - (attacked[ny * s + nx] + (knight[y * s + x] ? -1 : +1)));
    }
    return delta;
}

vector<char> solve() {
    double clock_begin = rdtsc();
    int sq_s = s * s;
    current_score = accumulate(board, board + sq_s, 0);
    const int knight_remaining[] = { 3, 1, 0, 2, 4, 6, 7, 5 };
    repeat (y, s) repeat (x, s) {
        int required = 0;
        repeat (i, 8) {
            int ny = y + knight_dy[i];
            int nx = x + knight_dx[i];
            if (not is_on_field(ny, nx)) continue;
            if (board[ny * s + nx] - attacked[ny * s + nx] > knight_remaining[i]) required += 1;
            if (board[ny * s + nx] - attacked[ny * s + nx] <= 0) required -= 1;
        }
        if (required >= 1) flip(y, x);
    }
    vector<char> result(knight, knight + sq_s);
    int best_score = current_score;
    double t = 0;
    double temp = INFINITY;
int force = 0;
    for (int iteration = 0; ; ++ iteration) {
        double clock_end = rdtsc();
        t = (clock_end - clock_begin) / TLE;
        if (t > 0.98) {
#ifdef VISUALIZE
fprintf(stderr, "t = %.2f: iteration = %d: force = %d\n", t, iteration, force);
#endif
            break;
        }
        temp = (1 - t);
        repeat (y, s) {
            repeat (x, s) {
                int delta = get_delta(y, x);
                if (delta <= 0 or bernoulli_distribution(exp(- delta / temp))(gen)) {
force += (delta > 0);
                    flip(y, x);
                    if (current_score < best_score) {
                        copy(knight, knight + sq_s, result.begin());
                        best_score = current_score;
#ifdef VISUALIZE
fprintf(stderr, "t = %.2f: iteration %d: score = %d\n", t, iteration, current_score);
#endif
                    }
                }
            }
        }
    }
    return result;
}

class KnightsAttacks { public: vector<string> placeKnights(vector<string> board); };
vector<string> KnightsAttacks::placeKnights(vector<string> a_board) {
    s = a_board.size();
    repeat (y, s) repeat (x, s) board[y * s + x] = a_board[y][x] - '0';
    vector<char> result = solve();
    assert (result.size() == s * s);
    vector<string> str(s, string(s, '.'));
    repeat (y, s) repeat (x, s) if (result[y * s + x]) str[y][x] = 'K';
    return str;
}
