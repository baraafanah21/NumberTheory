# Modular Multiplicative Inverse

The **inverse of $a$ modulo $m$** is the number $a^{-1}$ satisfying

$$a \cdot a^{-1} \equiv 1 \pmod m$$

It is the modular version of "one over $a$" — and it is the **only** way to divide under a
modulus.

**You need this for:** any fraction in a problem answered mod $10^9+7$ — binomial
coefficients, probabilities, averages, geometric sums.

**Before this:** [extended Euclid](../extended-euclidean-algorithm/).

---

## Why you need it at all

Under a modulus, addition, subtraction and multiplication behave normally. **Division does
not.** You cannot write `(a / b) % m` — integer division throws away the remainder and the
answer is simply wrong.

Instead, division becomes multiplication by the inverse:

$$\frac{a}{b} \bmod m \quad\Longrightarrow\quad a \cdot b^{-1} \bmod m$$

**Example.** Compute $\frac{7}{3} \bmod 11$. Since $3 \times 4 = 12 \equiv 1$, we have
$3^{-1} = 4$. So

$$\frac{7}{3} \equiv 7 \times 4 = 28 \equiv 6 \pmod{11}$$

Check: $3 \times 6 = 18 \equiv 7 \pmod{11}$ ✓ — exactly what "$7$ divided by $3$" should
mean.

---

## When does it exist?

$$\boxed{\ a^{-1} \bmod m \ \text{ exists} \quad\Longleftrightarrow\quad \gcd(a,\ m) = 1\ }$$

and when it exists it is **unique** modulo $m$.

Why: $ax \equiv 1 \pmod m$ is the same as $ax + my = 1$, and Bézout says the values
$ax + my$ are exactly the multiples of $\gcd(a,m)$. So $1$ is reachable only when that gcd
is $1$.

**Two consequences to keep in mind:**

- Modulo a **prime** $p$, every value $1, 2, \dots, p-1$ has an inverse — nothing below $p$
  shares a factor with it. This is why $10^9+7$ is chosen as a modulus.
- Modulo a **composite**, most values do not. $2$ has no inverse mod $6$: the multiples of
  $2$ are $2, 4, 0, 2, 4, 0, \ldots$ and never hit $1$.

---

## Three ways to compute it

### 1. Extended Euclid — works for any modulus

Run `extgcd(a, m)`. It returns $g$ and coefficients $x, y$ with $ax + my = g$. If $g = 1$
then reducing mod $m$ gives $ax \equiv 1$, so $x$ **is** the inverse.

```cpp
ll modInverse(ll a, ll m) {
    ll x, y;
    ll g = extgcd(((a % m) + m) % m, m, x, y);
    if (g != 1) return -1;                  // no inverse exists
    return ((x % m) + m) % m;               // extgcd may hand back a negative x
}
```

Both normalizations matter — `extgcd` returns signed coefficients.

### 2. Fermat's little theorem — prime modulus only

For prime $p$ with $p \nmid a$:

$$a^{\,p-1} \equiv 1 \pmod p \qquad\Longrightarrow\qquad a^{-1} \equiv a^{\,p-2} \pmod p$$

One call to fast exponentiation, no extra function needed.

> **Only valid when $m$ is prime.** On a composite modulus it does not fail — it returns a
> wrong number with no warning. Mod $12$: Fermat gives $5^{-1} = 1$, but $5\times1 = 5 \neq 1$.
> The true answer is $5$.

### 3. All of $1^{-1}, \dots, n^{-1}$ at once — $O(n)$, prime modulus

When you need many inverses (factorial tables for $\binom{n}{k}$), do not call the above
$n$ times. Use the recurrence:

```cpp
inv[1] = 1;
for (int i = 2; i <= n; ++i)
    inv[i] = (p - p / i) * inv[p % i] % p;
```

$O(n)$ total instead of $O(n\log p)$.

**Which to use**

| Situation | Use |
|---|---|
| modulus is prime, need one or two inverses | Fermat — shortest to write |
| modulus is composite or unknown | extended Euclid — the safe default |
| need $n$ inverses mod a prime | the $O(n)$ table |

---

## The main application: $\binom{n}{k} \bmod p$

$$\binom{n}{k} = \frac{n!}{k!\,(n-k)!} \quad\Longrightarrow\quad \binom{n}{k} \bmod p = n! \cdot \bigl(k!\bigr)^{-1} \cdot \bigl((n-k)!\bigr)^{-1} \bmod p$$

Precompute factorials and their inverses once; then every query is $O(1)$.

---

## Linear congruences: $ax \equiv b \pmod m$

When $\gcd(a,m) \neq 1$ you cannot just invert. Let $g = \gcd(a,m)$:

$$\text{solvable} \iff g \mid b, \qquad\text{and then there are } \textbf{exactly } g \text{ solutions mod } m$$

spaced $\frac mg$ apart. To solve: divide the whole congruence by $g$ to get
$\frac ag x \equiv \frac bg \pmod{\frac mg}$, where now the gcd *is* $1$, so the inverse
exists.

> Returning only one solution when there are $g$ is the classic half-right answer.

---

## Complexity

| Task | Time | Note |
|---|---|---|
| inverse via extended Euclid | $O(\log m)$ | any $m$ |
| inverse via Fermat | $O(\log p)$ | prime only |
| all inverses $1..n$ | $O(n)$ | prime only |
| solve $ax \equiv b \pmod m$ | $O(\log m + g)$ | returns all $g$ solutions |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Writing `(a / b) % m` | division does not work under a modulus; multiply by $b^{-1}$ |
| Fermat on a composite modulus | silently wrong; use extended Euclid |
| Assuming an inverse always exists | only when $\gcd(a,m) = 1$ |
| Using `extgcd`'s $x$ without normalizing | it can be negative |
| `a * inv` overflowing when $m > 2^{31}$ | use `__int128` for the product |
| Inverting $0$, or working mod $1$ | guard both |
| Calling `modInverse` inside a loop $n$ times | build the $O(n)$ table instead |
| Reporting one solution to $ax \equiv b$ | there are $\gcd(a,m)$ of them |

---

## Files

- [proofs.md](proofs.md) — why the inverse exists exactly when $\gcd=1$, why it is unique,
  a proof of Fermat's little theorem, and where the $O(n)$ recurrence comes from.
- [implementation.cpp](implementation.cpp) — four functions plus a $\binom{n}{k} \bmod p$
  demo.
