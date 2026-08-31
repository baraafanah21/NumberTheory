# Unique Factorization

The **fundamental theorem of arithmetic**, and the theorem this whole repo stands on.

> Every integer $n>1$ is a product of primes, in **exactly one** way apart from order.

$$n = p_1^{e_1}p_2^{e_2}\cdots p_k^{e_k}, \qquad p_1 < p_2 < \cdots < p_k$$

**Use:** it is what makes "the exponent of $p$ in $n$" a meaningful quantity — and every
formula stated in terms of a factorization. **Needs:**
[gcd](../gcd-and-euclidean-algorithm/) (Euclid's lemma)

---

## Why uniqueness is not obvious

Existence is easy: keep splitting, the pieces shrink. **Uniqueness is a real theorem** —
there are sensible systems where it fails.

Take only the **even** numbers and call something "prime" if it is not a product of two
*evens*. Then $2, 6, 10, 30$ all qualify ($6 = 2\times3$ does not count, $3$ is odd). Yet

$$60 = 2 \times 30 = 6 \times 10$$

Two different factorizations into "primes". What rescues $\mathbb{Z}$ is **Euclid's lemma**
— and this system fails it ($6 \mid 2\times30$, but $6$ divides neither).

Compare divisibility's trap: $6 \mid 4\times9$ while $6$ divides neither, because
$6 = 2\times3$ **splits** across the factors. A prime cannot split. That is the whole
difference.

---

## What it buys

**Divisors are the exponent-wise smaller numbers:**

$$d \mid n \quad\Longleftrightarrow\quad d = \prod p_i^{f_i} \ \text{ with } \ 0 \le f_i \le e_i$$

so, with each formula proved elsewhere in the repo:

$$\tau(n) = \prod_i (e_i+1), \qquad
\sigma(n) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}, \qquad
\varphi(n) = n\prod_{p\mid n}\Bigl(1-\frac1p\Bigr)$$

**gcd and lcm read off exponent-wise:**

$$\gcd(a,b) = \prod_p p^{\min(\alpha_p,\beta_p)}, \qquad \mathrm{lcm}(a,b) = \prod_p p^{\max(\alpha_p,\beta_p)}$$

> For **understanding**, not computing — factoring is far slower than Euclid. But it
> explains $\gcd\cdot\mathrm{lcm} = ab$ in one line, since $\min+\max = x+y$.

**And instant tests:** $n$ is a perfect $k$-th power iff every $e_i$ is divisible by $k$;
$\sqrt2$ is irrational (proofs §5).

---

## Computing it

```cpp
// canonical form, as (prime, exponent) pairs in increasing order.  O(sqrt n)
std::vector<std::pair<ll,int>> factorize(ll n) {
    std::vector<std::pair<ll,int>> f;
    for (ll p = 2; p <= n / p; ++p)          // p <= n/p, never p*p <= n
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            f.push_back({p, e});
        }
    if (n > 1) f.push_back({n, 1});          // a prime factor above sqrt is left
    return f;
}
```

The trailing `if` is the line people forget. Nothing here is new — what the theorem adds is
the guarantee this returns **the** factorization, not *a* factorization.

| one $n \le 10^{12}$ | trial division | $O(\sqrt n)$ |
|---|---|---|
| many $m \le 10^7$ | `spf` table from the [sieve](../sieve-of-eratosthenes/) | $O(\log m)$ each |

---

## Pitfalls

| | |
|---|---|
| forgetting the leftover prime | the `if (n > 1)` line |
| treating $1$ as prime | it would destroy uniqueness |
| `p * p <= n` overflows | `p <= n / p` |
| using min/max exponents to *compute* gcd | factoring is far slower than Euclid |
| assuming uniqueness needs no proof | see the even-numbers system above |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
