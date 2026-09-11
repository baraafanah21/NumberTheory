# Pollard's Rho Factorization

[Miller–Rabin](../miller-rabin/) tells you $n$ is composite. It does **not**
tell you a single factor. This does — for $n$ up to $9\times10^{18}$, in tens of thousands
of steps rather than billions.

$$O\bigl(n^{1/4}\bigr) \quad\text{instead of}\quad O\bigl(\sqrt n\bigr)$$

**Use:** factoring one large number — and everything downstream of a factorization
($\tau$, $\sigma$, $\varphi$, divisors) once $n$ is past $10^{12}$. **Needs:**
[Miller–Rabin](../miller-rabin/) (the base case) ·
[gcd](../gcd/). **Complements:**
[sieve](../sieve/) for many small numbers,
[unique factorization](../prime-factorization/) for the $O(\sqrt n)$ version.

---

## The gap this fills

Deciding *is $n$ prime* is fast. Producing *the factors of $n$* is not, and no known method
converts one into the other. That asymmetry is not a gap in this repo — it is what RSA is
built on.

| $n \le 10^7$, many numbers | [sieve](../sieve/) + `spf` | $O(\log n)$ each |
|---|---|---|
| $n \le 10^{12}$, one number | [trial division](../prime-factorization/) | $O(\sqrt n)$ |
| $n \le 9\times10^{18}$, one number | **Pollard's rho** | $O(n^{1/4})$ |

---

## The idea: collide modulo a prime you cannot see

Iterate $f(x) = x^2 + c$ from a random start, everything mod $n$. Now read that same
sequence **mod $p$**, for a prime $p \mid n$ you do not know. Since squaring commutes with
reduction, it is the same iteration on a set of size $p$ — and by the birthday bound it
repeats after about $\sqrt p$ steps.

```text
                        x4 ---- x5
                       /          \
   x0 ---- x1 ---- x2-x3           x6        read mod p, the walk must close
                       \          /          into a loop: a tail, then a cycle
                        x8 ---- x7           -- the shape of the letter rho
```

When two entries agree mod $p$ but differ mod $n$:

$$\boxed{\ x \equiv y \!\!\pmod p,\ \ x \not\equiv y \!\!\pmod n \ \Longrightarrow\ 1 < \gcd(|x-y|,\ n) < n\ }$$

The gcd mentions only $x$, $y$ and $n$ — all of which you have. **You detect a collision
modulo an unknown prime by its shadow on $n$.**

And since the smallest prime factor is at most $\sqrt n$, the $\sqrt p$ steps cost at most
$n^{1/4}$ — about $31{,}600$ for $n = 10^{18}$, or $\approx40{,}000$ once the birthday
constant $1.25$ is included.

---

## The algorithm

```cpp
ll pollardRho(ll n) {                            // n composite, and not even
    ll c = rand_in(1, n - 1), x = rand_in(0, n - 1), y = x, g = 1;
    auto f = [&](ll v) { return (ll)(((__int128)v * v + c) % n); };

    do {
        x = f(x);                                // tortoise: one step
        y = f(f(y));                             // hare: two steps
        g = gcd(std::llabs(x - y), n);           // the shadow of a collision mod p
    } while (g == 1);

    return g;                                    // g == n means: retry with a new c
}
```

That is the whole method. `implementation.cpp` differs only in being faster, never in what
it computes.

---

## Two speedups that matter

**Brent instead of Floyd.** Keep a checkpoint $x = x_{2^k}$ and walk $y$ over the next
$2^k$ values. Same $O(\lambda+\mu)$ guarantee, but **one** call to $f$ per step instead of
Floyd's three.

**Batch the gcds.** A `gcd` is a division loop; a `mulMod` is one multiply. So accumulate

$$q \leftarrow q\cdot|x-y| \bmod n$$

over $128$ steps and take **one** gcd. Nothing is lost: a prime $p \mid n$ divides the
product exactly when it divides one of the factors (Euclid's lemma, [proofs](proofs.md)
§6). The catch is that a batch can catch *two* primes at once and return $g = n$ — so keep
the batch's starting point and replay it step by step when that happens.

Together these are worth roughly an order of magnitude, which is the difference between
microseconds and milliseconds per number.

---

## Full factorization

Rho splits $n$ once, into two pieces that need not be prime. Recurse:

```cpp
void factorRec(ll n, std::map<ll,int> &out) {
    if (n == 1) return;
    if (isPrime(n)) { ++out[n]; return; }        // Miller-Rabin: the base case
    ll d = pollardRho(n);
    factorRec(d, out);
    factorRec(n / d, out);
}
```

**Strip the small primes by trial division first.** Two reasons: rho would spend its whole
$n^{1/4}$ budget hunting a factor one division finds, and $f(x)=x^2+c$ **cannot split
$n=4$** for any $c$ or any start — verified exhaustively in [proofs](proofs.md) §7.

Without `isPrime` the recursion has no stopping rule and calls rho on a prime, which never
returns. The primality test is not an optimisation here; it is what makes the algorithm
terminate.

---

## How to recognize it in a problem

Look for:

* **"factorize $n$"** with $n$ up to $10^{18}$ — trial division needs $10^9$ operations,
  this needs about $30{,}000$
* $\tau$, $\sigma$, $\varphi$, or the divisor list **of one huge number** — all of them
  reduce to having the factorization
* finding a **shared prime factor** between large values, or grouping values by a common
  prime
* "change each element by at most $1$ so they all share a factor" — sample elements, factor
  the few values around them, test the primes that appear
* any problem where the input is a single number near $10^{18}$ and the answer depends on
  its multiplicative structure

**The tell.** You need the *factors*, not just primality, and the number is past $10^{12}$.
Below that, trial division is simpler and fast enough — reaching for rho early is a common
over-engineering mistake.

**Anti-pattern.** Many numbers, all small, is the [sieve](../sieve/) with an `spf` table,
not this. And never call rho without a primality test first: on a prime input it does not
return.

---

## Complexity

| one split | $O(n^{1/4})$ expected — $\sim31{,}600$ steps at $10^{18}$ |
| --- | --- |
| full factorization | $O(n^{1/4}\log n)$ expected |
| $\tau$, $\sigma$, $\varphi$, all divisors of one big $n$ | the factorization dominates |
| trial division, for comparison | $O(\sqrt n) = 10^9$ at $n = 10^{18}$ |

> The $n^{1/4}$ is a **heuristic**, not a theorem: it assumes $x^2+c$ behaves like a random
> map, which nobody has proved. Correctness does not depend on it — only the speed does.

---

## Pitfalls

| | |
| --- | --- |
| calling rho on a prime | it never returns — `isPrime` first, always |
| $n = 4$ | no $(c, x_0)$ splits it; strip small primes before rho |
| $c = 0$ or $c = -2$ | degenerate orbits, cycle $\sim\log p$ instead of $\sqrt p$ |
| `x * x + c` in `long long` | wraps past $n > 3\times10^9$; `__int128`, as in Miller–Rabin |
| the batch product in `long long` | same overflow, one line further down |
| one `gcd` per step | the gcd dominates the multiply — batch $128$ |
| no replay when $g = n$ | the batch caught two primes; you lose a real split |
| expecting the smallest factor first | rho returns *a* factor; sort at the end |
| no retry loop | a degenerate start returns $g=n$ and must be redrawn |
| reaching for rho below $10^{12}$ | trial division is simpler and fast enough |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp) · [problems.md](problems.md)
