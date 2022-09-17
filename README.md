# Témák, feladatok

## Párosítások

### Advanced

#### [Fishermen](https://codeforces.com/contest/1728/problem/F)

`flows`; `graph matchings`; `greedy`; `*3100`

##### Feladat

- Kapsz $n\leq{}1000$ db $a_i\leq{}10^6$ számot, megpermutálod, első u.a. marad.
- Többi az előtte lévőnél szigorúan nagyobb & az eredetivel osztható legkisebb.
- Permutáld úgy hogy ezek összege minimális legyen.

##### Megoldás

- Páros gráf:
  - Egyik színosztály: $k_i = $ 1,2,3,4,5,... stb számok.
  - Másik színosztály: $a_i$-k.
  - Él, ha $a_i$-ből lehet $k_i$-t csinálni (osztható vele és nagyobb nála). ( $a_i \rightarrow k_j$ : Ha $a_i | k_i$ és $a_i \leq{} k_i$.)
  - Párosítást keresni, úgy hogy $k_i$-ből adott körben $x$ db van és mindig bővítem, illetve a párosítást is javító úttal.
    - Bináris keresés a méretére?
  - Tutorial am Kuhn's algot ír és még azon belül is optimalizálni kell.
