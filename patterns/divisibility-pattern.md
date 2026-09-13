# Pattern: counting multiples

> *"How many numbers up to $n$ are divisible by …"*

The cheapest pattern in the subject, and the one people most often write a loop for. The
answer is a division, not an iteration.

---

## Recognition clues

* **"how many $x \le n$ are divisible by $d$"**, or by any of several $d$
* **"divisible by all of"** a small fixed set → one lcm, then one division
* **"divisible by at least one of"** → inclusion–exclusion over subsets
* $n$ up to $10^{18}$ with a *counting* question — far too large to loop, so a formula must
  exist
* **"the $k$-th number with property P"** where P is a divisibility condition → binary
  search on the answer, with a counting function inside
* **for each $d$, do something to every multiple of $d$** — the sieve skeleton
* a number given as a **string** because it does not fit any integer type → digit tests
* **toggling** / lockers / bulbs → $\tau(n)$ parity, so the answer is the perfect squares

---

## Key observation

$$\#\{x : 1 \le x \le n,\ d \mid x\} \;=\; \Bigl\lfloor \frac nd \Bigr\rfloor$$

Constant time, any $n$. Everything else in this pattern is built on it:

* **several divisors at once** — inclusion–exclusion, with lcm as the combining rule:
  $\lfloor n/a\rfloor + \lfloor n/b\rfloor - \lfloor n/\mathrm{lcm}(a,b)\rfloor$
* **range $[l, r]$** — count to $r$, subtract the count to $l-1$
* **the inversion** — instead of asking each $m$ for its divisors, let each $d$ walk its
  own multiples. The inner loop runs $n/d$ times and $\sum_{d\le n} n/d = \Theta(n\log n)$,
  so it is not quadratic

---

## Relevant concepts

[divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) ·
[gcd](../concepts/gcd/) (for the lcm) · [sieve](../concepts/sieve/)

---

## Typical approach

1. Write the count as $\lfloor n/d\rfloor$ and check whether that alone answers it.
2. More than one divisor? Inclusion–exclusion over the subsets, combining with lcm. With
   $k$ divisors that is $2^k$ terms, so it is only viable for small $k$.
3. Asked for the $k$-th such number rather than the count? **Binary search the answer** and
   put the counting function inside. This turns almost every "$k$-th" variant into the
   counting variant.
4. Needed for every $m$ in a range? Invert to the divisor-visiting sieve.
5. Number too big to parse? Digit sum for $3$ and $9$, alternating digit sum for $11$, last
   $j$ digits for $2^j$ and $5^j$.

---

## Complexity

| one count $\lfloor n/d\rfloor$ | $O(1)$ |
|---|---|
| inclusion–exclusion over $k$ divisors | $O(2^k)$ |
| $k$-th such number, by binary search | $O(2^k \log n)$ |
| divisors of one $n$ | $O(\sqrt n)$ |
| $\tau$ or $\sigma$ for all $m \le n$ | $O(n\log n)$ |
| distinct values of $\lfloor n/d\rfloor$ | only $O(\sqrt n)$ of them — the divisor-block trick |

That last row is the escape hatch when $n = 10^{12}$ and a sieve will not fit.

---

## Common traps

| | |
|---|---|
| looping to count multiples | it is one division |
| `d * d <= n` | overflows near $3\times10^9$; write `d <= n / d` |
| double-counting $\sqrt n$ in the pair loop | guard `if (d != n / d)` |
| inclusion–exclusion combining with products | the rule is lcm, not $a \cdot b$ |
| the lcm overflowing inside inclusion–exclusion | cap it against $n$ and bail early |
| counting in $[l,r]$ with $\lfloor r/d\rfloor - \lfloor l/d\rfloor$ | it is $l-1$, not $l$ |
| $\sigma$ in an `int` | overflows before $n = 10^6$ |
| assuming $d \mid ab$ gives $d \mid a$ or $d \mid b$ | false unless $d$ is prime |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| CF 630J · 1100 | [Divisibility](https://codeforces.com/problemset/problem/630/J) | divisible by all of $2..10$ — one lcm, then $\lfloor n/d\rfloor$ |
| CF 762A · 1400 | [k-th divisor](https://codeforces.com/problemset/problem/762/A) | the $O(\sqrt n)$ pair loop, and the $d = n/d$ edge case |
| CF 1512G · 1700 | [Short Task](https://codeforces.com/problemset/problem/1512/G) | the divisor-visiting skeleton with `sigma[m] += d` |
| CF 26A · 900 | [Almost Prime](https://codeforces.com/problemset/problem/26/A) | count distinct prime divisors over a range, same skeleton |
| LC 1201 · Med | [Ugly Number III](https://leetcode.com/problems/ugly-number-iii/) | inclusion–exclusion on three divisors, under a binary search |
| LC 1492 · Med | [The kth Factor of n](https://leetcode.com/problems/the-kth-factor-of-n/) | divisors in $O(\sqrt n)$, sorted, no double count |
| LC 2427 · Easy | [Number of Common Factors](https://leetcode.com/problems/number-of-common-factors/) | common divisors are the divisors of the gcd |
| LC 1952 · Easy | [Three Divisors](https://leetcode.com/problems/three-divisors/) | $\tau(n)=3$ forces $n=p^2$ |
| LC 507 · Easy | [Perfect Number](https://leetcode.com/problems/perfect-number/) | $\sigma(n) = 2n$, one value at a time |

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
