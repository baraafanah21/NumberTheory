# Chinese Remainder Theorem

You know a number only by its **remainders**:

$$x \equiv 2 \pmod 3, \qquad x \equiv 3 \pmod 5, \qquad x \equiv 2 \pmod 7$$

CRT pins it down: $x = 23$, and every other solution differs by a multiple of
$3\cdot5\cdot7 = 105$.

**Use:** combining periodic conditions, reconstructing a number from residues, splitting a
big computation into small ones. **Needs:**
[modular inverse](../modular-multiplicative-inverse/)

---

## The statement

**Coprime version.** If $m_1,\dots,m_k$ are **pairwise** coprime, the system
$x \equiv a_i \pmod{m_i}$ has exactly one solution modulo $M = \prod m_i$.

> **Pairwise**, not "the gcd of all is 1". $\gcd(6,10,15)=1$, yet no two are coprime and
> CRT does not apply.

**General version.** Two congruences combine

$$\iff \quad \gcd(m_1,m_2) \ \big|\ (a_2 - a_1)$$

and the answer is then unique modulo $\mathrm{lcm}(m_1,m_2)$.

Two things people get wrong:

- With shared factors the system can be **contradictory** — $x \equiv 1 \pmod 6$ forces
  $x$ odd, $x \equiv 2 \pmod 8$ forces $x$ even. The check is not optional.
- The combined modulus is $\mathrm{lcm}$, **not** the product. They agree only for coprime
  moduli.

---

## How merging works

Everything follows from one substitution. The first congruence says $x = a_1 + m_1 t$; put
that into the second:

$$m_1\,t \equiv a_2 - a_1 \pmod{m_2}$$

an ordinary linear congruence in $t$, solvable exactly when $\gcd(m_1,m_2)$ divides
$a_2-a_1$. Solve, substitute back, get $x$ modulo $\mathrm{lcm}$.

For more than two, **merge pairwise** — the merged congruence has the *same solution set*
as the two it replaces, so nothing is lost.

```cpp
std::optional<Congruence> crtMerge(Congruence c1, Congruence c2) {
    ll x, y;
    ll g = extgcd(c1.mod, c2.mod, x, y);
    ll diff = c2.rem - c1.rem;
    if (diff % g != 0) return std::nullopt;          // contradictory
    ll lcm = c1.mod / g * c2.mod, step = c2.mod / g;
    ll t = mulMod(norm(diff / g, step), norm(x, step), step);
    return Congruence{ norm(c1.rem + mulMod(c1.mod % lcm, t, lcm), lcm), lcm };
}
```

---

## The direct formula (pairwise coprime only)

With $M = \prod m_i$, $M_i = M/m_i$, and $y_i = M_i^{-1} \bmod m_i$:

$$x \equiv \sum_{i} a_i\,M_i\,y_i \pmod M$$

**Why:** modulo $m_j$ every term with $i \neq j$ vanishes (as $m_j \mid M_i$), leaving
$a_jM_jy_j \equiv a_j$. Each $M_iy_i$ is an indicator — $1$ mod $m_i$, $0$ mod the others.

> Prefer merging in practice: it handles any moduli and never forms $M$, which can
> overflow when the $\mathrm{lcm}$ would not.

---

## What it is for

**Reconstructing from residues** — recover $x$ from $x \bmod m_i$, as long as
$\prod m_i > x$.

**Splitting a computation.** CRT is a bijection

$$\mathbb{Z}/M \ \longleftrightarrow\ \mathbb{Z}/m_1 \times \cdots \times \mathbb{Z}/m_k$$

and $+,-,\times$ act coordinate-wise. So run the whole calculation modulo several
word-sized primes, then reconstruct — exact big-number arithmetic with no big-number
library.

**Combining periods** — "every 4th day from Tuesday, every 6th hour from 3" is a CRT
system.

---

## Complexity

| merge two | $O(\log\min(m_1,m_2))$ |
|---|---|
| fold $k$, or the direct formula | $O(k\log M)$ |

---

## Pitfalls

| | |
|---|---|
| assuming any system is solvable | needs $\gcd(m_1,m_2) \mid (a_2-a_1)$ |
| using $m_1m_2$ as the merged modulus | it is $\mathrm{lcm}$ |
| confusing pairwise with setwise coprime | $\gcd(6,10,15)=1$, no pair coprime |
| $\mathrm{lcm}$ overflowing 64 bits | ten moduli near $10^9$ already do |
| `a1 + m1*t` overflowing | reduce with `mulMod` |
| the direct formula on non-coprime moduli | only valid pairwise coprime |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
