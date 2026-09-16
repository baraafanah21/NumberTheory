# Proofs — Divisors: $\tau$ and $\sigma$

**Borrowed:** the divisor characterization — $d \mid n \iff d = \prod p_i^{f_i}$ with
$0 \le f_i \le e_i$, distinct vectors giving distinct divisors
([prime factorization](../prime-factorization/proofs.md) §3), which already delivers
$\tau(n) = \prod(e_i+1)$ and $\sigma(n) = \prod(1+p_i+\cdots+p_i^{e_i})$; the pair bound
$\min(d,\ n/d) \le \sqrt n$ and $\tau$ odd $\iff$ square
([divisibility](../divisibility/proofs.md) §3–§4); and
$\sum_{d \le n} \lfloor n/d \rfloor = \Theta(n\log n)$
([divisibility](../divisibility/proofs.md) §5).

Those two folders own the formulas and the pair loop. What is left — and what is proved
here — is that $\tau$ and $\sigma$ are **multiplicative**, that the range sieve really
computes them, and that the integer square root in the code is exact.

---

## 1. $\tau$ and $\sigma$ are multiplicative

**Claim.** If $\gcd(a,b)=1$ then

$$(d_1,d_2) \ \longmapsto\ d_1 d_2$$

is a **bijection** from $\{d_1 : d_1 \mid a\} \times \{d_2 : d_2 \mid b\}$ onto
$\{d : d \mid ab\}$. Hence $\tau(ab) = \tau(a)\tau(b)$ and $\sigma(ab) = \sigma(a)\sigma(b)$.

**Proof.** Coprimality says exactly one thing, and it is the whole proof: **no prime divides
both $a$ and $b$**. So the primes of $ab$ split into those of $a$ and those of $b$, with no
prime on both sides, and the exponent of a prime in $ab$ equals its exponent in whichever
one of $a$, $b$ it came from.

*Well defined:* $a = d_1a'$ and $b = d_2b'$ give $ab = (d_1d_2)(a'b')$, so $d_1d_2 \mid ab$.

*Bijective:* by the borrowed characterization a divisor $d$ of $ab$ **is** an exponent
vector $(f_r)$ with $f_r$ at most the exponent of $r$ in $ab$, and distinct vectors give
distinct divisors. Split that vector along the partition of the primes: the part on $a$'s
primes is a divisor $d_1 \mid a$, the part on $b$'s primes is a divisor $d_2 \mid b$, and
$d = d_1d_2$. The partition is fixed, so this splitting and the map above are inverse to
each other. $\blacksquare$

The bijection turns a sum over divisors into a product of sums:

$$\sigma(ab) \;=\; \sum_{d \mid ab} d \;=\; \sum_{d_1 \mid a}\ \sum_{d_2 \mid b} d_1d_2
\;=\; \Bigl(\sum_{d_1 \mid a} d_1\Bigr)\Bigl(\sum_{d_2 \mid b} d_2\Bigr) \;=\; \sigma(a)\sigma(b)$$

Counting the terms instead of adding them gives $\tau(ab) = \tau(a)\tau(b)$. $\blacksquare$

**What breaks without coprimality.** Take $a=b=2$. The four pairs $(1,1),(1,2),(2,1),(2,2)$
have products $1,2,2,4$ — the map is no longer injective, because the prime $2$ now sits on
both sides and $2 = 1\cdot2 = 2\cdot1$ in two ways. That is precisely
$\tau(2)\tau(2) = 4$ against $\tau(4) = 3$.

**Justifies** the multiplicativity line in the README, and every sieve that assembles a
multiplicative function from its values on prime powers.

---

## 2. The $\sigma$ closed form — and why the code avoids it

**Claim.** $\sigma(p^e) = 1 + p + \cdots + p^{e} = \dfrac{p^{\,e+1}-1}{p-1}$.

**Proof.** By the borrowed characterization the divisors of $p^e$ are exactly
$p^0, p^1, \ldots, p^e$ — all of them, each once — so $\sigma(p^e)$ is that sum. For the
closed form, expand and telescope:

$$(p-1)\bigl(1+p+\cdots+p^{e}\bigr) \;=\; p^{\,e+1}-1$$

and divide by $p-1 \neq 0$. $\blacksquare$

With §1 this assembles into $\sigma(n) = \prod_i \frac{p_i^{\,e_i+1}-1}{p_i-1}$.

**Justifies summing term by term** in `sigmaFromFactors`
([implementation.cpp:67](implementation.cpp#L67)) instead of using the closed form. Over
$\mathbb{Z}$ the two are equal, but **under a modulus the division is not free**: $p-1$ has
an inverse mod $m$ only when $\gcd(p-1,m)=1$, and nothing about the problem guarantees
that. The running sum needs no inverse and cannot be wrong — which is the pitfall the
README lists for the $\frac{p^{e+1}-1}{p-1}$ form.

---

## 3. The divisor-visiting sieve computes $\tau$ and $\sigma$

**Claim.** After

```cpp
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d)
        tau[m] += 1;                 // or  sig[m] += d
```

every entry satisfies $\mathtt{tau}[m] = \tau(m)$ and $\mathtt{sig}[m] = \sigma(m)$ for
$1 \le m \le n$.

**Proof.** The inner loop visits $d, 2d, 3d, \ldots$ up to $n$ — exactly the multiples of
$d$ in $[1,n]$, each once. So the body executes exactly once for each pair $(d,m)$ with
$d \le n$, $m \le n$ and $d \mid m$.

Now fix $m$ and ask which executions touch it: the pairs $(d,m)$ with $d \mid m$ — that is,
one per divisor of $m$. Hence `tau[m]` is incremented $\tau(m)$ times, and `sig[m]`
accumulates $\sum_{d \mid m} d = \sigma(m)$. $\blacksquare$

**The idea, which is the part worth carrying away.** Both nestings enumerate the same set

$$\{(d,m) \ :\ 1 \le d,m \le n \ \text{ and } \ d \mid m\}$$

and differ only in the order of traversal. Asking each $m$ for its divisors costs
$O(\sqrt n)$ per $m$; letting each $d$ walk its multiples costs
$\sum_{d\le n} \lfloor n/d\rfloor = \Theta(n\log n)$ in total (borrowed). Same pairs, same
answers, better order. The body never looks at $m$'s factorization, which is exactly why
the skeleton transfers unchanged to $\varphi$, to $\mu$, to smallest prime factor — to
anything summed over divisors.

**On the width of the accumulator.** $\tau$ is tiny (§4), but $\sigma$ is not. The largest
value below $10^6$ is $\sigma(997920) = 4390848$, still inside 32 bits; individual values
cross $2^{31}$ only around $m \approx 5\times10^8$, the first highly composite witness being
$\sigma(498960000) = 2313384480$. The **running total** is the real
hazard: $\sum_{m \le 10^6}\sigma(m) = 822468118437$, past $2^{31}$ by a factor of $380$. So
`int` survives a bare $\sigma$ table at $10^6$ and nothing beyond it — neither a larger
table nor a prefix sum over this one. `sigmaUpTo`
([implementation.cpp:107](implementation.cpp#L107)) is 64-bit so that neither can become
the bug.

---

## 4. $\tau$ is small: $\tau(n) \le 2\lfloor\sqrt n\rfloor$

**Claim.** As stated, for $n \ge 1$.

**Proof.** By the borrowed pair bound, every divisor $d$ of $n$ satisfies
$\min(d,\ n/d) \le \sqrt n$, and $n/d$ is itself a divisor. So

$$d \ \longmapsto\ \min(d,\ n/d)$$

maps the $\tau(n)$ divisors into $\{1,\ldots,\lfloor\sqrt n\rfloor\}$. It is at most
two-to-one: the only divisors that can map to a given $v$ are $v$ and $n/v$. A map that is
at most two-to-one into a set of size $\lfloor\sqrt n\rfloor$ has domain at most
$2\lfloor\sqrt n\rfloor$. $\blacksquare$

**The bound is loose, and the truth is far stronger.** At $n = 720720$ it promises $1696$
while the real count is $240$. The true maximal order of $\tau$ is
$n^{O(1/\log\log n)}$ — sub-polynomial, which is why the README's record table reads
$103680$ at $n = 10^{18}$ and not billions. *Stated, not proved: that estimate needs
Chebyshev-type bounds on prime counts and is used nowhere in this repo.*

**Justifies** treating "enumerate every divisor of $n$" as an affordable step — but note
what the bound is about. It caps the **number** of divisors, not the cost of finding them.
Enumeration is cheap only once the factorization is in hand.

**$\tau(1) = \sigma(1) = 1$.** The number $1$ has exactly one divisor, itself. In the
formulas it is the empty product — $k=0$ primes — and an empty product is $1$, not $0$.
That is why `tauFromFactors` starts at `c = 1` and `sigmaFromFactors` at `s = 1`.

---

## 5. The integer square root in the code is exact

**Lemma.** For integers $r \ge 1$ and $n \ge 0$, with `/` the C++ integer division:

$$r \le n\,/\,r \quad\Longleftrightarrow\quad r^2 \le n$$

**Proof.** $\lfloor n/r \rfloor \ge r$ iff $n/r \ge r$ as reals — for an *integer* $r$ the
floor cannot separate the two — and multiplying by $r > 0$ gives $n \ge r^2$.
$\blacksquare$

**Claim.** After

```cpp
ll r = (ll)std::sqrt((double)n);
while (r > 0 && r > n / r) --r;
while ((r + 1) <= n / (r + 1)) ++r;
```

$r = \lfloor\sqrt n\rfloor$.

**Proof.** By the lemma the first loop decrements exactly while $r^2 > n$, so it exits with
$r^2 \le n$ (at worst at $r=0$, where $0 \le n$). The second increments exactly while
$(r+1)^2 \le n$, and $r$ only grows, so on exit

$$r^2 \le n < (r+1)^2$$

which is the definition of $\lfloor\sqrt n\rfloor$. Both loops terminate: the first
decreases a value bounded below by $0$, the second increases one bounded above by
$\sqrt n$. $\blacksquare$

**Why the corrections are needed at all.** A `double` carries a 53-bit mantissa, so past
$2^{53} \approx 9.01\times10^{15}$ the conversion `(double)n` already rounds and
`std::sqrt` can return a value on either side of the true root. Both loops are required —
the seed can be high *or* low — and each runs $O(1)$ times in practice, since the seed is
off by at most a step or two.

**Why the test is `r > n / r` and never `r * r > n`.** For $n$ near $9\times10^{18}$ the
seed is near $3\times10^9$, and `r * r` on a wrong-side seed overflows — the first pitfall
in the README, reappearing inside the fix for a different one. The final `r * r == n` is
safe only because by then $r = \lfloor\sqrt n\rfloor$, so $r^2 \le n$ and no overflow is
possible.

**Justifies `hasOddDivisorCount`**
([implementation.cpp:118](implementation.cpp#L118)): with $\lfloor\sqrt n\rfloor$ exact,
`r * r == n` decides "perfect square", and the borrowed $\tau$-odd result turns that into
the divisor-count parity in $O(1)$ — no factorization, no loop over divisors. The
locker-and-bulb problems are this line.
