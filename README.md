# Number Theory

Number theory for programming, built one concept at a time. Each concept is small, proved
properly, and comes with the code that actually gets used.

**Read them in this order** — each one uses the ones before it:

| # | Concept | What you get from it |
|---|---|---|
| 1 | [Progressions](arithmetic-and-geometric-progressions/) | summing a sequence without looping |
| 2 | [Divisibility](divisibility/) | divisors in $O(\sqrt n)$, sieves, digit tests |
| 3 | [GCD and the Euclidean algorithm](gcd-and-euclidean-algorithm/) | gcd, lcm, coprimality, reachability |
| 4 | [Extended Euclidean algorithm](extended-euclidean-algorithm/) | modular inverse, $ax+by=c$, CRT |

```text
1. progressions ──┐
                  ├──→ 3. gcd & Euclid ──→ 4. extended Euclid ──→ modular inverse, CRT
2. divisibility ──┘
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
