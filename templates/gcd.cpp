// gcd, lcm.  O(log min(a,b))
using ll = long long;

ll gcd(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b) { ll r = a % b; a = b; b = r; }
    return a;
}

// (a/g)*b, never a*b/g -- the product overflows for inputs whose lcm fits.
ll lcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    ll r = (a / gcd(a, b)) * b;
    return r < 0 ? -r : r;
}
