# Primitive Roots and Discrete Logarithm — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. All the orders.** Tabulate $\mathrm{ord}_7(a)$ for $a = 1,\dots,6$. Confirm every
order divides $\varphi(7) = 6$, and name the primitive roots.

**2. Count before you search.** How many primitive roots are there mod $11$? Give the
count from a formula first, then list them.

**3. Where there are none.** Show that $8$, $12$ and $15$ have no primitive root by
computing the largest order available in each. For $15$, note the gap: $\varphi(15) = 8$
but nothing beats order $4$.

**4. A small discrete log.** Solve $3^x \equiv 13 \pmod{17}$ by hand. How do you know a
solution exists before searching?

---

## With code

**5. The fast test.** Find the smallest primitive root mod $10^9+7$ and mod $998244353$.
Do not test $g^k$ for all $k$ — factor $p-1$ and test only $g^{(p-1)/q}$ for each prime
$q \mid (p-1)$. Report how many exponentiations each search took.

**6. BSGS.** Solve $5^x \equiv 33 \pmod{10^9+7}$ with baby-step giant-step. Report $x$ and
verify by exponentiating back. Roughly how many hash-map entries did you store?

**7. Counting roots of unity.** How many $x \in [1,100]$ satisfy $x^{12} \equiv 1
\pmod{101}$? Predict the count from $\gcd$ before running anything.

**8. Order divides $\varphi$.** Compute $\mathrm{ord}_{15}(a)$ for every $a$ coprime to
$15$. Check each divides $\varphi(15) = 8$ — then observe they all divide $4$, and explain
what that says about the group.

---

## Harder

**9. Exactly $\varphi(p-1)$ of them.** Prove that a prime $p$ has exactly $\varphi(p-1)$
primitive roots. The engine is $\sum_{d\mid n}\varphi(d) = n$ from the
[totient](../euler-totient-function/) folder plus the fact that $x^d \equiv 1$ has at most
$d$ roots mod $p$. Verify your count for $p = 17$ by listing them.

**10. Smallest $x$, honestly.** In BSGS with $x = in - j$, prove every $x \in [1, m]$ is
representable, then show why storing the **largest** $j$ per key and scanning $i$ ascending
yields the smallest positive $x$ — and construct a case where storing the first $j$ instead
returns a non-minimal answer.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 284A · 1400 | [Cows and Primitive Roots](https://codeforces.com/problemset/problem/284/A) | count the primitive roots mod $p$ — problem 9, as a judge task |
| CF 1106F · 2400 | [Lunar New Year and a Recursive Sequence](https://codeforces.com/problemset/problem/1106/F) | take discrete logs base $3$ mod $998244353$, turning a product recurrence into a linear one on exponents mod $p-1$; matrix-power it, then solve a linear congruence and exponentiate back |
| LC 1015 · Med | [Smallest Integer Divisible by K](https://leetcode.com/problems/smallest-integer-divisible-by-k/) | the length of the answer is a multiplicative order — and it exists only when $\gcd(K,10) = 1$ |

Discrete logs are rare on LeetCode and common in cryptography; if you want more of this,
the natural next steps are Pohlig–Hellman (BSGS sped up by factoring $p-1$) and
Diffie–Hellman, which is this folder's asymmetry used on purpose.

---

## Answers

**1.** $\mathrm{ord}(1) = 1$, $\mathrm{ord}(2) = 3$, $\mathrm{ord}(3) = 6$,
$\mathrm{ord}(4) = 3$, $\mathrm{ord}(5) = 6$, $\mathrm{ord}(6) = 2$. All divide $6$ ✓, and
the primitive roots are $\mathbf{3}$ and $\mathbf 5$ — $\varphi(6) = 2$ of them.

**2.** $\varphi(\varphi(11)) = \varphi(10) = \mathbf 4$, namely
$\mathbf{2, 6, 7, 8}$.

**3.** Mod $8$: $\varphi = 4$ but every unit squares to $1$, so the max order is $2$. Mod
$12$: same, max order $2$. Mod $15$: $\varphi = 8$, yet the orders are only
$1, 2, 4$ — max $\mathbf 4$. In each case max order $< \varphi(m)$, so no element
generates. Primitive roots exist only for $m = 1, 2, 4, p^k, 2p^k$.

**4.** $3$ is a primitive root mod $17$, so its powers hit every non-zero residue and a
solution is guaranteed. Stepping: $3,9,10,13$ — so $\mathbf{x = 4}$.

**5.** Smallest primitive root mod $10^9+7$ is $\mathbf 5$; mod $998244353$ it is
$\mathbf 3$. Here $10^9+6 = 2\cdot500000003$ (two distinct primes) and
$998244352 = 2^{23}\cdot7\cdot17$ (three), so a candidate costs two and three
exponentiations respectively — $5$ and $3$ are found almost immediately, versus the $O(p)$
of testing $g^k \ne 1$ for every $k$.

**6.** $x = \mathbf{533470860}$, and $5^{533470860} \equiv 33 \pmod{10^9+7}$ ✓. With
$n = \lceil\sqrt m\,\rceil \approx 31623$, the table holds about $31{,}623$ entries — which
is the whole point: $\sqrt m$ memory buys $\sqrt m$ time instead of $m$.

**7.** $\mathbf 4$ solutions, since $x^k \equiv 1 \pmod p$ has exactly $\gcd(k, p-1)$
solutions and $\gcd(12,100) = 4$. (Why: writing $x = g^t$ turns it into
$kt \equiv 0 \pmod{p-1}$, a linear congruence with $\gcd(k,p-1)$ solutions.)

**8.** Orders mod $15$: $a=1\to1$, $2\to4$, $4\to2$, $7\to4$, $8\to4$, $11\to2$, $13\to4$,
$14\to2$. Every one divides $4$, not merely $8$. So the exponent of the group is $4$ while
its size is $8$ — the group is $\mathbb Z_2\times\mathbb Z_4$, not cyclic, which is exactly
what "no primitive root" means.

**9.** Every element's order divides $p-1$. For each $d \mid (p-1)$ let $N(d)$ count the
elements of order exactly $d$. If $N(d) > 0$, pick one such $a$: its $d$ powers are $d$
distinct roots of $x^d \equiv 1$, and that congruence has **at most** $d$ roots mod a
prime, so they are all of them — among which exactly $\varphi(d)$ have order $d$. Hence
$N(d)$ is $0$ or $\varphi(d)$. But $\sum_{d\mid p-1}N(d) = p-1 = \sum_{d\mid p-1}\varphi(d)$,
so no term can fall short: $N(d) = \varphi(d)$ for **every** $d$ — in particular
$N(p-1) = \varphi(p-1) > 0$, which proves primitive roots exist and counts them at once.
For $p = 17$: $\varphi(16) = \mathbf 8$, namely $3, 5, 6, 7, 10, 11, 12, 14$.

**10.** With $n = \lceil\sqrt m\,\rceil$, any $x \in [1,m]$ has $i = \lceil x/n\rceil \in
[1,n]$ and $j = in - x \in [0,n)$, so the form covers everything. Scanning $i = 1,2,\dots$
ascending, the first $i$ that hits the table gives the smallest **giant** step; among the
$j$ stored for that key, the *largest* $j$ subtracts the most and so yields the smallest
$x = in - j$. Keys really do repeat: whenever $\mathrm{ord}(g) < n$ the baby steps cycle and
$b g^{j}$ collides with itself.

Concrete case — $3^x \equiv 3 \pmod{13}$, where $\mathrm{ord}_{13}(3) = 3$ and $n = 4$. The
baby steps for $j = 0,1,2,3$ are $3,\ 9,\ 1,\ 3$: the key $3$ appears at both $j=0$ and
$j=3$. The first giant step, $i=1$, lands on key $3$, so

$$\text{keep } j=0 \Rightarrow x = 4-0 = 4, \qquad \text{keep } j=3 \Rightarrow x = 4-3 = \mathbf 1$$

Both satisfy $3^x \equiv 3 \pmod{13}$, but only $x=1$ is smallest. The fix is one line —
assign `table[key] = j` unconditionally rather than inserting only when the key is absent.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
