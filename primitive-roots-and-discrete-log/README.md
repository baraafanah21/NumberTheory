# Primitive Roots and Discrete Logarithm

Euler's theorem says $a^{\varphi(m)} \equiv 1$. This concept asks the two obvious follow-up
questions:

1. **How soon** does $a^k$ first return to $1$? — that is the **order** of $a$.
2. Is there an $a$ whose powers run through **every** coprime residue? — a **primitive
   root**.

And then the reverse problem: given $g^x \equiv b$, find $x$ — the **discrete logarithm**.

**You need this for:** cycle lengths, "how many steps until it repeats", solving
$a^x \equiv b$, and anything built on generators.

**Before this:** [Euler's totient](../euler-totient-function/).

---

## Order

For $\gcd(a,m) = 1$, the **order** of $a$ modulo $m$ is

$$\mathrm{ord}_m(a) \;=\; \text{the smallest } k \ge 1 \ \text{ with } \ a^k \equiv 1 \pmod m$$

It exists because Euler guarantees $a^{\varphi(m)} \equiv 1$, so *some* exponent works.

**The two facts you use:**

$$a^k \equiv 1 \pmod m \quad\Longleftrightarrow\quad \mathrm{ord}_m(a) \mid k
\qquad\text{and in particular}\qquad \mathrm{ord}_m(a) \ \big|\ \varphi(m)$$

**Example, $m = 7$** (so $\varphi(7) = 6$):

| $a$ | powers $a^1, a^2, \dots$ | order |
|---|---|---|
| $1$ | $1$ | $1$ |
| $2$ | $2, 4, 1$ | $3$ |
| $3$ | $3, 2, 6, 4, 5, 1$ | **6** |
| $6$ | $6, 1$ | $2$ |

Every order divides $6$ ✓. And $3$ hits **all six** nonzero residues — that makes it a
primitive root.

---

## Primitive roots

$g$ is a **primitive root** mod $m$ when $\mathrm{ord}_m(g) = \varphi(m)$ — the largest
order possible. Equivalently:

$$g^0,\ g^1,\ g^2,\ \dots,\ g^{\varphi(m)-1} \quad\text{are \textbf{all} the residues coprime to } m$$

So a primitive root is a **generator**: every coprime residue is some power of it. That is
what makes discrete logarithms meaningful.

### When does one exist?

$$m = 1,\ 2,\ 4,\ p^k,\ \text{or } 2p^k \qquad (p \text{ an odd prime})$$

and **for no other $m$**. The case that matters: **every prime modulus has one.** There is
no primitive root mod $8$, mod $12$, or mod $15$.

### How many?

If a primitive root exists, there are exactly $\varphi(\varphi(m))$ of them. For a prime
$p$ that is $\varphi(p-1)$ — typically a decent fraction of $p$, so **searching finds one
fast**.

### How to test one — the useful part

Checking $g^k \neq 1$ for every $k < p-1$ would cost $O(p)$. You do not have to: it is
enough to check the **prime divisors** of $p-1$.

$$g \text{ is a primitive root mod } p \quad\Longleftrightarrow\quad
g^{\frac{p-1}{q}} \not\equiv 1 \pmod p \ \text{ for every prime } q \mid (p-1)$$

Since $p-1$ has at most about $15$ distinct prime factors, that is a handful of
exponentiations instead of $p$.

```cpp
// smallest primitive root mod prime p
ll findPrimitiveRoot(ll p) {
    if (p == 2) return 1;
    auto qs = distinctPrimeFactors(p - 1);
    for (ll g = 2; g < p; ++g) {
        bool ok = true;
        for (ll q : qs) if (powerMod(g, (p - 1) / q, p) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;
}
```

In practice the answer is small — under $100$ for essentially every prime you will meet.
(For $p = 10^9+7$ it is $5$.)

---

## Discrete logarithm

Given $g$, $b$, $m$ with $\gcd(g,m)=1$, find $x$ with

$$g^{\,x} \equiv b \pmod m$$

This is the inverse of modular exponentiation — and it is **hard**. Fast exponentiation
takes $O(\log n)$; the best general method here takes $O(\sqrt m)$. That asymmetry is what
public-key cryptography is built on.

### Baby-step giant-step — $O(\sqrt m)$

Write $x = i\,n - j$ with $n = \lceil\sqrt m\,\rceil$, $\ 1 \le i \le n$, $\ 0 \le j < n$.
Every $x$ in range has such a form. Then

$$g^{\,in-j} \equiv b \quad\Longleftrightarrow\quad \bigl(g^{\,n}\bigr)^{i} \equiv b\,g^{\,j}$$

The two sides now depend on **separate** variables, so:

1. **Baby steps** — compute $b\,g^{\,j}$ for $j = 0 \dots n-1$, store each in a hash map.
2. **Giant steps** — compute $(g^n)^i$ for $i = 1 \dots n$, and look each up.

A hit gives $x = i\,n - j$. Time and memory are both $O(\sqrt m)$ — meet in the middle.

---

## Complexity

| Task | Time |
|---|---|
| $\mathrm{ord}_m(a)$ | $O(\sqrt m)$ to factor $\varphi(m)$, then $O(\log^2 m)$ |
| find a primitive root mod $p$ | $O(\sqrt p)$ to factor $p-1$, then a few $O(\log p)$ tests |
| discrete log, baby-step giant-step | $O(\sqrt m)$ time and $O(\sqrt m)$ memory |
| modular exponentiation (the easy direction) | $O(\log n)$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Talking about order when $\gcd(a,m) \neq 1$ | $a^k$ never reaches $1$; the order is undefined |
| Assuming every modulus has a primitive root | only $1, 2, 4, p^k, 2p^k$ — there is none mod $8$ or $15$ |
| Testing all $k < p-1$ for primitive-root-ness | test only the prime divisors of $p-1$ |
| Checking prime *divisors of $p-1$* vs *divisors* | you need $q$ prime; using all divisors is slower but still correct |
| Forgetting $\mathrm{ord}(a) \mid \varphi(m)$ | it is what makes the fast order algorithm work |
| BSGS returning the first hit as smallest $x$ | store the **largest** $j$ per key, and scan $i$ ascending |
| BSGS with $\gcd(g,m) \neq 1$ | the standard version needs coprimality |
| Expecting discrete log to be as fast as exponentiation | it is not — that gap is the point |

---

## Files

- [proofs.md](proofs.md) — why the order divides $\varphi(m)$, the order of a power, why
  every prime has a primitive root (using $\sum_{d\mid n}\varphi(d) = n$), the fast test,
  and BSGS correctness.
- [implementation.cpp](implementation.cpp) — three functions with self-checks.
