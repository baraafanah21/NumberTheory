# Unique Factorization

Also called the **Fundamental Theorem of Arithmetic** — and it earns the name, because
almost everything else in this repository quietly assumes it.

> Every integer $n > 1$ can be written as a product of primes, and that way is
> **unique** apart from the order of the factors.

$$n \;=\; p_1^{e_1}\,p_2^{e_2}\cdots p_k^{e_k}, \qquad p_1 < p_2 < \cdots < p_k$$

Written this way — primes in increasing order, exponents $e_i \ge 1$ — the form is called
**canonical**, and it is completely determined by $n$.

**You need this for:** every formula in this repo that is stated "in terms of the prime
factorization" — $\tau(n)$, $\sigma(n)$, $\varphi(n)$, and gcd/lcm by exponents.

**Before this:** [gcd](../gcd-and-euclidean-algorithm/) — specifically Euclid's lemma.

---

## Why this is not obvious

Existence is easy — keep splitting until nothing splits further. **Uniqueness is the
hard part**, and it is genuinely a theorem: there are perfectly sensible number systems
where it is false.

**A system where factorization is not unique.** Take only the **even** numbers
$\{2, 4, 6, 8, \dots\}$ and multiply as usual. Call a number "prime" here if it is not a
product of two numbers *from this set*. Then $2, 6, 10, 30$ are all prime in this
system — for example $6 = 2\times 3$, but $3$ is not even, so that does not count as a
splitting. Yet

$$60 \;=\; 2 \times 30 \;=\; 6 \times 10$$

Two genuinely different factorizations into "primes". Nothing about the *definition* of a
prime prevents this — what saves the ordinary integers is Euclid's lemma, and this system
does not satisfy it ($6 \mid 2 \times 30$ but $6$ divides neither $2$ nor $30$ within the
set).

So uniqueness in $\mathbb{Z}$ is a real fact needing a real proof, not bookkeeping.

---

## What makes it work

The whole theorem rests on **Euclid's lemma**, proved in the
[gcd concept](../gcd-and-euclidean-algorithm/proofs.md):

$$p \text{ prime}, \quad p \mid a\,b \qquad\Longrightarrow\qquad p \mid a \ \text{ or } \ p \mid b$$

Compare this with the trap from [divisibility](../divisibility/): $6 \mid 4\times 9$ yet
$6$ divides neither, because $6 = 2\times3$ **splits** across the two factors. A prime
cannot split. That is the entire difference, and it is why uniqueness holds for primes and
fails in the even-numbers system above.

---

## What it buys you

Once the factorization is unique, "the exponent of $p$ in $n$" is a well-defined number,
and these all follow:

**Divisors are exactly the exponent-wise smaller numbers.**

$$d \mid n \quad\Longleftrightarrow\quad d = \prod p_i^{f_i} \ \text{ with } \ 0 \le f_i \le e_i$$

**So the divisor formulas hold** (used in [divisibility](../divisibility/)):

$$\tau(n) = \prod_{i}(e_i + 1), \qquad \sigma(n) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}$$

**And the totient formula** (used in [Euler's totient](../euler-totient-function/)):

$$\varphi(n) = n\prod_{p \mid n}\left(1 - \frac1p\right)$$

**gcd and lcm read off exponent-wise:**

$$\gcd(a,b) = \prod_p p^{\min(\alpha_p,\ \beta_p)}, \qquad \mathrm{lcm}(a,b) = \prod_p p^{\max(\alpha_p,\ \beta_p)}$$

> These two are for **understanding**, not for computing — getting the factorization is far
> harder than running Euclid. But they instantly explain why
> $\gcd(a,b)\cdot\mathrm{lcm}(a,b) = ab$: because $\min(x,y) + \max(x,y) = x + y$.

**Quick tests that become obvious:**

| Question | Answer in terms of exponents |
|---|---|
| Is $n$ a perfect square? | every $e_i$ is even |
| Is $n$ a perfect $k$-th power? | every $e_i$ is divisible by $k$ |
| Is $\sqrt{2}$ rational? | no — see [proofs.md](proofs.md) §5 |

---

## Computing the factorization

Nothing new here — the algorithms already exist in earlier concepts. What this theorem adds
is the guarantee that they return **the** answer rather than *an* answer.

| Range | Method | Time |
|---|---|---|
| one $n \le 10^{12}$ | trial division to $\sqrt n$ | $O(\sqrt n)$ |
| many $m \le 10^7$ | `spf` table from the [sieve](../sieve-of-eratosthenes/) | $O(\log m)$ each |
| one $n \le 10^{18}$ | Pollard's rho — not yet in this repo | $O(n^{1/4})$ |

```cpp
// canonical form of n, as (prime, exponent) pairs in increasing order.  O(sqrt n)
std::vector<std::pair<ll,int>> factorize(ll n) {
    std::vector<std::pair<ll,int>> f;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            f.push_back({p, e});
        }
    if (n > 1) f.push_back({n, 1});     // a prime factor larger than sqrt is left
    return f;
}
```

The trailing `if` is the line people forget: after the loop, whatever remains of $n$ is
either $1$ or a single prime bigger than the original $\sqrt n$.

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Forgetting the leftover prime after the loop | the `if (n > 1)` line |
| Treating $1$ as prime | it is not — it would destroy uniqueness ($6 = 2\cdot3 = 1\cdot2\cdot3 = \ldots$) |
| Factoring $0$ or negative numbers | the theorem is about $n > 1$; handle signs separately |
| Loop bound `p * p <= n` overflowing | use `p <= n / p` |
| Forgetting $n$ shrinks inside the loop | that is what makes it $O(\sqrt n)$ and not $O(\sqrt{n_0})$ per prime |
| Using the min/max exponent rule to *compute* gcd | factoring is far slower than Euclid |
| Assuming uniqueness needs no proof | see the even-numbers system above |

---

## Files

- [proofs.md](proofs.md) — existence by induction, uniqueness from Euclid's lemma, the
  divisor characterization, gcd/lcm by exponents, and $\sqrt2$ irrational.
- [implementation.cpp](implementation.cpp) — factorization plus the formulas it unlocks.
