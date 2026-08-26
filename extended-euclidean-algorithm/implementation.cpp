// Extended Euclidean Algorithm
// C++17.  Compile: g++ -std=c++17 -O2 -o extgcd implementation.cpp
//
// Four functions: the algorithm, modular inverse, linear Diophantine, CRT merge.

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
// Coefficients stay small, so nothing overflows inside here.
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
// 2. Modular inverse
// ---------------------------------------------------------------------------

// a^-1 mod m, or -1 when it does not exist.
//
// It exists exactly when gcd(a, m) == 1, because a*x = 1 (mod m) is the equation
// a*x + m*y = 1, and Bezout makes that solvable only when gcd(a,m) divides 1.
//
// Both normalizations are needed: the input may be negative, and extgcd returns a
// signed coefficient.
// Works for ANY modulus. O(log m)
ll modInverse(ll a, ll m) {
    if (m <= 1) return -1;
    ll x, y;
    ll g = extgcd(norm(a, m), m, x, y);
    if (g != 1) return -1;
    return norm(x, m);
}

// base^exp % mod, by repeated squaring.   O(log exp)
ll powerMod(ll base, ll exp, ll mod) {
    ll result = 1 % mod;
    base = norm(base, mod);
    while (exp > 0) {
        if (exp & 1) result = mulMod(result, base, mod);
        base = mulMod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// a^-1 mod p using Fermat: a^(p-2) = a^-1 when p is prime.
// PRIME p ONLY. On a composite modulus this returns a wrong value with no error at all,
// which is why modInverse above is the safer default.
ll modInversePrime(ll a, ll p) { return powerMod(a, p - 2, p); }

// ---------------------------------------------------------------------------
// 3. Solving a*x + b*y = c
// ---------------------------------------------------------------------------

struct Solution {
    ll x, y;          // one solution
    ll stepX, stepY;  // all solutions are x + k*stepX, y - k*stepY
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
// 4. Chinese Remainder Theorem
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
    std::cout << "  check: 240*(" << x << ") + 46*(" << y << ") = " << 240 * x + 46 * y << "\n";

    std::cout << "\n--- modular inverse ---\n";
    std::cout << "3^-1 mod 11 = " << modInverse(3, 11) << "    (3 x 4 = 12 = 1 mod 11)\n";
    std::cout << "5^-1 mod 12 = " << modInverse(5, 12) << "    (5 x 5 = 25 = 1 mod 12)\n";
    std::cout << "2^-1 mod 6  = " << modInverse(2, 6)
              << "   (-1: gcd(2,6) = 2, so no inverse exists)\n";
    std::cout << "dividing under a modulus: 7/3 mod 11 = 7 * 3^-1 = "
              << 7 * modInverse(3, 11) % 11 << "   (check: 3 x that = "
              << 3 * (7 * modInverse(3, 11) % 11) % 11 << " = 7)\n";

    std::cout << "\nFermat works only for prime moduli:\n";
    std::cout << "  mod 12 (composite): extgcd says 5^-1 = " << modInverse(5, 12)
              << ", Fermat says " << powerMod(5, 10, 12) << "\n";
    std::cout << "  check Fermat's answer: 5 x " << powerMod(5, 10, 12) << " = "
              << 5 * powerMod(5, 10, 12) % 12 << " mod 12 -- should be 1, so it is WRONG\n";

    std::cout << "\n--- solving a*x + b*y = c ---\n";
    if (auto s = solveDiophantine(6, 10, 8))
        std::cout << "6x + 10y = 8:  x = " << s->x << ", y = " << s->y << "  (check "
                  << 6 * s->x + 10 * s->y << ")\n  other solutions: x += " << s->stepX
                  << ", y -= " << s->stepY << "\n";
    std::cout << "6x + 10y = 9:  "
              << (solveDiophantine(6, 10, 9) ? "solvable" : "no solution -- gcd 2 does not divide 9")
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

    // Self-checks against exhaustive search.
    bool bezoutOk = true;
    for (ll a = 0; a <= 120; ++a)
        for (ll b = 0; b <= 120; ++b) {
            ll xx, yy;
            ll gg = extgcd(a, b, xx, yy);
            if (a * xx + b * yy != gg) bezoutOk = false;
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
              << (bezoutOk ? "ok" : "FAIL")
              << ", CRT matches exhaustive search for all moduli <= 20 "
              << (crtOk ? "ok" : "FAIL") << "\n";
    return 0;
}
