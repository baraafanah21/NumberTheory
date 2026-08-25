// GCD and the Euclidean Algorithm
// C++17
//
// Every routine here implements a result stated in README.md and proved in proofs.md.
// Cross-references are given as "Theorem N" (proofs.md) and "G-k" (README.md).
//
// C++17 already provides std::gcd and std::lcm in <numeric>. They are deliberately not
// used here: the point is to understand the algorithm. See demoStandardLibrary() for the
// two traps they do NOT protect you from.
//
// Compile: g++ -std=c++17 -O2 -o gcd implementation.cpp

#include <iostream>
#include <utility>
#include <vector>

using ll = long long;

// Absolute value as an unsigned type. Writing -x for x == LLONG_MIN is undefined
// behaviour, because +2^63 is not representable as a signed 64-bit integer; converting
// first and negating in unsigned arithmetic is well defined.
unsigned long long absToUnsigned(ll x) {
    return x < 0 ? -static_cast<unsigned long long>(x) : static_cast<unsigned long long>(x);
}

// ---------------------------------------------------------------------------
// The Euclidean algorithm
// ---------------------------------------------------------------------------

// gcd(a, b), iterative.   (Theorems 2 and 3)
//
// Each iteration replaces (a, b) by (b, a mod b), which by Theorem 2 preserves the whole
// set of common divisors; by Theorem 3 the second argument strictly decreases, so the
// loop ends. On exit b == 0 and gcd(a, 0) = |a| (G2).
//
// Inputs are normalized to non-negative first: '%' in C++ takes the sign of the dividend,
// so without this the result could come back negative.
//
// Precondition: |a| and |b| are representable (i.e. not LLONG_MIN).
// Time O(log min(a,b)) divisions, Space O(1).
ll gcdIterative(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b != 0) {
        ll r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// The same algorithm written as the recurrence it actually is.
// The recursion depth is O(log min(a,b)) -- under 90 frames for any 64-bit input, so the
// stack is not a concern. It is also a tail call, which compilers usually flatten.
// Time O(log min(a,b)), Space O(log min(a,b)) stack.
ll gcdRecursive(ll a, ll b) {
    if (b == 0) return a < 0 ? -a : a;
    return gcdRecursive(b, a % b);
}

// Number of division steps the Euclidean algorithm performs. Used only to demonstrate
// Lamé's theorem (Theorem 5): the worst case is consecutive Fibonacci numbers.
// Time O(log min(a,b)), Space O(1).
int euclidSteps(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    int steps = 0;
    while (b != 0) {
        ll r = a % b;
        a = b;
        b = r;
        ++steps;
    }
    return steps;
}

// ---------------------------------------------------------------------------
// Binary GCD (Stein's algorithm) -- no division at all
// ---------------------------------------------------------------------------

// gcd(a, b) using only shifts, comparisons and subtraction.   (Theorem 15)
//
// The three identities, for u, v > 0:
//   (a) both even : gcd(u,v) = 2*gcd(u/2, v/2)   -- extract the shared factor of 2
//   (b) v odd     : gcd(u,v) = gcd(u/2, v)       -- 2 is not common, so drop it
//   (c) both odd  : gcd(u,v) = gcd(u-v, v)       -- an ordinary Euclidean step
//
// Case (c) subtracts two odd numbers, so the result is even and case (b) fires next.
// Every iteration therefore removes at least one bit.
//
// Time O(log max(a,b)) iterations, Space O(1).
ll binaryGcd(ll a, ll b) {
    unsigned long long u = absToUnsigned(a);
    unsigned long long v = absToUnsigned(b);
    if (u == 0) return static_cast<ll>(v);
    if (v == 0) return static_cast<ll>(u);

    // Identity (a): strip the factors of 2 they share, and remember how many.
    int commonTwos = 0;
    while (((u | v) & 1ULL) == 0) {
        u >>= 1;
        v >>= 1;
        ++commonTwos;
    }

    // From here the gcd is odd, so identity (b) lets us strip 2s from either side freely.
    while ((u & 1ULL) == 0) u >>= 1;

    do {
        while ((v & 1ULL) == 0) v >>= 1;   // identity (b)
        if (u > v) std::swap(u, v);        // keep u <= v so the subtraction stays positive
        v -= u;                            // identity (c): both are odd here
    } while (v != 0);

    return static_cast<ll>(u << commonTwos);
}

// ---------------------------------------------------------------------------
// LCM and folds
// ---------------------------------------------------------------------------

// lcm(a, b) = |a*b| / gcd(a, b).   (Theorem 11 / G8)
//
// Written as (a/g)*b, never (a*b)/g: the product a*b overflows for inputs far smaller
// than those whose lcm overflows. The early division is exact because g divides a.
//
// lcm(0, x) = 0 by convention, and the guard also prevents dividing by a zero gcd.
// Precondition: the true lcm fits in a signed 64-bit integer.
// Time O(log min(a,b)), Space O(1).
ll lcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    ll g = gcdIterative(a, b);
    ll result = (a / g) * b;
    return result < 0 ? -result : result;
}

// gcd of a whole array, by folding with G9 (associativity, Theorem 12).
//
// The fold starts at 0 rather than v[0]: gcd(0, x) = |x| makes 0 the identity element,
// which removes the empty-array and first-element special cases at once.
// Once the running gcd reaches 1 it can never change, so we stop early.
// Time O(n log M), Space O(1).
ll gcdOfArray(const std::vector<ll> &v) {
    ll g = 0;
    for (ll x : v) {
        g = gcdIterative(g, x);
        if (g == 1) break;
    }
    return g;
}

// Are a and b coprime?   (G7's hypothesis)
// Note this does NOT mean either is prime: gcd(8, 9) = 1.
// Time O(log min(a,b)), Space O(1).
bool coprime(ll a, ll b) {
    return gcdIterative(a, b) == 1;
}

// ---------------------------------------------------------------------------
// Problem-solving patterns
// ---------------------------------------------------------------------------

// The "same remainder" pattern.
//
// Every d > 1 for which all v[i] leave the same remainder mod d must divide every
// difference v[i] - v[j], hence must divide this value. The valid moduli are exactly its
// divisors greater than 1. Returns 0 when all elements are equal (every d works).
// Time O(n log M), Space O(1).
ll gcdOfDifferences(const std::vector<ll> &v) {
    ll g = 0;
    for (std::size_t i = 1; i < v.size(); ++i) {
        g = gcdIterative(g, v[i] - v[0]);
    }
    return g;
}

// Lattice points on the segment from (x1,y1) to (x2,y2), endpoints included.
//
// The primitive step along the segment is (dx, dy) / g with g = gcd(|dx|, |dy|), so the
// segment is crossed in exactly g equal steps, visiting g + 1 integer points.
// A degenerate segment (both endpoints equal) contains the single point itself.
// Time O(log), Space O(1).
ll latticePointsOnSegment(ll x1, ll y1, ll x2, ll y2) {
    ll dx = x2 - x1;
    ll dy = y2 - y1;
    if (dx == 0 && dy == 0) return 1;
    return gcdIterative(dx, dy) + 1;
}

// base^exp over the integers, by binary exponentiation.
// Precondition: exp >= 0 and the result fits in 64 bits.
// Time O(log exp), Space O(1).
ll ipow(ll base, ll exp) {
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

// gcd(a^m - 1, a^n - 1) = a^gcd(m,n) - 1.   (Theorem 14 / G11)
//
// This collapses a gcd of two enormous numbers into a gcd of two small exponents: the
// Euclidean algorithm on the powers mirrors the subtractive one on the exponents.
// Precondition: a >= 2, m, n >= 0, and a^gcd(m,n) fits in 64 bits.
// Time O(log min(m,n) + log gcd(m,n)), Space O(1).
ll gcdOfPowersMinusOne(ll a, ll m, ll n) {
    return ipow(a, gcdIterative(m, n)) - 1;
}

// ---------------------------------------------------------------------------
// Demonstration
// ---------------------------------------------------------------------------

void demoEuclid() {
    std::cout << "--- The Euclidean algorithm ---\n";

    std::cout << "gcd(48, 18)     = " << gcdIterative(48, 18) << "   [expect 6]\n";
    std::cout << "gcd(1071, 462)  = " << gcdIterative(1071, 462) << "   [expect 21]\n";
    std::cout << "gcd(8, 9)       = " << gcdIterative(8, 9)
              << "   [expect 1 -- coprime, neither is prime]\n";

    // Order does not matter: a < b just swaps on the first step.
    std::cout << "gcd(18, 48)     = " << gcdIterative(18, 48) << "   [expect 6]\n";

    // Signs are normalized away (G1).
    std::cout << "gcd(-48, 18)    = " << gcdIterative(-48, 18) << "   [expect 6]\n";

    // Base cases (G2), including the gcd(0,0) convention.
    std::cout << "gcd(7, 0)       = " << gcdIterative(7, 0) << "   [expect 7]\n";
    std::cout << "gcd(0, 0)       = " << gcdIterative(0, 0) << "   [expect 0, by convention]\n";

    // The recursive form and the division-free form must agree everywhere.
    bool agree = true;
    for (ll a = 0; a <= 60; ++a) {
        for (ll b = 0; b <= 60; ++b) {
            if (gcdRecursive(a, b) != gcdIterative(a, b)) agree = false;
            if (binaryGcd(a, b) != gcdIterative(a, b)) agree = false;
        }
    }
    std::cout << "iterative == recursive == binary on all pairs 0..60: "
              << (agree ? "yes" : "NO") << "\n";

    // A pair of large coprime values, to show the algorithm does not care about size.
    std::cout << "gcd(1000000007, 998244353) = " << gcdIterative(1000000007LL, 998244353LL)
              << "   [expect 1 -- both prime]\n\n";
}

void demoComplexity() {
    std::cout << "--- Worst case is Fibonacci (Lame, Theorem 5) ---\n";

    // Consecutive Fibonacci numbers are the smallest inputs needing a given step count.
    ll prev = 1, cur = 1;
    for (int i = 0; i < 12; ++i) {
        ll next = prev + cur;
        prev = cur;
        cur = next;
    }
    std::cout << "gcd(" << cur << ", " << prev << ") takes " << euclidSteps(cur, prev)
              << " steps   [every quotient is 1 except the last -- the worst possible]\n";

    // Brute force confirmation on a small range: the worst pair really is Fibonacci.
    int worst = 0;
    ll worstA = 0, worstB = 0;
    for (ll a = 1; a < 100; ++a) {
        for (ll b = 1; b < a; ++b) {
            int s = euclidSteps(a, b);
            if (s > worst) { worst = s; worstA = a; worstB = b; }
        }
    }
    std::cout << "worst pair below 100 is (" << worstA << ", " << worstB << ") at " << worst
              << " steps   [F_11 and F_10]\n";

    // A same-sized non-Fibonacci pair finishes far sooner.
    std::cout << "gcd(" << cur << ", " << cur / 2 << ") takes "
              << euclidSteps(cur, cur / 2) << " steps\n";

    // Even the largest 64-bit inputs stay well inside the O(log) bound.
    std::cout << "gcd(9223372036854775807, 7540113804746346429) takes "
              << euclidSteps(9223372036854775807LL, 7540113804746346429LL)
              << " steps   [both near 2^63]\n\n";
}

void demoLcm() {
    std::cout << "--- LCM ---\n";

    std::cout << "lcm(12, 18) = " << lcm(12, 18) << "   [expect 36]\n";
    std::cout << "lcm(4, 6)   = " << lcm(4, 6) << "   [expect 12, not 24]\n";
    std::cout << "lcm(7, 5)   = " << lcm(7, 5) << "   [expect 35 -- coprime, so the product]\n";
    std::cout << "lcm(0, 5)   = " << lcm(0, 5) << "   [expect 0, by convention]\n";

    // Theorem 11 as an identity check: gcd * lcm == |a*b|.
    ll a = 12, b = 18;
    std::cout << "gcd(12,18) * lcm(12,18) = " << gcdIterative(a, b) * lcm(a, b)
              << "   [expect 216 = 12*18]\n";

    // The overflow trap: a*b would overflow here, but (a/g)*b does not.
    ll big1 = 6000000000LL, big2 = 9000000000LL;
    std::cout << "lcm(6e9, 9e9) = " << lcm(big1, big2)
              << "   [a*b would overflow; (a/g)*b does not]\n\n";
}

void demoPatterns() {
    std::cout << "--- Problem-solving patterns ---\n";

    std::cout << "gcd of {12, 18, 30} = " << gcdOfArray({12, 18, 30}) << "   [expect 6]\n";
    std::cout << "gcd of {6, 10, 15}  = " << gcdOfArray({6, 10, 15})
              << "   [expect 1 -- yet no two of them are coprime]\n";
    std::cout << "  check pairwise: gcd(6,10)=" << gcdIterative(6, 10)
              << " gcd(6,15)=" << gcdIterative(6, 15)
              << " gcd(10,15)=" << gcdIterative(10, 15) << "\n";

    // "Same remainder" pattern: 17, 32, 47 are all 2 mod 15 (and mod 3, and mod 5).
    std::vector<ll> v = {17, 32, 47};
    ll g = gcdOfDifferences(v);
    std::cout << "17, 32, 47 share a remainder modulo every divisor of " << g
              << "   [expect 15]\n";
    std::cout << "  17 mod 15 = " << 17 % 15 << ", 32 mod 15 = " << 32 % 15
              << ", 47 mod 15 = " << 47 % 15 << "\n";

    // Lattice points.
    std::cout << "lattice points on (0,0)-(12,8), endpoints included: "
              << latticePointsOnSegment(0, 0, 12, 8) << "   [expect 5, since gcd(12,8)=4]\n";
    std::cout << "lattice points on (0,0)-(5,3): "
              << latticePointsOnSegment(0, 0, 5, 3)
              << "   [expect 2 -- coprime, so only the endpoints]\n";

    // Reachability (Theorem 7): steps of 6 and 10 reach exactly the multiples of 2.
    std::cout << "steps of 6 and 10 reach exactly the multiples of "
              << gcdIterative(6, 10) << "   -- so 8 is reachable, 9 is not\n\n";
}

void demoExponentGcd() {
    std::cout << "--- gcd(a^m - 1, a^n - 1) = a^gcd(m,n) - 1 (Theorem 14) ---\n";

    // Verify the shortcut against a direct computation for values small enough to form.
    bool agree = true;
    for (ll a = 2; a <= 5; ++a) {
        for (ll m = 1; m <= 12; ++m) {
            for (ll n = 1; n <= 12; ++n) {
                if (ipow(a, m) > (1LL << 40) || ipow(a, n) > (1LL << 40)) continue;
                ll direct = gcdIterative(ipow(a, m) - 1, ipow(a, n) - 1);
                if (direct != gcdOfPowersMinusOne(a, m, n)) agree = false;
            }
        }
    }
    std::cout << "shortcut matches direct gcd for a in 2..5, m,n in 1..12: "
              << (agree ? "yes" : "NO") << "\n";

    std::cout << "gcd(2^6 - 1, 2^4 - 1) = gcd(63, 15) = " << gcdIterative(63, 15)
              << " = 2^gcd(6,4) - 1 = " << gcdOfPowersMinusOne(2, 6, 4) << "\n";

    // The payoff: exponents where the numbers themselves would never fit in 64 bits.
    std::cout << "gcd(2^1000000 - 1, 2^999999 - 1) = " << gcdOfPowersMinusOne(2, 1000000, 999999)
              << "   [gcd(1000000, 999999) = 1, so the Mersenne numbers are coprime]\n";
    std::cout << "gcd(2^36 - 1, 2^24 - 1) = " << gcdOfPowersMinusOne(2, 36, 24)
              << " = 2^12 - 1   [gcd(36,24) = 12]\n\n";
}

void demoStandardLibrary() {
    std::cout << "--- A note on std::gcd / std::lcm (C++17, <numeric>) ---\n";
    std::cout << "They exist and are correct for well-behaved inputs, but:\n";
    std::cout << "  * std::lcm has undefined behaviour if the RESULT overflows -- it does\n";
    std::cout << "    not save you from the a*b trap; you still owe the range check.\n";
    std::cout << "  * both are UB when |argument| is not representable in the result type\n";
    std::cout << "    (passing LLONG_MIN), and mixing signed with unsigned arguments\n";
    std::cout << "    silently picks a surprising common type.\n";
    std::cout << "Knowing why the loop above is three lines is worth more than the call.\n";
}

int main() {
    demoEuclid();
    demoComplexity();
    demoLcm();
    demoPatterns();
    demoExponentGcd();
    demoStandardLibrary();
    return 0;
}
