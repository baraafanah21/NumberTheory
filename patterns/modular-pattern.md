# Pattern: the answer is huge, print it modulo $p$

> *"Output the answer modulo $10^9+7$."*

Almost never the subject of the problem. It is a formatting rule that turns a counting
problem into an arithmetic-hygiene problem, and most losses here are bookkeeping, not
insight.

---

## Recognition clues

* the literal phrase **"modulo $10^9+7$"** or **"modulo $998244353$"**
* **"the answer may be very large"** — a count of permutations, subsets, paths, ways
* **"the last $k$ digits"** — that is mod $10^k$
* a probability or expected value asked as an integer → they want $p\,q^{-1} \bmod M$
* a **fraction** anywhere in your derivation, printed under a modulus
* an average, a ratio, a division by $2$ in a closed form
* building a huge number digit by digit where only its residue matters

---

## Key observation

**$+$, $-$ and $\times$ pass straight through a modulus. Division does not.**

$$a \equiv b,\ c \equiv d \;\Longrightarrow\; a+c \equiv b+d, \quad ac \equiv bd \pmod m$$

That licence to reduce at every step is the whole pattern: nothing ever grows past $m^2$.
Division is the single exception, and it becomes a multiplication:

$$\frac ab \bmod m \;\longrightarrow\; a \cdot b^{-1} \bmod m, \qquad \text{needs } \gcd(b,m)=1$$

Two more facts decide most implementations. The modulus $10^9+7$ is **prime**, so Fermat
gives $b^{-1} = b^{\,p-2}$. And exponents reduce modulo $\varphi(m)$, **not** modulo $m$.

---

## Relevant concepts

[modular-arithmetic](../concepts/modular-arithmetic/) ·
[fast-power](../concepts/fast-power/) · [modular-inverse](../concepts/modular-inverse/) ·
[euler-phi](../concepts/euler-phi/)

---

## Typical approach

1. Derive the formula with no modulus at all. Get the mathematics right first.
2. Replace every $+$, $-$, $\times$ with its reducing version and every $\div$ with a
   multiplication by an inverse.
3. Factorials in the formula? Precompute `fact` and `invFact` once with a **single**
   exponentiation, then every $\binom nk$ is $O(1)$.
4. Subtraction anywhere? Normalize — `(a - b + m) % m`.
5. Check the modulus size. Above $\sim3\times10^9$, every multiply needs `__int128`.
6. Print. Confirm the result is non-negative.

---

## Complexity

| $+$, $-$, $\times$, normalize | $O(1)$ |
|---|---|
| one inverse, prime modulus | $O(\log p)$ |
| one inverse, any modulus | $O(\log m)$ via extended Euclid |
| $n$ inverses, prime modulus | $O(n)$ with the table |
| $\binom nk$ after an $O(n)$ build | $O(1)$ per query |

---

## Common traps

| | |
|---|---|
| `a * b % m` when $m > 3\times10^9$ | wraps silently; use `__int128` |
| `(a - b) % m` when $a < b$ | negative output, wrong answer or a crash as an index |
| `(a / b) % m` | not division; multiply by $b^{-1}$ |
| Fermat's inverse on a **composite** modulus | silently wrong, no error |
| assuming an inverse exists | only when $\gcd(b,m)=1$ |
| reducing the **exponent** mod $m$ | exponents reduce mod $\varphi(m)$ |
| calling the inverse routine inside a loop | build the table |
| `r = 1` as an accumulator when $m$ may be $1$ | write `1 % m` |
| forgetting to reduce a running sum | overflow after $\sim9$ additions near $10^{18}$ |
| a $2^{64}$ hash | not prime; anti-hash tests exist |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| CF 913A · 800 | [Modular Exponentiation](https://codeforces.com/problemset/problem/913/A) | $2^n$ against $m$; the naive loop is the trap |
| CF 495B · 1600 | [Modular Equations](https://codeforces.com/problemset/problem/495/B) | solve $n \bmod x = k$ for $x$ — a congruence read backwards |
| CF 1514C · 1600 | [Product 1 Modulo N](https://codeforces.com/problemset/problem/1514/C) | pair each unit with its inverse; the self-inverse ones are left |
| CF 300C · 1800 | [Beautiful Numbers](https://codeforces.com/problemset/problem/300/C) | $\binom nk \bmod p$ over many $k$, with a time limit |
| LC 50 · Med | [Pow(x, n)](https://leetcode.com/problems/powx-n/) | the squaring loop with no modulus, plus negative exponents |
| LC 1922 · Med | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | $5^{\lceil n/2\rceil}4^{\lfloor n/2\rfloor}$ with $n \le 10^{15}$ |
| LC 2550 · Med | [Count Collisions of Monkeys](https://leetcode.com/problems/count-collisions-of-monkeys-on-a-polygon/) | $2^n - 2$, where the subtraction is what people lose |
| LC 1015 · Med | [Smallest Integer Divisible by K](https://leetcode.com/problems/smallest-integer-divisible-by-k/) | build $1, 11, 111, \dots$ as residues; the true numbers overflow instantly |
| LC 2400 · Med | [Ways to Reach a Position](https://leetcode.com/problems/number-of-ways-to-reach-a-position-after-exactly-k-steps/) | one binomial mod $10^9+7$ |
| LC 1735 · Hard | [Count Ways to Make Array With Product](https://leetcode.com/problems/count-ways-to-make-array-with-product/) | stars and bars per prime — the full factorial setup |

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
