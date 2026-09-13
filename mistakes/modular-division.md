# Modular division

There is no division in modular arithmetic. Writing one is not a rounding error, it is a
different operation that happens to compile.

```cpp
(a / b) % m          // WRONG -- integer division discards the remainder first
a * modInverse(b, m) % m     // right
```

$+$, $-$ and $\times$ pass through a modulus untouched. Division is the single exception,
and it becomes a multiplication by an inverse.

---

## The inverse does not always exist

$$a^{-1} \bmod m \ \text{ exists} \iff \gcd(a, m) = 1$$

Modulo a **prime**, every nonzero residue has one, which is why $10^9+7$ is the standard
modulus. Modulo a **composite**, most do not: mod $6$, the multiples of $2$ run
$2,4,0,2,4,0,\dots$ and never reach $1$.

Assuming existence is the quiet failure. Check, or use a routine that returns a sentinel.

---

## Fermat on a composite modulus

```cpp
ll inv = powMod(a, p - 2, p);      // ONLY valid when p is prime
```

On a composite modulus this does not throw, does not return an error, and does not produce
a flagged value. It returns **a wrong number that looks fine**. Modulo $12$ it gives
$5^{-1} = 1$, yet $5 \times 1 = 5 \neq 1$. The true answer is $5$.

| modulus prime, one or two inverses | Fermat, $O(\log p)$ |
|---|---|
| modulus composite or unknown | extended Euclid, $O(\log m)$ |
| $n$ inverses mod a prime | the $O(n)$ table |

LeetCode 372 uses modulus $1337 = 7 \times 191$ precisely to punish this.

---

## Cancellation is not free

From $ac \equiv bc \pmod m$ you may **not** conclude $a \equiv b \pmod m$:

$$2\cdot3 \equiv 2\cdot0 \pmod 6, \quad\text{yet}\quad 3 \not\equiv 0 \pmod 6$$

What survives is the same congruence on a **smaller modulus**:

$$ac \equiv bc \pmod m \implies a \equiv b \ \Bigl(\mathrm{mod}\ \tfrac{m}{\gcd(c,m)}\Bigr)$$

---

## $ax \equiv b \pmod m$ has $\gcd(a,m)$ solutions

Not one. With $g = \gcd(a,m)$ the congruence is solvable iff $g \mid b$, and there are then
**exactly $g$** solutions modulo $m$, spaced $\frac mg$ apart.

Returning the first one is the classic half-right answer: it passes the samples and fails
the tests.

---

## Hidden divisions

These are divisions that do not look like one:

| expression | why it is a division |
|---|---|
| $\dfrac{r^n-1}{r-1}$ | the geometric sum needs $(r-1)^{-1}$, which may not exist |
| $\dfrac{p^{e+1}-1}{p-1}$ | the $\sigma$ formula — sum the powers directly instead |
| $\dfrac{n(n+1)}{2}$ | halve the even factor first, or multiply by $2^{-1}$ |
| an average, a probability, an expected value | the answer is $p\,q^{-1} \bmod M$ |
| $\binom nk$ | factorials underneath |

For the geometric sum, the modulus-safe route uses only $+$ and $\times$:

$$G(2k) = G(k)\bigl(1+r^{\,k}\bigr), \qquad G(n) = G(n-1) + r^{\,n-1}\ (n \text{ odd})$$

---

## Performance, not correctness

Calling `modInverse` inside a loop is $O(n\log m)$ when $O(n)$ is available. For factorials,
invert **once** and walk down: $\mathrm{inv}((i-1)!) = \mathrm{inv}(i!)\cdot i$. One
exponentiation for the whole table.

---

## How to catch it

* Grep your submission for `/` and justify every one that sits under a modulus.
* Verify each inverse: `assert(mulMod(a, inv, m) == 1 % m);`
* If the modulus is not a named prime constant, assume it is composite until proven
  otherwise.
* Test $\gcd(a,m) \neq 1$ deliberately.

---

## Where it appears

[modular-inverse](../concepts/modular-inverse/) ·
[modular-arithmetic](../concepts/modular-arithmetic/) ·
[euler-phi](../concepts/euler-phi/) · [divisors](../concepts/divisors/) ·
[progressions](../concepts/progressions/) ·
[extended-euclid](../concepts/extended-euclid/)

---

[mistakes index](README.md)
