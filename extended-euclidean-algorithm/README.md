# Extended Euclidean Algorithm

The [gcd concept](../gcd-and-euclidean-algorithm/) proved that integers $x, y$ exist with

$$a\,x + b\,y = \gcd(a,b)$$

but never produced them. This algorithm produces them — and that single addition gives you
**division under a modulus**, which is what most modular-arithmetic problems actually need.

**You need this for:** modular inverse, solving $ax + by = c$, and the Chinese Remainder
Theorem.

**Before this:** [gcd](../gcd-and-euclidean-algorithm/).

---

## The algorithm

```cpp
// returns g = gcd(a,b), and sets x, y so that a*x + b*y == g
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }    // a*1 + 0*0 = a
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
```

**Where those two update lines come from.** The recursive call hands back

$$b\,x_1 + (a \bmod b)\,y_1 = g$$

Replace $a \bmod b$ with $a - \lfloor a/b\rfloor\,b$ and regroup by $a$ and $b$:

$$a\,y_1 \;+\; b\Bigl(x_1 - \Bigl\lfloor\tfrac{a}{b}\Bigr\rfloor y_1\Bigr) \;=\; g$$

Reading off the coefficients gives $x = y_1$ and $y = x_1 - \lfloor a/b\rfloor\,y_1$.
**Re-derive this rather than memorizing it** — it takes ten seconds and you will not mix up
which coefficient gets the quotient.

$O(\log\min(a,b))$, same as ordinary gcd. The coefficients stay small, so they cannot
overflow.

---

## Use 1: modular inverse

The inverse of $a$ modulo $m$ is the number $a^{-1}$ with $a \cdot a^{-1} \equiv 1$. It is
how you **divide** under a modulus:

$$\frac{a}{b} \bmod m \quad\text{means}\quad a \cdot b^{-1} \bmod m$$

$$\boxed{\ a^{-1} \bmod m \ \text{ exists} \quad\Longleftrightarrow\quad \gcd(a,m) = 1\ }$$

Because $ax \equiv 1 \pmod m$ means $ax + m(-k) = 1$, which is solvable exactly when
$\gcd(a,m)$ divides $1$.

```cpp
// a^-1 mod m, or -1 if it does not exist. Works for ANY m.
ll modInverse(ll a, ll m) {
    ll x, y;
    ll g = extgcd(((a % m) + m) % m, m, x, y);
    if (g != 1) return -1;                     // no inverse
    return ((x % m) + m) % m;                  // extgcd can return a negative x
}
```

**Both normalizations are required** — `extgcd` happily returns negative coefficients.

**Shortcut when $m$ is prime.** Fermat's little theorem gives $a^{-1} \equiv a^{\,m-2}$, so
one call to fast exponentiation does it. **Only for prime $m$** — on a composite modulus it
returns a wrong answer silently, with no error.

---

## Use 2: solving $ax + by = c$

$$\text{solvable} \quad\Longleftrightarrow\quad \gcd(a,b) \mid c$$

If so: run `extgcd` to get a solution of $ax + by = g$, then **multiply both coefficients
by $c/g$**. All other solutions are

$$x = x_0 + k\cdot\frac{b}{g}, \qquad y = y_0 - k\cdot\frac{a}{g}, \qquad k \in \mathbb{Z}$$

> Note the steps are $\frac bg$ and $\frac ag$, **not** $b$ and $a$. Using $b$ skips real
> solutions whenever $g > 1$ — a silent wrong answer.

---

## Use 3: Chinese Remainder Theorem

Merging

$$x \equiv a_1 \pmod{m_1}, \qquad x \equiv a_2 \pmod{m_2}$$

$$\text{possible} \quad\Longleftrightarrow\quad \gcd(m_1,m_2) \ \big|\ (a_2 - a_1)$$

and the answer is unique modulo $\mathrm{lcm}(m_1, m_2)$.

Two things people get wrong:

- With **coprime** moduli it always works — but with shared factors it can genuinely
  **fail**, so the check is not optional.
- The combined modulus is $\mathrm{lcm}(m_1,m_2)$, **not** $m_1 m_2$.

Fold a list of congruences by merging them two at a time.

---

## Complexity

| Task | Time |
|---|---|
| `extgcd` | $O(\log\min(a,b))$ |
| modular inverse (any $m$) | $O(\log m)$ |
| modular inverse ($m$ prime, via Fermat) | $O(\log m)$ |
| solving $ax+by=c$ | $O(\log\min(a,b))$ |
| merging two congruences | $O(\log)$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Using `extgcd`'s $x$ directly (it may be negative) | `((x % m) + m) % m` |
| Fermat's inverse on a **composite** modulus | silently wrong; use `extgcd` |
| Assuming an inverse always exists | only when $\gcd(a,m)=1$ |
| Forgetting to scale by $c/g$ | you solved $=g$, not $=c$ |
| Stepping by $b$ instead of $b/g$ | you skip solutions |
| `a * modInverse(b, m)` overflowing when $m > 2^{31}$ | use `__int128` |
| Assuming CRT always merges | needs $\gcd(m_1,m_2) \mid (a_2-a_1)$ |
| Using $m_1m_2$ as the merged modulus | it is $\mathrm{lcm}(m_1,m_2)$ |

---

## Files

- [proofs.md](proofs.md) — why the recursion is correct, why an inverse needs $\gcd = 1$,
  where the solution family comes from, and the CRT condition.
- [implementation.cpp](implementation.cpp) — four functions.
