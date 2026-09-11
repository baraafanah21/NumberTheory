# Fast Power — Problems

Ten problems, hardest last. Every **check value** here was computed, not guessed — if your
code prints something else, the bug is yours. Answers at the bottom.

---

## By hand

**1. Count the work.** Write $100$ in binary. How many **squarings** and how many **extra
multiplications** does $a^{100}$ cost through the loop, and how does that compare with the
naive $99$? Then give the worst case for any 64-bit exponent.

**2. Trace the loop.** Compute $3^{13} \bmod 7$ by writing out the table of
`exp`, `base`, `r` at every iteration. Four rows. Then check your answer a second way,
using the fact that $3^6 \equiv 1 \pmod 7$.

**3. The `1 % m` line.** What does `powMod(7, 5, 1)` return if the accumulator is
initialised to `1` instead of `1 % m`? What is the right answer, and why does *every*
modular routine in this repo start with `1 % m`?

**4. Which are legal.** For each, say whether binary exponentiation applies, and why:
raising a $2\times2$ matrix to the $n$-th power; composing a permutation with itself $n$
times; taking $a^{-3} \bmod 7$; taking $a^{n}$ where the operation is subtraction.

---

## With code

**5. The plain case.** Compute $2^{100} \bmod (10^9+7)$. Also report the exact value of
$2^{100}$ to see why you cannot get there by computing the number first.

**6. Fibonacci at $10^{18}$.** Using the identity

$$\begin{pmatrix}1&1\\1&0\end{pmatrix}^{\!n} = \begin{pmatrix}F_{n+1}&F_n\\F_n&F_{n-1}\end{pmatrix}$$

compute $F_{10^{18}} \bmod (10^9+7)$, with $F_0 = 0$, $F_1 = 1$. Verify your matrix code
against the iterative sequence for $n \le 500$ before trusting it.

**7. Walks in a graph.** Take the triangle $K_3$ with adjacency matrix all-ones-off-diagonal.
How many walks of length $10$ start and end at the same vertex? Get it from $A^{10}$, then
confirm against the closed form $\frac{2^n + 2(-1)^n}{3}$.

**8. Where `*` breaks.** Run `powMod(3, 10^9, m)` with $m = 4000000000000000037$, once with
`base * base % m` and once with a `__int128` `mulMod`. Report both. Then find the rough
threshold on $m$ below which the plain version is still safe.

---

## Harder

**9. Prove it.** Show that $r \cdot \mathtt{base}^{\,\mathtt{exp}}$ is invariant across one
iteration of the loop, in both the bit-set and bit-clear cases. Then explain why that
invariant plus the termination condition gives $r = a^{\,n}$, and say what the invariant
becomes for the matrix version.

**10. Is binary optimal?** Binary exponentiation is $O(\log n)$, but it is **not** always
the cheapest possible chain of multiplications. Find a chain computing $a^{15}$ in fewer
multiplications than the loop uses, and say how many each takes. Then explain why nobody
bothers with the optimal version in a contest.

---

## Judge problems

| where | problem | what it drills |
|---|---|---|
| LC 50 · Med | [Pow(x, n)](https://leetcode.com/problems/powx-n/) | the loop with no modulus, plus the negative-exponent case of problem 4 |
| LC 1922 · Med | [Count Good Numbers](https://leetcode.com/problems/count-good-numbers/) | $5^{\lceil n/2\rceil}4^{\lfloor n/2\rfloor} \bmod 10^9+7$ with $n \le 10^{15}$ |
| LC 372 · Med | [Super Pow](https://leetcode.com/problems/super-pow/) | problem 5's exponent arriving as a digit array, on a composite modulus |
| CF 1327A · 1100 | [Sum of Odd Integers](https://codeforces.com/problemset/problem/1327/A) | a constructive check where the power is the cheap part |
| CF 913A · 800 | [Modular Exponentiation](https://codeforces.com/problemset/problem/913/A) | $2^n$ against $m$, where $n \le 10^8$ makes the naive loop the trap |
| LC 2550 · Med | [Count Collisions of Monkeys on a Polygon](https://leetcode.com/problems/count-collisions-of-monkeys-on-a-polygon/) | $2^n - 2 \bmod 10^9+7$ — the subtraction is where people lose it, since the result can go negative |

Matrix exponentiation deserves its own drilling once the scalar version is automatic. The
classic entry point is any linear recurrence with $n$ up to $10^{18}$.

---

## Answers

**1.** $100 = 1100100_2$, seven bits, with $\mathrm{popcount} = 3$. The loop does
$\lfloor\log_2 100\rfloor = \mathbf{6}$ squarings and $3-1 = \mathbf{2}$ extra
multiplications, so **8 multiplications** against the naive $99$. Worst case for a 64-bit
exponent is $63$ squarings plus $63$ multiplications, so $\mathbf{126}$ — which is why the
cost of this routine never appears in a complexity analysis.

**2.** $13 = 1101_2$. Reading bits from the bottom:

| step | `exp` | bit | `r` after | `base` after |
|---|---|---|---|---|
| 1 | $13$ | $1$ | $1\cdot3 = 3$ | $3^2 = 9 \equiv 2$ |
| 2 | $6$ | $0$ | $3$ | $2^2 = 4$ |
| 3 | $3$ | $1$ | $3\cdot4 = 12 \equiv 5$ | $4^2 = 16 \equiv 2$ |
| 4 | $1$ | $1$ | $5\cdot2 = 10 \equiv \mathbf{3}$ | — |

So $3^{13} \equiv \mathbf{3} \pmod 7$. Second route: $3^6 \equiv 1$, and
$13 = 2\cdot6 + 1$, so $3^{13} \equiv 3^1 = 3$ ✓. That second route is
[Euler's totient](../euler-phi/) arriving early — $\varphi(7) = 6$.

**3.** With `r = 1` it returns $\mathbf{1}$, which is wrong: **every** integer is $\equiv 0
\pmod 1$, so the answer must be $\mathbf{0}$. `1 % m` costs nothing and removes the case
entirely. It matters because $m=1$ is a legal input that graders do use, and because the
same line appears in factorial tables, matrix identities and CRT merges — anywhere an
identity element is written down.

**4.** Matrix power: **yes**, matrix multiplication is associative with the identity matrix
as unit. Permutation composition: **yes**, same reason, unit is the identity permutation.
$a^{-3} \bmod 7$: **not directly** — compute $a^{-1}$ by
[modular inverse](../modular-inverse/) first, then cube it. Subtraction: **no**, it is not
associative, since $(a-b)-c \neq a-(b-c)$. Associativity is the entire requirement; the
values being numbers is irrelevant.

**5.** $2^{100} \equiv \mathbf{976371285} \pmod{10^9+7}$. The exact value is

$$2^{100} = 1267650600228229401496703205376$$

a 31-digit number, roughly $10^{12}$ times past the `long long` ceiling. There is no order
of operations that computes it first and reduces afterwards.

**6.** $F_{10^{18}} \equiv \mathbf{209783453} \pmod{10^9+7}$. Sanity anchors on the way:
$F_{10} = 55$, and $F_{90} = 2880067194370816120 \equiv 210345902$. The usual bug is
forgetting to reduce **inside** `matMul` — entries near $10^9$ multiply to $10^{18}$, and
summing two of those overflows.

**7.** $\mathbf{342}$. The closed form gives $\frac{2^{10} + 2}{3} = \frac{1026}{3} = 342$
✓, and the off-diagonal entries are $\frac{2^{10}-1}{3} = 341$. The reason the closed form
exists is that $A$ has eigenvalues $2, -1, -1$; the matrix power is doing the same work
without needing them.

**8.** Both runs at $m = 4000000000000000037$:

| `base * base % m` | garbage — the product reaches $1.6\times10^{37}$ |
|---|---|
| `mulMod` via `__int128` | $\mathbf{2463715516319723756}$ |

The plain version is safe exactly while $(m-1)^2 < 2^{63}$, so up to
$m \approx 3.04\times10^9$. That is why $10^9+7$ lets you get away with it and why anything
at $10^{18}$ does not.

**9.** Let $I = r \cdot \mathtt{base}^{\,\mathtt{exp}}$.

*Bit clear.* `exp` becomes $\frac{\mathtt{exp}}{2}$ and `base` becomes
$\mathtt{base}^2$, so the new value is
$r\,(\mathtt{base}^2)^{\mathtt{exp}/2} = r\,\mathtt{base}^{\mathtt{exp}} = I$.

*Bit set.* First $r \leftarrow r\cdot\mathtt{base}$ and `exp` drops by one, which turns $I$
into $(r\,\mathtt{base})\,\mathtt{base}^{\,\mathtt{exp}-1} = I$; then the even case applies.

Initially $r = 1$ and $\mathtt{exp} = n$, so $I = a^{\,n}$ throughout. The loop exits at
$\mathtt{exp} = 0$, where $I = r\cdot\mathtt{base}^0 = r$. Hence $r = a^{\,n}$.
$\blacksquare$

For matrices the invariant is identical with $r$ starting at the identity matrix — the proof
uses only associativity and the unit law, never commutativity, which is exactly why it
transplants.

**10.** $15 = 1111_2$, so the loop pays $3$ squarings and $3$ multiplications:
$\mathbf{6}$. But the chain

$$a \to a^2 \to a^3 \to a^6 \to a^{12} \to a^{15}$$

reaches it in $\mathbf{5}$ — square, multiply by $a$, square, square, multiply by $a^3$.
Finding the shortest such **addition chain** is NP-hard in general and the saving is a
constant factor on an already-negligible $O(\log n)$, so it never pays off in a contest.
It matters in cryptography, where one exponentiation is the entire workload.

---

[README.md](README.md) · [implementation.cpp](implementation.cpp)
