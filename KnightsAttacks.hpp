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
#include <glpk.h>
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

vector<double> linsolve_glpk(vector<vector<double> > const & a, vector<double> const & b, vector<double> const & c) {
    int preserved_term_out = glp_term_out(GLP_OFF);
    int h = b.size();
    int w = c.size();
    glp_prob *lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MAX);
    glp_add_cols(lp, w);
    repeat (x, w) {
        glp_set_col_bnds(lp, x + 1, GLP_LO, 0, INFINITY); // non-negative
        glp_set_obj_coef(lp, x + 1, c[x]);
    }
    glp_add_rows(lp, h);
    repeat (y, h) {
        glp_set_row_bnds(lp, y + 1, GLP_UP, - INFINITY, b[y]);
    }
    vector<int> ia, ja;
    vector<double> ar;
    ia.push_back(0);
    ja.push_back(0);
    ar.push_back(0);  // dummy for 1-based
    constexpr double eps = 1e-8;
    repeat (y, h) repeat (x, w) {
        if (abs(a[y][x]) > eps) {
            ia.push_back(y + 1);
            ja.push_back(x + 1);
            ar.push_back(a[y][x]);
        }
    }
    glp_load_matrix(lp, ar.size() - 1, ia.data(), ja.data(), ar.data());
    glp_simplex(lp, NULL);
    assert (glp_get_status(lp) == GLP_OPT);
    vector<double> result(w);
    repeat (x, w) {
        result[x] = glp_get_col_prim(lp, x + 1);
    }
    glp_delete_prob(lp);
    glp_free_env();
    glp_term_out(preserved_term_out);
    return result;
}

void linsolve_tableau(vector<double> & z, vector<vector<double> > & tableau, vector<double> & k, vector<int> & ix) {
vector<double> debug_c = z;
    constexpr double eps = 1e-8;
    vector<vector<double > > & m = tableau;
    int h = k.size();
    int w = z.size();
    while (true) {
        // select column
        int x = min_element(whole(z)) - z.begin();
        if (z[x] > - eps) break;
        // select row
        vector<double> delta(h, INFINITY);
        repeat (y, h) if (m[y][x] > + eps) {
            delta[y] = k[y] / m[y][x];
        }
        int y = min_element(whole(delta)) - delta.begin();
        if (isinf(delta[y])) break;
        // eliminate
        ix[y] = x;
        double m_y_x = m[y][x];
        repeat (j, w) {
            m[y][j] /= m_y_x;
        }
        k[y] /= m_y_x;
        double z_x = z[x];
        repeat (j, w) {
            z[j] -= z_x * m[y][j];
        }
        repeat (i, h) if (i != y) {
            double m_i_x = m[i][x];
            repeat (j, w) {
                m[i][j] -= m_i_x * m[y][j];
            }
            k[i] -= m_i_x * k[y];
        }
    }
}

/*
 * max : c^T x
 * sub.to. : Ax <= b
 *            x >= 0
 */
vector<double> linsolve(vector<vector<double> > const & a, vector<double> const & b, vector<double> const & c) {
    // prepare
    constexpr double eps = 1e-8;
    int h = b.size();
    int w = c.size();
    // make tableau
    int artificial = 0;
    vector<vector<double> > tableau(h, vector<double>(w + h));  // the tableau
    vector<double> k = b;
    repeat (y, h) {
        if (b[y] >= 0) {
            copy(whole(a[y]), tableau[y].begin());
            tableau[y][w + y] = 1;  // slack variable
        } else {
            repeat (x, w) {
                tableau[y][x] = - a[y][x];
            }
            tableau[y][w + y] = -1;  // surplus variable
            k[y] *= -1;
            artificial += 1;
        }
    }
    vector<int> ix(h);  // labels of rows
    iota(whole(ix), w);
    // two-phase method
    if (artificial) {
fprintf(stderr, "artificial %d\n", artificial);
        vector<double> z(w + h + artificial);  // target function
        int var = 0;
        repeat (y, h) {
            tableau[y].resize(w + h + artificial);
            if (b[y] < 0) {
                tableau[y][w + h + var] = 1;
                repeat (x, w + h) {
                    z[x] -= tableau[y][x];
                }
                ix[y] = w + h + var;
                ++ var;
            }
        }
        linsolve_tableau(z, tableau, k, ix);
        repeat (y, h) {
            assert (ix[y] < w + h);
            tableau[y].resize(w + h);
        }
    }
    // solve
    vector<double> z(w + h);  // target function
    repeat (x, w) z[x] = - c[x];
    if (artificial) {
        repeat (y, h) {
            int x = ix[y];
            int z_x = z[x];
            if (z_x) {
                repeat (j, w + h) {
                    z[j] -= z_x * tableau[y][j];
                }
            }
        }
    }
    linsolve_tableau(z, tableau, k, ix);
    // finalize
    vector<double> result(w + h);
    repeat (y, h) {
        result[ix[y]] = k[y];
    }
    return result;
}


const int knight_dy[] = { -1, -2, -2, -1, 1, 2, 2, 1 };
const int knight_dx[] = { 2, 1, -1, -2, -2, -1, 1, 2 };
const int knight_2dy[] = { -1, -1, 1, 1,  -1, -3, -3, -3, -3, -1, 1, 3, 3, 3, 3, 1,  0, -4, 0, 4 };
const int knight_2dx[] = { 1, -1, -1, 1,  3, 3, 1, -1, -3, -3, -3, -3, -1, 1, 3, 3,  4, 0, -4, 0 };
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
    vector<vector<double> > a(3 * sq_s, vector<double>(2 * sq_s));
    vector<double> b(3 * sq_s);
    repeat (i, sq_s) {  // relaxed binary constraints
        a[i][i] = 1;
        b[i] = 1;
    }
    repeat (y, s) repeat (x, s) {  // abs(board - knight) <= delta
        repeat (i, 8) {
            int ny = y + knight_dy[i];
            int nx = x + knight_dx[i];
            if (not is_on_field(ny, nx)) continue;
            a[sq_s + (y * s + x) * 2    ][ny * s + nx] += 1;
            a[sq_s + (y * s + x) * 2 + 1][ny * s + nx] -= 1;
        }
        a[sq_s + (y * s + x) * 2    ][sq_s + y * s + x] = -1;
        a[sq_s + (y * s + x) * 2 + 1][sq_s + y * s + x] = -1;
        b[sq_s + (y * s + x) * 2    ] = + board[y * s + x];
        b[sq_s + (y * s + x) * 2 + 1] = - board[y * s + x];
    }
    vector<double> c(2 * sq_s);
    repeat_from (i, sq_s, 2 * sq_s) {  // minimize sum of delta
        c[i] = -1;
    }
    vector<double> relaxed = linsolve(a, b, c);
repeat (y, s) {
repeat (x, s) {
fprintf(stderr, "%4.1lf", relaxed[y * s + x]);
}
fprintf(stderr, "\n");
}
    vector<char> result(sq_s);
    repeat (y, s) repeat (x, s) {
        result[y * s + x] = round(relaxed[y * s + x]);
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
