# Euler's Totient Function

$\varphi(n)$ counts how many numbers in $1, 2, \dots, n$ are **coprime to $n$**:

$$\varphi(n) \;=\; \#\{\, k : 1 \le k \le n,\ \gcd(k,n) = 1 \,\}$$

**You need this for:** inverses modulo a **composite** number, reducing huge exponents,
counting coprime pairs, and cycle lengths.

**Before this:** [modular inverse](../modular-multiplicative-inverse/),
[sieve](../sieve-of-eratosthenes/).

---

## Getting a feel for it

$$
\begin{aligned}
\varphi(1) &= 1 && \{1\}\\
\varphi(6) &= 2 && \{1, 5\} \quad\text{— } 2,3,4,6 \text{ all share a factor with } 6\\
\varphi(7) &= 6 && \{1,2,3,4,5,6\} \quad\text{— a prime is coprime to everything below it}\\
\varphi(9) &= 6 && \{1,2,4,5,7,8\} \quad\text{— only the multiples of } 3 \text{ are removed}
\end{aligned}
$$

Two patterns fall straight out:

$$\varphi(p) = p - 1 \qquad\text{and}\qquad \varphi(p^k) = p^k - p^{\,k-1}$$

The second is just counting: among $1 \dots p^k$, the numbers sharing a factor with $p^k$
are exactly the multiples of $p$, and there are $p^{k-1}$ of them.

---

## The formula

$$\boxed{\ \varphi(n) \;=\; n \prod_{p \,\mid\, n} \left(1 - \frac{1}{p}\right)\ }$$

the product running over the **distinct** primes dividing $n$.

**Example.** $n = 12 = 2^2 \times 3$:

$$\varphi(12) = 12\left(1 - \tfrac12\right)\left(1 - \tfrac13\right) = 12 \cdot \tfrac12 \cdot \tfrac23 = 4$$

and indeed $\{1, 5, 7, 11\}$ are the four numbers coprime to $12$. ✓

The formula rests on $\varphi$ being **multiplicative**:

$$\gcd(m,n) = 1 \quad\Longrightarrow\quad \varphi(m\,n) = \varphi(m)\,\varphi(n)$$

> The coprimality condition is essential. $\varphi(2)\varphi(2) = 1$ but $\varphi(4) = 2$.

---

## Euler's theorem — the payoff

$$\gcd(a, m) = 1 \quad\Longrightarrow\quad a^{\,\varphi(m)} \equiv 1 \pmod m$$

**This generalizes Fermat.** When $m = p$ is prime, $\varphi(p) = p-1$ and it reads
$a^{\,p-1} \equiv 1$ — exactly Fermat's little theorem.

### Consequence 1: inverses for any modulus

$$a^{-1} \equiv a^{\,\varphi(m)-1} \pmod m$$

This fills the gap left in the [inverse concept](../modular-multiplicative-inverse/), where
the exponentiation shortcut only worked for **prime** moduli. Now it works for any $m$ —
provided you know $\varphi(m)$, which costs a factorization. (Extended Euclid is still
faster when you just need one inverse; this route matters when $\varphi(m)$ is already in
hand.)

### Consequence 2: shrinking huge exponents

$$\gcd(a,m) = 1 \quad\Longrightarrow\quad a^{\,k} \equiv a^{\,k \bmod \varphi(m)} \pmod m$$

So an exponent with $10^5$ digits collapses to something below $\varphi(m)$.

> **The trap.** This requires $\gcd(a,m) = 1$. When it fails the rule is **wrong** — e.g.
> $a=2$, $m=4$, $k=4$: $2^4 = 16 \equiv 0$, but $2^{4 \bmod \varphi(4)} = 2^0 = 1$.
> For general $a$, use the safe form (valid for **all** $a$, once $k \ge \log_2 m$):
> $$a^{\,k} \equiv a^{\,(k \bmod \varphi(m)) \,+\, \varphi(m)} \pmod m$$

---

## Computing it

### One value — $O(\sqrt n)$

Apply the product formula while trial-dividing:

```cpp
ll phi(ll n) {
    ll result = n;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            while (n % p == 0) n /= p;      // strip p out completely
            result -= result / p;           // result *= (1 - 1/p), in integers
        }
    if (n > 1) result -= result / n;        // one large prime factor is left
    return result;
}
```

`result -= result / p` is how you multiply by $\left(1-\frac1p\right)$ without leaving the
integers. The trailing `if` matters: after the loop, $n$ is either $1$ or a single prime
larger than $\sqrt{\text{original}}$.

### Every value up to $n$ — $O(n\log\log n)$

Same skeleton as the sieve. Start each entry at itself, then let each prime scale down its
own multiples:

```cpp
std::vector<int> phi(n + 1);
for (int i = 0; i <= n; ++i) phi[i] = i;
for (int p = 2; p <= n; ++p)
    if (phi[p] == p)                        // untouched, so p is prime
        for (int m = p; m <= n; m += p)
            phi[m] -= phi[m] / p;
```

The test `phi[p] == p` doubles as a primality check — nothing has scaled $p$ down yet.

---

## A useful identity

$$\sum_{d \,\mid\, n} \varphi(d) \;=\; n$$

For $n = 12$: $\ \varphi(1)+\varphi(2)+\varphi(3)+\varphi(4)+\varphi(6)+\varphi(12)
= 1+1+2+2+2+4 = 12$ ✓

It appears whenever you group $1 \dots n$ by $\gcd$ with $n$, and is the standard way to
invert totient sums.

---

## Complexity

| Task | Time | Space |
|---|---|---|
| $\varphi(n)$ for one $n$ | $O(\sqrt n)$ | $O(1)$ |
| $\varphi(n)$ with an `spf` table | $O(\log n)$ | — |
| $\varphi(1..n)$, all at once | $O(n\log\log n)$ | $O(n)$ |
| $a^{-1} \bmod m$ via Euler | $O(\sqrt m + \log m)$ | $O(1)$ |

---

## Common mistakes

| Mistake | Fix |
|---|---|
| Using $\varphi(mn) = \varphi(m)\varphi(n)$ without $\gcd(m,n)=1$ | $\varphi(2)\varphi(2)=1$ but $\varphi(4)=2$ |
| Including a repeated prime twice in the product | the product runs over **distinct** primes |
| Forgetting the leftover prime after the trial-division loop | the `if (n > 1)` line |
| Writing `result * (1 - 1/p)` in integers | $1/p$ truncates to $0$; use `result -= result / p` |
| Reducing an exponent mod $\varphi(m)$ when $\gcd(a,m) \neq 1$ | use the $+\varphi(m)$ safe form |
| Applying Euler's theorem when $\gcd(a,m) \neq 1$ | it is simply false then |
| $\varphi(1)$ | it is $1$, not $0$ |
| Using Euler's inverse when extended Euclid would do | Euler needs a factorization; extgcd does not |

---

## Files

- [proofs.md](proofs.md) — why $\varphi$ is multiplicative (via CRT), where the product
  formula comes from, a proof of Euler's theorem, and the divisor-sum identity.
- [implementation.cpp](implementation.cpp) — four functions with self-checks.
