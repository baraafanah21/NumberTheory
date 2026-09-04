# Modular Multiplicative Inverse

$$a \cdot a^{-1} \equiv 1 \pmod m$$

The modular version of "one over $a$" — and the **only** way to divide under a modulus.

**Use:** any fraction in a problem answered mod $10^9+7$ — binomials, probabilities,
averages. **Needs:** [extended Euclid](../extended-euclidean-algorithm/) **Next:**
[CRT](../chinese-remainder-theorem/)

---

## Why you need it

Addition, subtraction and multiplication behave normally under a modulus. **Division does
not** — `(a / b) % m` throws away the remainder and is simply wrong. Instead:

$$\frac ab \bmod m \quad\Longrightarrow\quad a \cdot b^{-1} \bmod m$$

**Example.** $\frac73 \bmod 11$: since $3\times4 = 12 \equiv 1$, we have $3^{-1} = 4$, so
$\frac73 \equiv 28 \equiv 6$. Check: $3\times6 = 18 \equiv 7$ ✓ — exactly what "7 divided
by 3" should mean.

---

## When it exists

$$\boxed{\ a^{-1} \bmod m \ \text{ exists} \iff \gcd(a,m) = 1\ }$$

and it is then unique mod $m$. Because $ax \equiv 1$ is $ax + my = 1$, and Bézout makes
that solvable only when $\gcd(a,m)$ divides $1$.

- Modulo a **prime** $p$, every $1,\dots,p-1$ has an inverse — which is why $10^9+7$ is
  the standard modulus.
- Modulo a **composite**, most do not. Mod $6$ the multiples of $2$ run
  $2,4,0,2,4,0,\dots$ and never reach $1$.

---

## Three ways to compute it

**1. Extended Euclid — any modulus.** `extgcd(a,m)` returns $x,y$ with $ax+my=g$; if
$g=1$, reducing mod $m$ leaves $ax \equiv 1$, so $x$ **is** the inverse.

```cpp
ll modInverse(ll a, ll m) {
    ll x, y;
    if (extgcd(((a % m) + m) % m, m, x, y) != 1) return -1;   // no inverse
    return ((x % m) + m) % m;                                 // extgcd may return x < 0
}
```

Both normalizations are required.

**2. Fermat — prime modulus only.** $a^{\,p-1} \equiv 1$, so $a^{-1} \equiv a^{\,p-2}$.
One call to fast exponentiation.

> On a composite modulus this does not fail — it returns a **wrong number silently**. Mod
> $12$ it gives $5^{-1} = 1$, yet $5\times1 = 5 \neq 1$. The true answer is $5$.

**3. All of $1^{-1}\dots n^{-1}$ mod a prime — $O(n)$.** From $p = qi + r$:

```cpp
inv[1] = 1;
for (int i = 2; i <= n; ++i) inv[i] = (p - p / i) * inv[p % i] % p;
```

| modulus prime, one or two inverses | Fermat |
|---|---|
| modulus composite or unknown | extended Euclid |
| $n$ inverses mod a prime | the $O(n)$ table |

---

## Main use: $\binom{n}{k} \bmod p$

$$\binom nk \bmod p = n!\cdot\bigl(k!\bigr)^{-1}\cdot\bigl((n-k)!\bigr)^{-1} \bmod p$$

Precompute factorials and their inverses once; every query is then $O(1)$.

---

## When $\gcd(a,m) \neq 1$: $ax \equiv b \pmod m$

With $g = \gcd(a,m)$:

$$\text{solvable} \iff g \mid b, \qquad\text{and there are then \textbf{exactly } } g \text{ solutions mod } m$$

spaced $\frac mg$ apart. Divide the congruence through by $g$ — the gcd becomes $1$, so the
inverse exists. **Returning one solution when there are $g$ is the classic half-right
answer.**

---

## Complexity

| inverse via extended Euclid | $O(\log m)$, any $m$ |
|---|---|
| inverse via Fermat | $O(\log p)$, prime only |
| all inverses $1..n$ | $O(n)$, prime only |
| solve $ax \equiv b \pmod m$ | $O(\log m + g)$ |

---

## Pitfalls

| | |
|---|---|
| `(a / b) % m` | division does not work; multiply by $b^{-1}$ |
| Fermat on a composite modulus | silently wrong |
| assuming an inverse always exists | only when $\gcd(a,m)=1$ |
| using `extgcd`'s $x$ unnormalized | it can be negative |
| `a * inv` overflowing when $m > 2^{31}$ | `__int128` |
| calling `modInverse` $n$ times in a loop | build the $O(n)$ table |
| reporting one solution to $ax \equiv b$ | there are $\gcd(a,m)$ |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
