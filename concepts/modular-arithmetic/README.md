# Modular Arithmetic

$$a \equiv b \pmod m \quad\Longleftrightarrow\quad m \mid (a - b)$$

Two numbers are the *same* if they differ by a multiple of $m$. Defined by
**divisibility**, not by `%` — the operator is how you compute a representative, not what
the statement means.

**Use:** every problem that says "output the answer modulo $10^9+7$", plus cycles, parity
and periodic conditions. **Needs:** [divisibility](../divisibility/) **Next:**
[fast power](../fast-power/) · [modular inverse](../modular-inverse/)

**Tier: Foundation.** Nothing later in the repo works without it.

---

## The one rule that matters

**Congruence survives $+$, $-$ and $\times$.** If $a \equiv b$ and $c \equiv d$, then

$$a + c \equiv b + d, \qquad a - c \equiv b - d, \qquad a\,c \equiv b\,d \pmod m$$

*Why:* write $a = b + km$ and $c = d + \ell m$. Then

$$ac = bd + m\,(b\ell + dk + k\ell m)$$

and the whole second term is a multiple of $m$. $\blacksquare$

That one line is the licence to **reduce at every step**. You never need the true value of a
product, only its residue, so nothing ever grows past $m^2$.

$$(a \cdot b \cdot c) \bmod m \;=\; \bigl((a \bmod m)\cdot(b \bmod m) \bmod m\bigr)\cdot(c \bmod m) \bmod m$$

Repeated multiplication gives exponents for free: $a \equiv b \Rightarrow a^k \equiv b^k$.

> **But the exponent itself is not reducible.** $a^k \bmod m$ is *not*
> $a^{k \bmod m} \bmod m$. Exponents live modulo $\varphi(m)$, not $m$, and only when
> $\gcd(a,m)=1$ — see [Euler's totient](../euler-phi/).

---

## Division is the exception

$$\frac ab \bmod m \quad\text{is \textbf{not}}\quad \frac{a \bmod m}{b \bmod m}$$

`(a / b) % m` discards the remainder and is simply a different number. Worse, **cancellation
fails**: $2\cdot3 \equiv 2\cdot 0 \pmod 6$, yet $3 \not\equiv 0 \pmod 6$. You cannot divide
both sides by $2$.

The salvageable version keeps a gcd correction:

$$ac \equiv bc \pmod m \quad\Longrightarrow\quad a \equiv b \ \Bigl(\mathrm{mod}\ \tfrac{m}{\gcd(c,m)}\Bigr)$$

To actually divide you need a multiplicative inverse, which exists only when
$\gcd(b,m)=1$. That is a concept of its own — [modular inverse](../modular-inverse/).

---

## The two traps in C++

**1. `%` can return a negative number.** C++ truncates toward zero, so the sign follows the
*dividend*:

```cpp
-17 % 5   ==  -2      // C++
-17 mod 5  ==  3      // the mathematical residue
```

Harmless in a comparison, fatal as an array index or a hash key. Normalize once, in one
place:

```cpp
ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }
```

Subtraction is where this bites — `(a - b) % m` is negative whenever $a < b$. Write
`norm(a - b, m)`, or the branch-free `(a - b + m) % m` when both are already reduced.

**2. Multiplication overflows before the `%` runs.** `a * b % m` evaluates `a * b` first.

| $m$ | largest product | fits in `long long`? |
|---|---|---|
| $10^9+7$ | $\approx10^{18}$ | yes, barely — `ll` holds $9.22\times10^{18}$ |
| $10^{12}$ | $10^{24}$ | **no** |
| $9\times10^{18}$ | $8\times10^{37}$ | **no** |

Above $\sim3\times10^9$ you need a wider intermediate:

```cpp
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }
```

This is the single most common silent wrong answer in number-theory problems. It does not
crash, it does not warn, it just returns nonsense.

---

## Choosing a modulus

| $10^9+7$ | prime, the default | products fit in `long long` |
|---|---|---|
| $998244353$ | prime, $= 119\cdot2^{23}+1$ | has $2^{23}$-rd roots of unity, so NTT works |
| $2^{64}$ | free, via `unsigned long long` overflow | **not** prime; anti-hash tests exploit it |

Both standard moduli are prime, which is why Fermat's little theorem is usually available
for inverses. A composite modulus quietly removes that option.

---

## How to recognize it in a problem

Look for:

* **"output the answer modulo $10^9+7$"** or `998244353` — the answer is a huge count, and
  the modulus is a formatting device, not the subject
* a count that obviously exceeds $10^{18}$ — permutations, subsets, paths, ways
* **"the last $k$ digits"** — that is mod $10^k$
* clock, cycle, wrap-around, "every $k$-th", day-of-week, circular array indexing
* **parity** arguments — mod $2$ is still modular arithmetic
* two quantities that must agree "up to a multiple of $m$"
* a `/` appearing anywhere in a formula you must print mod $p$ → you need
  [modular inverse](../modular-inverse/), not this concept alone

**Anti-pattern.** If the modulus is small and the *exponent* is huge, the tool is
[fast power](../fast-power/) plus [Euler's totient](../euler-phi/), not repeated
multiplication.

---

## Complexity

| $+$, $-$, $\times$, `norm` | $O(1)$ |
|---|---|
| `mulMod` with `__int128` | $O(1)$, a few times slower than `*` |
| $a^n \bmod m$ | $O(\log n)$ — [fast power](../fast-power/) |
| division under a modulus | $O(\log m)$ — [modular inverse](../modular-inverse/) |

---

## Pitfalls

| | |
|---|---|
| `a * b % m` overflowing before the `%` | `mulMod` with `__int128` |
| `-17 % 5 == -2` used as an index | `norm(x, m)` |
| `(a - b) % m` when $a < b$ | `norm(a - b, m)` |
| `(a / b) % m` | division needs an inverse |
| cancelling a common factor | only valid modulo $m/\gcd(c,m)$ |
| reducing the **exponent** mod $m$ | exponents reduce mod $\varphi(m)$ |
| forgetting a final `norm` before printing | a negative answer fails the checker |
| accumulating a sum without reducing | overflow after $\sim9$ additions near $10^{18}$ |
| `pow()` from `<cmath>` | returns `double`, wrong past $2^{53}$ |
| hashing mod $2^{64}$ | not prime, and anti-hash tests exist |

---

[implementation.cpp](implementation.cpp) · [problems.md](problems.md)
