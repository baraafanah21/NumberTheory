# Proofs — Divisibility

Six results. Each one: the claim, what it means, the proof, and why it matters in code.

**Notation.** $a \mid b$ means $b = ak$ for some integer $k$. $\tau(n)$ is the number of
positive divisors of $n$.

**The proof technique for this whole file:** unpack the definition into a witness $k$, do
algebra, repack. Divisibility is defined by multiplication, so its proofs never divide.

---

## 1. Linearity

**Claim.**

$$d \mid a \ \text{ and } \ d \mid b \qquad\Longrightarrow\qquad d \mid (a\,x + b\,y) \quad \text{for all integers } x, y$$

**In words.** Whatever divides two numbers divides every combination you can build from
them by multiplying and adding.

**Proof.** By definition there are integers $m, n$ with $a = dm$ and $b = dn$. Then

$$a\,x + b\,y = (dm)x + (dn)y = d\,(mx + ny)$$

and $mx + ny$ is an integer, so $d$ divides the whole thing. $\blacksquare$

**Special cases you will use constantly:** $d \mid (a+b)$, $d \mid (a-b)$, $d \mid ac$.

**The version that becomes the Euclidean algorithm.** If $d \mid b$, then

$$d \mid a \quad\Longleftrightarrow\quad d \mid (a - qb)$$

Both directions are the claim above, once with coefficients $(1, -q)$ and once with
$(1, q)$. So subtracting a multiple of $b$ from $a$ changes **nothing** about which numbers
divide both — which is exactly why the Euclidean algorithm is allowed to do it over and
over.

---

## 2. The division algorithm, and why `b % a == 0` is a valid test

**Claim.** For $b \neq 0$ there are **unique** integers $q, r$ with

$$a = q\,b + r, \qquad 0 \le r < |b|$$

**In words.** Every division leaves a remainder, that remainder is smaller than the
divisor, and there is only one possible answer.

**Proof of existence.** Among all values $a - qb$ that are $\ge 0$, pick the smallest one;
call it $r$ (such values exist, and a non-empty set of non-negative integers always has a
least element). Suppose $r \ge |b|$. Then $r - |b|$ is still $\ge 0$ and is still of the
form $a - q'b$ — but it is *smaller* than $r$, contradicting that $r$ was smallest. So
$r < |b|$. $\blacksquare$

**Proof of uniqueness.** Suppose $q_1b + r_1 = q_2b + r_2$ with both remainders in
$[0, |b|)$. Rearranging and taking absolute values:

$$|q_1 - q_2|\cdot|b| = |r_2 - r_1|$$

Two numbers in a window of width $|b|$ differ by less than $|b|$, so
$|q_1-q_2|\cdot|b| < |b|$, giving $|q_1 - q_2| < 1$. A non-negative integer below $1$ is
$0$, so $q_1 = q_2$, and then $r_1 = r_2$. $\blacksquare$

**Why uniqueness is the part that matters.** If $a \mid b$ then $b = ak + 0$ is *a* valid
decomposition. Uniqueness says it is *the* decomposition — so the remainder really is $0$,
and `b % a == 0` is an if-and-only-if test rather than a one-way check.

**In code.** C++ returns a remainder with the sign of the dividend, so `-17 % 5 == -2`
rather than $3$. Normalize with `((a % b) + b) % b` before using it as an index or
comparing against a range.

---

## 3. Why looping to $\sqrt n$ finds every divisor

**Claim.** If $d \mid n$ then $\frac nd \mid n$, and

$$\min\!\left(d,\ \frac{n}{d}\right) \;\le\; \sqrt{n}$$

**In words.** Divisors come in pairs that multiply to $n$. In each pair, one member is at
most $\sqrt n$ — so walking up to $\sqrt n$ meets every pair.

**Proof.** First, $n/d$ is a divisor: since $d \mid n$ we can write $n = d\cdot\frac nd$,
which exhibits $n/d$ as a factor.

Now suppose, for contradiction, that **both** $d > \sqrt n$ and $\frac nd > \sqrt n$.
Multiplying two inequalities between positive numbers:

$$n = d\cdot\frac{n}{d} \;>\; \sqrt n \cdot \sqrt n = n$$

which says $n > n$ — impossible. So at least one of the pair is $\le \sqrt n$. $\blacksquare$

**In code.** Every divisor is therefore either found directly as some $d \le \sqrt n$, or
emitted as the partner $n/d$ of one:

```cpp
for (ll d = 1; d <= n / d; ++d)
    if (n % d == 0) { use(d); if (d != n / d) use(n / d); }
```

The `d != n / d` guard exists because when $n$ is a perfect square the pair collapses to a
single divisor, which would otherwise be counted twice. And the loop condition is
`d <= n / d`, not `d * d <= n` — the latter overflows for large $n$.

---

## 4. $\tau(n)$ is odd exactly when $n$ is a perfect square

**Claim.** $\tau(n)$ is odd $\iff n = m^2$ for some integer $m$.

**In words.** Since divisors pair up, they come two at a time — so the total is even unless
one divisor is paired with *itself*.

**Proof.** Pair each divisor $d$ with $n/d$. Every pair contributes $2$ to the count, so

$$\tau(n) \ \equiv\ \#\{\text{divisors paired with themselves}\} \pmod 2$$

A divisor is paired with itself when

$$d = \frac{n}{d} \quad\Longleftrightarrow\quad d^2 = n$$

- If $n$ is **not** a perfect square there is no such $d$, every divisor is in a genuine
  pair, and $\tau(n)$ is even.
- If $n = m^2$ then $d = m$ works, and it is the only one (a positive number has one
  positive square root). So exactly one self-pair, and $\tau(n)$ is odd. $\blacksquare$

**In code — the classic problem.** *"$n$ lockers, all closed. Pass $k$ toggles every $k$-th
locker. Which are open at the end?"* Locker $m$ is toggled once per divisor of $m$, so it
ends open exactly when $\tau(m)$ is odd — that is, when $m$ is a perfect square. The answer
is $\lfloor\sqrt n\rfloor$, with no simulation at all.

---

## 5. Why the sieve is $O(n\log n)$ and not $O(n^2)$

**Claim.** The nested loop

```cpp
for (int d = 1; d <= n; ++d)
    for (int m = d; m <= n; m += d) ...
```

runs $\Theta(n\log n)$ times in total.

**In words.** It *looks* like two loops to $n$, but the inner one only runs $n/d$ times,
and those shrink fast.

**Proof.** The total number of steps is

$$\sum_{d=1}^{n}\left\lfloor\frac{n}{d}\right\rfloor \;\approx\; n\sum_{d=1}^{n}\frac{1}{d} \;=\; n\,H_n$$

so everything depends on the **harmonic number** $H_n = 1 + \frac12 + \cdots + \frac1n$.

Group the terms into blocks by powers of two: block $j$ holds the indices $d$ with
$2^{\,j} \le d < 2^{\,j+1}$. That block has $2^{\,j}$ terms, each of size between
$\frac{1}{2^{\,j+1}}$ and $\frac{1}{2^{\,j}}$. So each block sums to between

$$2^{\,j}\cdot\frac{1}{2^{\,j+1}} = \frac12 \qquad\text{and}\qquad 2^{\,j}\cdot\frac{1}{2^{\,j}} = 1$$

There are about $\log_2 n$ such blocks, so

$$\frac{\log_2 n}{2} \;\le\; H_n \;\le\; \log_2 n + 1 \qquad\Longrightarrow\qquad H_n = \Theta(\log n) \qquad \blacksquare$$

**Key idea.** Blocks double in length while their terms halve in size, so each block
contributes a constant. The count of blocks is logarithmic — that is the whole reason.

**In code.** The sieve is safe up to about $n = 10^7$. As a bonus, the same sum says the
**average** number of divisors below $n$ is about $\ln n$ — only about $12$ at $n = 10^5$,
so divisors are rare and storing them per number is cheap.

---

## 6. Digit tests

**Claim.** Let $n = \sum_k d_k 10^k$ have digit sum $S$ and alternating digit sum
$A = \sum_k (-1)^k d_k$. Then

$$9 \mid n \iff 9 \mid S, \qquad 3 \mid n \iff 3 \mid S, \qquad 11 \mid n \iff 11 \mid A$$

**In words.** $10$ is one more than $9$, so every power of $10$ is one more than a multiple
of $9$. Replacing each $10^k$ by $1$ therefore changes $n$ only by a multiple of $9$.

**Proof for 9.** Every $10^k - 1$ is a string of nines, hence a multiple of $9$:

$$10^k - 1 = \underbrace{99\cdots9}_{k \text{ nines}} = 9 \times \underbrace{11\cdots1}_{k}$$

Now subtract the digit sum from the number, term by term:

$$n - S \;=\; \sum_k d_k 10^k - \sum_k d_k \;=\; \sum_k d_k\bigl(10^k - 1\bigr)$$

Every summand is a multiple of $9$, so by **linearity** (§1), $9 \mid (n - S)$. Then:

- if $9 \mid n$, then $9 \mid \bigl(n - (n-S)\bigr) = S$;
- if $9 \mid S$, then $9 \mid \bigl(S + (n-S)\bigr) = n$.

So the two are equivalent. Since $3 \mid 9$, the same argument works for $3$. $\blacksquare$

**Proof for 11.** Here $10 \equiv -1$, so we compare $10^k$ with $(-1)^k$. Each
$10^k - (-1)^k$ is divisible by $11$ (it is a difference of $k$-th powers of $10$ and $-1$,
so $10 - (-1) = 11$ divides it — see the progressions concept). Then

$$n - A = \sum_k d_k\bigl(10^k - (-1)^k\bigr)$$

is a multiple of $11$, and the same two-line argument finishes it. $\blacksquare$

**The general recipe.** Any weight $c$ with $d \mid (10 - c)$ gives a test. That is why $7$
has no clean base-10 rule — no small $c$ works.

**In code.** This lets you test a $10^5$-digit number held as a `std::string`, without ever
forming the integer.
