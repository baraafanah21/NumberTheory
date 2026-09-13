# Patterns

> *"I see this type of problem. What should I think about?"*

The [concepts](../concepts/) are organised by **technique**. This section is organised by
**problem shape**, which is what you actually have in front of you when the technique is
still unknown.

Read a concept to learn a tool. Read a pattern to find out which tool you need.

---

## Start here: what does the problem look like

| the statement says … | pattern |
|---|---|
| you may repeat an operation; what is reachable | [invariants and reachability](gcd-pattern.md) |
| make the elements equal, or all share a remainder | [invariants and reachability](gcd-pattern.md) |
| how many $x \le n$ are divisible by … | [counting multiples](divisibility-pattern.md) |
| the $k$-th number with some divisibility property | [counting multiples](divisibility-pattern.md) |
| output the answer modulo $10^9+7$ | [print it mod p](modular-pattern.md) |
| the answer is a fraction, or a probability | [print it mod p](modular-pattern.md) |
| is it prime / count the primes / factor these | [which prime tool](prime-pattern.md) |
| exactly $k$ divisors, perfect power, square-free | [think in exponents](factorization-pattern.md) |
| trailing zeros, or anything about $n!$'s primes | [think in exponents](factorization-pattern.md) |
| after $n$ operations, with $n$ up to $10^{18}$ | [n is up to 1e18](big-exponent-pattern.md) |
| a linear recurrence at absurd $n$ | [n is up to 1e18](big-exponent-pattern.md) |
| two cycles, when do they coincide | [simultaneous conditions](congruence-pattern.md) |
| several remainder conditions at once | [simultaneous conditions](congruence-pattern.md) |

---

## The seven patterns

| pattern | the one-line version |
|---|---|
| [invariants and reachability](gcd-pattern.md) | adding and subtracting existing values never changes the gcd |
| [counting multiples](divisibility-pattern.md) | it is $\lfloor n/d\rfloor$, not a loop |
| [print it mod p](modular-pattern.md) | $+$, $-$, $\times$ pass through; division needs an inverse |
| [which prime tool](prime-pattern.md) | count how many numbers you must test, not how big they are |
| [think in exponents](factorization-pattern.md) | drop the primes, keep the exponent vector |
| [n is up to $10^{18}$](big-exponent-pattern.md) | huge $n$ with a small state means a $\log n$ algorithm |
| [simultaneous conditions](congruence-pattern.md) | merge pairwise, and check solvability first |

Every pattern file has the same seven sections: recognition clues, key observation, relevant
concepts, typical approach, complexity, common traps, and example problems drawn from this
repository.

---

## Two questions that resolve most problems

**1. What does the operation preserve?** If a problem lets you repeat something, find the
invariant before looking for an algorithm. The gcd, a remainder, a parity, an exponent
vector — one of these is usually conserved, and naming it is usually the solution.

**2. How many, and how big?** These two numbers pick the tool almost mechanically. Many
small values means sieve. One huge value means Miller–Rabin or rho. A huge count with a
small state means binary exponentiation. Read the constraints before the statement.

---

## Where the patterns overlap

They are not disjoint, and a real problem often sits in two.

* [think in exponents](factorization-pattern.md) needs a factorization, so it almost always
  enters through [which prime tool](prime-pattern.md)
* [print it mod p](modular-pattern.md) is the substrate under most counting problems, so it
  co-occurs with nearly everything
* [simultaneous conditions](congruence-pattern.md) and
  [counting multiples](divisibility-pattern.md) meet whenever a problem asks for the $n$-th
  solution rather than the count
* [invariants and reachability](gcd-pattern.md) supplies the lcm that
  [simultaneous conditions](congruence-pattern.md) runs on

When two patterns apply, the one naming the **hard** step is the one to follow. The other
is bookkeeping.

---

[repository root](../README.md) · [concepts](../concepts/) ·
[templates](../templates/README.md) · [problems](../problems/README.md)
