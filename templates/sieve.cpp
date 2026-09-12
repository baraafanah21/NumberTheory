// Primes up to n.  O(n log log n) time, O(n) space
#include <vector>
using ll = long long;

std::vector<char> sieve(int n) {
    std::vector<char> isPrime(n + 1, 1);
    if (n >= 0) isPrime[0] = 0;
    if (n >= 1) isPrime[1] = 0;
    for (int p = 2; (ll)p * p <= n; ++p)            // stop at sqrt(n)
        if (isPrime[p])
            for (ll m = (ll)p * p; m <= n; m += p)  // start at p*p
                isPrime[m] = 0;
    return isPrime;
}
