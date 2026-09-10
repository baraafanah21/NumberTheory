# Pollard's Rho — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. The asymmetry.** At $n = 10^{18}$: roughly how many operations does
[Miller–Rabin](../miller-rabin-primality-test/) need to decide *is $n$ prime*, and how many
does trial division need to produce *a factor of $n$*? Name the everyday system whose
security is exactly this gap.

**2. Trace it on paper.** Take $n = 8051$, $f(x) = x^2+1$, and start both pointers at
$x_0 = y_0 = 2$. Run Floyd — $x$ one step, $y$ two — and after each round compute
$\gcd(|x-y|,\ n)$. How many rounds until a factor appears, and what is $8051$?

**3. Why the gcd is a factor.** Prove: if $p \mid n$ is prime, $x \equiv y \pmod p$ and
$x \not\equiv y \pmod n$, then $1 < \gcd(|x-y|, n) < n$. Then answer the two follow-ups:
what does $\gcd = n$ mean, and why does the algorithm never need to know $p$?

**4. Birthday arithmetic.** A random map on a set of size $p$ closes its loop after about
$\sqrt{\pi p/2} \approx 1.25\sqrt p$ steps. If the smallest prime factor of $n$ is near
$10^9$, how many iterations should you expect? Why does that make the *worst* case for
$n \le 10^{18}$ about $n^{1/4}$ rather than $\sqrt n$?

---

## With code

**5. Three factorizations.** Factor $2^{63}-1$, $1000000016000000063$, and $10^{18}$. The
first has six distinct primes, the second is the product of the two moduli you use every
day, and the third should come back instantly — say why it is the easiest of the three.

**6. Measure the claim.** Factor $999999943999999559$ twice: once with rho, once with a
trial-division loop to $\sqrt n$. Report both wall times and the ratio. Predict the ratio
before you run it.

**7. Divisor counts out of reach.** Compute $\tau(n)$ for $n = 963761198400$,
$n = 897612484786617600$ and $n = 10^{18}$. The $O(\sqrt n)$ loop from
[divisibility](../divisibility/) cannot touch the middle one — how long would it have
taken?

**8. Break it on purpose.** Run rho on $N = 999999937 \times 1000000007$ with $c = 0$ from
$20$ random starts, capping each run at $20{,}000$ iterations; count how many find a
factor. Repeat with a random $c$ each time. Explain the difference — what is special about
the orbit of $f(x) = x^2$?

---

## Harder

**9. Batching is lossless.** Prove that for non-zero $d_1,\dots,d_k$ and
$q \equiv \prod_i d_i \pmod n$,

$$\gcd(q,n) > 1 \iff \gcd(d_i,n) > 1 \ \text{ for some } i$$

(one direction needs Euclid's lemma). Then construct a batch where $\gcd(q,n) = n$ although
no single $d_i$ shares all of $n$, and say exactly what `pollardRho` does when that happens.

**10. Two things that make the recursion legal.** (a) Prove by exhaustion that
$f(x) = x^2+c$ can **never** split $n = 4$, for any $c$ and any start — then say which line
of `factorize` this justifies. (b) Prove `factorRec` terminates and returns the canonical
factorization, and say precisely what goes wrong if you delete the `isPrime` base case.

---

## Judge problems

Rho is a *library* algorithm: it rarely headlines a problem, it sits underneath one. The
scale is the tell — if the values are past $10^{12}$ and you need their factors, this is
the tool.

| where | problem | what it drills |
|---|---|---|
| CF 1305F · 2500 | [Kuroni and the Punishment](https://codeforces.com/problemset/problem/1305/F) | make every element share a prime, moving each by $\pm1$: sample elements at random, factor the values around them, then test each prime that turns up |
| Library Checker | [Factorize](https://judge.yosupo.jp/problem/factorize) | the algorithm by itself, at $n < 2^{62}$ — the cleanest place to check your implementation |
| LC 952 · Hard | [Largest Component Size by Common Factor](https://leetcode.com/problems/largest-component-size-by-common-factor/) | the same pipeline — factor every value, union by shared prime — at a scale where a sieve suffices |
| LC 1390 · Med | [Four Divisors](https://leetcode.com/problems/four-divisors/) | $\tau(n) = 4$ means $p^3$ or $pq$; factorization decides which |
| LC 2507 · Med | [Smallest Value After Replacing With Sum of Prime Factors](https://leetcode.com/problems/smallest-value-after-replacing-with-sum-of-prime-factors/) | factor, sum, repeat — a fixed-point loop over factorizations |

LeetCode's constraints never reach the range where rho is *needed*; the three above are the
same shape at sieve scale. SPOJ's `FACT0`/`FACT1` are the classic tasks that do require it.

---

## Answers

**1.** Miller–Rabin: $12$ bases $\times$ about $60$ modular squarings $\approx$ **750
multiplications**. Trial division: up to $\sqrt{10^{18}} = \mathbf{10^9}$ divisions — over
a million times more. **RSA** depends on precisely this: the public key is a product of two
large primes, and anyone can verify primality while nobody can undo the multiplication.

**2.** Three rounds:

| round | $x$ | $y$ | $\gcd(\lvert x-y\rvert, 8051)$ |
|---|---|---|---|
| 1 | $5$ | $26$ | $1$ |
| 2 | $26$ | $7474$ | $1$ |
| 3 | $677$ | $871$ | $\mathbf{97}$ |

$\gcd(194, 8051) = 97$, and $8051 = \mathbf{83 \times 97}$. Three rounds — against $83$
divisions for trial division.

**3.** $p \mid (x-y)$ and $p \mid n$, so $p \mid \gcd(|x-y|,n)$ and the gcd is at least
$p > 1$. If it were $n$, then $n \mid (x-y)$, contradicting $x \not\equiv y \pmod n$.
$\gcd = n$ means the sequence collided modulo *every* prime factor at once — the run is
wasted, and you restart with a fresh $c$ and start value. And $p$ never appears in the
computation: `gcd(|x - y|, n)` uses only numbers you hold, which is the entire trick.

**4.** $1.25\sqrt{10^9} \approx \mathbf{39{,}600}$ iterations. The worst case for a
composite $n$ is two primes of equal size, $p \approx q \approx \sqrt n$; the work is then
$\sqrt p = \sqrt{\sqrt n} = n^{1/4}$. At $n = 10^{18}$ that order of magnitude is
$31{,}600$ — the same $\approx39{,}600$ once the constant is kept — against $10^9$ for
trial division. Note the exponent, not the constant, is what matters: $n^{1/4}$ **squares**
the reach of $O(\sqrt n)$.

**5.**

$$2^{63}-1 = 7^2 \times 73 \times 127 \times 337 \times 92737 \times 649657$$
$$1000000016000000063 = 1000000007 \times 1000000009$$
$$10^{18} = 2^{18} \times 5^{18}$$

The last is the **easiest**: trial division of the small primes strips it completely and
rho is never called. Difficulty tracks the *second largest* prime factor, not the size of
$n$.

**6.** Rho: about $\mathbf{0.2}$ **ms**. Trial division: $\mathbf{2192}$ **ms** — a factor
of roughly $\mathbf{10^4}$. The number is $999999937 \times 1000000007$, two primes of
nearly equal size, which is the worst case for trial division and the case rho exists for.

**7.** $\tau(963761198400) = \mathbf{6720}$,
$\tau(897612484786617600) = \mathbf{103680}$, $\tau(10^{18}) = \mathbf{361}$. The middle
one has $\sqrt n \approx 9.5\times10^8$, so the divisibility-folder loop would need about a
billion iterations — a second or two, against microseconds here. (It is
$2^8 3^4 5^2 7^2 \cdot 11 \cdot 13 \cdot 17 \cdot 19 \cdot 23 \cdot 29 \cdot 31 \cdot 37$,
built to have many divisors.)

**8.** With $c = 0$, only $\mathbf{2}$ of $20$ starts split $N$ within $20{,}000$
iterations; with a random $c$, **every** run splits it, taking between $8{,}800$ and
$29{,}900$ iterations. The reason: $f(x) = x^2$ gives $x_i = x_0^{\,2^{i}}$, so the cycle
length is the multiplicative order of $2$ modulo $\mathrm{ord}_p(x_0)$ — on the order of
$\log p$, not $\sqrt p$. The orbit closes long before enough values have been visited for a
birthday collision. ($c = -2$ degenerates the same way, through Lucas doubling.)

**9.** ($\Leftarrow$) If a prime $p$ divides $d_i$ and $n$, then $p \mid \prod_i d_i$;
writing $\prod_i d_i = q + tn$ and using $p \mid n$ gives $p \mid q$, so $\gcd(q,n) \ge p$.
($\Rightarrow$) If a prime $p$ divides $\gcd(q,n)$, then $p \mid \prod_i d_i$, and
**Euclid's lemma** forces $p \mid d_i$ for some $i$.

A batch returning $n$: take $n = 15$ with differences $d_1 = 3$ and $d_2 = 5$. Neither
shares all of $n$, but $q = 15 \equiv 0$, so $\gcd(q,n) = 15 = n$ — two different primes
collected in one batch. `pollardRho` handles it by keeping `ys`, the value at the **start**
of the batch, and replaying that batch one step at a time; the individual gcd then returns
$3$ or $5$ instead of $15$. Only if the step-by-step replay also gives $n$ is the run
genuinely dead, and a new $c$ is drawn.

**10.** (a) There are only $4$ choices of $c$ and $4$ of $x_0$ modulo $4$; running every
one of the $16$ pairs, no iteration ever produces $\gcd(|x-y|,4) = 2$ — the count of
successful splits is exactly zero, not merely rare. This justifies stripping small primes
by trial division before rho is ever called (and returning $2$ immediately for even $n$).
(b) Each recursive call receives a strictly smaller positive integer, so the recursion
depth is bounded and it terminates; correctness is induction on $n$, with Miller–Rabin
exact below $2^{64}$ for the base case and **unique factorization** guaranteeing the two
halves recombine into *the* factorization. Delete `isPrime` and the recursion has no
stopping rule: it calls `pollardRho` on a prime, where no proper factor exists, and the
retry loop spins forever.

---

[README.md](README.md) · [proofs.md](proofs.md) · [implementation.cpp](implementation.cpp)
