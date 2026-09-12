// Chinese remainder theorem, general moduli.  Needs: modpow.cpp, extgcd.cpp
#include <optional>
#include <vector>
using ll = long long;

struct Congruence { ll rem, mod; };                 // x = rem (mod mod)

// Merge two.  Solvable only when gcd(m1,m2) divides (a2 - a1) -- with shared factors the
// two can genuinely contradict.  The combined modulus is lcm, NOT m1*m2.
// O(log min(m1,m2))
std::optional<Congruence> crtMerge(Congruence c1, Congruence c2) {
    ll x, y;
    ll g = extgcd(c1.mod, c2.mod, x, y);
    ll diff = c2.rem - c1.rem;
    if (diff % g != 0) return std::nullopt;

    ll lcm = c1.mod / g * c2.mod, step = c2.mod / g;
    ll t = mulMod(norm(diff / g, step), norm(x, step), step);
    return Congruence{norm(c1.rem + mulMod(c1.mod % lcm, t, lcm), lcm), lcm};
}

// Fold a list.  Accumulator starts at x = 0 (mod 1), which every integer satisfies.
// The modulus grows to the lcm of everything and can overflow.  O(k log M)
std::optional<Congruence> crtAll(const std::vector<Congruence> &system) {
    Congruence acc{0, 1};
    for (const Congruence &c : system) {
        auto merged = crtMerge(acc, c);
        if (!merged) return std::nullopt;
        acc = *merged;
    }
    return acc;
}
