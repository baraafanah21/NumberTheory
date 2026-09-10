# Euler's Totient Function

$$\varphi(n) = \#\{\,k : 1 \le k \le n,\ \gcd(k,n) = 1\,\}$$

How many numbers up to $n$ are **coprime to $n$**.

**Use:** inverses modulo a **composite**, reducing huge exponents, counting coprime pairs.
**Needs:** [modular inverse](../modular-multiplicative-inverse/) ·
[sieve](../sieve-of-eratosthenes/) **Next:**
[primitive roots](../primitive-roots-and-discrete-log/)

---

## Getting a feel

| $\varphi(1) = 1$ | $\{1\}$ |
|---|---|
| $\varphi(6) = 2$ | $\{1,5\}$ — $2,3,4,6$ all share a factor |
| $\varphi(7) = 6$ | a prime is coprime to everything below it |
| $\varphi(9) = 6$ | only the multiples of $3$ are removed |

$$\varphi(p) = p-1, \qquad \varphi(p^k) = p^k - p^{\,k-1}$$

The second is just counting: among $1\dots p^k$ the numbers sharing a factor are exactly
the $p^{k-1}$ multiples of $p$.

---

## The formula

$$\boxed{\ \varphi(n) = n \prod_{p \,\mid\, n}\Bigl(1 - \frac1p\Bigr)\ }$$

over the **distinct** primes dividing $n$. For $n = 12 = 2^2\cdot3$:

$$\varphi(12) = 12\cdot\tfrac12\cdot\tfrac23 = 4 \qquad \{1,5,7,11\}\ ✓$$

It rests on $\varphi$ being **multiplicative**:
$\ \gcd(m,n)=1 \Rightarrow \varphi(mn) = \varphi(m)\varphi(n)$.

> Coprimality is essential: $\varphi(2)\varphi(2) = 1$ but $\varphi(4) = 2$.

---

## Euler's theorem — the payoff

$$\gcd(a,m)=1 \quad\Longrightarrow\quad a^{\,\varphi(m)} \equiv 1 \pmod m$$

**This generalizes Fermat**: for prime $m=p$, $\varphi(p)=p-1$ and it reads
$a^{\,p-1}\equiv1$.

**Inverses for any modulus.** $\ a^{-1} \equiv a^{\,\varphi(m)-1} \pmod m$ — filling the
gap where the exponentiation shortcut needed a **prime**. (It costs a factorization to get
$\varphi(m)$; extended Euclid is still faster for a one-off inverse.)

**Shrinking exponents.** $\ a^{\,k} \equiv a^{\,k \bmod \varphi(m)} \pmod m$, so a
$10^5$-digit exponent collapses.

> **The trap:** that needs $\gcd(a,m)=1$. With $a=2$, $m=4$, $k=4$: $2^4 \equiv 0$, but
> $2^{4 \bmod \varphi(4)} = 2^0 = 1$. For general $a$ use the safe form, valid for **all**
> $a$ once $k \ge \log_2 m$:
> $$a^{\,k} \equiv a^{\,(k \bmod \varphi(m)) + \varphi(m)} \pmod m$$

---

## Computing it

**One value — $O(\sqrt n)$.** Apply the product formula while trial-dividing:

```cpp
ll phi(ll n) {
    ll r = n;
    for (ll p = 2; p <= n / p; ++p)
        if (n % p == 0) {
            while (n % p == 0) n /= p;      // strip the whole power
            r -= r / p;                     // multiply by (1 - 1/p), in integers
        }
    if (n > 1) r -= r / n;                  // one large prime factor is left
    return r;
}
```

`r -= r / p` is how you apply $\bigl(1-\frac1p\bigr)$ without leaving the integers — $1/p$
would truncate to $0$. The trailing `if` matters.

**Every value up to $n$ — $O(n\log\log n)$.** Same skeleton as the sieve:

```cpp
for (int i = 0; i <= n; ++i) phi[i] = i;
for (int p = 2; p <= n; ++p)
    if (phi[p] == p)                        // untouched, so p is prime
        for (int m = p; m <= n; m += p)
            phi[m] -= phi[m] / p;
```

`phi[p] == p` doubles as a primality test.

---

## A useful identity

$$\sum_{d \,\mid\, n} \varphi(d) = n$$

For $n=12$: $\ 1+1+2+2+2+4 = 12$ ✓. It appears whenever you group $1\dots n$ by $\gcd$
with $n$ — and it is what makes the primitive-root theorem work.

---

## Complexity

| $\varphi(n)$ for one $n$ | $O(\sqrt n)$ |
|---|---|
| $\varphi(n)$ with an `spf` table | $O(\log n)$ |
| $\varphi(1..n)$ | $O(n\log\log n)$, $O(n)$ space |

---

## Pitfalls

| | |
|---|---|
| $\varphi(mn)=\varphi(m)\varphi(n)$ without $\gcd(m,n)=1$ | $\varphi(2)\varphi(2)=1$, $\varphi(4)=2$ |
| a repeated prime counted twice | the product uses **distinct** primes |
| forgetting the leftover prime | the `if (n > 1)` line |
| `r * (1 - 1/p)` in integers | $1/p$ truncates; use `r -= r / p` |
| reducing an exponent when $\gcd(a,m)\neq1$ | use the $+\varphi(m)$ safe form |
| $\varphi(1)$ | it is $1$ |
| Euler's inverse when extgcd would do | Euler needs a factorization |

---

[proofs.md](proofs.md) · [implementation.cpp](implementation.cpp) · [problems.md](problems.md)
