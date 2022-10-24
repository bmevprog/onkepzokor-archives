# How Did You Know?

ACM ICPC Hungary 2022

- [Feladat](https://domjudge.cms.inf.elte.hu/public/problems/46/text)

## Megoldás

- 1-100 közötti számból gondolhatott Alex $n\leq{}10$ darab egymást követőre.
- Ha tudjuk az n-et akkor az összegük csak páros vagy csak páratlan vagy mindkettő lehet?
- Ez első körben függ attól, hogy n páros-e vagy páratlan:
  - Ha n páratlan, akkor az összeg mindkettő lehet, hiszen:
    - Ha veszem az első n darab számot ( $[1, n]$ ), akkor ezeknek az összege vagy páros vagy páratlan.
    - Ezután eltolom 1-el jobbra az intervallumot ( $[2, n]$ ), akkor minden szám értéke 1-el nőtt.
    - Ekkor az összegük $n$-el nőtt, ami páratlan.
    - Tehát bármi is volt a paritása az $[1, n]$ összegének, az most az ellenkezőjére változott.
    - Tehát páros és páratlan is lehet az összeg.
    - (Itt fontos, hogy $n\leq{}10$, tehát az $[1,100]$ intervallumba beleesik az $n+1$.)
  - Ha n páros, akkor az összeg attól függ, hogy mennyi a négyes maradéka = hány páratlan szám van az $n$ darab között.
    - Ha a $4$-es maradék $0$, akkor páros sok páratlan szám van, amiknek az összege páros. A páros számok összege szintén páros, tehát ilyenkor páros a teljes összeg.
    - Ha a $4$-es maradék $2$, akkor páratlan sok páratlan szám van, amiknek az összege páratlan. A páros számok összege továbbra is páros, tehát ilyenkor páratlan a teljes összeg.
    
## Észrevételek

- Az általam leadott forráskód kihasználja azt, hogy az integerek implicit bool-á castolódnak.
  - A 0 false, minden más true.
  - Így pl. $n$ oszthatósága $k$-val így ellenőrizhető: `if (n%k) { /* Nem osztható. */ } else { /* Osztható. */ }`
