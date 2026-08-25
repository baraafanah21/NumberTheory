# Why it works — Divisibility

Only the proofs that change what you write. Each one ends with the line of code it
justifies.

---

## 1. Linearity (D1)

> $d \mid a$ and $d \mid b$ $\Rightarrow$ $d \mid (ax + by)$ for all integers $x, y$.

$a = dm$, $b = dn$, so $ax + by = d(mx + ny)$. Done.

**Special case that is the Euclidean step:** if $d \mid b$, then
$d \mid a \iff d \mid (a - qb)$ — apply the above with $(1, -q)$ and $(1, q)$.

**Justifies:** every digit test below, and the whole GCD concept.

---

## 2. The division algorithm, and why `b % a == 0` is a valid test (D2)

> For $b \neq 0$ there are **unique** $q, r$ with $a = qb + r$ and $0 \leq r < \lvert b \rvert$.

*Existence:* take $r$ to be the smallest non-negative value of $a - qb$ (it exists by
well-ordering). If $r \geq \lvert b \rvert$ you could subtract $\lvert b \rvert$ once more
and stay non-negative, contradicting minimality. So $r < \lvert b \rvert$.

*Uniqueness:* if $q_1b + r_1 = q_2b + r_2$ then $\lvert q_1 - q_2\rvert\lvert b \rvert = \lvert r_2 - r_1\rvert < \lvert b \rvert$,
forcing $\lvert q_1 - q_2 \rvert < 1$, hence $q_1 = q_2$ and $r_1 = r_2$.

**Why uniqueness matters in code:** $a \mid b$ gives $b = ak + 0$, a valid decomposition.
Uniqueness says it is the *only* one, so the remainder really is $0$ — that is what makes
`b % a == 0` an if-and-only-if test rather than a one-way check.

**C++ warning:** the language returns $r$ with the sign of the *dividend*, so
`-17 % 5 == -2`, not $3$. Normalize with `((a % b) + b) % b` whenever the value is used as
an index or compared against $0 \leq r < \lvert b \rvert$.

---

## 3. Why the $O(\sqrt n)$ divisor loop finds everything (D3)

> If $d \mid n$ then $n/d \mid n$, and $\min(d,\ n/d) \leq \sqrt n$.

$n/d$ divides $n$ because $n = (n/d)\cdot d$. For the bound: if both $d > \sqrt n$ and
$n/d > \sqrt n$, multiplying gives $n = d\cdot\frac{n}{d} > n$ — absurd.

So every divisor is either found directly as some $d \leq \sqrt n$, or is emitted as the
partner $n/d$ of one. Nothing is missed.

**Justifies:**

```cpp
for (ll d = 1; d <= n / d; ++d)
    if (n % d == 0) { use(d); if (d != n / d) use(n / d); }
```

The `d != n / d` guard exists because when $n$ is a perfect square the pair collapses to a
single divisor, which would otherwise be counted twice.

---

## 4. $\tau(n)$ odd $\iff$ $n$ is a perfect square (D4)

The map $d \mapsto n/d$ pairs the divisors up. A pair contributes $2$ to $\tau(n)$, so

$$\tau(n) \equiv \#\{\text{self-paired divisors}\} \pmod 2$$

and $d$ is self-paired iff $d = n/d$ iff $n = d^2$. A non-square has none (count even); a
square has exactly one, namely $\sqrt n$ (count odd).

**Justifies:** the locker/switch problem. Cell $m$ is toggled once per divisor, so it ends
flipped iff $\tau(m)$ is odd iff $m$ is a square. Answer: $\lfloor\sqrt n\rfloor$, no
simulation.

---

## 5. Trial division only needs $\sqrt n$

> If $n > 1$ is composite it has a divisor in $[2, \sqrt n]$.

Write $n = ab$ with $1 < a \leq b$. Then $a^2 \leq ab = n$, so $a \leq \sqrt n$.

**Justifies:** `for (d = 2; d <= n/d; ++d) if (n % d == 0) return false;` — a composite
always betrays itself below $\sqrt n$, so finding nothing proves primality.

---

## 6. Why the sieve is $O(n\log n)$, not $O(n^2)$

The nested loop runs $\lfloor n/d \rfloor$ times for each $d$, so the total is

$$\sum_{d=1}^{n}\left\lfloor\frac{n}{d}\right\rfloor \approx n\sum_{d=1}^{n}\frac{1}{d} = nH_n$$

**Bounding $H_n$ (dyadic blocks).** Group indices into blocks $[2^j,\ 2^{j+1})$. Block $j$
has $2^j$ terms, each between $\frac{1}{2^{j+1}}$ and $\frac{1}{2^j}$, so each block sums to
between $\frac12$ and $1$. There are $\approx \log_2 n$ blocks, hence

$$\frac{\log_2 n}{2} \ \leq\ H_n \ \leq\ \log_2 n + 1 \qquad\Rightarrow\qquad H_n = \Theta(\log n)$$

**Justifies:** the multiples skeleton looks quadratic and is not. It is safe up to
$n \approx 10^7$.

**Bonus:** the same sum says the *average* number of divisors below $n$ is about $\ln n$ —
about $12$ at $n = 10^5$. Divisors are rare; storing all of them per number is cheap.

---

## 7. Digit tests

**For 9 and 3.** $9 \mid 10^k - 1$ for every $k$ (indeed $10^k - 1 = \underbrace{9\cdots9}_{k}$).
Writing $n = \sum d_k 10^k$ and $S = \sum d_k$:

$$n - S = \sum_k d_k\bigl(10^k - 1\bigr)$$

Every term is a multiple of $9$, so $9 \mid (n - S)$. By linearity, $9 \mid n \iff 9 \mid S$.
Since $3 \mid 9$, the same works for $3$.

**For 11.** $11 \mid 10^k - (-1)^k$, so with the alternating sum $A = \sum (-1)^k d_k$,

$$n - A = \sum_k d_k\bigl(10^k - (-1)^k\bigr)$$

is a multiple of $11$, giving $11 \mid n \iff 11 \mid A$.

**For $2^j$ and $5^j$.** $10^j = 2^j5^j$, so splitting $n = L + 10^jH$ ($L$ = last $j$
digits) makes $10^jH$ a multiple of $2^j$; linearity gives $2^j \mid n \iff 2^j \mid L$.

**The general recipe:** any weight $c$ with $d \mid (10 - c)$ works. That is why $7$ has no
clean base-10 test.

**Justifies:** reading a $10^5$-digit number as a `std::string` and testing it without ever
forming the integer.

---

## 8. The trap: $d \mid ab$ does not give $d \mid a$ or $d \mid b$ (D7)

$$6 \mid 36 = 4 \cdot 9, \qquad 6 \nmid 4, \qquad 6 \nmid 9$$

$6 = 2\cdot 3$ splits across the two factors, so neither contains all of it. The
implication holds exactly when $d$ is **prime** — that is Euclid's lemma, and it needs
Bézout, not divisibility. See
[gcd-and-euclidean-algorithm/proofs.md](../gcd-and-euclidean-algorithm/proofs.md).

**Justifies:** never reduce "is `x` divisible by `d`" to checking `d`'s behaviour on
factors of `x` unless `d` is prime. This is a silent-WA class of bug.

---

## 9. $\tau$ and $\sigma$ from the factorization (D6)

Divisors of $n = \prod p_i^{e_i}$ are exactly $\prod p_i^{f_i}$ with $0 \leq f_i \leq e_i$.
Choosing each $f_i$ independently gives $\tau(n) = \prod (e_i+1)$, and expanding the product

$$\sigma(n) = \prod_i \left(\sum_{f=0}^{e_i} p_i^{\,f}\right) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}$$

where each inner sum is a geometric series (see the progressions concept).

**Justifies:** computing $\tau$ and $\sigma$ in $O(\log n)$ once you have the factorization,
instead of $O(\sqrt n)$ from scratch — the usual move when you already have a smallest-prime-factor
table.
