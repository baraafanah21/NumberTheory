// GCD and the Euclidean Algorithm
// C++17.  Compile: g++ -std=c++17 -O2 -o gcd implementation.cpp
//
// Four functions. std::gcd / std::lcm exist in <numeric>, but std::lcm is undefined
// behaviour when the result overflows -- the version here is not.

#include <iostream>
#include <vector>

using ll = long long;

// Each step replaces (a, b) with (b, a mod b). That keeps every common divisor
// (proofs.md section 1), and the second value strictly shrinks, so the loop ends.
// When b hits 0 the answer is a, since gcd(a, 0) = |a|.
//
// Absolute values on entry: '%' takes the sign of the dividend, so without them a
// negative input could produce a negative result.
//
// O(log min(a,b)) -- under 90 iterations for any 64-bit input.
ll gcd(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}

// lcm(a, b) = |a*b| / gcd(a, b).
//
// Written as (a/g)*b, NOT (a*b)/g. The two are equal mathematically, but a*b overflows
// for inputs far smaller than those whose lcm overflows. Dividing first is exact,
// because g divides a by definition.
//
// The zero guard matters twice: lcm(0, x) is 0, and without it we would divide by a
// zero gcd.
// O(log min(a,b))
ll lcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    ll r = (a / gcd(a, b)) * b;
    return r < 0 ? -r : r;
}

// gcd of a whole array.
//
// Start the accumulator at 0, not at v[0]: gcd(0, x) = |x| makes 0 the identity, which
// removes the empty-array and first-element special cases. Once the running gcd hits 1
// it can never change, so stop early.
// O(n log M)
ll gcdOfArray(const std::vector<ll> &v) {
    ll g = 0;
    for (ll x : v) {
        g = gcd(g, x);
        if (g == 1) break;
    }
    return g;
}

// The "same remainder" pattern.
//
// If every v[i] leaves the same remainder modulo d, then d divides every difference
// v[i] - v[j]. So the valid d are exactly the divisors of this value.
// Returns 0 when all elements are equal (every d works then).
// O(n log M)
ll gcdOfDifferences(const std::vector<ll> &v) {
    ll g = 0;
    for (std::size_t i = 1; i < v.size(); ++i) g = gcd(g, v[i] - v[0]);
    return g;
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

int main() {
    std::cout << "--- gcd ---\n";
    std::cout << "gcd(48, 18)    = " << gcd(48, 18) << "   [6]\n";
    std::cout << "gcd(1071, 462) = " << gcd(1071, 462) << "   [21, in three steps]\n";
    std::cout << "gcd(8, 9)      = " << gcd(8, 9) << "   [1 -- coprime, though neither is prime]\n";
    std::cout << "gcd(-48, 18)   = " << gcd(-48, 18) << "   [6 -- signs do not matter]\n";
    std::cout << "gcd(7, 0)      = " << gcd(7, 0) << "   [7]\n";
    std::cout << "gcd(0, 0)      = " << gcd(0, 0) << "   [0, by convention]\n";

    std::cout << "\n--- lcm ---\n";
    std::cout << "lcm(12, 18) = " << lcm(12, 18) << "   [36]\n";
    std::cout << "lcm(4, 6)   = " << lcm(4, 6) << "   [12, not 24]\n";
    std::cout << "lcm(7, 5)   = " << lcm(7, 5) << "   [35 -- coprime, so the full product]\n";
    std::cout << "gcd * lcm   = " << gcd(12, 18) * lcm(12, 18) << "   [216 = 12 x 18]\n";
    std::cout << "lcm(6e9, 9e9) = " << lcm(6000000000LL, 9000000000LL)
              << "   [a*b would overflow here; (a/g)*b does not]\n";

    std::cout << "\n--- arrays ---\n";
    std::cout << "gcd of {12, 18, 30} = " << gcdOfArray({12, 18, 30}) << "   [6]\n";
    std::cout << "gcd of {6, 10, 15}  = " << gcdOfArray({6, 10, 15}) << "   [1]\n";
    std::cout << "  but pairwise: gcd(6,10)=" << gcd(6, 10) << " gcd(6,15)=" << gcd(6, 15)
              << " gcd(10,15)=" << gcd(10, 15)
              << "\n  -> the whole set is coprime while NO pair is. These are different things.\n";

    std::cout << "\n--- the 'same remainder' trick ---\n";
    ll g = gcdOfDifferences({17, 32, 47});
    std::cout << "17, 32, 47 leave the same remainder modulo every divisor of " << g << "\n";
    std::cout << "  check mod 15: " << 17 % 15 << ", " << 32 % 15 << ", " << 47 % 15
              << "   and mod 5: " << 17 % 5 << ", " << 32 % 5 << ", " << 47 % 5 << "\n";

    std::cout << "\n--- reachability (Bezout) ---\n";
    std::cout << "steps of 6 and 10 reach exactly the multiples of " << gcd(6, 10)
              << ", so 8 is reachable and 9 is not\n";

    // Self-check against a slow but obviously correct method.
    bool ok = true;
    for (ll a = 1; a <= 200; ++a)
        for (ll b = 1; b <= 200; ++b) {
            ll best = 1;
            for (ll d = 1; d <= a && d <= b; ++d)
                if (a % d == 0 && b % d == 0) best = d;
            if (gcd(a, b) != best) ok = false;
            if (lcm(a, b) != a / best * b) ok = false;
        }
    std::cout << "\nself-check: gcd and lcm match brute force for all pairs 1..200: "
              << (ok ? "ok" : "FAIL") << "\n";
    return 0;
}
