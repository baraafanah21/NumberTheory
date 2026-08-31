# Proofs — Progressions

---

## 1. Sum of an AP

**Claim.** $\ S_n = \dfrac{n}{2}\bigl(2a+(n-1)d\bigr) = \dfrac{n}{2}(a_1+a_n)$

**Proof.** Write the sum forwards and backwards and add columns. The $k$-th column is

$$a_k + a_{n+1-k} = \bigl[a+(k-1)d\bigr] + \bigl[a+(n-k)d\bigr] = 2a+(n-1)d$$

The $k$ cancels — moving right adds $d$, moving left subtracts it — so all $n$ columns are
equal and $2S_n = n\bigl(2a+(n-1)d\bigr)$. $\blacksquare$

**Remember it as** *number of terms × average of the ends*. Setting $a=d=1$ gives
$1+2+\cdots+n = \frac{n(n+1)}{2}$.

---

## 2. The AP sum stays an exact integer

**Claim.** Exactly one of $n$ and $\bigl(2a+(n-1)d\bigr)$ is even.

**Proof.** $2a$ is even, so the bracket has the parity of $(n-1)d$. If $n$ is odd then
$n-1$ is even, making the bracket even; if $n$ is even then $n$ is. $\blacksquare$

**Justifies** halving the even factor. `n/2 * bracket` truncates for odd $n$;
`n * bracket / 2` can overflow first.

---

## 3. Sum of a GP

**Claim.** $\ S_n = a\dfrac{r^{\,n}-1}{r-1}$ for $r \neq 1$; $\ S_n = na$ for $r=1$.

**Proof.** Multiplying by $r$ shifts every term one place, so subtracting cancels
everything but the ends — the sum **telescopes**:

$$
\begin{aligned}
rS_n &= \phantom{a+{}} ar + ar^2 + \cdots + ar^{\,n-1} + ar^{\,n}\\
S_n  &= a + ar + ar^2 + \cdots + ar^{\,n-1}
\end{aligned}
\qquad\Longrightarrow\qquad S_n(r-1) = a\bigl(r^{\,n}-1\bigr)$$

Divide when $r \neq 1$. $\blacksquare$

**The $r=1$ case is real, not defensive.** The last line becomes $0=0$ and carries no
information; instead every term equals $a$, so $S_n = na$.

**Two corollaries.** $\sigma(p^k) = 1+p+\cdots+p^k = \frac{p^{k+1}-1}{p-1}$, and
$1+2+4+\cdots+2^k = 2^{k+1}-1 < 2\cdot2^k$ — a doubling sum is dominated by its last term,
which is why amortized doubling costs $O(n)$.

---

## 4. $(a-b) \mid (a^n - b^n)$

**Claim.** $\ a^n - b^n = (a-b)\displaystyle\sum_{i=0}^{n-1} a^i b^{\,n-1-i}$

**Proof.** Call the sum $T$. Expanding $(a-b)T$ gives two sums of the same terms
$a^k b^{\,n-k}$, one over $k=1\dots n$ and one over $k=0\dots n-1$. Everything in
$1\dots n-1$ cancels, leaving $a^nb^0 - a^0b^n$. And $T$ is a sum of products of integers,
hence an integer. $\blacksquare$

**Check.** $a=5,b=2,n=3$: $T = 25+10+4 = 39$ and $3\cdot39 = 117 = 125-8$. ✓

**Justifies (Mersenne).** If $n = de$ then $2^n-1 = (2^d)^e - 1^e$, so
$(2^d-1) \mid (2^n-1)$. For composite $n$ pick $1<d<n$: then $1 < 2^d-1 < 2^n-1$, a proper
factor. So check whether $n$ is composite *before* primality-testing $2^n-1$.

One direction only — $2^{11}-1 = 2047 = 23\times89$ despite $11$ being prime.

---

## 5. Geometric sums mod $m$, division-free

**Claim.** With $G(n) = \sum_{i=0}^{n-1} r^i$:
$\ G(2k) = G(k)\bigl(1+r^{\,k}\bigr)$ and $G(n) = G(n-1) + r^{\,n-1}$.

**Proof.** Split the $2k$ terms in half. Substituting $i = k+j$ in the second half:

$$\sum_{i=k}^{2k-1} r^i = \sum_{j=0}^{k-1} r^{\,k+j} = r^{\,k}\sum_{j=0}^{k-1} r^{\,j} = r^{\,k}G(k)$$

so $G(2k) = G(k) + r^{\,k}G(k)$. The odd case peels off the last term. $\blacksquare$

**Justifies** using this instead of the closed form. Dividing by $r-1$ under a modulus
means multiplying by $(r-1)^{-1}$, which **exists only when $\gcd(r-1,m)=1$** — e.g. mod
$9$ with $r=4$, $\gcd(3,9)=3$ and there is no inverse. This recursion uses only $+$ and
$\times$.

**Cost.** The even rule halves $n$, the odd rule makes $n$ even — $O(\log n)$ calls, each
computing one power: $O(\log^2 n)$.
