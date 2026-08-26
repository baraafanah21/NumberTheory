# Proofs — Progressions

Five results. Each one: the claim, what it means in plain words, the proof, and why it
matters when you write code.

**Notation.** $a$ = first term, $n$ = number of terms, $S_n$ = sum of the first $n$ terms,
$d$ = common difference (AP), $r$ = common ratio (GP).

---

## 1. The sum of an arithmetic progression

**Claim.**

$$S_n \;=\; \frac{n}{2}\Bigl(2a + (n-1)d\Bigr) \;=\; \frac{n}{2}\bigl(a_1 + a_n\bigr)$$

**In words.** The sum equals *how many terms* times *the average of the first and last
term*. If you remember only that sentence, you can rebuild both formulas.

**Proof.** Write the sum forwards, then backwards underneath it:

$$
\begin{aligned}
S_n &= a_1 \;+\; a_2 \;+\; \cdots \;+\; a_{n-1} \;+\; a_n\\
S_n &= a_n \;+\; a_{n-1} \;+\; \cdots \;+\; a_2 \;+\; a_1
\end{aligned}
$$

Add the two lines **column by column**. The $k$-th column is $a_k + a_{n+1-k}$, and using
$a_k = a + (k-1)d$:

$$a_k + a_{n+1-k} \;=\; \bigl[a + (k-1)d\bigr] + \bigl[a + (n-k)d\bigr] \;=\; 2a + (n-1)d$$

The $k$ **cancels**, so every column has the same value. There are $n$ columns, so

$$2S_n = n\bigl(2a + (n-1)d\bigr) \qquad\Longrightarrow\qquad S_n = \frac{n}{2}\bigl(2a+(n-1)d\bigr) \qquad \blacksquare$$

**Why the $k$ cancels:** moving one step right along the top row *adds* $d$, and one step
left along the bottom row *subtracts* $d$. The two changes destroy each other.

**Setting $a = d = 1$** gives the formula everyone uses:

$$1 + 2 + \cdots + n = \frac{n(n+1)}{2}$$

**In code.** This is `apSum`. Without it, summing an AP with $n = 10^{18}$ is impossible.

---

## 2. Why the AP sum stays an exact integer

**Claim.** Exactly one of $n$ and $\bigl(2a + (n-1)d\bigr)$ is even.

**In words.** $S_n$ has a $\div 2$ in it, but the answer is always a whole number — and you
can always find a factor that divides evenly *before* multiplying.

**Proof.** $2a$ is even, so $2a + (n-1)d$ has the same parity as $(n-1)d$.

- If $n$ is **odd**, then $n - 1$ is even, so $(n-1)d$ is even, so the bracket is even.
- If $n$ is **even**, then $n$ itself is the even factor.

Either way one of the two factors is even, so their product is divisible by $2$. $\blacksquare$

**Why it matters in code.** Integer division truncates. Writing

```cpp
return n/2 * bracket;        // WRONG when n is odd: n/2 loses the .5
return n * bracket / 2;      // right, but n*bracket may overflow first
```

The fix is to test which factor is even and halve *that* one:

```cpp
return (n % 2 == 0) ? (n/2) * bracket : n * (bracket/2);
```

Exact, and it halves one factor before multiplying, buying a bit of headroom.

---

## 3. The sum of a geometric progression

**Claim.**

$$S_n = a\cdot\frac{r^{\,n}-1}{r-1} \quad (r \neq 1), \qquad\qquad S_n = na \quad (r = 1)$$

**In words.** Multiplying the sum by $r$ shifts every term one position along. Subtracting
the original then cancels almost everything — only the two ends survive. This is called
**telescoping**.

**Proof.** Start with $S_n = a + ar + ar^2 + \cdots + ar^{\,n-1}$ and multiply by $r$:

$$
\begin{aligned}
r\,S_n &= \phantom{a + {}} ar + ar^2 + \cdots + ar^{\,n-1} + ar^{\,n}\\
S_n &= a + ar + ar^2 + \cdots + ar^{\,n-1}
\end{aligned}
$$

Every term $ar^i$ with $1 \le i \le n-1$ appears in **both** rows, so subtracting kills it.
Only $ar^{\,n}$ (top row only) and $a$ (bottom row only) remain:

$$r S_n - S_n = a r^{\,n} - a \qquad\Longrightarrow\qquad S_n\,(r-1) = a\bigl(r^{\,n}-1\bigr)$$

If $r \neq 1$ we may divide by $r - 1$, giving the formula. $\blacksquare$

**The $r = 1$ case.** The line $S_n(r-1) = a(r^n-1)$ becomes $0 = 0$ — true, but it tells
us nothing, and dividing is illegal. So handle it separately: if $r = 1$ every term equals
$a$, hence $S_n = na$.

**In code.** The `if (r == 1)` branch is not defensive programming — the derivation
genuinely fails there.

**Two consequences worth knowing:**

$$\sigma(p^k) = 1 + p + \cdots + p^k = \frac{p^{\,k+1}-1}{p-1} \qquad\text{(sum of divisors of a prime power)}$$

$$1 + 2 + 4 + \cdots + 2^k = 2^{\,k+1} - 1 \;<\; 2\cdot 2^k \qquad\text{(a doubling sum is dominated by its last term)}$$

The second explains why a doubling array costs $O(n)$ total, and why $n + \frac n2 + \frac n4 + \cdots < 2n$.

---

## 4. $(a-b)$ always divides $(a^n - b^n)$

**Claim.**

$$a^n - b^n \;=\; (a-b)\sum_{i=0}^{n-1} a^i\,b^{\,n-1-i} \qquad\Longrightarrow\qquad (a-b) \mid (a^n - b^n)$$

**In words.** A difference of two $n$-th powers can *always* be factored, and the identity
even tells you the other factor.

**Proof.** Call the sum $T = \sum_{i=0}^{n-1} a^i b^{\,n-1-i}$ and multiply it out:

$$(a-b)\,T \;=\; \sum_{i=0}^{n-1} a^{\,i+1}b^{\,n-1-i} \;-\; \sum_{i=0}^{n-1} a^{\,i}b^{\,n-i}$$

Both sums contain the same kind of term, $a^k b^{\,n-k}$ — the first for $k = 1,\dots,n$,
the second for $k = 0,\dots,n-1$. Every $k$ from $1$ to $n-1$ appears in **both** and
cancels. What survives is the top end of the first sum minus the bottom end of the second:

$$(a-b)\,T = a^n b^0 - a^0 b^n = a^n - b^n$$

Since $a$ and $b$ are integers, $T$ is a sum of products of integers, so $T$ is an integer.
That is exactly what $(a-b) \mid (a^n-b^n)$ means. $\blacksquare$

**Check it.** $a = 5,\ b = 2,\ n = 3$: $\ T = 5^2 + 5\cdot2 + 2^2 = 39$, and
$(5-2)\cdot 39 = 117 = 125 - 8$. ✓

**Consequence (Mersenne numbers).** If $d \mid n$, write $n = de$; then

$$2^n - 1 = \bigl(2^d\bigr)^e - 1^e$$

so $(2^d - 1) \mid (2^n - 1)$. If $n$ is composite, choose $d$ with $1 < d < n$: then
$1 < 2^d - 1 < 2^n - 1$, a **proper** factor, so $2^n - 1$ is composite.

**In code.** Before primality-testing $2^n - 1$, check whether $n$ is composite — it settles
the question instantly. Careful: this is one-directional. $n$ prime does *not* make
$2^n-1$ prime ($2^{11} - 1 = 2047 = 23 \times 89$).

---

## 5. Geometric sums modulo $m$, without division

**Claim.** Let $G(n) = 1 + r + \cdots + r^{\,n-1}$. Then

$$G(2k) = G(k)\cdot\bigl(1 + r^{\,k}\bigr), \qquad\qquad G(n) = G(n-1) + r^{\,n-1}$$

**In words.** Split the sum into two halves. The second half is just the first half
multiplied by $r^{\,k}$ — so you only need to compute one of them.

**Proof.** Break the $2k$ terms into the first $k$ and the last $k$:

$$G(2k) \;=\; \underbrace{\sum_{i=0}^{k-1} r^i}_{G(k)} \;+\; \sum_{i=k}^{2k-1} r^i$$

In the second sum put $i = k + j$, so $j$ runs $0$ to $k-1$:

$$\sum_{i=k}^{2k-1} r^i \;=\; \sum_{j=0}^{k-1} r^{\,k+j} \;=\; r^{\,k}\sum_{j=0}^{k-1} r^{\,j} \;=\; r^{\,k}\,G(k)$$

So $G(2k) = G(k) + r^{\,k}G(k) = G(k)\bigl(1 + r^{\,k}\bigr)$. The odd case is just peeling
off the last term. $\blacksquare$

**Why you need this instead of the closed form.** The formula $\frac{r^n-1}{r-1}$ requires
**dividing** by $r-1$. Under a modulus, dividing means multiplying by the inverse
$(r-1)^{-1}$, and that inverse **only exists when $\gcd(r-1,\ m) = 1$**. For example mod
$9$ with $r = 4$: $\gcd(3, 9) = 3$, so no inverse exists and the closed form is unusable.

The recursion above uses only $+$ and $\times$, so it is correct for **every** modulus —
including the composite ones chosen deliberately to break the naive approach.

**Cost.** The even rule halves $n$; the odd rule makes $n$ even so the even rule fires next.
So $O(\log n)$ calls, each computing one power in $O(\log n)$: total $O(\log^2 n)$.
