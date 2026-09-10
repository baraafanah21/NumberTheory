# Primitive Roots and Discrete Logarithm

Euler says $a^{\varphi(m)} \equiv 1$. Two follow-up questions:

1. **How soon** does $a^k$ first return to $1$? — the **order** of $a$.
2. Is there an $a$ whose powers hit **every** coprime residue? — a **primitive root**.

Then the reverse problem: given $g^x \equiv b$, find $x$ — the **discrete logarithm**.

**Use:** cycle lengths, generators, solving $a^x \equiv b$. **Needs:**
[Euler's totient](../euler-phi/)

---

## Order

For $\gcd(a,m)=1$, $\ \mathrm{ord}_m(a)$ is the smallest $k \ge 1$ with $a^k \equiv 1$. It
exists because Euler guarantees $a^{\varphi(m)} \equiv 1$.

$$a^{\,k} \equiv 1 \pmod m \iff \mathrm{ord}_m(a) \mid k
\qquad\text{so in particular}\qquad \mathrm{ord}_m(a) \ \big|\ \varphi(m)$$

**Mod 7** ($\varphi(7)=6$):

| $a$ | powers | order |
|---|---|---|
| $2$ | $2,4,1$ | $3$ |
| $3$ | $3,2,6,4,5,1$ | **6** |
| $6$ | $6,1$ | $2$ |

Every order divides $6$ ✓ — and $3$ hits all six nonzero residues, making it a primitive
root.

---

## Primitive roots

$g$ is a **primitive root** mod $m$ when $\mathrm{ord}_m(g) = \varphi(m)$ — the largest
possible. Equivalently $g^0, g^1, \dots, g^{\varphi(m)-1}$ are **all** the coprime
residues, so $g$ **generates** them. That is what makes discrete logs meaningful.

**Existence:** only for $m = 1,\ 2,\ 4,\ p^k,\ 2p^k$ ($p$ an odd prime). The case that
matters: **every prime modulus has one.** There is none mod $8$, $12$ or $15$.

**How many:** exactly $\varphi(\varphi(m))$ — usually a decent fraction, so searching finds
one fast.

**The test.** Checking $g^k \neq 1$ for all $k < p-1$ costs $O(p)$. It is enough to rule
out the **prime divisors** of $p-1$:

$$g \text{ is a primitive root mod } p \iff g^{\frac{p-1}{q}} \not\equiv 1 \ \text{ for every prime } q \mid (p-1)$$

Since $p-1$ has at most about $15$ distinct prime factors, that is a handful of
exponentiations. The answer is nearly always small — for $p = 10^9+7$ it is $5$.

---

## Discrete logarithm

Given $\gcd(g,m)=1$, find $x$ with $g^x \equiv b \pmod m$. This is the **hard** direction:
exponentiation is $O(\log n)$, while the best general method here is $O(\sqrt m)$. That
asymmetry is what public-key cryptography rests on.

**Baby-step giant-step.** Write $x = in - j$ with $n = \lceil\sqrt m\,\rceil$,
$1 \le i \le n$, $0 \le j < n$. Then

$$g^{\,in-j} \equiv b \iff \bigl(g^{\,n}\bigr)^{i} \equiv b\,g^{\,j}$$

The two sides now depend on **separate** variables — so tabulate $b\,g^{\,j}$ in a hash
map (baby steps), then look up $(g^n)^i$ (giant steps). Meet in the middle: $O(\sqrt m)$
time and memory.

---

## Complexity

| $\mathrm{ord}_m(a)$ | $O(\sqrt m)$ to factor $\varphi(m)$, then $O(\log^2 m)$ |
|---|---|
| find a primitive root mod $p$ | $O(\sqrt p)$ to factor $p-1$, then a few tests |
| discrete log (BSGS) | $O(\sqrt m)$ time and memory |
| modular exponentiation (the easy way) | $O(\log n)$ |

---

## Pitfalls

| | |
|---|---|
| order when $\gcd(a,m)\neq1$ | $a^k$ never reaches $1$; undefined |
| assuming every modulus has a primitive root | only $1,2,4,p^k,2p^k$ |
| testing all $k < p-1$ | test only the prime divisors of $p-1$ |
| forgetting $\mathrm{ord}(a) \mid \varphi(m)$ | it is what makes the fast order algorithm work |
| BSGS returning the first hit as smallest $x$ | store the **largest** $j$ per key, scan $i$ ascending |
| BSGS with $\gcd(g,m)\neq1$ | the standard version needs coprimality |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp) · [problems.md](problems.md)
