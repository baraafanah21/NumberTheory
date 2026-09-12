// tau and sigma for every m <= n.  Each divisor visits its own multiples, so the total
// work is n*H_n.  O(n log n) -- not quadratic.  Swap the body for any divisor-indexed sum.
#include <vector>
using ll = long long;

std::vector<int> tauUpTo(int n) {
    std::vector<int> tau(n + 1, 0);
    for (int d = 1; d <= n; ++d)
        for (int m = d; m <= n; m += d)
            ++tau[m];
    return tau;
}

// 64-bit: sigma over a range of 1e6 already passes 2^31.
std::vector<ll> sigmaUpTo(int n) {
    std::vector<ll> sig(n + 1, 0);
    for (ll d = 1; d <= n; ++d)
        for (ll m = d; m <= n; m += d)
            sig[m] += d;
    return sig;
}
