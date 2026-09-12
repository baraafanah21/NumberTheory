// Euler's totient.
#include <vector>
using ll = long long;

// One value.  'r -= r / p' applies (1 - 1/p) in integers; r * (1 - 1/p) truncates to 0.
// O(sqrt n)
ll phi(ll n) {
    if (n <= 0) return 0;
    ll r = n;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            r -= r / p;
        }
    if (n > 1) r -= r / n;                          // one large prime factor is left
    return r;
}

// Every value up to n.  phi[p] == p doubles as a primality test.  O(n log log n)
std::vector<int> phiSieve(int n) {
    std::vector<int> f(n + 1);
    for (int i = 0; i <= n; ++i) f[i] = i;
    for (int p = 2; p <= n; ++p)
        if (f[p] == p)
            for (int m = p; m <= n; m += p)
                f[m] -= f[m] / p;
    return f;
}
