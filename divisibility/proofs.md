# Proofs — Divisibility

Every proof here has the same shape: unpack the definition into a witness $k$, do algebra,
repack. Divisibility is defined by multiplication, so its proofs never divide.

---

## 1. Linearity

**Claim.** $d \mid a$ and $d \mid b$ $\Rightarrow$ $d \mid (ax+by)$ for all integers $x,y$.

**Proof.** $a = dm$ and $b = dn$, so $ax + by = d(mx+ny)$. $\blacksquare$

**The form that becomes the Euclidean algorithm.** If $d \mid b$, then

$$d \mid a \quad\Longleftrightarrow\quad d \mid (a - qb)$$

— the claim applied with coefficients $(1,-q)$ and then $(1,q)$. Subtracting a multiple of
$b$ changes **nothing** about which numbers divide both, which is why the step can be
repeated forever.

---

## 2. The division algorithm

**Claim.** For $b \neq 0$ there are **unique** $q,r$ with $a = qb + r$ and $0 \le r < |b|$.

**Existence.** Among the values $a - qb$ that are $\ge 0$, take the smallest, $r$. If
$r \ge |b|$ then $r - |b|$ is still $\ge 0$ and still of that form, but smaller —
contradicting minimality. $\blacksquare$

**Uniqueness.** If $q_1b+r_1 = q_2b+r_2$ then $|q_1-q_2|\,|b| = |r_2-r_1| < |b|$, since
both remainders lie in a window of width $|b|$. So $|q_1-q_2| < 1$, forcing $q_1=q_2$ and
then $r_1=r_2$. $\blacksquare$

**Justifies `b % a == 0`.** If $a \mid b$ then $b = ak+0$ is *a* valid decomposition;
uniqueness makes it *the* one, so the remainder really is $0$ and the test is an
if-and-only-if.

---

## 3. The $O(\sqrt n)$ divisor loop finds everything

**Claim.** If $d \mid n$ then $n/d \mid n$, and $\min(d,\ n/d) \le \sqrt n$.

**Proof.** $n = (n/d)\cdot d$ exhibits $n/d$ as a divisor. If both $d > \sqrt n$ and
$n/d > \sqrt n$, multiplying gives $n = d\cdot\frac nd > n$ — absurd. $\blacksquare$

**Justifies** emitting $d$ and $n/d$ while looping to $\sqrt n$: every divisor is either
found directly or is the partner of one. The `d != n/d` guard exists because for a perfect
square the pair collapses to a single divisor.

---

## 4. $\tau(n)$ odd $\iff n$ is a perfect square

**Claim.** As stated.

**Proof.** Pair each divisor $d$ with $n/d$. Each pair contributes $2$, so

$$\tau(n) \equiv \#\{\text{self-paired divisors}\} \pmod 2$$

and $d$ is self-paired iff $d = n/d$ iff $n = d^2$. A non-square has none; a square has
exactly one, namely $\sqrt n$. $\blacksquare$

**Justifies the locker problem.** Cell $m$ is toggled once per divisor, so it ends flipped
iff $\tau(m)$ is odd iff $m$ is a square. Answer $\lfloor\sqrt n\rfloor$, no simulation.

---

## 5. Why the sieve is $O(n\log n)$, not $O(n^2)$

**Claim.** $\sum_{d=1}^{n}\lfloor n/d\rfloor = \Theta(n\log n)$.

**Proof.** The sum is $\approx n H_n$ with $H_n = 1+\frac12+\cdots+\frac1n$. Group the
terms into blocks $[2^{\,j},\ 2^{\,j+1})$: block $j$ holds $2^{\,j}$ terms, each between
$\frac{1}{2^{\,j+1}}$ and $\frac{1}{2^{\,j}}$, so each block sums to between $\frac12$ and
$1$. There are about $\log_2 n$ blocks, hence

$$\frac{\log_2 n}{2} \ \le\ H_n \ \le\ \log_2 n + 1 \qquad\Longrightarrow\qquad H_n = \Theta(\log n) \quad\blacksquare$$

**The idea:** blocks double in length while their terms halve, so each contributes a
constant; the *number* of blocks is what is logarithmic.

**Justifies** the multiples skeleton up to about $n = 10^7$. It also says the **average**
divisor count below $n$ is $\approx \ln n$ — only about $12$ at $n = 10^5$.

---

## 6. Digit tests

**Claim.** With digit sum $S$ and alternating digit sum $A = \sum_k (-1)^k d_k$:

$$9 \mid n \iff 9 \mid S, \qquad 3 \mid n \iff 3 \mid S, \qquad 11 \mid n \iff 11 \mid A$$

**Proof for 9.** Every $10^k-1$ is a string of nines, so $9 \mid (10^k-1)$. Then

$$n - S = \sum_k d_k 10^k - \sum_k d_k = \sum_k d_k\bigl(10^k-1\bigr)$$

is a sum of multiples of $9$, so $9 \mid (n-S)$ by linearity. Hence $9 \mid n$ gives
$9 \mid \bigl(n-(n-S)\bigr) = S$, and $9 \mid S$ gives $9 \mid \bigl(S+(n-S)\bigr) = n$.
Since $3 \mid 9$, the same runs for $3$. $\blacksquare$

**For 11**, use $11 \mid \bigl(10^k - (-1)^k\bigr)$ — a difference of $k$-th powers of $10$
and $-1$, so $10-(-1) = 11$ divides it. Then $n - A = \sum_k d_k\bigl(10^k-(-1)^k\bigr)$
and the same two lines finish it. $\blacksquare$

**The recipe.** Any weight $c$ with $d \mid (10-c)$ gives a test — which is why $7$ has no
clean base-10 rule.

**Justifies** testing a $10^5$-digit `std::string` without ever forming the integer.
