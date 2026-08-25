// GCD and the Euclidean Algorithm -- CP templates
// C++17.  Compile: g++ -std=c++17 -O2 -o gcd implementation.cpp
//
// Paste-ready. std::gcd / std::lcm exist in <numeric>, but std::lcm has UB when the
// result overflows -- these do not.

#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

using ll = long long;

// ===========================================================================
// The algorithm
// ===========================================================================

// O(log min(a,b)), O(1) space. Under 90 iterations for any 64-bit input.
// abs on entry because % takes the sign of the dividend.
ll gcdIterative(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}

// Same thing as the recurrence. Depth is O(log), so the stack is never a concern.
ll gcdRecursive(ll a, ll b) {
    if (b == 0) return a < 0 ? -a : a;
    return gcdRecursive(b, a % b);
}

// lcm(a,b) = |ab| / gcd(a,b), computed as (a/g)*b.
// Dividing first is exact (g | a) and avoids the a*b overflow.
// The zero guard also prevents dividing by a zero gcd.
ll lcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    ll r = (a / gcdIterative(a, b)) * b;
    return r < 0 ? -r : r;
}

// Division-free gcd (Stein). Only shifts, comparisons, subtraction.
// The unsigned conversion also makes LLONG_MIN safe.
// O(log max) iterations.
ll binaryGcd(ll a, ll b) {
    unsigned long long u = a < 0 ? -(unsigned long long)a : (unsigned long long)a;
    unsigned long long v = b < 0 ? -(unsigned long long)b : (unsigned long long)b;
    if (!u) return (ll)v;
    if (!v) return (ll)u;

    int commonTwos = 0;
    while (((u | v) & 1ULL) == 0) { u >>= 1; v >>= 1; ++commonTwos; }
    while ((u & 1ULL) == 0) u >>= 1;
    do {
        while ((v & 1ULL) == 0) v >>= 1;
        if (u > v) std::swap(u, v);
        v -= u;                              // odd - odd, so even: next loop strips it
    } while (v);
    return (ll)(u << commonTwos);
}

// Step counter -- only for demonstrating the Fibonacci worst case.
int euclidSteps(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    int steps = 0;
    while (b) { ll r = a % b; a = b; b = r; ++steps; }
    return steps;
}

// ===========================================================================
// Folds
// ===========================================================================

// Start at 0: gcd(0, x) = |x| makes 0 the identity, killing the empty/first-element
// special cases. Early exit at 1 -- it can never change after that.
// O(n log M)
ll gcdOfArray(const std::vector<ll> &v) {
    ll g = 0;
    for (ll x : v) { g = gcdIterative(g, x); if (g == 1) break; }
    return g;
}

bool coprime(ll a, ll b) { return gcdIterative(a, b) == 1; }

// ===========================================================================
// Patterns
// ===========================================================================

// "Same remainder" pattern: every d for which all v[i] agree mod d divides every
// difference, hence divides this. The answers are its divisors.
// Returns 0 when all elements are equal (every d works).
// O(n log M)
ll gcdOfDifferences(const std::vector<ll> &v) {
    ll g = 0;
    for (std::size_t i = 1; i < v.size(); ++i) g = gcdIterative(g, v[i] - v[0]);
    return g;
}

// Integer points on the segment, endpoints included: gcd(|dx|,|dy|) + 1.
ll latticePointsOnSegment(ll x1, ll y1, ll x2, ll y2) {
    ll dx = x2 - x1, dy = y2 - y1;
    if (dx == 0 && dy == 0) return 1;
    return gcdIterative(dx, dy) + 1;
}

// base^exp, no overflow check. O(log exp)
ll ipow(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

// gcd(a^m - 1, a^n - 1) = a^gcd(m,n) - 1.
// Collapses a gcd of astronomically large numbers into one of two small exponents.
ll gcdOfPowersMinusOne(ll a, ll m, ll n) {
    return ipow(a, gcdIterative(m, n)) - 1;
}

// For each subarray gcd value, how many subarrays have it.
//
// The gcds of subarrays ending at i take only O(log M) distinct values, because
// extending left either keeps the gcd or at least halves it. So this is O(n log M)
// rather than the O(n^2) the problem shape suggests.
// O(n log M) time, O(log M) working space.
std::map<ll, ll> subarrayGcdCounts(const std::vector<ll> &v) {
    std::map<ll, ll> total;
    std::vector<std::pair<ll, ll>> cur, nxt;   // (gcd value, how many subarrays)
    for (ll x : v) {
        nxt.clear();
        nxt.push_back({x, 1});
        for (auto [g, c] : cur) {
            ll ng = gcdIterative(g, x);
            if (ng == nxt.back().first) nxt.back().second += c;
            else nxt.push_back({ng, c});
        }
        cur = nxt;
        for (auto [g, c] : cur) total[g] += c;
    }
    return total;
}

// ===========================================================================
// Demo / self-check
// ===========================================================================

void demoBasics() {
    std::cout << "--- the algorithm ---\n";
    std::cout << "gcd(48,18)=" << gcdIterative(48, 18)
              << "  gcd(1071,462)=" << gcdIterative(1071, 462)
              << "  gcd(8,9)=" << gcdIterative(8, 9) << " (coprime, neither prime)\n";
    std::cout << "gcd(-48,18)=" << gcdIterative(-48, 18)
              << "  gcd(7,0)=" << gcdIterative(7, 0)
              << "  gcd(0,0)=" << gcdIterative(0, 0) << " (convention)\n";

    bool agree = true;
    for (ll a = 0; a <= 60; ++a)
        for (ll b = 0; b <= 60; ++b)
            if (gcdRecursive(a, b) != gcdIterative(a, b) ||
                binaryGcd(a, b) != gcdIterative(a, b)) agree = false;
    std::cout << "iterative == recursive == binary, all pairs 0..60: "
              << (agree ? "yes" : "NO") << "\n\n";
}

void demoComplexity() {
    std::cout << "--- worst case is Fibonacci ---\n";
    ll prev = 1, cur = 1;
    for (int i = 0; i < 12; ++i) { ll nx = prev + cur; prev = cur; cur = nx; }
    std::cout << "gcd(" << cur << "," << prev << ") takes " << euclidSteps(cur, prev)
              << " steps; gcd(" << cur << "," << cur / 2 << ") takes "
              << euclidSteps(cur, cur / 2) << "\n";

    int worst = 0; ll wa = 0, wb = 0;
    for (ll a = 1; a < 100; ++a)
        for (ll b = 1; b < a; ++b) {
            int s = euclidSteps(a, b);
            if (s > worst) { worst = s; wa = a; wb = b; }
        }
    std::cout << "worst pair below 100: (" << wa << "," << wb << ") at " << worst
              << " steps  [F_11, F_10]\n";
    std::cout << "two values near 2^63 take "
              << euclidSteps(9223372036854775807LL, 7540113804746346429LL) << " steps\n\n";
}

void demoLcm() {
    std::cout << "--- lcm ---\n";
    std::cout << "lcm(12,18)=" << lcm(12, 18) << "  lcm(4,6)=" << lcm(4, 6)
              << "  lcm(7,5)=" << lcm(7, 5) << " (coprime: the product)"
              << "  lcm(0,5)=" << lcm(0, 5) << "\n";
    std::cout << "gcd*lcm == |ab|: " << gcdIterative(12, 18) * lcm(12, 18) << " == 216\n";
    std::cout << "lcm(6e9, 9e9) = " << lcm(6000000000LL, 9000000000LL)
              << "   [a*b would overflow; (a/g)*b does not]\n\n";
}

void demoPatterns() {
    std::cout << "--- patterns ---\n";
    std::cout << "gcd{12,18,30}=" << gcdOfArray({12, 18, 30})
              << "  gcd{6,10,15}=" << gcdOfArray({6, 10, 15})
              << " but pairwise gcds are " << gcdIterative(6, 10) << ","
              << gcdIterative(6, 15) << "," << gcdIterative(10, 15)
              << " -- set-coprime is NOT pairwise-coprime\n";

    std::cout << "17,32,47 share a remainder mod every divisor of "
              << gcdOfDifferences({17, 32, 47}) << " (all are 2 mod 15)\n";

    std::cout << "lattice points (0,0)-(12,8): " << latticePointsOnSegment(0, 0, 12, 8)
              << "   (0,0)-(5,3): " << latticePointsOnSegment(0, 0, 5, 3) << " (coprime)\n";

    std::cout << "steps of 6 and 10 reach exactly the multiples of "
              << gcdIterative(6, 10) << ": 8 yes, 9 no\n";

    // Subarray gcds: check the O(n log M) method against brute force.
    std::vector<ll> v = {12, 18, 6, 24, 9};
    std::map<ll, ll> fast = subarrayGcdCounts(v);
    std::map<ll, ll> brute;
    for (std::size_t i = 0; i < v.size(); ++i) {
        ll g = 0;
        for (std::size_t j = i; j < v.size(); ++j) { g = gcdIterative(g, v[j]); ++brute[g]; }
    }
    std::cout << "subarray gcd counts == brute force: " << (fast == brute ? "yes" : "NO")
              << "   (";
    for (auto [g, c] : fast) std::cout << g << ":" << c << " ";
    std::cout << ")\n\n";
}

void demoExponentGcd() {
    std::cout << "--- gcd(a^m-1, a^n-1) = a^gcd(m,n)-1 ---\n";
    bool agree = true;
    for (ll a = 2; a <= 5; ++a)
        for (ll m = 1; m <= 12; ++m)
            for (ll n = 1; n <= 12; ++n) {
                if (ipow(a, m) > (1LL << 40) || ipow(a, n) > (1LL << 40)) continue;
                if (gcdIterative(ipow(a, m) - 1, ipow(a, n) - 1) !=
                    gcdOfPowersMinusOne(a, m, n)) agree = false;
            }
    std::cout << "shortcut == direct gcd for a in 2..5, m,n in 1..12: "
              << (agree ? "yes" : "NO") << "\n";
    std::cout << "gcd(2^36-1, 2^24-1) = " << gcdOfPowersMinusOne(2, 36, 24) << " = 2^12-1\n";
    std::cout << "gcd(2^1000000-1, 2^999999-1) = " << gcdOfPowersMinusOne(2, 1000000, 999999)
              << "   [300000-digit numbers, answered from gcd(1000000,999999)]\n";
}

int main() {
    demoBasics();
    demoComplexity();
    demoLcm();
    demoPatterns();
    demoExponentGcd();
    return 0;
}
