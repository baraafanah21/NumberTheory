# Chinese Remainder Theorem

You know a number only by its **remainders**:

$$x \equiv 2 \pmod 3, \qquad x \equiv 3 \pmod 5, \qquad x \equiv 2 \pmod 7$$

CRT says these pin $x$ down completely — here $x = 23$, and every other solution differs by
a multiple of $3\cdot5\cdot7 = 105$.

**You need this for:** combining periodic conditions, reconstructing a number from
residues, and splitting one big computation into several small ones.

**Before this:** [modular inverse](../modular-multiplicative-inverse/).

---

## The statement

**Coprime version (the classic one).** If $m_1, m_2, \dots, m_k$ are **pairwise** coprime,
then for any residues $a_1, \dots, a_k$ the system

$$x \equiv a_i \pmod{m_i} \qquad (i = 1, \dots, k)$$

has **exactly one** solution modulo $M = m_1m_2\cdots m_k$.

> **Pairwise**, not "the gcd of all of them is 1". $\gcd(6,10,15) = 1$, but no two of them
> are coprime and CRT does not apply.

**General version (any moduli).** Two congruences

$$x \equiv a_1 \pmod{m_1}, \qquad x \equiv a_2 \pmod{m_2}$$

can be combined **if and only if**

$$\gcd(m_1, m_2) \ \big|\ (a_2 - a_1)$$

and the result is then unique modulo $\mathrm{lcm}(m_1, m_2)$.

Two things people get wrong here:

- With shared factors the system can be **contradictory**. $x \equiv 1 \pmod 6$ forces $x$
  odd; $x \equiv 2 \pmod 8$ forces $x$ even. No solution — so the check is not optional.
- The combined modulus is $\mathrm{lcm}$, **not** the product. They agree only when the
  moduli are coprime.

---

## How to merge two congruences

Everything follows from one substitution. The first congruence says

$$x = a_1 + m_1\,t \qquad\text{for some integer } t$$

Put that into the second:

$$a_1 + m_1 t \equiv a_2 \pmod{m_2} \qquad\Longleftrightarrow\qquad m_1\,t \equiv a_2 - a_1 \pmod{m_2}$$

which is an ordinary **linear congruence** in $t$ — solvable exactly when
$\gcd(m_1,m_2)$ divides $a_2 - a_1$. Solve it, put $t$ back, and you get $x$ modulo
$\mathrm{lcm}(m_1,m_2)$.

For more than two congruences, **merge pairwise**: fold the list, combining the running
answer with the next congruence each time.

```cpp
// x = rem (mod mod)
struct Congruence { ll rem, mod; };

std::optional<Congruence> crtMerge(Congruence c1, Congruence c2) {
    ll x, y;
    ll g = extgcd(c1.mod, c2.mod, x, y);
    ll diff = c2.rem - c1.rem;
    if (diff % g != 0) return std::nullopt;          // contradictory
    ll lcm  = c1.mod / g * c2.mod;
    ll step = c2.mod / g;
    ll t = mulMod(norm(diff / g, step), norm(x, step), step);
    return Congruence{ norm(c1.rem + mulMod(c1.mod % lcm, t, lcm), lcm), lcm };
}
```

---

## The direct formula (pairwise coprime only)

When the moduli are pairwise coprime there is a closed form. With
$M = \prod m_i$ and $M_i = M / m_i$:

$$x \;\equiv\; \sum_{i=1}^{k} a_i\,M_i\,y_i \pmod M, \qquad\text{where } y_i = M_i^{-1} \bmod m_i$$

**Why it works, in one line:** modulo $m_j$, every term with $i \neq j$ vanishes (because
$m_j$ divides $M_i$), and the surviving term is $a_j M_j y_j \equiv a_j \cdot 1 = a_j$.

Each $M_i y_i$ acts like an indicator: it is $1$ mod $m_i$ and $0$ mod every other modulus.

> Use the merge version in practice — it handles non-coprime moduli too, and needs no
> product $M$ that might overflow.

---

## What it is actually for

**Reconstructing a number from residues.** Given $x \bmod m_i$ for several small $m_i$, you
recover $x$ as long as $\prod m_i$ exceeds it.

**Splitting a computation (multi-modular arithmetic).** To compute something huge exactly:
do the whole calculation modulo several different primes — each fitting comfortably in a
machine word — then reconstruct. Since CRT is a bijection

$$\mathbb{Z}/M \ \longleftrightarrow\ \mathbb{Z}/m_1 \times \cdots \times \mathbb{Z}/m_k$$

and $+$, $-$, $\times$ act coordinate-wise, the reconstructed answer is exactly right.
This is how big-integer libraries and exact linear algebra avoid overflow.

**Combining periodic conditions.** "Every 4th day starting Tuesday, and every 6th hour
starting at 3" is a CRT system.

---

## Complexity

| Task | Time |
|---|---|
| merge two congruences | $O(\log \min(m_1,m_2))$ |
| fold $k$ congruences | $O(k \log M)$ |
| direct formula, $k$ coprime moduli | $O(k \log M)$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Assuming any system is solvable | needs $\gcd(m_1,m_2) \mid (a_2-a_1)$ |
| Using $m_1m_2$ as the merged modulus | it is $\mathrm{lcm}(m_1,m_2)$ |
| Confusing pairwise coprime with setwise | $\gcd(6,10,15)=1$, yet no pair is coprime |
| $\mathrm{lcm}$ overflowing 64 bits | ten moduli near $10^9$ already exceed it — check, or use `__int128` |
| `a1 + m1*t` overflowing before the reduction | reduce with `mulMod` |
| Negative remainders from `%` | normalize into $[0, \mathrm{mod})$ |
| Applying the direct formula to non-coprime moduli | it is only valid pairwise coprime; use merging |

---

## Files

- [proofs.md](proofs.md) — the merge condition, the coprime case and its bijection, folding
  many congruences, the direct formula, and why splitting a computation is valid.
- [implementation.cpp](implementation.cpp) — three functions with self-checks.
