# Reaching for the wrong tool

These produce **correct answers too slowly**, or correct-looking answers that are not
proofs. Different failure mode from the rest of this section: the logic is fine, the
judgment is not.

---

## Rebuilding a table per test case

```cpp
while (t--) { auto isPrime = sieve(1000000); ... }   // 1e6 work, t times
```

Sieves, factorial tables, inverse tables and `spf` arrays are built **once, globally**,
before reading input. With $t = 10^4$ this single mistake is the difference between
comfortable and hopeless.

The same applies to calling `modInverse` inside a loop when the $O(n)$ table exists, and to
recomputing $a^n$ where the value could be hoisted.

---

## Picking the wrong prime tool

The constraints decide, and the deciding question is **how many numbers**, not how big.

| how many | how big | tool |
|---|---|---|
| many | $\le 10^7$ | sieve |
| many, need factorizations | $\le 10^7$ | `spf` sieve, then $O(\log m)$ each |
| a window | $r \le 10^{12}$, $r-l$ small | segmented sieve |
| one | $\le 10^{18}$ | Miller–Rabin |
| one, need its factors | $\le 9\times10^{18}$ | Pollard's rho |

| | |
|---|---|
| sieving $10^{12}$ directly | memory; use the segmented version |
| trial-dividing when you already have `spf` | $O(\log m)$ was available |
| reaching for rho below $10^{12}$ | trial division is simpler and fast enough |
| factoring in order to compute a gcd | Euclid is far faster |
| Euler's inverse when `extgcd` would do | Euler needs a factorization first |

---

## Calling a probabilistic test a proof

A **plain Fermat test is not a primality test.** Carmichael numbers — $561$, $1105$,
$1729$, $2465$, $2821$, $6601$, $8911$ — pass for *every* base coprime to them, and there
are infinitely many. No number of bases fixes it.

Using two or three **random** Miller–Rabin bases is also not a proof when a deterministic
set exists. The first twelve primes are verified for $n < 3.18\times10^{23}$, covering all
of `long long`. Dropping one to save time breaks part of the range: the first eleven only
reach $3.8\times10^{18}$.

---

## Doing exponentially more work than needed

| | |
|---|---|
| testing every $k < p-1$ for a primitive root | test only the **prime divisors** of $p-1$ |
| looping to count multiples of $d$ | it is $\lfloor n/d\rfloor$ |
| asking each $m$ for its divisors | invert it: each $d$ visits its multiples, $O(n\log n)$ |
| searching numbers for "exactly $k$ divisors" | search the exponent **shapes** instead |
| one `gcd` per step in Pollard's rho | batch 128 multiplies per gcd |

---

## Structural mistakes that cost correctness, not just time

**Calling Pollard's rho on a prime.** It never returns. The primality test is not an
optimisation there; it is what makes the recursion terminate.

**No retry loop in rho.** A degenerate $(c, x_0)$ returns $g = n$ and must be redrawn.
Likewise $c = 0$ and $c = -2$ give degenerate orbits with cycle length $\sim\log p$ instead
of $\sqrt p$.

**No replay when a batch returns $g = n$.** The batch caught two primes at once and you
lose a genuine split.

**Expecting rho to return the smallest factor.** It returns *a* factor. Sort at the end.

**$n = 4$.** No $(c, x_0)$ splits it, for any $c$. Strip small primes by trial division
first.

---

## Data-structure choices

`std::vector<bool>` is a packed bit array, not an array of bools. It uses eight times less
memory and is measurably slower. For a sieve where speed matters, `vector<char>`; where
memory binds, keep the bits.

Similarly, the plain sieve is often **faster in practice** than the linear sieve despite
the worse bound, because it is cache-friendly and branch-free. Use the linear one when you
want the prime list and `spf` together.

---

## How to catch it

* Read the constraints **before** the statement, and compute the operation count before
  writing code.
* Count how many times each precomputation runs. If it is inside the test loop, move it.
* When a solution is correct but too slow, the fix is usually a different tool, not a
  micro-optimisation of the wrong one.

---

## Where it appears

[sieve](../concepts/sieve/) · [miller-rabin](../concepts/miller-rabin/) ·
[pollard-rho](../concepts/pollard-rho/) ·
[prime-factorization](../concepts/prime-factorization/) ·
[divisors](../concepts/divisors/) · [euler-phi](../concepts/euler-phi/) ·
[modular-inverse](../concepts/modular-inverse/) ·
[primitive-roots](../concepts/primitive-roots/) ·
[divisibility](../concepts/divisibility/)

---

[mistakes index](README.md)
