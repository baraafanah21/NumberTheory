# NumberTheory

A personal number theory repository, built one concept at a time. Each concept lives in
its own directory with three files:

- `README.md` — definition, intuition, formulation, properties, examples, algorithm,
  problem-solving patterns, common mistakes
- `proofs.md` — rigorous proofs of every stated property
- `implementation.cpp` — C++17 implementation with time and space complexities

## Concepts

| Concept | Depends on |
|---|---|
| [arithmetic-and-geometric-progressions](arithmetic-and-geometric-progressions/) | — |
| [gcd-and-euclidean-algorithm](gcd-and-euclidean-algorithm/) | divisibility*, progressions (G11) |

\* divisibility is not written yet; the four facts used are stated as explicit
prerequisites in that concept's §0.

Mathematical notation in the markdown files uses LaTeX (`$…$`, `$$…$$`), which renders on
GitHub and in the VS Code markdown preview (`Ctrl+Shift+V`).
