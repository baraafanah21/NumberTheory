# Connections

> _"I know the tools. How do they fit together?"_

The concepts are easier to remember as a dependency map than as fifteen isolated
chapters. Start with the shape of the problem, then follow the edge to the smallest
tool that answers it.

## The mental map

```text
divisibility ──→ divisors
│
└──→ sieve ──→ prime factorization ──→ Euler phi
                                       │
                                       └──→ primitive roots ──→ discrete log

gcd ──┬──→ extended Euclid ──→ CRT
      │
      └──→ modular arithmetic ──┬──→ modular inverse
                                │
                                └──→ fast power ──→ Miller-Rabin ──→ Pollard rho

progressions — closed forms; independent of everything above
```

The arrows mean **often depends on**, not "must be read immediately before". For
example, Pollard rho uses fast modular multiplication and Miller-Rabin, while CRT
uses gcd and extended Euclid when the moduli are not assumed coprime.

## Four routes through the map

### A divisibility route

Use [divisibility](../concepts/divisibility/) to turn a statement into a multiple or
remainder condition. Move to [divisors](../concepts/divisors/) when the question asks
for every divisor, and to [sieve](../concepts/sieve/) when the same work is repeated
for many values. If the answer depends on powers of primes, finish with
[prime factorization](../concepts/prime-factorization/).

### A congruence route

Normalize expressions with [modular arithmetic](../concepts/modular-arithmetic/).
Use [fast power](../concepts/fast-power/) for a huge exponent and
[modular inverse](../concepts/modular-inverse/) when division appears. The inverse
route usually passes through [extended Euclid](../concepts/extended-euclid/) or
[Euler's totient](../concepts/euler-phi/), depending on the modulus and its factors.

### A simultaneous-conditions route

Translate each condition into a congruence, then combine them with the
[Chinese remainder theorem](../concepts/chinese-remainder-theorem/). First check
compatibility with a gcd; pairwise coprime moduli are the easy special case.

### A prime route

For many small values, use the [sieve](../concepts/sieve/). For one value near
$10^{18}$, use [Miller-Rabin](../concepts/miller-rabin/). If its factors are needed,
add [Pollard rho](../concepts/pollard-rho/), which calls primality testing recursively.
For questions about generators or $g^x \equiv b$, use
[primitive roots](../concepts/primitive-roots/) after checking the relevant group.

## The recurring handoff

Most solutions are a chain of three decisions:

1. **What is preserved?** Look for a gcd, remainder, parity, or prime-exponent vector.
2. **What is repeated?** Replace loops over a value with a formula, a sieve, or binary
   exponentiation.
3. **What is the expensive operation?** Choose the prime tool or the modular tool from
   the constraints, then check its preconditions.

The [patterns](../patterns/README.md) section starts from the problem statement; this
page explains where the selected technique leads next.

---

[repository root](../README.md) · [cheatsheet](../cheatsheet/README.md) ·
[mistakes](../mistakes/README.md) · [patterns](../patterns/README.md)
