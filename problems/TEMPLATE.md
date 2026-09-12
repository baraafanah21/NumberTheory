# Template — one solved problem

Copy this file, rename it `<judge>-<id>-<slug>.md` (for example `cf-1458a-row-gcd.md`),
and fill it in **after** you have solved the problem or given up and read the editorial.

Keep it short. This is a record of the *reasoning*, not an editorial — if a section has
nothing worth saying, delete the section rather than padding it. A good write-up fits on
one screen.

Only write one up when the problem taught you something. A problem you solved cleanly in
five minutes does not need a file; the ones that cost you an hour, or a wrong submission,
are the ones worth the effort.

---

```markdown
# <Problem name>

| | |
|---|---|
| platform | Codeforces / LeetCode / … |
| id | 1458A |
| link | <url> |
| difficulty | 1600 |
| main concept | [gcd](../concepts/gcd/) |
| secondary | [divisibility](../concepts/divisibility/) |
| date solved | YYYY-MM-DD |
| solved without help | yes / no — read editorial / no — hint only |

## What it asks

One or two sentences, in your own words. If you cannot compress the statement, you have
not understood it yet.

## Key observation

The one line that turns the problem from hard to easy. Almost every problem has exactly
one. Write it as a claim, not as a description of the algorithm.

> Example: $\gcd(a_1+b,\ a_2+b,\ \dots) = \gcd(a_1+b,\ a_2-a_1,\ a_3-a_1,\ \dots)$, so the
> per-query part collapses to a single gcd against a precomputed constant.

## Why that is true

Two or three lines. Enough that reading it in six months rebuilds the argument. Link to the
concept's `proofs.md` instead of reproving anything that already lives there.

## My approach

What you actually tried, in order — including the direction that did not work, and the
observation that made you abandon it. **This is the most valuable section.** The dead end is
usually more instructive than the solution, because the same dead end will look tempting
again.

## Mistakes

What went wrong, and what the symptom looked like:

* wrong answer on test 3 — forgot that $\gcd(0,x) = x$ makes the first element a special
  case that is *not* special
* overflow in `a * b / g`, silent, only visible at $10^{18}$
* assumed the array gcd being 1 meant pairwise coprime

If you got it right first try, write "none" and move on. Do not invent mistakes.

## Complexity

$O(n\log M)$ preprocessing, $O(\log M)$ per query.

## Final solution

```cpp
// the code you submitted, trimmed of debugging
```

## What to remember

One sentence, phrased as a trigger for next time — the pattern, not the problem.

> When a query adds the same constant to every element, the differences are invariant, so
> take the gcd of the differences once.
```

---

## The fields, and why each is there

| field | why |
|---|---|
| **main / secondary concept** | so the problem shows up when you reread that folder |
| **key observation** | the reusable part; the rest is implementation |
| **my approach** | preserves the dead ends, which is what you actually forget |
| **mistakes** | feeds the `mistakes/` section when a pattern repeats across problems |
| **what to remember** | written as a *recognition trigger*, matching the concept READMEs |

Skip **difficulty** if the judge does not publish one. Skip **why that is true** when the
observation is obvious once stated.

---

[problems index](README.md) · [concepts](../concepts/) · [repository root](../README.md)
