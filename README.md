# NumberTheory
Recently, I wasted a lot of time jumping between books and resources. To help you avoid the same mistake, here’s my condensed path with extra details you can skip if needed.
A personal number theory repository, built one concept at a time. Each concept lives in
its own directory with three files:

- `README.md` — definition, intuition, formulation, properties, examples, algorithm,
  problem-solving patterns, common mistakes
- `proofs.md` — rigorous proofs of every stated property
- `implementation.cpp` — C++17 implementation with time and space complexities

## Concepts

| Concept | Depends on | Key results |
|---|---|---|
| [arithmetic-and-geometric-progressions](arithmetic-and-geometric-progressions/) | — | AP/GP sums, $(a-b) \mid (a^n-b^n)$, $\sigma(p^k)$ |
| [divisibility](divisibility/) | — | division algorithm, $\sqrt n$ divisor pairing, digit tests |
| [gcd-and-euclidean-algorithm](gcd-and-euclidean-algorithm/) | divisibility, progressions | Euclid's algorithm, Bézout, Euclid's lemma |

## Dependency graph

```text
progressions ─────────────┐
                          ├──→ gcd & Euclid ──→ (extended Euclid → modular inverse → CRT)
divisibility ─────────────┘         │
                                    └──→ Euclid's lemma ──→ unique factorization
```

## Conventions

- Mathematical notation in the markdown files uses LaTeX (`$…$`, `$$…$$`), which renders
  on GitHub and in the VS Code markdown preview (`Ctrl+Shift+V`).
- When a concept borrows a result it does not prove, the borrowing is stated explicitly
  and linked, never assumed silently.
- Every `implementation.cpp` compiles clean under `g++ -std=c++17 -Wall -Wextra` and its
  demo output is verified.
