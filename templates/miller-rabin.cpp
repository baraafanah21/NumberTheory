// Deterministic primality for every 64-bit n.  Needs: modpow.cpp
// ~750 modular multiplications at n near 1e18, against 1e9 for trial division.
using ll = long long;

// n - 1 = d * 2^s with d odd, then square up the chain.
bool passesBase(ll n, ll a) {
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }

    ll x = powMod(a, d, n);
    if (x == 1 || x == n - 1) return true;          // the r = 0 case
    for (int r = 1; r < s; ++r) {
        x = mulMod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;                                    // a is a witness: n is composite
}

// The first 12 primes are a verified witness set for n < 3.18e23.  Dropping one breaks
// part of long long: the first 11 only reach 3.8e18.
bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    for (ll a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (!passesBase(n, a)) return false;
    return true;
}
