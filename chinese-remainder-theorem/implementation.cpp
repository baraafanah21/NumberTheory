// Chinese Remainder Theorem -- g++ -std=c++17 -O2 implementation.cpp

#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

using ll = long long;

ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll modInverse(ll a, ll m) {
    if (m <= 1) return -1;
    ll x, y;
    return extgcd(norm(a, m), m, x, y) != 1 ? -1 : norm(x, m);
}

struct Congruence { ll rem, mod; };      // x = rem (mod mod)

// ---------------------------------------------------------------- merge

// Merge two congruences.  Works for ANY moduli.
//
// Solvable only when gcd(m1,m2) divides (a2 - a1): with shared factors the two can
// genuinely contradict (x = 1 mod 6 forces x odd, x = 2 mod 8 forces x even).
// The combined modulus is lcm, NOT m1*m2 -- those agree only for coprime moduli.
//
// Derivation: every solution of the first is x = a1 + m1*t; substituting into the second
// gives the linear congruence m1*t = (a2 - a1) mod m2, which we solve for t.
// O(log min(m1,m2))
std::optional<Congruence> crtMerge(Congruence c1, Congruence c2) {
    ll x, y;
    ll g = extgcd(c1.mod, c2.mod, x, y);
    ll diff = c2.rem - c1.rem;
    if (diff % g != 0) return std::nullopt;

    ll lcm  = c1.mod / g * c2.mod;
    ll step = c2.mod / g;                          // t is determined modulo this
    ll t = mulMod(norm(diff / g, step), norm(x, step), step);
    return Congruence{norm(c1.rem + mulMod(c1.mod % lcm, t, lcm), lcm), lcm};
}

// Fold a whole list.  Merging pairwise is exact: the merged congruence has the SAME
// solution set as the two it replaces.  The accumulator starts at x = 0 (mod 1), which
// every integer satisfies -- the identity, so there is no empty-list case.
// Caution: the modulus grows to the lcm of everything and can overflow.
// O(k log M)
std::optional<Congruence> crtAll(const std::vector<Congruence> &system) {
    Congruence acc{0, 1};
    for (const Congruence &c : system) {
        auto merged = crtMerge(acc, c);
        if (!merged) return std::nullopt;
        acc = *merged;
    }
    return acc;
}

// ---------------------------------------------------------------- direct formula

// x = sum of a_i * M_i * y_i (mod M), with M_i = M/m_i and y_i = M_i^-1 mod m_i.
// Each M_i*y_i is an indicator: 1 modulo m_i, 0 modulo every other modulus.
//
// Needs PAIRWISE coprimality -- a set gcd of 1 is not enough.  With (6,10,15) we get
// M_1 = 150, which shares the factor 6 with m_1, so no inverse exists.
// Prefer crtAll: it handles any moduli and never forms M, which can overflow even when
// the lcm would not.
// O(k log M)
std::optional<Congruence> crtCoprimeDirect(const std::vector<Congruence> &system) {
    ll M = 1;
    for (const Congruence &c : system) M *= c.mod;

    ll result = 0;
    for (const Congruence &c : system) {
        if (c.mod == 1) continue;                  // vacuous: every x satisfies it
        ll Mi = M / c.mod;
        ll yi = modInverse(Mi % c.mod, c.mod);
        if (yi < 0) return std::nullopt;           // not pairwise coprime
        result = norm(result + mulMod(mulMod(norm(c.rem, M), Mi % M, M), yi % M, M), M);
    }
    return Congruence{result, M};
}

// ---------------------------------------------------------------- demo

int main() {
    std::vector<Congruence> classic = {{2, 3}, {3, 5}, {2, 7}};
    if (auto r = crtAll(classic)) {
        std::cout << "x = 2 (mod 3), 3 (mod 5), 2 (mod 7)  ->  x = " << r->rem << " (mod "
                  << r->mod << ")\n";
        std::cout << "  check: " << r->rem % 3 << " " << r->rem % 5 << " " << r->rem % 7 << "\n";
    }
    if (auto r = crtCoprimeDirect(classic))
        std::cout << "  the direct formula agrees: " << r->rem << "\n";

    std::cout << "\nnon-coprime moduli\n";
    if (auto r = crtMerge({2, 6}, {8, 12}))
        std::cout << "  x = 2 (mod 6), 8 (mod 12)  ->  " << r->rem << " (mod " << r->mod
                  << ")\t[lcm 12, not 72]\n";
    std::cout << "  x = 1 (mod 6), 2 (mod 8)   ->  "
              << (crtMerge({1, 6}, {2, 8}) ? "solvable" : "NO SOLUTION")
              << "\t[one forces x odd, the other even]\n";

    std::cout << "\npairwise coprime is stronger than setwise\n";
    std::vector<Congruence> tricky = {{1, 6}, {2, 10}, {3, 15}};
    std::cout << "  moduli 6,10,15: set gcd is " << std::gcd(std::gcd(6, 10), 15)
              << ", but no pair is coprime\n";
    std::cout << "  direct formula: "
              << (crtCoprimeDirect(tricky) ? "worked" : "refused -- no inverse exists") << "\n";

    std::cout << "\nsplitting a computation across moduli\n";
    const ll a = 123456789LL, b = 987654321LL;
    std::vector<Congruence> pieces;
    for (ll p : {1000003LL, 1000033LL, 1000037LL}) pieces.push_back({mulMod(a % p, b % p, p), p});
    if (auto r = crtAll(pieces)) {
        std::cout << "  a*b from three word-sized moduli: " << r->rem << "\n";
        std::cout << "  actual a*b:                       " << a * b << "\n";
        std::cout << "  product of moduli " << r->mod << " exceeds a*b, so it is exact\n";
    }

    bool mergeOk = true, foldOk = true, directOk = true;
    for (ll m1 = 1; m1 <= 20; ++m1)
        for (ll m2 = 1; m2 <= 20; ++m2)
            for (ll a1 = 0; a1 < m1; ++a1)
                for (ll a2 = 0; a2 < m2; ++a2) {
                    auto got = crtMerge({a1, m1}, {a2, m2});
                    ll lcm = m1 / std::gcd(m1, m2) * m2, want = -1;
                    for (ll x = 0; x < lcm; ++x)
                        if (x % m1 == a1 && x % m2 == a2) { want = x; break; }
                    if ((want >= 0) != got.has_value()) mergeOk = false;
                    else if (got && (got->rem != want || got->mod != lcm)) mergeOk = false;
                    if (std::gcd(m1, m2) == 1) {
                        auto d = crtCoprimeDirect({{a1, m1}, {a2, m2}});
                        if (!d || !got || d->rem != got->rem || d->mod != got->mod)
                            directOk = false;
                    }
                }
    for (ll m1 = 1; m1 <= 9; ++m1)
      for (ll m2 = 1; m2 <= 9; ++m2)
        for (ll m3 = 1; m3 <= 9; ++m3)
          for (ll a1 = 0; a1 < m1; ++a1)
            for (ll a2 = 0; a2 < m2; ++a2)
              for (ll a3 = 0; a3 < m3; ++a3) {
                  auto got = crtAll({{a1, m1}, {a2, m2}, {a3, m3}});
                  ll lcm = m1 / std::gcd(m1, m2) * m2;
                  lcm = lcm / std::gcd(lcm, m3) * m3;
                  ll want = -1;
                  for (ll x = 0; x < lcm; ++x)
                      if (x % m1 == a1 && x % m2 == a2 && x % m3 == a3) { want = x; break; }
                  if ((want >= 0) != got.has_value()) foldOk = false;
                  else if (got && (got->rem != want || got->mod != lcm)) foldOk = false;
              }
    std::cout << "\nself-check: merge vs exhaustive " << (mergeOk ? "ok" : "FAIL")
              << ", three-way fold vs exhaustive " << (foldOk ? "ok" : "FAIL")
              << ", direct formula == merging " << (directOk ? "ok" : "FAIL") << "\n";
}
