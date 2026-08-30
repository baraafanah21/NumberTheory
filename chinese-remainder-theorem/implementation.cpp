// Chinese Remainder Theorem
// C++17.  Compile: g++ -std=c++17 -O2 -o crt implementation.cpp
//
// Three routines: merging two congruences (any moduli), folding a whole list, and the
// direct formula for pairwise coprime moduli.

#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

using ll = long long;

ll mulMod(ll a, ll b, ll mod) { return (ll)((__int128)a * b % mod); }
ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }

// From the extended Euclid concept: g = gcd(a,b) with a*x + b*y == g.
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// From the modular inverse concept: a^-1 mod m, or -1 if gcd(a,m) != 1.
ll modInverse(ll a, ll m) {
    if (m <= 1) return -1;
    ll x, y;
    if (extgcd(norm(a, m), m, x, y) != 1) return -1;
    return norm(x, m);
}

// ---------------------------------------------------------------------------
// x = rem (mod mod)
// ---------------------------------------------------------------------------

struct Congruence { ll rem, mod; };

// ---------------------------------------------------------------------------
// 1. Merge two congruences -- works for ANY moduli
// ---------------------------------------------------------------------------

// Solvable only when gcd(m1,m2) divides (a2 - a1): with shared factors the two can
// genuinely contradict each other (x = 1 mod 6 forces x odd, x = 2 mod 8 forces x even).
//
// The combined modulus is lcm(m1,m2), NOT m1*m2 -- those agree only for coprime moduli.
//
// Derivation: every solution of the first is x = a1 + m1*t. Substituting into the second
// gives the linear congruence m1*t = (a2 - a1) mod m2, which we solve for t.
// O(log min(m1,m2))
std::optional<Congruence> crtMerge(Congruence c1, Congruence c2) {
    ll x, y;
    ll g = extgcd(c1.mod, c2.mod, x, y);
    ll diff = c2.rem - c1.rem;
    if (diff % g != 0) return std::nullopt;          // contradictory system

    ll lcm  = c1.mod / g * c2.mod;
    ll step = c2.mod / g;                            // t is determined modulo this
    ll t = mulMod(norm(diff / g, step), norm(x, step), step);
    return Congruence{norm(c1.rem + mulMod(c1.mod % lcm, t, lcm), lcm), lcm};
}

// ---------------------------------------------------------------------------
// 2. Fold a whole list
// ---------------------------------------------------------------------------

// Merging pairwise is exact, not approximate: the merged congruence has the SAME
// solution set as the two it replaces, so nothing is lost at any step.
//
// The accumulator starts at "x = 0 (mod 1)", which every integer satisfies -- the
// identity element, so there is no empty-list special case.
//
// Returns nothing if any pair is contradictory.
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

// ---------------------------------------------------------------------------
// 3. The direct formula -- PAIRWISE coprime moduli only
// ---------------------------------------------------------------------------

// x = sum of a_i * M_i * y_i  (mod M),  where M = product of m_i, M_i = M / m_i,
// and y_i = M_i^-1 mod m_i.
//
// Each M_i*y_i is an indicator: 1 modulo m_i, and 0 modulo every other modulus, since
// m_j divides M_i whenever j != i.
//
// Requires PAIRWISE coprimality -- gcd of the whole set being 1 is not enough. With
// (6,10,15) the set gcd is 1, but M_1 = 150 shares the factor 6 with m_1 = 6, so no
// inverse exists. Returns nothing in that case.
//
// Prefer crtAll in practice: it handles any moduli and never forms M, which can
// overflow even when the lcm would not.
// O(k log M)
std::optional<Congruence> crtCoprimeDirect(const std::vector<Congruence> &system) {
    ll M = 1;
    for (const Congruence &c : system) M *= c.mod;

    ll result = 0;
    for (const Congruence &c : system) {
        if (c.mod == 1) continue;                    // vacuous: every x satisfies it
        ll Mi = M / c.mod;
        ll yi = modInverse(Mi % c.mod, c.mod);
        if (yi < 0) return std::nullopt;             // moduli were not pairwise coprime
        result = norm(result + mulMod(mulMod(norm(c.rem, M), Mi % M, M), yi % M, M), M);
    }
    return Congruence{result, M};
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- the classic example ---\n";
    std::vector<Congruence> classic = {{2, 3}, {3, 5}, {2, 7}};
    if (auto r = crtAll(classic)) {
        std::cout << "x = 2 (mod 3), x = 3 (mod 5), x = 2 (mod 7)\n";
        std::cout << "  ->  x = " << r->rem << " (mod " << r->mod << ")\n";
        std::cout << "  check: " << r->rem << " mod 3 = " << r->rem % 3 << ", mod 5 = "
                  << r->rem % 5 << ", mod 7 = " << r->rem % 7 << "\n";
    }
    if (auto r = crtCoprimeDirect(classic))
        std::cout << "  the direct formula agrees: " << r->rem << " (mod " << r->mod << ")\n";

    std::cout << "\n--- non-coprime moduli ---\n";
    if (auto r = crtMerge({2, 6}, {8, 12}))
        std::cout << "x = 2 (mod 6), x = 8 (mod 12)  ->  x = " << r->rem << " (mod "
                  << r->mod << ")   [lcm is 12, not 72]\n";
    std::cout << "x = 1 (mod 6), x = 2 (mod 8)   ->  "
              << (crtMerge({1, 6}, {2, 8}) ? "solvable" : "NO SOLUTION")
              << "   [one forces x odd, the other forces x even]\n";

    std::cout << "\n--- pairwise coprime is stronger than setwise ---\n";
    std::vector<Congruence> tricky = {{1, 6}, {2, 10}, {3, 15}};
    std::cout << "moduli 6, 10, 15: gcd of all three is " << std::gcd(std::gcd(6, 10), 15)
              << ", but no two are coprime\n";
    std::cout << "  direct formula: "
              << (crtCoprimeDirect(tricky) ? "worked" : "refused -- no inverse exists") << "\n";
    std::cout << "  pairwise merge: "
              << (crtAll(tricky) ? "worked anyway" : "no solution for this system") << "\n";

    std::cout << "\n--- splitting a computation across moduli ---\n";
    // Compute 123456789 * 987654321 exactly, using only arithmetic mod three small primes.
    const ll a = 123456789LL, b = 987654321LL;
    std::vector<ll> primes = {1000003, 1000033, 1000037};
    std::vector<Congruence> pieces;
    for (ll p : primes)
        pieces.push_back({mulMod(a % p, b % p, p), p});
    if (auto r = crtAll(pieces)) {
        std::cout << "a*b computed only modulo " << primes[0] << ", " << primes[1] << ", "
                  << primes[2] << "\n";
        std::cout << "  reconstructed: " << r->rem << "\n";
        std::cout << "  actual a*b:    " << a * b << "\n";
        std::cout << "  the product of the moduli (" << r->mod
                  << ") exceeds a*b, so the answer is exact\n";
    }

    // ---- self-checks ----
    bool mergeOk = true;
    for (ll m1 = 1; m1 <= 20; ++m1)
        for (ll m2 = 1; m2 <= 20; ++m2)
            for (ll a1 = 0; a1 < m1; ++a1)
                for (ll a2 = 0; a2 < m2; ++a2) {
                    auto got = crtMerge({a1, m1}, {a2, m2});
                    ll lcm = m1 / std::gcd(m1, m2) * m2;
                    ll want = -1;
                    for (ll x = 0; x < lcm; ++x)
                        if (x % m1 == a1 && x % m2 == a2) { want = x; break; }
                    if ((want >= 0) != got.has_value()) mergeOk = false;
                    else if (got && (got->rem != want || got->mod != lcm)) mergeOk = false;
                }

    bool foldOk = true;
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

    bool directOk = true;
    for (ll m1 = 1; m1 <= 12; ++m1)
        for (ll m2 = 1; m2 <= 12; ++m2) {
            if (std::gcd(m1, m2) != 1) continue;
            for (ll a1 = 0; a1 < m1; ++a1)
                for (ll a2 = 0; a2 < m2; ++a2) {
                    auto d = crtCoprimeDirect({{a1, m1}, {a2, m2}});
                    auto f = crtAll({{a1, m1}, {a2, m2}});
                    if (!d || !f || d->rem != f->rem || d->mod != f->mod) directOk = false;
                }
        }

    std::cout << "\nself-check: merge vs exhaustive search " << (mergeOk ? "ok" : "FAIL")
              << ", three-way fold vs exhaustive " << (foldOk ? "ok" : "FAIL")
              << ", direct formula == merging " << (directOk ? "ok" : "FAIL") << "\n";
    return 0;
}
