# Mistakes

Built by merging the **Pitfalls** tables from all fifteen concept folders — 110 rows —
into seven themed files. Nothing here is hypothetical; every entry came from a real trap
already documented in this repository.

This section is meant to **grow**. When a problem costs you a wrong submission, add the
mistake to the file it belongs in, with the symptom you actually saw.

---

## Start from the symptom

| what you are seeing | look here |
|---|---|
| wrong answer only on the largest tests | [overflow](overflow.md) |
| a negative number where one is impossible | [overflow](overflow.md) · [negative modulo](negative-modulo.md) |
| crash or garbage on an array index | [negative modulo](negative-modulo.md) |
| the checker rejects a correct-looking answer | [negative modulo](negative-modulo.md) |
| off by a small factor, consistently | [modular division](modular-division.md) |
| right on samples, wrong on a composite modulus | [modular division](modular-division.md) · [exponents](exponents.md) |
| right for small exponents, wrong for huge ones | [exponents](exponents.md) |
| a formula that works sometimes | [coprimality](coprimality.md) |
| count off by exactly one | [boundaries](boundaries.md) |
| a missing prime factor | [boundaries](boundaries.md) |
| correct but too slow | [wrong tool](wrong-tool.md) |
| a primality test that accepts a composite | [wrong tool](wrong-tool.md) |
| the program hangs and never finishes | [wrong tool](wrong-tool.md) · [exponents](exponents.md) |

---

## The seven files

| file | the theme |
|---|---|
| [overflow](overflow.md) | `int` vs `long long`, `a * b % m`, `d * d <= n`, accumulators |
| [negative modulo](negative-modulo.md) | `%` takes the sign of the dividend, and what that ruins |
| [modular division](modular-division.md) | there is no division; inverses, cancellation, hidden fractions |
| [exponents](exponents.md) | the exponent reduces mod $\varphi(m)$, and Euler versus Fermat |
| [coprimality](coprimality.md) | theorems applied without checking $\gcd = 1$ |
| [boundaries](boundaries.md) | off-by-one, leftover primes, sieve limits, identity cases |
| [wrong tool](wrong-tool.md) | correct but too slow, and probabilistic tests called proofs |

---

## The five that cost the most

If you only remember a handful, these are the ones that produce **silent** wrong answers
rather than visible failures.

1. **`a * b % m` overflows** once $m > \sim3\times10^9$. No warning. Use `__int128`.
2. **`%` returns negative** for a negative dividend. Normalize before indexing or printing.
3. **`(a / b) % m` is not division.** Multiply by the inverse, and check it exists.
4. **Fermat's inverse on a composite modulus** returns a plausible wrong number.
5. **Exponents reduce mod $\varphi(m)$, not mod $m$** — and only when $\gcd(a,m)=1$.

---

## Three counterexamples worth memorising

They break most of the false statements in this section on their own.

| | breaks |
|---|---|
| $\gcd(6,10,15) = 1$, no pair coprime | setwise versus pairwise coprimality, CRT |
| $\varphi(2)\varphi(2) = 1 \neq 2 = \varphi(4)$ | multiplicativity without coprime arguments |
| $6 \mid 4\times9$, dividing neither | composite divisors splitting across a product |

And one number: **$561 = 3\times11\times17$**, the smallest Carmichael number, which passes
the Fermat test for every base coprime to it.

---

## How to add to this

Keep the format of the existing files: state the mistake in code or one line, say **why**
it happens, give the **symptom**, then the fix. A mistake with no symptom attached is hard
to use later, because you search this file by what you are seeing, not by what you did.

When a mistake shows up in a solved problem, note it in that problem's write-up too — see
[problems/TEMPLATE.md](../problems/TEMPLATE.md).

---

[repository root](../README.md) · [concepts](../concepts/) ·
[patterns](../patterns/README.md) · [templates](../templates/README.md)
