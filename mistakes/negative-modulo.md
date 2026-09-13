# Negative values and the `%` operator

C++ truncates division **toward zero**, so `%` takes the sign of the **dividend**, not the
divisor. That disagrees with the mathematical residue, which is always in $[0, m)$.

```cpp
-17 % 5   == -2        // C++
-17 % -5  == -2
 17 % -5  ==  2
```

The mathematical answer for $-17 \bmod 5$ is $\mathbf{3}$.

---

## Why it matters

Harmless in an equality test. **Fatal** as an array index, a hash key, a loop bound, or a
printed answer. A negative index is undefined behaviour, which in practice means a crash or
silent corruption.

Fix it in exactly one place and use that everywhere:

```cpp
ll norm(ll x, ll m) { x %= m; return x < 0 ? x + m : x; }
```

Or, when both operands are already reduced into $[0, m)$, the cheaper branch-free form
`(a - b + m) % m`.

---

## Where it actually bites

**Subtraction.** This is the common one, because $a < b$ happens constantly:

```cpp
(a - b) % m          // negative whenever a < b
norm(a - b, m)       // right
```

**Forgetting the final normalize before printing.** A negative answer fails the checker
even when every intermediate step was correct.

**`extgcd` coefficients.** Bézout routinely returns a negative $x$ — for
$\gcd(240,46)=2$ it gives $x=-9$. Using it unnormalized as a
[modular inverse](../concepts/modular-inverse/) produces a negative "inverse".

```cpp
ll modInverse(ll a, ll m) {
    ll x, y;
    if (extgcd(norm(a, m), m, x, y) != 1) return -1;
    return norm(x, m);                    // both normalizations are required
}
```

**gcd of negative inputs.** Returns a negative number unless you take `abs` on entry.
Signs never matter to a gcd, so strip them immediately.

**Comparing against $-1$ in Miller–Rabin.** The test is "is this $\equiv -1$", but in code
the value is stored as $n-1$. Comparing to a literal `-1` never matches.

---

## Also here: modulus edge cases

| | |
|---|---|
| `n % 0` | undefined behaviour — guard $m \neq 0$ |
| `r = 1` as an accumulator when $m$ may be $1$ | write `1 % m`, else it returns $1$ instead of $0$ |

The $m = 1$ case is legal input and graders do use it. Every residue is $0$ modulo $1$, so
any routine returning $1$ there is wrong.

---

## How to catch it

* Test with $a < b$ deliberately, and with negative inputs, not just the happy path.
* Test $m = 1$ on every modular routine you write.
* If an index ever comes from arithmetic, normalize at the point of use, not somewhere
  upstream where a later edit can bypass it.
* Assert the invariant in local builds: `assert(0 <= x && x < m);` before every array
  access that uses a residue.

---

## Where it appears

[modular-arithmetic](../concepts/modular-arithmetic/) ·
[divisibility](../concepts/divisibility/) ·
[modular-inverse](../concepts/modular-inverse/) ·
[extended-euclid](../concepts/extended-euclid/) · [gcd](../concepts/gcd/) ·
[miller-rabin](../concepts/miller-rabin/) · [fast-power](../concepts/fast-power/)

---

[mistakes index](README.md)
