# acm-szakkor-by-viki

## [Fishermen](https://codeforces.com/contest/1728/problem/F)

`flows`; `graph matchings`; `greedy`; `*3100`

### Feladat

- Kapsz $n\leq{}1000$ db $a_i\leq{}10^6$ számot, megpermutálod, első u.a. marad.
- Többi az előtte lévőnél szigorúan nagyobb & az eredetivel osztható legkisebb.
- Permutáld úgy hogy ezek összege minimális legyen.

### Megoldás

- Első ötlet: növekvő sorrend.
- Probléma: 1 11 12
