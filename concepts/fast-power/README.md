# Fast Power (Binary Exponentiation)

$$a^{\,n} \ \text{ in } \ O(\log n) \ \text{ multiplications, not } \ O(n)$$

**Use:** any exponent past a few thousand — Fermat inverses, huge counts, linear
recurrences, matrix powers. **Needs:**
[modular arithmetic](../modular-arithmetic/) **Next:**
[modular inverse](../modular-inverse/) · [Euler's totient](../euler-phi/)

**Tier: Foundation.** Five other folders in this repo call it.

---

## The idea

Squaring doubles the exponent for the price of one multiplication:

$$a^{16} = \bigl(\!\bigl(\!\bigl(a^2\bigr)^2\bigr)^2\bigr)^2 \qquad \text{four multiplications, not fifteen}$$

Exponents that are not powers of two split along their **binary digits**. Since
$13 = 1101_2 = 8 + 4 + 1$:

$$a^{13} = a^{8}\cdot a^{4}\cdot a^{1}$$

So walk the bits of $n$ from the bottom. Keep a running `base` that squares every step, and
multiply it into the answer exactly when the current bit is set.

```cpp
ll powMod(ll base, ll exp, ll m) {
    ll r = 1 % m;                       // 1 % m, not 1 -- m may be 1
    base = ((base % m) + m) % m;        // negative or oversized bases
    while (exp > 0) {
        if (exp & 1) r = mulMod(r, base, m);
        base = mulMod(base, base, m);
        exp >>= 1;
    }
    return r;
}
```

**Why it is right.** The quantity $r \cdot \mathtt{base}^{\,\mathtt{exp}}$ never changes.
Halving `exp` and squaring `base` leaves it alone; when the low bit is set, moving one
factor of `base` into `r` also leaves it alone. The loop ends at $\mathtt{exp}=0$, where the
invariant reads $r = a^{\,n}$. $\blacksquare$

Cost is exactly $\lfloor\log_2 n\rfloor$ squarings plus $\mathrm{popcount}(n)-1$ extra
multiplications — at most $\sim120$ operations for any 64-bit exponent.

---

## The two things people get wrong

**Do not fold `mulMod` back into `*`.** The squaring line multiplies two values that are
each up to $m-1$, so it overflows for any $m$ above $\sim3\times10^9$ — see
[modular arithmetic](../modular-arithmetic/). At $m = 10^9+7$ a plain `*` is safe; above
that it silently returns a different number.

**A negative exponent is not this algorithm.** $a^{-1}$ under a modulus means the
[modular inverse](../modular-inverse/), and $a^{-k} = (a^{-1})^{k}$. Without a modulus,
integer $a^{-k}$ is not an integer at all.

---

## Same loop, other operations

Nothing above used the fact that the values are numbers. Binary exponentiation works for
**any associative operation with an identity**, and that generality is where most of its
competitive value lies.

| what you square | what $a^n$ then means |
|---|---|
| integers mod $m$ | ordinary modular power |
| $k\times k$ matrices | linear recurrences in $O(k^3\log n)$ — Fibonacci at $n=10^{18}$ |
| an adjacency matrix | number of walks of length $n$ between two vertices |
| a permutation | applying a shuffle $n$ times |
| a function, under composition | the $n$-th iterate |
| $\max$-plus matrices | cheapest path using exactly $n$ edges |

**Multiplication itself** is the degenerate case: `mulMod` by repeated doubling is the same
loop with $+$ in place of $\times$, which is how you multiply mod $m$ on a machine with no
`__int128`.

---

## How to recognize it in a problem

Look for:

* an **exponent up to $10^{18}$**, or an exponent given as a string of digits
* **"modulo $10^9+7$"** together with any power at all
* a linear recurrence — $f(n) = c_1f(n-1)+\cdots+c_kf(n-k)$ — with $n$ far past $10^7$
  → matrix power
* **"how many paths of length exactly $n$"** in a small graph → adjacency-matrix power
* applying the same transformation $n$ times, where the transformation composes
* a $\binom nk \bmod p$ that needs one Fermat inverse
* $n$ doublings of anything

**The tell.** Whenever $n$ is enormous but the *state* is small, the answer is almost
always a $\log n$ algorithm, and this is the simplest one.

**Anti-pattern.** If the exponent is huge and the modulus is **not** prime, reducing the
exponent needs [Euler's totient](../euler-phi/) first. And if you need $a^{n}$ for *many*
consecutive $n$, a precomputed table beats a $\log$ call per query.

---

## Complexity

| $a^n \bmod m$ | $O(\log n)$ multiplications |
|---|---|
| with `__int128` `mulMod` | $O(\log n)$, constant factor $\sim3\times$ a bare `*` |
| $k\times k$ matrix power | $O(k^3\log n)$ |
| $a^n$ as an exact integer | $O(\log n)$ multiplications, but the result overflows fast |

---

## Pitfalls

| | |
|---|---|
| `r = 1` when $m$ may be $1$ | `r = 1 % m`, else it returns $1$ instead of $0$ |
| a negative or oversized base | normalize before the loop |
| `base * base % m` above $m \sim 3\times10^9$ | `mulMod` with `__int128` |
| a negative exponent | that is an inverse, not a power |
| `exp >>= 1` on a signed negative | infinite loop; guard $n \ge 0$ |
| `std::pow` for integer powers | returns `double`, wrong past $2^{53}$ |
| reducing the exponent mod $m$ | exponents reduce mod $\varphi(m)$ |
| $0^0$ | this code returns $1$; check what the problem wants |
| recomputing $a^n$ inside a loop | precompute, or hoist the call |
| matrix power without reducing entries | every matrix multiply must reduce too |

---

[implementation.cpp](implementation.cpp) · [problems.md](problems.md)
