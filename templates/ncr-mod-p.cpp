// C(n,k) mod a prime, O(1) per query after an O(n) build.  Needs: modpow.cpp
#include <vector>
using ll = long long;

struct Binomial {
    ll p;
    std::vector<ll> fact, invFact;

    // One exponentiation for the whole inverse table, then walk down: inv((i-1)!) = inv(i!)*i
    Binomial(int n, ll prime) : p(prime), fact(n + 1), invFact(n + 1) {
        fact[0] = 1 % p;
        for (int i = 1; i <= n; ++i) fact[i] = mulMod(fact[i - 1], i, p);
        invFact[n] = powMod(fact[n], p - 2, p);
        for (int i = n; i >= 1; --i) invFact[i - 1] = mulMod(invFact[i], i, p);
    }

    ll choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return mulMod(fact[n], mulMod(invFact[k], invFact[n - k], p), p);
    }

    ll perm(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return mulMod(fact[n], invFact[n - k], p);
    }
};
