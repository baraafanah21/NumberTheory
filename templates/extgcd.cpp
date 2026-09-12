// Bezout coefficients and linear Diophantine equations.  O(log min(a,b))
using ll = long long;

// returns g = gcd(a,b), sets a*x + b*y == g.  Call with a, b >= 0.
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// One solution of a*x + b*y = c, or false when gcd(a,b) does not divide c.
// All solutions: x + k*(b/g), y - k*(a/g).  Step by b/g, NOT by b.
bool diophantine(ll a, ll b, ll c, ll &x, ll &y, ll &g) {
    if (a == 0 && b == 0) { x = y = 0; g = 0; return c == 0; }
    g = extgcd(a, b, x, y);
    if (c % g != 0) return false;
    ll k = c / g;
    x *= k;
    y *= k;
    return true;
}
