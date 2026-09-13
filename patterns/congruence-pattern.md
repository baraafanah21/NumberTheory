# Pattern: simultaneous conditions and coincidences

> *"Two or more periodic things, and you need when they line up."*

One congruence is a linear solve. Two or more is the Chinese remainder theorem, and the
interesting part is almost always the **solvability check**, not the merge.

---

## Recognition clues

* several conditions of the form **$x \equiv a_i \pmod{m_i}$**, however worded
* **"every $a$ days"** and **"every $b$ hours"**, asked when they coincide
* two cycles, two blinking lights, two schedules, two rotations
* a number described only by its **remainders**, to be reconstructed
* **"the smallest $x$ such that"** a list of modular conditions all hold
* **"the $n$-th number divisible by $a$ or $b$"** → lcm and counting, then binary search
* solve **$ax \equiv b \pmod m$** for $x$
* a modulus that **factors into coprime prime powers**, inviting a split-and-recombine
* exact arithmetic on huge integers via several word-sized primes

---

## Key observation

Two congruences combine **if and only if**

$$\gcd(m_1, m_2) \ \big|\ (a_2 - a_1)$$

and the result is unique modulo $\mathrm{lcm}(m_1, m_2)$ — **not** modulo $m_1m_2$. Those
agree only when the moduli are coprime.

The merge itself is one substitution. The first congruence says $x = a_1 + m_1t$; put that
into the second and you get an ordinary linear congruence in $t$:

$$m_1 t \equiv a_2 - a_1 \pmod{m_2}$$

Solve, substitute back, done. For more than two, **merge pairwise** — the merged congruence
has exactly the same solution set as the two it replaces, so nothing is lost.

The other half of the pattern: once you have $x \equiv r \pmod L$, the solutions in
$[1, N]$ are an arithmetic progression, so **counting them is a division**. That is what
turns "the $n$-th such number" into a binary search.

---

## Relevant concepts

[chinese-remainder-theorem](../concepts/chinese-remainder-theorem/) ·
[extended-euclid](../concepts/extended-euclid/) ·
[modular-inverse](../concepts/modular-inverse/) · [gcd](../concepts/gcd/)

---

## Typical approach

1. Write every condition as $x \equiv a_i \pmod{m_i}$. Getting the statement into this form
   is usually most of the work.
2. **Check solvability pairwise.** With shared factors the system is often contradictory,
   and this check is part of the algorithm, not an optimisation.
3. Merge pairwise with `crtMerge`, folding from the identity $x \equiv 0 \pmod 1$.
4. Watch the modulus grow. It becomes the lcm of everything, and ten moduli near $10^9$
   already overflow 64 bits.
5. Asked to **count** or for the **$n$-th** solution? You now have one progression — count
   with a division, and binary search if the question is inverted.
6. Only one congruence? That is `solveLinearCongruence`, and remember it has
   $\gcd(a,m)$ solutions, not one.

---

## Complexity

| merge two congruences | $O(\log\min(m_1,m_2))$ |
|---|---|
| fold $k$ of them | $O(k\log M)$ |
| solve $ax \equiv b \pmod m$ | $O(\log m + g)$, with $g = \gcd(a,m)$ |
| count solutions in $[1,N]$ | $O(1)$ after the merge |
| binary search the $n$-th | $O(\log N)$ counting calls |

---

## Common traps

| | |
|---|---|
| assuming any system is solvable | needs $\gcd(m_1,m_2) \mid (a_2-a_1)$ |
| using $m_1m_2$ as the merged modulus | it is the lcm |
| confusing pairwise with setwise coprime | $\gcd(6,10,15)=1$, yet no pair is coprime |
| the direct $\sum a_iM_iy_i$ formula on non-coprime moduli | only valid pairwise coprime |
| the lcm overflowing 64 bits | it grows fast; merging avoids ever forming $\prod m_i$ |
| `a1 + m1*t` overflowing | reduce with `mulMod` |
| returning one solution to $ax \equiv b$ | there are $\gcd(a,m)$ of them |
| forgetting the $x \equiv 0 \pmod 1$ identity for an empty fold | it removes the empty case |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| CF 687B · 1800 | [Remainders Game](https://codeforces.com/problemset/problem/687/B) | when do the $x \bmod c_i$ determine $x \bmod k$ — decide per prime power |
| CF 919E · 2100 | [Congruence Equation](https://codeforces.com/problemset/problem/919/E) | $a^n$ has period $p-1$ while $n$ lives mod $p$ — two moduli combined |
| CF 1500B · 2200 | [Two chandeliers](https://codeforces.com/problemset/problem/1500/B) | count coincidences of two cycles up to $t$, then binary search $t$ |
| CF 495B · 1600 | [Modular Equations](https://codeforces.com/problemset/problem/495/B) | $n \bmod x = k$ solved for the modulus instead of the unknown |
| LC 878 · Hard | [Nth Magical Number](https://leetcode.com/problems/nth-magical-number/) | two periods merged through lcm, then counted and inverted |
| LC 1201 · Med | [Ugly Number III](https://leetcode.com/problems/ugly-number-iii/) | three periods, inclusion–exclusion, binary search |
| LC 1015 · Med | [Smallest Integer Divisible by K](https://leetcode.com/problems/smallest-integer-divisible-by-k/) | a single congruence walked as residues |

A genuine multi-modulus CRT merge is rare on LeetCode and uncommon below about 1800 on
Codeforces. The counting-and-binary-search half of this pattern is far more frequent than
the merge itself, and worth drilling first.

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
