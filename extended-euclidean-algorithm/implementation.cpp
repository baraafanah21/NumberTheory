// Extended Euclidean Algorithm
// C++17.  Compile: g++ -std=c++17 -O2 -o extgcd implementation.cpp
//
// Two functions: the algorithm itself, and solving a*x + b*y = c.
//
// The two biggest uses of extgcd have their own concepts:
//   ../modular-multiplicative-inverse/   dividing under a modulus
//   ../chinese-remainder-theorem/        combining congruences

#include <iostream>
#include <optional>

using ll = long long;

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

    std::cout << "\nself-check: a*x + b*y = g for all pairs 0..120 "
              << (bezoutOk ? "ok" : "FAIL") << ", Diophantine solvability and solutions "
              << (diophOk ? "ok" : "FAIL") << "\n";
    return 0;
}
