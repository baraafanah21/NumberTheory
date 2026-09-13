# Overflow

The most expensive category, because it is **silent**. No crash, no warning, no compiler
diagnostic — just a different number. Everything below is a real row from a concept's
pitfalls table.

**The ceiling:** `long long` holds up to $9223372036854775807 \approx 9.22\times10^{18}$.
`int` stops at $2147483647 \approx 2.1\times10^9$, which most number-theory problems clear
in the first test case.

---

## The one that costs the most: `a * b % m`

```cpp
a * b % m            // evaluates a*b FIRST, then reduces
```

The product is formed before the modulus is applied, so it wraps whenever
$a\,b > 9.22\times10^{18}$.

| $m$ | largest product | safe? |
|---|---|---|
| $10^9+7$ | $\approx10^{18}$ | yes, barely |
| $3\times10^9$ | $\approx9\times10^{18}$ | the boundary |
| $10^{12}$ | $10^{24}$ | **no** |
| $9\times10^{18}$ | $8\times10^{37}$ | **no** |

```cpp
ll mulMod(ll a, ll b, ll m) { return (ll)((__int128)a * b % m); }
```

**Rule of thumb:** above $m \approx 3\times10^9$, every single multiply needs `__int128`.
This bites in [Miller–Rabin](../concepts/miller-rabin/), where $n$ reaches $10^{18}$, and
in [Pollard's rho](../concepts/pollard-rho/) twice — once in `x*x + c` and again in the
batched product one line below.

---

## The loop bound

```cpp
for (ll d = 1; d * d <= n; ++d)      // WRONG: d*d overflows near 3e9
for (ll d = 1; d <= n / d; ++d)      // right
```

Identical meaning, no product formed. Same for `p * p <= n` when factorizing and in the
sieve's outer loop, where `p` is often an `int` and `p * p` overflows at $p > 46341$.

---

## Divide before multiplying

```cpp
a * b / g          // WRONG: overflows for inputs whose lcm fits comfortably
(a / g) * b        // right, and exact because g divides a
```

With $a = b = 10^{18}$ the true lcm is $10^{18}$ and fits, but `a * b` is $10^{36}$. The
division-first form is not a hack; it is exact, because $g \mid a$ leaves no remainder.

The same applies to $\frac{n(n+1)}{2}$: halve the even factor **before** multiplying, since
exactly one of $n$ and $n+1$ is even.

---

## Accumulators

| what | why |
|---|---|
| $\sigma(n)$ in an `int` | passes $2^{31}$ before $n = 10^6$ |
| a running sum without reducing | nine additions near $10^{18}$ is enough |
| a matrix multiply without reducing entries | two products near $10^9$ overflow when summed |
| the lcm in a CRT fold | ten moduli near $10^9$ already exceed 64 bits |
| `a1 + m1*t` in a CRT merge | reduce with `mulMod` |
| `a * inv` when $m > 2^{31}$ | `__int128` |

---

## How to catch it

* **Test at the constraint ceiling, not with small inputs.** Overflow is invisible below
  the boundary, so small tests pass and the submission fails.
* **A negative value where one is impossible** is proof of a wrap. A residue mod $10^9+7$
  can never be negative. Do not rely on spotting it though — the wrap is just as often
  positive and looks perfectly ordinary.
* Compile with `-fsanitize=undefined` locally. Signed overflow is undefined behaviour and
  the sanitizer reports it.
* When in doubt, use `__int128` for the intermediate. It costs roughly three times a bare
  multiply, which is almost never the binding constraint.

---

## Where it appears

[modular-arithmetic](../concepts/modular-arithmetic/) ·
[fast-power](../concepts/fast-power/) · [gcd](../concepts/gcd/) ·
[divisibility](../concepts/divisibility/) · [divisors](../concepts/divisors/) ·
[miller-rabin](../concepts/miller-rabin/) · [pollard-rho](../concepts/pollard-rho/) ·
[chinese-remainder-theorem](../concepts/chinese-remainder-theorem/) ·
[progressions](../concepts/progressions/)

---

[mistakes index](README.md)
