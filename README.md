# NumberTheory
Recently, I wasted a lot of time jumping between books and resources. To help you avoid the same mistake, here’s my condensed path with extra details you can skip if needed.
A personal number theory repository, built one concept at a time — each one small, proved
properly, and paired with the code that actually gets used.

**Read them in this order** — each one uses the ones before it:

| # | Concept | What you get from it |
|---|---|---|
| 1 | [Progressions](arithmetic-and-geometric-progressions/) | summing a sequence without looping |
| 2 | [Divisibility](divisibility/) | divisors in $O(\sqrt n)$, sieves, digit tests |
| 3 | [GCD and the Euclidean algorithm](gcd-and-euclidean-algorithm/) | gcd, lcm, coprimality, reachability |
| 4 | [Extended Euclidean algorithm](extended-euclidean-algorithm/) | Bézout coefficients, $ax+by=c$, CRT |
| 5 | [Modular multiplicative inverse](modular-multiplicative-inverse/) | dividing under a modulus, $\binom{n}{k} \bmod p$ |
| 6 | [Sieve of Eratosthenes](sieve-of-eratosthenes/) | all primes up to $n$, fast factorization |
| 7 | [Euler's totient function](euler-totient-function/) | inverses for any modulus, huge exponents |

```text
1. progressions ──┐
                  ├──→ 3. gcd & Euclid ──→ 4. extended Euclid ──→ 5. modular inverse
2. divisibility ──┤                                 │                     │
                  │                                 └──→ CRT ─────┐       │
                  └──→ 6. sieve ────────────────────────────────→ 7. Euler's totient
```

## What is in each folder

- **`README.md`** — the idea in plain language, the formulas with every symbol named, the
  one or two algorithms you actually implement, and the mistakes that cost you.
- **`proofs.md`** — why each formula is true. Every proof states the claim, explains it in
  words, proves it, then says which line of code it justifies.
- **`implementation.cpp`** — only the functions that matter, with complexities, plus a demo
  that checks itself.

## Conventions

- Maths is written in LaTeX, so it renders on GitHub and in the VS Code preview
  (`Ctrl+Shift+V`).
- When a concept uses a result it does not prove, it says so and links to where it *is*
  proved.
- Every `implementation.cpp` compiles clean under `g++ -std=c++17 -Wall -Wextra`, and its
  demo verifies its own output against brute force.

```sh
g++ -std=c++17 -O2 -o demo divisibility/implementation.cpp && ./demo
```
