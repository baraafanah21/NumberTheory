# NumberTheory
Recently, I wasted a lot of time jumping between books and resources. To help you avoid the same mistake, here’s my condensed path with extra details you can skip if needed.
A personal number theory repository, built one concept at a time — each one small, proved
properly, and paired with the applied code that actually gets used.

**Read them in this order** — each one uses the ones before it:

| # | Concept | What you get from it |
|---|---|---|
| 1 | [Progressions](concepts/progressions/) | summing a sequence without looping |
| 2 | [Divisibility](concepts/divisibility/) | divisors in $O(\sqrt n)$, sieves, digit tests |
| 3 | [GCD and the Euclidean algorithm](concepts/gcd/) | gcd, lcm, coprimality, reachability |
| 4 | [Unique factorization](concepts/prime-factorization/) | why prime factorization is *the* factorization |
| 5 | [Extended Euclidean algorithm](concepts/extended-euclid/) | Bézout coefficients, $ax+by=c$, CRT |
| 6 | [Modular multiplicative inverse](concepts/modular-inverse/) | dividing under a modulus, $\binom{n}{k} \bmod p$ |
| 7 | [Chinese remainder theorem](concepts/chinese-remainder-theorem/) | combining congruences, splitting a computation |
| 8 | [Sieve of Eratosthenes](concepts/sieve/) | all primes up to $n$, fast factorization |
| 9 | [Euler's totient function](concepts/euler-phi/) | inverses for any modulus, huge exponents |
| 10 | [Primitive roots and discrete log](concepts/primitive-roots/) | cycle lengths, generators, solving $g^x \equiv b$ |
| 11 | [Miller–Rabin primality test](concepts/miller-rabin/) | is *this* number prime, for $n$ up to $10^{18}$ |
| 12 | [Pollard's rho factorization](concepts/pollard-rho/) | the *factors* of one number, in $O(n^{1/4})$ |

```text
1. progressions ──┐
                  │                     ┌──→ 4. unique factorization ──┐
2. divisibility ──┼──→ 3. gcd & Euclid ─┤                              │
                  │                     └──→ 5. extended Euclid ──┬──→ 6. modular inverse
                  │                                               │       │        │
                  │                                               └→ 7. CRT ←┘     │
                  │                                                       11. Miller–Rabin ──→ 12. rho
                  └──→ 8. sieve ──────────────────────────→ 9. Euler's totient
                                                                    │
                                                    10. primitive roots ←┘
```

**Primes: which tool.** Many numbers below $10^7$ → the **sieve** (8). One number up to
$10^{18}$ → **Miller–Rabin** (11). Its **factors** → **Pollard's rho** (12), which calls
Miller–Rabin to know when to stop splitting.

Concept 3 proves **Euclid's lemma**, which is what makes concept 4 true — and concept 4 is
what makes the formulas in 8 and 9 well defined.

## What is in each folder

- **`README.md`** — the idea in plain language, the formulas with every symbol named, the
  one or two algorithms you actually implement, and the mistakes that cost you.
- **`proofs.md`** — why each formula is true. Every proof states the claim, explains it in
  words, proves it, then says which line of code it justifies.
- **`implementation.cpp`** — only the functions that matter, with complexities, plus a demo
  that checks itself.
- **`problems.md`** — ten problems, by hand then in code then harder, with worked answers.
  Every check value in them was computed, so a mismatch means a bug in your code. Each ends
  with a table of **Codeforces and LeetCode problems** that drill the same idea.

## Conventions

- Maths is written in LaTeX, so it renders on GitHub and in the VS Code preview
  (`Ctrl+Shift+V`).
- When a concept uses a result it does not prove, it says so and links to where it *is*
  proved.
- Every `implementation.cpp` compiles clean under `g++ -std=c++17 -Wall -Wextra`, and its
  demo verifies its own output against brute force.

```sh
g++ -std=c++17 -O2 -o demo concepts/divisibility/implementation.cpp && ./demo
```
