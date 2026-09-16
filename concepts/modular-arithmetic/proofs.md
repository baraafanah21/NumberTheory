# Proofs — Modular Arithmetic

**Borrowed** from [divisibility](../divisibility/proofs.md): **linearity** ($d \mid a$ and
$d \mid b$ $\Rightarrow$ $d \mid ax+by$, §1) and the **division algorithm** (unique $q,r$
with $a = qb+r$ and $0 \le r < |b|$, §2). From [gcd](../gcd/proofs.md):
**Euclid's lemma** (§4), needed once, in §5.

Everything here unpacks the same definition:

$$a \equiv b \pmod m \quad\Longleftrightarrow\quad m \mid (a-b)$$

Note what that does **not** mention: the `%` operator. Congruence is a statement about
divisibility; `%` is one way to compute a representative of a class. Keeping the two apart
is what makes the negative-remainder trap a non-event rather than a bug.

---

## 1. Congruence is an equivalence relation

**Claim.** For fixed $m > 0$, $\equiv \pmod m$ is reflexive, symmetric and transitive.

**Proof.** *Reflexive:* $m \mid 0 = a - a$. *Symmetric:* if $m \mid (a-b)$ then
$m \mid -(a-b) = b-a$, by linearity with coefficient $-1$. *Transitive:* if $m \mid (a-b)$
and $m \mid (b-c)$ then linearity gives $m$ dividing their sum,

$$(a-b) + (b-c) = a-c \qquad \blacksquare$$

**Why this is worth stating.** It licenses the word *"the same"*. Congruence carves
$\mathbb{Z}$ into disjoint classes, so "reduce $a$ mod $m$" means "replace $a$ by another
member of its class", and §3 says the arithmetic cannot tell the difference. Without
transitivity, reducing twice in a chain of operations would need a separate argument each
time.

---

## 2. Every integer has exactly one representative in $[0,m)$

**Claim.** For $m > 0$ and any $a$, there is exactly one $r$ with $0 \le r < m$ and
$a \equiv r \pmod m$. Hence there are exactly $m$ classes.

**Proof.** The division algorithm gives unique $q,r$ with $a = qm + r$ and $0 \le r < m$.
Then $a - r = qm$, so $a \equiv r$. If $r'$ also qualified, then $m \mid (r - r')$ while
$|r - r'| < m$, forcing $r = r'$. $\blacksquare$

**What C++ actually returns.** `%` truncates toward zero, so it follows the sign of the
dividend: `-17 % 5 == -2`. What it guarantees is weaker than the claim above —

$$x = \mathtt{a\ \%\ m} \quad\Longrightarrow\quad x \equiv a \pmod m \ \text{ and } \ -m < x < m$$

— a representative, but from the window $(-m, m)$, which holds two per class. If $x < 0$
then $0 < x + m < m$, so $x+m$ lands in $[0,m)$ and by uniqueness it **is** the residue.

**Justifies `norm`** ([implementation.cpp:15](implementation.cpp#L15)) in exactly its
written form: one `%`, then a single conditional `+ m`. Nothing more is needed, because the
C++ result was never more than one step out. The reason to care is that a negative
representative is harmless in a comparison and fatal as an array index or a hash key.

---

## 3. Congruence survives $+$, $-$ and $\times$

**Claim.** If $a \equiv b$ and $c \equiv d \pmod m$, then

$$a+c \equiv b+d, \qquad a-c \equiv b-d, \qquad ac \equiv bd \pmod m$$

**Proof.** Write $a = b + km$ and $c = d + \ell m$. For the sum and difference,

$$(a \pm c) - (b \pm d) = (k \pm \ell)m$$

For the product, multiply out:

$$ac = (b+km)(d+\ell m) = bd + m\,(b\ell + dk + k\ell m)$$

so $ac - bd$ is a multiple of $m$. $\blacksquare$

**Corollary (powers).** $a \equiv b \Rightarrow a^k \equiv b^k$ for $k \ge 0$, by induction
on $k$: the base case is $1 \equiv 1$, and the step is the product rule applied to
$a^{k} \equiv b^{k}$ and $a \equiv b$. $\blacksquare$

**This is the licence to reduce at every step.** You never need the true value of an
expression, only its class, so any operand may be swapped for its residue at any time. That
is what keeps intermediates bounded — and the bound is the subject of §4.

> **The exponent is not covered by this.** The corollary reduces the *base*, never the
> *exponent*. $a^k \bmod m$ is not $a^{k \bmod m} \bmod m$: at $a=2$, $k=5$, $m=5$ the left
> side is $32 \bmod 5 = 2$ and the right is $2^0 = 1$. Exponents reduce modulo $\varphi(m)$,
> and only when $\gcd(a,m)=1$ — see [Euler's totient](../euler-phi/proofs.md) §4.

**Justifies** `productMod` and `sumMod` reducing inside the loop rather than at the end, and
`addMod`/`subMod` being correct on already-reduced operands.

---

## 4. Where the machine breaks: the exact overflow threshold

**Claim.** If both operands are reduced — $0 \le a, b \le m-1$ — then $ab \le (m-1)^2$, and
this fits in a signed 64-bit integer exactly when

$$m \le 3037000500$$

**Proof.** The product of reduced operands is largest at $a = b = m-1$. A signed 64-bit
integer holds up to $2^{63}-1 = 9223372036854775807$, and

$$3037000499^2 = 9223372030926249001 \ \le\ 2^{63}-1, \qquad
3037000500^2 = 9223372037000250000 \ >\ 2^{63}-1$$

so the largest safe operand is $3037000499$, i.e. the largest safe modulus is
$m = 3037000500$. $\blacksquare$

At the default $m = 10^9+7$ the worst product is $(10^9+6)^2 \approx 1.0\times10^{18}$,
comfortably inside — which is *why* that modulus is the default. At $m = 10^{12}$ the worst
product is $10^{24}$ and `a * b % m` is simply a different number.

**Justifies `mulMod`** ([implementation.cpp:25](implementation.cpp#L25)). `a * b % m`
evaluates `a * b` **first**, in 64 bits, and only then reduces; the `__int128` intermediate
holds the product so the reduction sees the true value. This is the repo's canonical silent
failure: no crash, no warning, no compiler diagnostic, just a wrong answer on the large
tests.

**Addition is not in the same danger.** Reduced operands give $a + b \le 2m-2 < 2m$, which
fits while $m \le 2^{62}$, about $4.6\times10^{18}$. That is why `addMod`
([implementation.cpp:31](implementation.cpp#L31)) can test `s >= m` and subtract instead of
paying for a `%` — a conditional subtraction, valid precisely because the sum is known to
be below $2m$.

---

## 5. Cancellation fails, and the exact repair

**Claim.** For any $c$, with $g = \gcd(c,m)$:

$$ac \equiv bc \pmod m \quad\Longleftrightarrow\quad a \equiv b \ \left(\mathrm{mod}\ \tfrac{m}{g}\right)$$

**Proof.** Write $m = g\,m'$ and $c = g\,c'$, so that $\gcd(m',c')=1$. Then

$$ac \equiv bc \ (\mathrm{mod}\ m)
\iff m \mid c(a-b)
\iff g m' \mid g c'(a-b)
\iff m' \mid c'(a-b)$$

and since $\gcd(m',c')=1$, **Euclid's lemma** upgrades the last statement to
$m' \mid (a-b)$. Every step is an equivalence, so the claim holds in both directions.
$\blacksquare$

**The standard counterexample is this theorem, not an exception to it.**
$2\cdot3 \equiv 2\cdot0 \pmod 6$ yet $3 \not\equiv 0 \pmod 6$. Here $g = \gcd(2,6) = 2$, so
what the theorem promises is $3 \equiv 0 \pmod 3$ — true. Cancelling the $2$ was legal all
along; keeping the modulus at $6$ was not.

**Justifies `cancelModulus`** ([implementation.cpp:76](implementation.cpp#L76)), which
returns exactly $m/\gcd(c,m)$ — the modulus the cancelled statement actually lives in.

**And it says when division is available at all.** Cancelling $c$ leaves the modulus alone
iff $g = 1$. That is the same condition as the existence of $c^{-1}$, proved in
[modular inverse](../modular-inverse/proofs.md) §1 — so $\gcd(c,m)=1$ is not two
coincidental facts but one, seen from either side.

---

## 6. Horner reduces a number too big to parse

**Claim.** For a decimal string $d_0d_1\cdots d_{L-1}$ denoting
$N = \sum_{i} d_i\,10^{\,L-1-i}$, the recurrence

$$r_0 = 0, \qquad r_{i+1} = 10\,r_i + d_i$$

ends at $r_L = N$; and reducing mod $m$ after every step leaves $r_L \equiv N \pmod m$.

**Proof.** That $r_L = N$ is Horner's rule, by induction: $r_i$ is the value of the first
$i$ digits, and appending a digit multiplies by $10$ and adds it. For the reduction, §3
says each of those two operations respects congruence, so replacing $r_i$ by any member of
its class changes no later class. An induction over the $L$ steps carries this to the end.
$\blacksquare$

**Justifies `modOfBigDecimal`** ([implementation.cpp:65](implementation.cpp#L65)) — a
60-digit input reduced without ever being held in an integer type, because no intermediate
exceeds $10m$.

**Two details in that line are not decoration.** `addMod` and `subMod` assume **reduced**
operands (§4 — that is what bounds the sum below $2m$), so both the digit and the constant
are reduced before use: `(c - '0') % m` matters when $m \le 9$, and `10 % m` when
$m \le 10$. At $m = 1$ every class is $0$, and these reductions are what make the function
return $0$ rather than a stray $1$.
