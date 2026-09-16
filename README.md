# NumberTheory

Recently, I wasted a lot of time jumping between books and resources. To help you avoid the same mistake, here’s my condensed path with extra details you can skip if needed.
A personal number theory repository, built one concept at a time — each one small, proved
properly, and paired with the applied code that actually gets used.

**Read the concepts in this order** — each one uses the ones before it. After concept
15, use the linked reference and problem-solving guides.

| #   | Concept                                                          | What you get from it                              |
| --- | ---------------------------------------------------------------- | ------------------------------------------------- |
| 1   | [Progressions](concepts/progressions/)                           | summing a sequence without looping                |
| 2   | [Divisibility](concepts/divisibility/)                           | divisors in $O(\sqrt n)$, sieves, digit tests     |
| 3   | [Divisors](concepts/divisors/)                                   | enumerate and count divisors                      |
| 4   | [GCD and the Euclidean algorithm](concepts/gcd/)                 | gcd, lcm, coprimality, reachability               |
| 5   | [Unique factorization](concepts/prime-factorization/)            | why prime factorization is _the_ factorization    |
| 6   | [Extended Euclidean algorithm](concepts/extended-euclid/)        | Bézout coefficients, $ax+by=c$, CRT               |
| 7   | [Modular arithmetic](concepts/modular-arithmetic/)               | congruences, normalization, safe operations       |
| 8   | [Modular multiplicative inverse](concepts/modular-inverse/)      | dividing under a modulus, $\binom{n}{k} \bmod p$  |
| 9   | [Fast power](concepts/fast-power/)                               | computing $a^b \bmod m$ in $O(\log b)$            |
| 10  | [Chinese remainder theorem](concepts/chinese-remainder-theorem/) | combining congruences, splitting a computation    |
| 11  | [Sieve of Eratosthenes](concepts/sieve/)                         | all primes up to $n$, fast factorization          |
| 12  | [Euler's totient function](concepts/euler-phi/)                  | inverses for any modulus, huge exponents          |
| 13  | [Primitive roots and discrete log](concepts/primitive-roots/)    | cycle lengths, generators, solving $g^x \equiv b$ |
| 14  | [Miller–Rabin primality test](concepts/miller-rabin/)            | is _this_ number prime, for $n$ up to $10^{18}$   |
| 15  | [Pollard's rho factorization](concepts/pollard-rho/)             | the _factors_ of one number, in $O(n^{1/4})$      |

After the concepts, use [connections](connections/README.md) for the mental map,
[patterns](patterns/README.md) to choose a technique from a problem statement,
[cheatsheet](cheatsheet/README.md) for a compact reference, and
[mistakes](mistakes/README.md) when an answer goes wrong.

```text
2 divisibility ──→ 3 divisors
  │
  └──→ 4 gcd ──→ 5 factorization ──→ 11 sieve
         │         │
         │         └──→ 12 phi ──→ 13 primitive roots
         │
         └──→ 6 extended Euclid ──→ 10 CRT

7 modular arithmetic ──┬──→ 8 inverse
                       │
                       └──→ 9 fast power ──→ 14 Miller–Rabin ──→ 15 rho

1 progressions — closed forms; independent of everything above
```

The two tracks meet at **8, the modular inverse**: extended Euclid (6) gives it on any
modulus, while fast power (9) and phi (12) give it on a prime one. Primitive roots (13)
need both phi and fast power.

**Primes: which tool.** Many numbers below $10^7$ → the **sieve** (11). One number up to
$10^{18}$ → **Miller–Rabin** (14). Its **factors** → **Pollard's rho** (15), which calls
Miller–Rabin to know when to stop splitting.

Concept 4 proves **Euclid's lemma**, which is what makes concept 5 true — and concept 5 is
what makes the formulas in 11 and 12 well defined.

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
- When a concept uses a result it does not prove, it says so and links to where it _is_
  proved.
- Every `implementation.cpp` compiles clean under `g++ -std=c++17 -Wall -Wextra`, and its
  demo verifies its own output against brute force.

```sh
g++ -std=c++17 -O2 -o demo concepts/divisibility/implementation.cpp && ./demo
```
