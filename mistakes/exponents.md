# Exponents

Everything here comes from one confusion: **the base lives modulo $m$, the exponent does
not.**

---

## The exponent reduces mod $\varphi(m)$, never mod $m$

```cpp
powMod(a, k % m, m)          // WRONG
```

$$\gcd(a,m)=1 \implies a^{\,k} \equiv a^{\,k \bmod \varphi(m)} \pmod m$$

Smallest counterexample to the wrong version: $a=2$, $k=5$, $m=3$. Then $2^5 = 32 \equiv 2$,
while $2^{5 \bmod 3} = 2^2 \equiv 1$. Different.

---

## And even that needs $\gcd(a,m) = 1$

This is the trap one level deeper. With $a=2$, $m=4$, $k=4$:

$$2^4 = 16 \equiv 0 \pmod 4, \qquad 2^{4 \bmod \varphi(4)} = 2^0 = 1$$

For a general base use the **safe form**, valid for every $a$ once $k \ge \log_2 m$:

$$a^{\,k} \equiv a^{\,(k \bmod \varphi(m)) + \varphi(m)} \pmod m$$

Use it whenever the base is not guaranteed coprime to the modulus — which, in a problem
where $a$ is input, is always. Power-tower problems recurse on exactly this, with $\varphi$
reaching $1$ in $O(\log m)$ levels.

---

## Fermat and Euler, stated precisely

| | requires | gives |
|---|---|---|
| Fermat | $p$ **prime**, $p \nmid a$ | $a^{\,p-1} \equiv 1 \pmod p$ |
| Euler | $\gcd(a,m) = 1$ | $a^{\,\varphi(m)} \equiv 1 \pmod m$ |

Euler generalizes Fermat: for prime $m = p$, $\varphi(p) = p-1$. Applying either without
its hypothesis produces a wrong number with no error — see
[modular-division](modular-division.md) for the Fermat-inverse version of this.

**Reading Fermat backwards is valid**: if $a^{\,n-1} \not\equiv 1$, then $n$ is definitely
composite. Reading it forwards is not: passing proves nothing, because Carmichael numbers
pass for every coprime base. See [wrong-tool](wrong-tool.md).

---

## Implementation traps in the power loop

| | |
|---|---|
| `r = 1` when $m$ may be $1$ | write `1 % m`, else it returns $1$ instead of $0$ |
| a negative or oversized base | normalize before the loop |
| `base * base % m` above $m \sim 3\times10^9$ | needs `__int128` — see [overflow](overflow.md) |
| a **negative exponent** | that is an inverse, not a power: $a^{-k} = (a^{-1})^{k}$ |
| `exp >>= 1` on a signed negative value | infinite loop; guard $n \ge 0$ |
| $0^0$ | this loop returns $1$; check what the problem wants |
| matrix power without reducing entries | every matrix multiply must reduce too |
| recomputing $a^n$ inside a loop | hoist it, or precompute a table |

---

## `std::pow` is not an integer operation

```cpp
pow(2, 62)           // returns a double
```

`<cmath>`'s `pow` returns a `double`, which holds only 53 bits of mantissa. Past $2^{53}$
it silently rounds, so `(ll)pow(10, 18)` can come out one off. Write the squaring loop, or
a small integer `ipow`.

The same applies to `sqrt` when used for an exact integer square root — compute the
candidate in floating point, then **correct it with integer comparisons**.

---

## Related: orders

The multiplicative order of $a$ mod $m$ is only defined when $\gcd(a,m) = 1$; otherwise
$a^k$ never returns to $1$. And $\mathrm{ord}_m(a) \mid \varphi(m)$ always, which is what
makes the fast algorithm work — you test the divisors of $\varphi(m)$, not every $k$.

---

## How to catch it

* Whenever you write `%` next to an exponent, stop and ask which modulus belongs there.
* Test $m = 1$ and a composite $m$ with $\gcd(a,m) \neq 1$.
* Check the power routine against a naive repeated-multiplication loop for small inputs
  before trusting it at $10^{18}$.

---

## Where it appears

[fast-power](../concepts/fast-power/) · [euler-phi](../concepts/euler-phi/) ·
[modular-arithmetic](../concepts/modular-arithmetic/) ·
[modular-inverse](../concepts/modular-inverse/) ·
[primitive-roots](../concepts/primitive-roots/) ·
[progressions](../concepts/progressions/)

---

[mistakes index](README.md)
