// Extended Euclidean Algorithm
// C++17.  Compile: g++ -std=c++17 -O2 -o extgcd implementation.cpp
//
// Three functions: the algorithm, solving a*x + b*y = c, and merging congruences.
// The modular inverse -- the most common use of extgcd -- has its own concept:
// ../modular-multiplicative-inverse/

#include <iostream>
#include <optional>
#include <vector>

using ll = long long;

// (a * b) % mod, exact for any mod below 2^63. __int128 holds the product first.
ll mulMod(ll a, ll b, ll mod) { return (ll)((__int128)a * b % mod); }

// Least non-negative residue: '%' can return a negative value in C++.
ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }

// ---------------------------------------------------------------------------
// 1. The algorithm
// ---------------------------------------------------------------------------

// Returns g = gcd(a,b) and sets x, y with a*x + b*y == g.
//
// The recursive call gives  b*x1 + (a mod b)*y1 = g.  Substituting
// a mod b = a - (a/b)*b and regrouping by a and b gives
//     a*y1 + b*(x1 - (a/b)*y1) = g
// so x = y1 and y = x1 - (a/b)*y1. That is the whole derivation.
//
// The coefficients stay small, so nothing overflows inside here.
// O(log min(a,b))
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// ---------------------------------------------------------------------------
// 2. Solving a*x + b*y = c
// ---------------------------------------------------------------------------

struct Solution {
    ll x, y;          // one solution
    ll stepX, stepY;  // every solution is x + k*stepX, y - k*stepY
};

// Solvable exactly when gcd(a,b) divides c.
//
// Two easy mistakes are handled here:
//   - the Bezout pair solves = g, so it must be scaled by c/g
//   - the steps are b/g and a/g, NOT b and a. With g > 1, stepping by b skips
//     g-1 out of every g solutions.
// Precondition: a and b not both zero.   O(log min(a,b))
std::optional<Solution> solveDiophantine(ll a, ll b, ll c) {
    ll x, y;
    ll g = extgcd(a < 0 ? -a : a, b < 0 ? -b : b, x, y);
    if (g == 0 || c % g != 0) return std::nullopt;
    if (a < 0) x = -x;
    if (b < 0) y = -y;
    ll scale = c / g;
    return Solution{x * scale, y * scale, b / g, a / g};
}

// ---------------------------------------------------------------------------
// 3. Chinese Remainder Theorem
// ---------------------------------------------------------------------------

struct Congruence { ll rem, mod; };   // x = rem (mod mod)

// Merge x = a1 (mod m1) with x = a2 (mod m2).
//
// Possible only when gcd(m1,m2) divides (a2 - a1) -- with shared factors the two can
// genuinely contradict each other, so this check is not optional.
// The combined modulus is lcm(m1,m2), NOT m1*m2.
// O(log)
std::optional<Congruence> crtMerge(Congruence c1, Congruence c2) {
    ll x, y;
    ll g = extgcd(c1.mod, c2.mod, x, y);
    ll diff = c2.rem - c1.rem;
    if (diff % g != 0) return std::nullopt;

    ll lcm = c1.mod / g * c2.mod;
    ll step = c2.mod / g;
    ll t = mulMod(norm(diff / g, step), norm(x, step), step);
    return Congruence{norm(c1.rem + mulMod(c1.mod % lcm, t, lcm), lcm), lcm};
}

// Merge a whole list, two at a time. Returns nothing if any pair contradicts.
std::optional<Congruence> crtAll(const std::vector<Congruence> &v) {
    Congruence acc{0, 1};
    for (const Congruence &c : v) {
        auto merged = crtMerge(acc, c);
        if (!merged) return std::nullopt;
        acc = *merged;
    }
    return acc;
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- extgcd ---\n";
    ll x, y;
    ll g = extgcd(240, 46, x, y);
    std::cout << "extgcd(240, 46): g = " << g << ", x = " << x << ", y = " << y << "\n";
    std::cout << "  check: 240*(" << x << ") + 46*(" << y << ") = " << 240 * x + 46 * y
              << "\n  note x came back negative -- that is normal\n";

    std::cout << "\n--- solving a*x + b*y = c ---\n";
    if (auto s = solveDiophantine(6, 10, 8)) {
        std::cout << "6x + 10y = 8:  x = " << s->x << ", y = " << s->y << "   (check "
                  << 6 * s->x + 10 * s->y << ")\n";
        std::cout << "  other solutions: x += " << s->stepX << ", y -= " << s->stepY
                  << "   so x=" << s->x - s->stepX << ", y=" << s->y + s->stepY
                  << " works too (check " << 6 * (s->x - s->stepX) + 10 * (s->y + s->stepY)
                  << ")\n";
    }
    std::cout << "6x + 10y = 9:  "
              << (solveDiophantine(6, 10, 9) ? "solvable"
                                             : "no solution -- gcd 2 does not divide 9")
              << "\n";

    std::cout << "\n--- CRT ---\n";
    if (auto r = crtAll({{2, 3}, {3, 5}, {2, 7}}))
        std::cout << "x=2 (mod 3), x=3 (mod 5), x=2 (mod 7)  ->  x = " << r->rem << " mod "
                  << r->mod << "\n";
    if (auto r = crtMerge({2, 6}, {8, 12}))
        std::cout << "x=2 (mod 6), x=8 (mod 12)  ->  x = " << r->rem << " mod " << r->mod
                  << "   (lcm is 12, not 72)\n";
    std::cout << "x=1 (mod 6), x=2 (mod 8)  ->  "
              << (crtMerge({1, 6}, {2, 8}) ? "solvable" : "no solution")
              << "   (one forces x odd, the other forces x even)\n";

    // ---- self-checks ----
    bool bezoutOk = true;
    for (ll a = 0; a <= 120; ++a)
        for (ll b = 0; b <= 120; ++b) {
            ll xx, yy;
            ll gg = extgcd(a, b, xx, yy);
            if (a * xx + b * yy != gg) bezoutOk = false;
        }

    bool diophOk = true;
    for (ll a = -20; a <= 20; ++a)
        for (ll b = -20; b <= 20; ++b) {
            if (a == 0 && b == 0) continue;
            ll xx, yy;
            ll gg = extgcd(a < 0 ? -a : a, b < 0 ? -b : b, xx, yy);
            for (ll c = -30; c <= 30; ++c) {
                auto r = solveDiophantine(a, b, c);
                if (r.has_value() != (c % gg == 0)) { diophOk = false; continue; }
                if (r && a * r->x + b * r->y != c) diophOk = false;
            }
        }

    bool crtOk = true;
    for (ll m1 = 1; m1 <= 20; ++m1)
        for (ll m2 = 1; m2 <= 20; ++m2)
            for (ll a1 = 0; a1 < m1; ++a1)
                for (ll a2 = 0; a2 < m2; ++a2) {
                    auto got = crtMerge({a1, m1}, {a2, m2});
                    ll gg = extgcd(m1, m2, x, y);
                    ll lcm = m1 / gg * m2;
                    ll found = -1;
                    for (ll v = 0; v < lcm; ++v)
                        if (v % m1 == a1 && v % m2 == a2) { found = v; break; }
                    if ((found >= 0) != got.has_value()) crtOk = false;
                    else if (got && (got->rem != found || got->mod != lcm)) crtOk = false;
                }

    std::cout << "\nself-check: a*x + b*y = g for all pairs 0..120 "
              << (bezoutOk ? "ok" : "FAIL") << ", Diophantine solvability and solutions "
              << (diophOk ? "ok" : "FAIL")
              << ", CRT vs exhaustive search for all moduli <= 20 "
              << (crtOk ? "ok" : "FAIL") << "\n";
    return 0;
}
