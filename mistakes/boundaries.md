# Boundaries and off-by-one

Individually trivial, collectively responsible for more wrong answers than any clever
mistake. Every one of these is a single character.

---

## The divisor pair loop

```cpp
for (ll d = 1; d <= n / d; ++d)
    if (n % d == 0) {
        use(d);
        if (d != n / d) use(n / d);      // do NOT emit sqrt(n) twice
    }
```

Two separate errors live here. Dropping the `d != n / d` guard double-counts the square
root, so $\tau(36)$ comes out $10$ instead of $9$. And the loop **emits out of order** —
$1, 36, 2, 18, 3, 12, \dots$ — so sort if the caller indexes the result.

---

## The leftover prime

```cpp
for (ll p = 2; p <= n / p; ++p)
    if (n % p == 0) { /* strip the whole power */ }
if (n > 1) f.push_back({n, 1});          // THE line people forget
```

After trial division to $\sqrt n$, whatever remains above $1$ is a prime factor larger than
$\sqrt n$. Omitting that line loses it silently: $14$ factorizes as $2$ alone.

The same trailing `if` appears in $\varphi$ — `if (n > 1) r -= r / n;` — and is forgotten
just as often.

---

## Sieve boundaries

| | |
|---|---|
| forgetting $0$ and $1$ are not prime | set both false explicitly |
| starting the inner loop at $2p$ | correct but slower; start at $p^2$ |
| running the outer loop to $n$ | wasted passes; stop at $\sqrt n$ |
| `p * p` in an `int` | overflows at $p > 46341$; cast or loop `p <= n / p` |

Both optimisations follow from one fact: a composite is always caught by its **smallest**
prime factor. So any multiple $kp$ with $k < p$ is already gone, and any composite $m \le n$
has a prime factor $\le\sqrt n$.

---

## Counting terms

| | |
|---|---|
| dropping the $+1$ in a term count | steps plus one equals terms |
| $a + nd$ for the $n$-th term | it is $a + (n-1)d$ |
| counting in $[l, r]$ as $\lfloor r/d\rfloor - \lfloor l/d\rfloor$ | it is $l-1$, not $l$ |
| forgetting $r = 1$ in the geometric sum | division by zero; that case is $na$ |

$$\text{number of terms} = \frac{\text{last} - \text{first}}{d} + 1$$

---

## Identity and empty cases

| | truth |
|---|---|
| $\tau(1)$, $\sigma(1)$ | both $1$ — the empty product, not $0$ |
| $\varphi(1)$ | $1$ |
| $\gcd(0, 0)$ | $0$ by convention, so an all-zero array returns $0$ |
| $\gcd(0, x)$ | $\lvert x\rvert$ — which is why folding from $0$ needs no first-element case |
| `lcm(0, x)` | guard it: the gcd is zero and you would divide by it |
| $1$ treated as prime | it is not; allowing it would destroy unique factorization |

---

## Loop-start subtleties

**Miller–Rabin's inner loop starts at $r = 1$, not $r = 0$.** The $r = 0$ case is the
initial value of $x$, already tested before the loop. Starting at $0$ squares once too
early.

**BSGS must keep the largest $j$ per key and scan $i$ ascending.** Otherwise it returns
*some* solution rather than the **smallest** one, which is usually what is asked.

---

## How to catch it

* Test $n = 0$ and $n = 1$ on everything. They are legal inputs and they are where these
  live.
* Test a **perfect square** against every divisor routine.
* Test a number with a large prime factor, such as $2 \times 999999937$, against every
  factorizer.
* Check any counting routine against a brute-force loop for all $n$ up to a few thousand.
  Every implementation in this repository does exactly that, and it is why these bugs are
  not in it.

---

## Where it appears

[divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) ·
[sieve](../concepts/sieve/) ·
[prime-factorization](../concepts/prime-factorization/) ·
[euler-phi](../concepts/euler-phi/) · [progressions](../concepts/progressions/) ·
[gcd](../concepts/gcd/) · [miller-rabin](../concepts/miller-rabin/) ·
[primitive-roots](../concepts/primitive-roots/)

---

[mistakes index](README.md)
