# Pattern: think in exponents

> *"The problem is about $n$, but the answer only depends on the exponents in its
> factorization."*

The most powerful reframing in the subject. Translate the statement into exponent space and
a hard-looking condition usually becomes an independent choice per prime.

---

## Recognition clues

* **"exactly $k$ divisors"**, "how many factors", $\tau(n)$ or $\sigma(n)$ named
* **perfect square, cube, or $k$-th power** — of $n$, or of a product
* **square-free**, "no prime appears twice", "no divisor is a perfect square"
* **trailing zeros** of $n!$, or in an arbitrary base
* the **product of an array**, asked about its primes rather than its value
* **"how many pairs multiply to a perfect $k$-th power"**
* $\gcd$ and $\mathrm{lcm}$ described as $\min$ and $\max$ per prime
* reconstructing $n$ from a list of its divisors
* grouping values that **share a prime factor**
* a small prime set (the primes below $30$, say) hinting at a **bitmask**

---

## Key observation

By [unique factorization](../concepts/prime-factorization/), a divisor of
$n = \prod p_i^{e_i}$ is exactly a choice of $f_i \in [0, e_i]$ for each prime —
**independently**. So:

$$\tau(n) = \prod_i (e_i + 1), \qquad
\sigma(n) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}, \qquad
\varphi(n) = n\prod_{p \mid n}\Bigl(1 - \frac1p\Bigr)$$

and every one of them is a **product over primes**, because the choices do not interact.
That independence is why the pattern works: a condition on $n$ becomes $k$ separate small
conditions.

Two specific levers:

$$n \text{ is a perfect } k\text{-th power} \iff k \mid e_i \text{ for every } i$$

$$\text{exponent of } p \text{ in } n! \;=\; \sum_{j\ge1} \Bigl\lfloor \frac{n}{p^{\,j}} \Bigr\rfloor \quad \text{(Legendre)}$$

---

## Relevant concepts

[prime-factorization](../concepts/prime-factorization/) ·
[divisors](../concepts/divisors/) · [sieve](../concepts/sieve/) ·
[pollard-rho](../concepts/pollard-rho/) · [euler-phi](../concepts/euler-phi/)

---

## Typical approach

1. Factorize. Which tool depends on the constraints — see
   [prime-pattern](prime-pattern.md).
2. **Throw away the primes, keep the exponent vector.** Most conditions do not care which
   primes they are.
3. Restate the requirement per prime. "Perfect $k$-th power" becomes "every exponent
   divisible by $k$"; "square-free" becomes "every exponent is $0$ or $1$".
4. If the condition is modular, **reduce each exponent mod $k$** first. Two numbers pair
   into a $k$-th power exactly when their residue vectors sum to zero.
5. For "exactly $k$ divisors", search the **shapes** $k = \prod(e_i+1)$, not the numbers,
   and assign the largest exponent to the smallest prime.
6. Small prime support? Encode the exponent parities as a **bitmask** and count subsets.

---

## Complexity

| factorize one $n \le 10^{12}$ | $O(\sqrt n)$ |
|---|---|
| factorize one $n \le 9\times10^{18}$ | $O(n^{1/4})$ |
| factorize many $m \le 10^7$ with `spf` | $O(\log m)$ each |
| $\tau$, $\sigma$ from a factorization | $O(k)$, with $k \le 15$ distinct primes |
| enumerating the divisors of one $n \le 10^{18}$ | $\le 103680$ of them |
| bitmask over the primes below $30$ | $2^{10}$ states |

The divisor bound is what makes "loop over all divisors" affordable: at most a hundred
thousand, once you have the factorization.

---

## Common traps

| | |
|---|---|
| forgetting the leftover prime after trial division | the `if (n > 1)` line |
| `p * p <= n` | overflows; write `p <= n / p` |
| $\tau(ab) = \tau(a)\tau(b)$ without $\gcd(a,b)=1$ | $\tau(2)\tau(2)=4$, $\tau(4)=3$ |
| the $\sigma$ formula's division under a modulus | sum the powers directly instead |
| factoring to compute a gcd | Euclid is far faster |
| treating $1$ as prime | it would break uniqueness |
| counting distinct primes when you need total exponents | different quantities |
| trial division past $10^{12}$ | that is Pollard's rho |
| $\sigma$ in an `int` | overflows before $10^6$ |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| LC 172 · Med | [Factorial Trailing Zeroes](https://leetcode.com/problems/factorial-trailing-zeroes/) | Legendre for $p=5$, the cleanest instance |
| CF 1114C · 1700 | [Trailing Loves](https://codeforces.com/problemset/problem/1114/C) | Legendre in an arbitrary base — per prime, then the minimum |
| CF 1165D · 1600 | [Almost All Divisors](https://codeforces.com/problemset/problem/1165/D) | reconstruct $n$ from its divisors by $d \leftrightarrow n/d$ |
| CF 1228C · 1700 | [Primes and Multiplication](https://codeforces.com/problemset/problem/1228/C) | exponent counting over a whole range, mod $10^9+7$ |
| CF 1225D · 1800 | [Power Products](https://codeforces.com/problemset/problem/1225/D) | reduce exponents mod $k$; pairs match on complementary vectors |
| CF 1305F · 2500 | [Kuroni and the Punishment](https://codeforces.com/problemset/problem/1305/F) | sample, factor the neighbourhood, test each prime found |
| LC 2521 · Med | [Distinct Prime Factors of Product](https://leetcode.com/problems/distinct-prime-factors-of-product-of-array/) | the union of prime supports; no product needed |
| LC 2572 · Med | [Square-Free Subsets](https://leetcode.com/problems/count-the-number-of-square-free-subsets/) | exponent parity as a bitmask over the primes below $30$ |
| LC 1735 · Hard | [Count Ways to Make Array With Product](https://leetcode.com/problems/count-ways-to-make-array-with-product/) | distribute each prime's exponent independently — stars and bars per prime |
| LC 1390 · Med | [Four Divisors](https://leetcode.com/problems/four-divisors/) | $\tau(n)=4$ means $p^3$ or $pq$ |
| LC 952 · Hard | [Largest Component by Common Factor](https://leetcode.com/problems/largest-component-size-by-common-factor/) | factor every value, union by shared prime |
| LC 2507 · Med | [Replace With Sum of Prime Factors](https://leetcode.com/problems/smallest-value-after-replacing-with-sum-of-prime-factors/) | factor, sum, repeat to a fixed point |

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
