# Pattern: invariants and reachability

> *"Operations keep changing the numbers, but something about them never changes."*

The gcd is the most common conserved quantity in the subject. When a problem lets you
repeat some operation and asks what is achievable, the gcd is usually the answer.

---

## Recognition clues

* the words **greatest common divisor**, common factor, coprime, "in lowest terms"
* an operation you may repeat any number of times: subtract one element from another,
  replace a pair by its difference, add a fixed step, move by $\pm a$ or $\pm b$
* **"can you reach exactly $c$"** from a starting state
* **"make all elements equal"** by adding or subtracting a constant
* the same constant added to every element, then something asked about the result
* **"all leave the same remainder"** modulo some unknown $d$
* two periodic things that must coincide → lcm, reached through the gcd
* a repeating block in a string or array, where the period must divide the length

---

## Key observation

**Adding or subtracting multiples of existing values cannot change their gcd.**

$$\gcd(a, b) = \gcd(a - b,\ b) = \gcd(a \bmod b,\ b)$$

Two consequences do most of the work:

$$\text{reachable with steps } \pm a, \pm b \;=\; \text{the multiples of } \gcd(a,b)$$

$$a_i \text{ all congruent mod } d \iff d \mid \gcd(a_2 - a_1,\ a_3 - a_1,\ \dots)$$

The second turns a statement about **remainders** into a single gcd, which is the move
most of these problems are built around.

---

## Relevant concepts

[gcd](../concepts/gcd/) · [extended-euclid](../concepts/extended-euclid/) ·
[divisibility](../concepts/divisibility/)

---

## Typical approach

1. Identify the operation and ask what it preserves. If it only adds or subtracts existing
   values, the gcd is invariant.
2. If the question is *"is $c$ reachable"*, test $\gcd(\dots) \mid c$ and stop. You do not
   need the coefficients.
3. If it asks *how* to reach it, run `extgcd` and scale by $c/g$.
4. If it is about shared remainders, take the gcd of the **differences** from any one
   element. The valid moduli are its divisors.
5. Fold an array with `g = 0; for (x : v) g = gcd(g, x);` — the zero start removes the
   first-element case.

---

## Complexity

| gcd, lcm | $O(\log\min(a,b))$ |
|---|---|
| fold over $n$ values | $O(n\log M)$ |
| `extgcd`, solving $ax+by=c$ | $O(\log\min(a,b))$ |

The gcd is effectively free: under 90 iterations for any 64-bit input, so it never drives
the complexity.

---

## Common traps

| | |
|---|---|
| `a * b / g` for lcm | overflows; write `(a / g) * b` |
| whole-array gcd of $1$ read as pairwise coprime | $\gcd(6,10,15)=1$, yet no pair is |
| forgetting $\gcd(0,0) = 0$ | an all-zero array returns $0$ |
| negative inputs | take `abs` on entry |
| stepping by $b$ instead of $b/g$ in $ax+by=c$ | you skip $g-1$ of every $g$ solutions |
| assuming $\gcd(a,b)=1$ means one of them is prime | $\gcd(8,9)=1$ |
| using `extgcd`'s $x$ without normalizing | it is frequently negative |

---

## Example problems here

| where | problem | the shape |
|---|---|---|
| CF 664A · 800 | [Complicated GCD](https://codeforces.com/problemset/problem/664/A) | consecutive integers are coprime, so the answer is $1$ unless $a=b$ |
| CF 2034A · 800 | [King Keykhosrow's Mystery](https://codeforces.com/problemset/problem/2034/A) | smallest $m$ with $m \bmod a = m \bmod b$ — an lcm in disguise |
| CF 1325A · 800 | [EhAb AnD gCd](https://codeforces.com/problemset/problem/1325/A) | construct from $\gcd + \mathrm{lcm}$; $(1, x-1)$ works |
| CF 876B · 1300 | [Divisiblity of Differences](https://codeforces.com/problemset/problem/876/B) | equal remainders mod $m$ means bucket by remainder |
| CF 1458A · 1600 | [Row GCD](https://codeforces.com/problemset/problem/1458/A) | the gcd of differences is invariant under a shared offset |
| CF 7C · 1800 | [Line](https://codeforces.com/problemset/problem/7/C) | $Ax+By+C=0$ in integers — solvability then scaling |
| LC 365 · Med | [Water and Jug Problem](https://leetcode.com/problems/water-and-jug-problem/) | pure reachability: is $c$ a multiple of $\gcd(a,b)$ |
| LC 914 · Easy | [X of a Kind](https://leetcode.com/problems/x-of-a-kind-in-a-deck-of-cards/) | fold gcd over counts, test $\ge 2$ |
| LC 1071 · Easy | [GCD of Strings](https://leetcode.com/problems/greatest-common-divisor-of-strings/) | Euclid on lengths; the period divides both |
| LC 1250 · Hard | [Check If It Is a Good Array](https://leetcode.com/problems/check-if-it-is-a-good-array/) | Bézout for $n$ numbers — gcd of the whole array is $1$ |
| LC 2543 · Hard | [Check if Point Is Reachable](https://leetcode.com/problems/check-if-point-is-reachable/) | run the moves backwards; the gcd must be a power of two |

---

[patterns index](README.md) · [concepts](../concepts/) · [problems](../problems/README.md)
