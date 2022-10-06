# 05 - Palindrome Basis

https://codeforces.com/group/2cnVva0QXt/contest/399604/problem/05

## Megoldás

- n legfeljebb 40,000, azaz eddig előállítunk minden palindromot gyorsan -> p[] tömbben tároljuk őket.
- DP-s megoldás:
  - DP[sum, i]: Az első i db palindromot felhasználva a sum összeg hányféleképpen állítható elő.
  - Két eset van: i. palindromot vagy felhasználjuk sum-ban, vagy nem.
  - DP[sum, i] = DP[sum, i-1] + DP[sum-p[i], i].
    - (Ha bármelyik index negatív definiáljuk 0-nak a DP tömb ottani értékét.)
    - Az összeg második tagja DP[sum-p[i], i] specifikusan nem i-1 az index itt, mivel egy palindromot többször is felhasználhatunk az összegben.
  - DP[0, 0] = 1.
- Előzetesen kiszámoljuk egyszer a tömböt max(n_j)-re, minden teszteset 1 lookup.