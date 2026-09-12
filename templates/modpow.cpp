// Modular arithmetic base: every other modular template needs these.
using ll = long long;

ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }

// __int128 is required once m > ~3e9; a*b%m silently wraps above that.
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }

ll powMod(ll base, ll exp, ll m) {
    ll r = 1 % m;                       // 1 % m, not 1: m may be 1
    base = norm(base, m);
    while (exp > 0) {
        if (exp & 1) r = mulMod(r, base, m);
        base = mulMod(base, base, m);
        exp >>= 1;
    }
    return r;
}
