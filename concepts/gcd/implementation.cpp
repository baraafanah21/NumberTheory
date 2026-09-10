// GCD and the Euclidean Algorithm -- g++ -std=c++17 -O2 implementation.cpp

#include <iostream>
#include <vector>

using ll = long long;

// Each step replaces (a,b) with (b, a mod b), which keeps every common divisor, and the
// second value strictly shrinks so the loop ends.  gcd(a,0) = |a| gives the answer.
// abs on entry because % takes the sign of the dividend.
// O(log min(a,b)) -- under 90 iterations for any 64-bit input.
ll gcd(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}

// The same recurrence, written the way the formula reads.  Identical speed; the recursion
// depth is O(log), so there is no stack risk.
ll gcdRec(ll a, ll b) { return b == 0 ? (a < 0 ? -a : a) : gcdRec(b, a % b); }

// |a*b| / gcd(a,b), written as (a/g)*b.  The product a*b overflows for inputs whose lcm
// fits; dividing first is exact because g divides a.  The zero guard also stops a
// division by a zero gcd.
// O(log min(a,b))
ll lcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    ll r = (a / gcd(a, b)) * b;
    return r < 0 ? -r : r;
}

// Fold from 0: gcd(0,x) = |x| makes 0 the identity, removing the empty and
// first-element cases.  Once the running gcd is 1 it can never change.
// O(n log M)
ll gcdOfArray(const std::vector<ll> &v) {
    ll g = 0;
    for (ll x : v) {
        g = gcd(g, x);
        if (g == 1) break;
    }
    return g;
}

// "Same remainder" pattern: if every v[i] agrees modulo d, then d divides every
// difference -- so the valid d are exactly the divisors of this value.
// Returns 0 when all elements are equal.
// O(n log M)
ll gcdOfDifferences(const std::vector<ll> &v) {
    ll g = 0;
    for (std::size_t i = 1; i < v.size(); ++i) g = gcd(g, v[i] - v[0]);
    return g;
}

// ---------------------------------------------------------------- demo

int main() {
    std::cout << "gcd\n";
    std::cout << "  gcd(48,18)    = " << gcd(48, 18) << "\n";
    std::cout << "  gcd(1071,462) = " << gcd(1071, 462) << "\t[three steps]\n";
    std::cout << "  gcd(8,9)      = " << gcd(8, 9) << "\t[coprime, neither prime]\n";
    std::cout << "  gcd(-48,18)   = " << gcd(-48, 18) << "\t[signs do not matter]\n";
    std::cout << "  gcd(7,0)      = " << gcd(7, 0) << "   gcd(0,0) = " << gcd(0, 0)
              << "\t[convention]\n";

    std::cout << "\nlcm\n";
    std::cout << "  lcm(12,18) = " << lcm(12, 18) << "   lcm(4,6) = " << lcm(4, 6)
              << "\t[12, not 24]\n";
    std::cout << "  lcm(7,5)   = " << lcm(7, 5) << "\t[coprime, so the full product]\n";
    std::cout << "  gcd*lcm    = " << gcd(12, 18) * lcm(12, 18) << "\t[= 12 x 18]\n";
    std::cout << "  lcm(6e9,9e9) = " << lcm(6000000000LL, 9000000000LL)
              << "\t[a*b would overflow; (a/g)*b does not]\n";

    std::cout << "\narrays\n";
    std::cout << "  gcd{12,18,30} = " << gcdOfArray({12, 18, 30})
              << "   gcd{6,10,15} = " << gcdOfArray({6, 10, 15}) << "\n";
    std::cout << "  but pairwise: " << gcd(6, 10) << " " << gcd(6, 15) << " " << gcd(10, 15)
              << "\t-> the set is coprime while NO pair is\n";

    std::cout << "\nsame-remainder trick\n";
    std::cout << "  17,32,47 agree modulo every divisor of " << gcdOfDifferences({17, 32, 47})
              << "\t(all are 2 mod 15, and 2 mod 5)\n";

    std::cout << "\nreachability (Bezout)\n";
    std::cout << "  steps of 6 and 10 reach exactly the multiples of " << gcd(6, 10)
              << "\t-> 8 yes, 9 no\n";

    bool ok = true, recOk = true;
    for (ll a = 1; a <= 200; ++a)
        for (ll b = 1; b <= 200; ++b) {
            ll best = 1;
            for (ll d = 1; d <= a && d <= b; ++d)
                if (a % d == 0 && b % d == 0) best = d;
            if (gcd(a, b) != best || lcm(a, b) != a / best * b) ok = false;
            if (gcdRec(a, b) != gcd(a, b)) recOk = false;
        }
    std::cout << "\nself-check: gcd and lcm vs brute force " << (ok ? "ok" : "FAIL")
              << ", loop == recursion " << (recOk ? "ok" : "FAIL") << "\n";
}
