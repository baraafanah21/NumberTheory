# Miller–Rabin Primality Test

Decide whether **one** number is prime, for $n$ up to $10^{18}$ — far past anything you can
sieve.

Trial division costs $O(\sqrt n)$, which is $10^9$ operations at $n = 10^{18}$. Miller–Rabin
does it in a few hundred multiplications.

**Use:** primality of a single large number. **Needs:**
[modular inverse](../modular-inverse/) (Fermat's little theorem) ·
[fast power](../fast-power/) (every base test is one `powMod`).
**Complements:** [sieve](../sieve/), which handles _many small_ numbers.
**Next:** [Pollard's rho](../pollard-rho/), which turns "composite" into the
actual factors.

---

## Starting point: Fermat, and why it is not enough

Fermat says: if $p$ is prime and $p \nmid a$, then $a^{\,p-1} \equiv 1 \pmod p$.

Read it backwards — if $a^{\,n-1} \not\equiv 1 \pmod n$, then $n$ is **definitely
composite**, and $a$ is called a **witness**. One fast exponentiation, and no factor
required.

**But the converse fails.** Some composites pass for _every_ base coprime to them —
**Carmichael numbers**, the smallest being

$$561 = 3 \times 11 \times 17$$

There are infinitely many, so the plain Fermat test can never be made reliable by adding
bases.

---

## The fix: use square roots of 1

Modulo a **prime**, the only solutions of $x^2 \equiv 1$ are $x \equiv \pm1$. Composites
have extra ones — mod $8$, $x^2 \equiv 1$ has four solutions $\{1,3,5,7\}$. Miller–Rabin
hunts for those extras.

Write $n-1$ with all factors of two pulled out:

$$n - 1 = d\cdot 2^{\,s}, \qquad d \text{ odd}$$

Now look at the sequence $a^{\,d},\ a^{\,2d},\ a^{\,4d},\ \dots,\ a^{\,2^{s}d} = a^{\,n-1}$,
each the square of the last. If $n$ is prime the last entry is $1$ by Fermat, and stepping
backwards, the entry just before the first $1$ must be $-1$. So:

$$\boxed{\ n \text{ prime} \implies a^{\,d} \equiv 1 \quad\text{or}\quad a^{\,2^{r}d} \equiv -1 \ \text{ for some } 0 \le r < s\ }$$

If **neither** holds, $n$ is composite — and this time no Carmichael number escapes.

```cpp
bool isProbablePrime(ll n, ll a) {           // one base
    if (n % a == 0) return n == a;
    ll d = n - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }      // n - 1 = d * 2^s, d odd

    ll x = powMod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int r = 1; r < s; ++r) {
        x = mulMod(x, x, n);                 // square up the chain
        if (x == n - 1) return true;
    }
    return false;                            // a is a witness: n is composite
}
```

---

## Making it deterministic

For a composite $n$, **at least $\frac34$ of the bases are witnesses** — so random bases
give error $\le 4^{-k}$. But you do not need randomness at all: for bounded $n$, fixed base
sets are known to be **exactly right**.

$$\text{bases } \{2,3,5,7,11,13,17,19,23,29,31,37\} \ \text{ is deterministic for } n < 3.18\times10^{23}$$

which covers every 64-bit integer with room to spare. Twelve bases, no probability, no
seed.

> A faster set, $\{2,\ 325,\ 9375,\ 28178,\ 450775,\ 9780504,\ 1795265022\}$, is
> deterministic for all $n < 2^{64}$ using only **seven** bases.

---

## How to recognize it in a problem

Look for:

* **"is $n$ prime"** with $n$ up to $10^{18}$ — a single number, far past any sieve
* constraints around $10^{12}$ to $10^{18}$ on a value whose primality decides the answer
* a factored expression that is prime only in one case — $a^2-b^2 = (a-b)(a+b)$ is prime
  only when $a-b=1$ and $a+b$ is prime
* **few queries, huge values**; the opposite of the sieve's profile
* checking whether a constructed candidate is prime inside a search loop
* as the **stopping rule** inside [Pollard's rho](../pollard-rho/), where it is not an
  optimisation but what makes the recursion terminate

**The tell.** Count the numbers you must test. One or a handful of large ones is
Miller–Rabin; a whole range of small ones is the [sieve](../sieve/).

**Anti-pattern.** Do not use a plain Fermat test — Carmichael numbers such as $561$ pass
for every coprime base, and there are infinitely many. And do not use two or three random
bases and call it proof when a known deterministic set covers all 64-bit inputs.

---

## Complexity

| one base                       | $O(\log n)$ modular multiplications          |
| ------------------------------ | -------------------------------------------- |
| full test, 12 bases            | $\sim 12 \log n \approx 750$ multiplications |
| trial division, for comparison | $O(\sqrt n) = 10^9$ at $n = 10^{18}$         |

Each `mulMod` needs `__int128`, since $n$ up to $9\times10^{18}$ makes products reach
$10^{37}$.

**Choosing:** many numbers below $10^7$ → [sieve](../sieve/). One number
up to $10^{18}$ → Miller–Rabin. Need the _factors_ of a big number →
[Pollard's rho](../pollard-rho/), which uses Miller–Rabin to know when to
stop.

---

## Pitfalls

|                                             |                                                               |
| ------------------------------------------- | ------------------------------------------------------------- |
| `a * b % n` without `__int128`              | overflows for $n > 2^{31}$ — the classic silent failure       |
| forgetting $a \bmod n = 0$                  | happens when $n$ is itself a small base; return `n == a`      |
| $n < 2$, or $n$ even                        | handle before the main loop                                   |
| using 2–3 random bases and calling it proof | use the deterministic set                                     |
| trusting a plain Fermat test                | Carmichael numbers ($561$, $1105$, $1729$, …) pass every base |
| comparing against $-1$                      | it is $n-1$ in code                                           |
| looping `r` from $0$                        | the $r=0$ case is the initial $x$; the loop starts at $1$     |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp) · [problems.md](problems.md)
