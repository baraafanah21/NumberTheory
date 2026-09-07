# Miller–Rabin Primality Test — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Where Fermat fails.** Check that $2^{560} \equiv 1 \pmod{561}$, so $561$ passes the
Fermat test at base $2$ — then write $560 = d\cdot2^{s}$ and follow the squaring chain
$2^{d},\ 2^{2d},\ 2^{4d},\dots$ Which entry proves $561$ composite, and what exactly is
wrong with it?

**2. A strong pseudoprime.** $2047 = 23\times89$ passes **Miller–Rabin** at base $2$, not
just Fermat. Verify that, then find the smallest base that exposes it.

**3. Why twelve bases.** The base set $\{2,3,5,7\}$ is not enough: the smallest composite
that survives all four is $3215031751$. Factor it, confirm it passes those four bases, and
find a base that catches it.

**4. Carmichael numbers.** List every Carmichael number below $10^4$ (composite $n$ passing
Fermat for **every** base coprime to $n$). What do their factorizations have in common?

---

## With code

**5. One big number.** Decide whether $10^{18}+9$ is prime, and whether $10^{18}+7$ is.
Then find the next prime above $10^{18}$. Trial division would need $10^9$ operations per
number — time both if you want the lesson to stick.

**6. Density up high.** Among the first $10^5$ odd numbers greater than $10^{12}$, how many
are prime? Compare against the prediction from $1/\ln n$ (doubled, since you only tested
odds).

**7. The overflow that eats everyone.** Write `mulMod` **without** `__int128` and run
`isPrime` on a prime near $10^{18}$. What does it report, and why is the failure silent?
How many bits does the intermediate product actually need?

**8. Off-by-one, both directions.** In `passesBase`, try `for (int r = 1; r <= s; ++r)`
(one squaring too many) and `for (int r = 1; r < s - 1; ++r)` (one too few). For each: say
which extra or missing value of the chain gets tested, then decide whether it changes any
verdict. Exactly one of the two is a real bug — find the smallest $n$ it gets wrong.

---

## Harder

**9. From a failed test to a factor.** Prove that modulo a prime $p$, $x^2 \equiv 1$ forces
$x \equiv \pm1$. Then: when Miller–Rabin finds an $x \not\equiv \pm1$ with
$x^2 \equiv 1 \pmod n$, show $\gcd(x-1,\ n)$ is a **non-trivial factor** of $n$ — and
extract one from the base-$2$ run on $561$.

**10. The $\frac34$ bound, and what determinism buys.** For composite $n$, at least
$\frac34$ of the bases in $[1,n-1]$ are witnesses. Assuming that, how many random bases put
the error below $10^{-18}$? Compare against the $12$ fixed bases that are *provably* exact
below $3.18\times10^{23}$, and say why the fixed set is not merely "the same thing with a
seed".

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| CF 1033B · 1100 | [Square Difference](https://codeforces.com/problemset/problem/1033/B) | $a^2-b^2 = (a-b)(a+b)$ is prime only if $a-b=1$ and $a+b$ is prime — with $a \le 10^{11}$, that primality test is the whole problem |
| CF 679A · 1400 | [Bear and Prime 100](https://codeforces.com/problemset/problem/679/A) | interactive: decide primality of a hidden $n \le 100$ from divisibility queries — deterministic-witness thinking in miniature |
| LC 866 · Med | [Prime Palindrome](https://leetcode.com/problems/prime-palindrome/) | generate candidates cleverly, test each one — the test is the inner loop |
| LC 2523 · Med | [Closest Prime Numbers in Range](https://leetcode.com/problems/closest-prime-numbers-in-range/) | at $10^6$ a sieve wins; useful as the contrast case for "which tool" |

Mainstream judges rarely force a $10^{18}$ primality test — the honest home for this
algorithm is SPOJ's classic primality tasks, Library Checker's `primality_test`, and
Pollard's rho factorization, where Miller–Rabin is the subroutine that decides when to stop
splitting.

---

## Answers

**1.** $560 = 35\cdot2^4$, so $d = 35$, $s = 4$. The chain is

$$2^{35} \equiv 263,\quad 263^2 \equiv 166,\quad 166^2 \equiv 67,\quad 67^2 \equiv 1,\quad 1^2 \equiv 1 \pmod{561}$$

It does end at $1$ — that is the Fermat test passing. But the entry just before the first
$1$ is $\mathbf{67}$, and modulo a prime the only square roots of $1$ are $\pm1$, i.e. $1$
and $560$. So $67$ is a forbidden square root of $1$, and $561$ is composite.

**2.** $2046 = 1023\cdot2$, and $2^{1023} \equiv 1 \pmod{2047}$ — the chain starts at $1$,
so base $2$ is fooled outright. $2047$ is in fact the **smallest** strong pseudoprime base
$2$. Base $\mathbf 3$ catches it.

**3.** $3215031751 = 151\cdot751\cdot28351$. It passes bases $2, 3, 5, 7$ and fails at
$\mathbf{11}$. It is the smallest composite passing all of $\{2,3,5,7\}$ — which is why
"two or three bases and call it proof" is not a strategy, and why the tabulated
deterministic sets exist.

**4.** $\mathbf{561, 1105, 1729, 2465, 2821, 6601, 8911}$. Every one is **squarefree with
at least three distinct prime factors** — that is Korselt's criterion ($n$ squarefree and
$(p-1) \mid (n-1)$ for every $p \mid n$). No amount of extra bases fixes the Fermat test on
these; only the square-root-of-$1$ refinement does.

**5.** $10^{18}+9$ is **prime**; $10^{18}+7$ is **composite**. The next prime above
$10^{18}$ is $\mathbf{10^{18}+3}$ — a gap of $3$. Twelve bases at $\sim60$ modular
multiplications each is under a thousand operations, against $10^9$ for trial division.

**6.** $\mathbf{7243}$ of them. The estimate: density $1/\ln(10^{12}) = 1/27.6$, doubled to
$1/13.8$ for odds only, times $10^5$ — about $7240$. Prime density really does decay like
$1/\ln n$, and at $10^{12}$ that is one in $28$.

**7.** With plain `long long`, `a * b` for $a,b$ near $10^{18}$ overflows: the product needs
about **120 bits** ($10^{36} \approx 2^{119.6}$), against the $63$ available. Signed
overflow is undefined behaviour and in practice wraps, so `mulMod` returns a plausible-
looking wrong residue and `isPrime` reports confident nonsense — no crash, no warning. That
is why every multiplication goes through `__int128`.

**8.** The loop as written tests $a^{2^{r}d}$ for $r = 1,\dots,s-1$; the $r=0$ case is the
`x` computed before it.

*One too many* ($r \le s$) additionally tests $a^{2^{s}d} = a^{\,n-1} \equiv -1$. For a
prime that value is $1$, so it never fires — and no odd composite below $10^5$ satisfies it
either. Pure wasted work, no wrong answers.

*One too few* ($r < s-1$) drops $a^{2^{s-1}d}$ — the **last** place a $-1$ can appear, and
for many primes the only one. That is the real bug: it rejects genuine primes. The smallest
is $\mathbf{n = 5}$ ($d=1$, $s=2$: the loop body never runs, so $2^2 \equiv 4 = n-1$ is
never seen). $13$, $17$, $29$, $37$ follow — over $3000$ primes below $60000$ get called
composite. Errors that reject primes are at least loud; the direction that would *accept*
composites is the one this algorithm is carefully built to avoid.

**9.** $x^2 \equiv 1 \pmod p$ means $p \mid (x-1)(x+1)$; since $p$ is prime, Euclid's lemma
forces $p \mid x-1$ or $p \mid x+1$, i.e. $x \equiv \pm1$. Now suppose $x^2 \equiv 1
\pmod n$ with $x \not\equiv \pm1$. Then $n \mid (x-1)(x+1)$ but $n$ divides neither factor,
so $\gcd(x-1, n)$ is neither $1$ nor $n$ — a proper factor. From problem 1, $x = 67$ gives
$\gcd(66, 561) = \mathbf{33}$, and indeed $561 = 33\cdot17$.

**10.** Independent random bases give error $\le 4^{-k}$, so $10^{-18}$ needs
$k \ge 30$ — more than twice the work of the deterministic set, for a *worse* guarantee.
The fixed twelve are not "randomness with a seed": they come with a **proof**, verified by
exhaustive search over all composites below $3.18\times10^{23}$, that no composite in that
range survives them. The probabilistic bound says "almost surely right"; the fixed set says
"right", and for 64-bit inputs there is no reason to accept the weaker claim.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
