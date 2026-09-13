# Coprimality and multiplicativity

A cluster of errors with one shape: applying a theorem **without checking its hypothesis**.
The hypothesis is almost always $\gcd = 1$, and it is almost never decoration.

---

## Pairwise coprime is not the same as setwise coprime

$$\gcd(6, 10, 15) = 1, \qquad\text{yet}\qquad \gcd(6,10)=2,\ \gcd(6,15)=3,\ \gcd(10,15)=5$$

The whole set is coprime while **no pair is**. Pairwise coprimality is strictly stronger,
and it is the one [CRT](../concepts/chinese-remainder-theorem/) needs.

This is the single most reused counterexample in the repository. Memorise the triple.

---

## Multiplicative functions need coprime arguments

$$\gcd(a,b) = 1 \implies f(ab) = f(a)f(b)$$

Drop the hypothesis and it fails immediately:

| | with $\gcd \neq 1$ | truth |
|---|---|---|
| $\varphi$ | $\varphi(2)\varphi(2) = 1$ | $\varphi(4) = 2$ |
| $\tau$ | $\tau(2)\tau(2) = 4$ | $\tau(4) = 3$ |
| $\sigma$ | $\sigma(2)\sigma(2) = 9$ | $\sigma(4) = 7$ |

The reason is structural: with a shared prime the exponents **add** rather than staying
independent, so the per-prime choices are no longer separate.

Related: the $\varphi$ product formula runs over **distinct** primes. Counting a repeated
prime twice is the same error wearing different clothes.

---

## $\gcd(a,b) = 1$ does not mean either is prime

$\gcd(8,9) = 1$ and neither is prime. Coprimality is a statement about a **pair**, not
about the numbers individually.

---

## A composite divisor does not split across a product

$$d \mid ab \quad\not\Longrightarrow\quad d \mid a \ \text{ or } \ d \mid b$$

$6 \mid 4 \times 9$, yet $6$ divides neither — because $6 = 2\times3$ **splits** across the
two factors. The implication needs $d$ **prime**, and that is exactly Euclid's lemma. It is
also what makes factorization unique.

Reasoning that silently splits a composite is the most common wrong step in divisibility
arguments.

---

## CRT solvability is not automatic

Two congruences combine **iff**

$$\gcd(m_1,m_2) \ \big|\ (a_2 - a_1)$$

With shared factors the system is often genuinely contradictory: $x \equiv 1 \pmod 6$
forces $x$ odd, $x \equiv 2 \pmod 8$ forces $x$ even. No solution exists.

Two more in the same family:

* the merged modulus is $\mathrm{lcm}(m_1,m_2)$, **not** $m_1m_2$ — they agree only for
  coprime moduli
* the direct $\sum a_iM_iy_i$ formula is valid **only** for pairwise coprime moduli; with
  $(6,10,15)$ the inverse it needs does not exist

---

## Orders and primitive roots

| | |
|---|---|
| $\mathrm{ord}_m(a)$ when $\gcd(a,m) \neq 1$ | undefined — $a^k$ never reaches $1$ |
| assuming every modulus has a primitive root | only $m = 1, 2, 4, p^k, 2p^k$ |
| BSGS with $\gcd(g,m) \neq 1$ | the standard version requires coprimality |

There is no primitive root mod $8$, $12$ or $15$. Every **prime** modulus has one, which is
the case that usually matters.

---

## How to catch it

* When you invoke a named theorem, write its hypothesis down next to the call. If you
  cannot state it, you should not be using it.
* Test with a deliberately non-coprime pair. Most of these errors are invisible on coprime
  inputs, and coprime inputs are what samples contain.
* Keep $(6,10,15)$ and $(2,2)$ as standing counterexamples — between them they break
  almost everything in this file.

---

## Where it appears

[gcd](../concepts/gcd/) ·
[chinese-remainder-theorem](../concepts/chinese-remainder-theorem/) ·
[euler-phi](../concepts/euler-phi/) · [divisors](../concepts/divisors/) ·
[divisibility](../concepts/divisibility/) ·
[prime-factorization](../concepts/prime-factorization/) ·
[primitive-roots](../concepts/primitive-roots/) ·
[modular-inverse](../concepts/modular-inverse/)

---

[mistakes index](README.md)
