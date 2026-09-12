// Smallest prime factor for every m <= n, plus the prime list.  Linear sieve, O(n).
// Factorizing any m <= n is then O(log m).
#include <utility>
#include <vector>
using ll = long long;

std::vector<int> spfSieve(int n, std::vector<int> &primes) {
    std::vector<int> spf(n + 1, 0);
    primes.clear();
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) { spf[i] = i; primes.push_back(i); }
        for (int p : primes) {
            if (p > spf[i] || (ll)i * p > n) break;  // the break that makes it linear
            spf[i * p] = p;
        }
    }
    return spf;
}

// (prime, exponent) pairs, increasing.  Precondition: 1 <= m <= n.  O(log m)
std::vector<std::pair<int, int>> factorizeSpf(int m, const std::vector<int> &spf) {
    std::vector<std::pair<int, int>> f;
    while (m > 1) {
        int p = spf[m], e = 0;
        while (m % p == 0) { m /= p; ++e; }
        f.push_back({p, e});
    }
    return f;
}
