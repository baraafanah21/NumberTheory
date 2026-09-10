# Proofs — Pollard's Rho

**Borrowed:** **Euclid's lemma** ([gcd](../gcd-and-euclidean-algorithm/proofs.md)),
**unique factorization** ([unique factorization](../unique-factorization/proofs.md)), and
the **Miller–Rabin test**
([Miller–Rabin](../miller-rabin-primality-test/proofs.md)), which supplies the base case
of the recursion.

---

## 1. Why trial division runs out of room

**Claim.** For $n = pq$ with $p \le q$ both prime, trial division performs at least
$p - 1 \approx \sqrt n$ divisions.

**Proof.** The loop tests $2,3,\dots$ in order and $n$ has no divisor below $p$, so nothing
is found until $p$ is reached. With $p \approx q \approx \sqrt n$ that is
$\Theta(\sqrt n)$. $\blacksquare$

At $n = 10^{18}$ that is $10^9$ divisions for **one** number. Note this is the *same*
barrier Miller–Rabin already broke for the yes/no question — but a primality test does not
hand you the factors, and no known method turns one into the other.

---

## 2. A collision modulo $p$ hands you a factor

**Claim.** Let $p$ be a prime divisor of $n$ with $p < n$. If

$$x \equiv y \pmod p \qquad\text{but}\qquad x \not\equiv y \pmod n$$

then $g = \gcd(|x-y|,\ n)$ is a **proper** divisor of $n$: $1 < g < n$.

**Proof.** $p \mid (x-y)$ and $p \mid n$, so $p \mid g$ and therefore $g \ge p > 1$. If
$g = n$ then $n \mid (x-y)$, contradicting $x \not\equiv y \pmod n$. So $1 < g < n$.
$\blacksquare$

**This is the whole trick.** The statement mentions $p$, but the computation does not:
`gcd(|x - y|, n)` uses only numbers you have. You detect a collision modulo an unknown
prime by its shadow on $n$.

---

## 3. The sequence is eventually periodic — the $\rho$ shape

**Claim.** For any $f:\ \mathbb{Z}/n \to \mathbb{Z}/n$ and any start $x_0$, the sequence
$x_{i+1} = f(x_i)$ is eventually periodic: there are $\lambda \ge 0$ (tail) and $\mu \ge 1$
(cycle) with $x_{i+\mu} = x_i$ for all $i \ge \lambda$.

**Proof.** The values lie in a set of size $n$, so among $x_0,\dots,x_n$ two are equal, say
$x_a = x_b$ with $a<b$. Applying $f$ repeatedly gives $x_{a+t} = x_{b+t}$ for all
$t \ge 0$. $\blacksquare$

Drawn, the tail leads into the loop and the picture is the letter $\rho$ — which is where
the algorithm gets its name. **Justifies** cycle detection: something must repeat, so it is
only a question of noticing when.

---

## 4. Why a collision comes after about $\sqrt p$ steps

**The birthday bound.** Among $k$ values drawn from a set of size $p$ there are
$\binom k2 \approx k^2/2$ pairs, each colliding with chance $1/p$, so a collision becomes
likely once $k^2/2 \approx p$, i.e. $k \approx \sqrt p$. For a **uniformly random** map on
$p$ points, the expected tail-plus-cycle length is exactly

$$\sqrt{\pi p/8} + \sqrt{\pi p / 8} \;=\; \sqrt{\pi p/2} \;\approx\; 1.25\sqrt p$$

(Flajolet–Odlyzko; cited, not derived here.)

**Applied to rho.** Run the iteration modulo $n$, but *read* it modulo $p$: since
$f(x) = x^2+c$ commutes with reduction, $x_i \bmod p$ is the same iteration on
$\mathbb{Z}/p$. It collides after $\Theta(\sqrt p)$ steps, and the smallest prime factor
satisfies $p \le \sqrt n$ — so the expected work is

$$O\bigl(\sqrt p\bigr) = O\bigl(n^{1/4}\bigr)$$

$10^{18} \to$ about $31{,}600$ steps, against $10^9$ for trial division.

> **Be honest about what is proved here.** That $x^2+c$ behaves like a random map is a
> *heuristic*. Nobody has proved a worst-case bound for Pollard's rho; the $n^{1/4}$ is an
> expectation under a model, backed by sixty years of it working. The **correctness** of
> the algorithm (§2) is unconditional — only the running time rests on the heuristic.

---

## 5. Cycle detection: Floyd and Brent

**Floyd.** Advance $x$ one step and $y$ two steps. Once $i \ge \lambda$ and
$\mu \mid i$, we have $x_i = x_{2i}$; such an $i$ exists below $\lambda + \mu$, so the
meeting happens within $O(\lambda+\mu)$ steps — three evaluations of $f$ per step.

**Brent.** Hold a checkpoint $x = x_{2^k}$ and compare it against the next $2^k$ values of
$y$. Once $2^k \ge \max(\lambda, \mu)$ the window
$(2^k,\ 2^{k+1}]$ contains a full period starting past the tail, so it contains an index
$j$ with $x_j = x_{2^k}$.

**Why Brent is used here.** Same $O(\lambda+\mu)$ guarantee, but **one** evaluation of $f$
per step instead of three, and the checkpoint is a fixed value — which is exactly what
lets the differences be batched in §6.

---

## 6. Batching the gcds is exact

A `gcd` costs a loop of divisions; a `mulMod` costs one multiply. So instead of one gcd per
step, multiply $m = 128$ differences together and take a single gcd. This is safe:

**Claim.** Let $d_1,\dots,d_k$ be non-zero residues and $q \equiv \prod_i d_i \pmod n$.
Then $\gcd(q,n) > 1$ **if and only if** $\gcd(d_i,n) > 1$ for some $i$.

**Proof.** ($\Leftarrow$) Let a prime $p$ divide both $d_i$ and $n$. Then
$p \mid \prod_i d_i$, and since $\prod_i d_i = q + tn$ with $p \mid n$, also $p \mid q$.
Hence $p \mid \gcd(q,n)$.
($\Rightarrow$) Let a prime $p$ divide $\gcd(q,n)$. From $p \mid n$ and $p \mid q$ we get
$p \mid \prod_i d_i$, and by **Euclid's lemma** $p$ divides one of the factors $d_i$.
$\blacksquare$

So batching never *misses* a factor. What it can do is find **too much**: if two different
primes of $n$ enter the same batch, or some $d_i \equiv 0$, the gcd comes back as $n$ and
the individual culprit is lost. That is why the code saves `ys` — the start of the batch —
and replays it one step at a time when $g = n$. The replay is the slow path, entered at
most once per successful split.

---

## 7. When it fails, and why retrying works

$g = n$ even after the step-by-step replay means the sequence collided modulo **every**
prime factor of $n$ at the same step. The start $x_0$ and the constant $c$ are then useless
— but a fresh pair gives a different sequence, so the retry loop is not a repetition of the
same failure.

**Two constants must be avoided.**

- $c = 0$ makes $f(x) = x^2$, so $x_i = x_0^{\,2^{i}}$; the cycle length is the
  multiplicative order of $2$ modulo $\mathrm{ord}(x_0)$, which is around $\log p$, not
  $\sqrt p$. Far too short to collide usefully.
- $c = -2$ is conjugate to Chebyshev/Lucas doubling ($x = t + t^{-1} \Rightarrow
  f(x) = t^2 + t^{-2}$) and degenerates the same way. (Both cited.)

**And $n = 4$ cannot be split at all.** Exhaustive check over every $c$ and every $x_0$
modulo $4$: no pair ever produces $\gcd(|x-y|,4) = 2$. This is not a rounding of "rare" —
it is zero. **Justifies** the first line of `pollardRho`, which returns $2$ for even $n$
before the iteration can run, and the trial division of small primes in `factorize`.

---

## 8. The recursion is correct and terminates

**Claim.** `factorRec` returns the canonical factorization of $n$.

**Proof.** Strong induction on $n$. If $n=1$ there is nothing to emit. If Miller–Rabin says
$n$ is prime, that is *certain* below $2^{64}$ (Miller–Rabin §4), so emitting $n$ is right.
Otherwise $n$ is composite and `pollardRho` returns $d$ with $1 < d < n$; both $d$ and
$n/d$ are strictly smaller, so by induction each is factored correctly, and by **unique
factorization** the multiset union of the two is *the* factorization of $n$.
$\blacksquare$

**Termination.** Every recursive call has a strictly smaller positive argument, so the
depth is at most $\log_2 n$. There are at most $\log_2 n$ prime factors with multiplicity,
so at most that many splits; the cost is dominated by the hardest single split, giving
$O(n^{1/4}\log n)$ expected overall.

**Note what the base case is doing.** Without a primality test the recursion cannot stop —
it would keep calling rho on a prime, which never returns. Miller–Rabin is not an
optimisation here; it is what makes the algorithm well defined.

---

## 9. Why `__int128` is not optional

Identical to Miller–Rabin §5, and now needed in one more place: the batch product
$q \leftarrow q\,|x-y| \bmod n$ multiplies two numbers that are each up to $n-1$. With
$n$ near $9\times10^{18}$ the product reaches $8\times10^{37} \approx 2^{126}$, so
plain `long long` wraps and $q$ becomes a wrong residue — after which every `gcd(q, n)` is
meaningless and the factorization silently returns garbage or hangs.
