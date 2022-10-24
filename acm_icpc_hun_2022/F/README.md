# Freybug Frenzy

ACM ICPC Hungary 2022

- [Feladat](https://domjudge.cms.inf.elte.hu/public/problems/44/text)

## Megoldás

- Egy kicsit nehéz rájönni a feladat szövegéből hogy mit vár el:
  - Tulajdonképpen azt kell nézni, hogy az $n$ elemből az összes lehetséges módon kiválasztunk $k$ darabot és ezekre a lehetőségekre számoljuk ki a módosított spawn chance-ek átlagát.
  - A módosított spawn chance-ket úgy kapjuk, hogy adott $k$ kiválasztásra, hogy a nem kiválasztottak valószínűségét hozzáadjuk a sorrendben az utánuk következő első már kiválasztott elemhez.
- Ez már jó megoldás lenne, csak lassú, hiszen $n$ alatt a $k$, ha $k,n \leq{} 500$, túl sok lenne.
- Helyette úgy gondolkozunk, hogy amikor az $i$. elem-re számoljuk az átlagot, kiszámoljuk azt, hogy hány olyan $n$ alatt a $k$ kiválasztás van, amire az előtte lévő $j$. elem valószínűsége hozzáadódik az $i$-jéhez.
- Ilyen akkor van, ha a $[j,i-1]$ intervallumon nem választottunk ki senkit, az $i$. elemet pedig kiválasztottuk, továbbá a maradék $n-(i-j+1)$ elemből kell $k-1$-et kiválasztani, ez akkor $n-(i-j+1)$ alatt a $k-1$-féleképpen lehetséges, a nevezőben pedig az $n$ alatt a $k$ össz-valószínűség szerepel.

$$\frac{\binom{n-(i-j+1)}{k-1}}{\binom{n}{k}} = \frac{\frac{(n-(i-j+1))!}{(n-(i-j+1)-(k-1))!(k-1)!}}{\frac{n!}{k!(n-k)!}} = \frac{\frac{(n-i+j-1))!}{(n-i+j-k)!(k-1)!}}{\frac{n!}{k!(n-k)!}} = \frac{k!(n-k)!(n-i+j-1))!}{n!(n-i+j-k)!(k-1)!} = \frac{k\cdot{}(n-i+j-1)\cdot{}\dots{}\cdot{}(n-i+j-k+1)}{n\cdot{}\dots{}\cdot{}(n-k+1)} $$

- A fenti képletet inkrementálisan $j=i-1$-től kezdve számolhatjuk ki, ilyenkor a korábbi $j$-re kiszámolt értéket kell $\frac{n-(i-j+1)}{k-1}$-el szorozni.
 
 ## Észrevételek
 
 - A fenti logikában sokaknak feltűnhet, hogy tulajdonképpen összefüggő eseményekre számoltunk valamilyen átlagot (várható értéket).
 
 - Fontos ismerni a valszámos progversenyes feladatokhoz, hogy a várható érték lineáris, **counter-intuitív módon az összefüggő események esetében is**, tehát az összeg várható értéke a várható érték összege, vagyis a fenti logikát megtehetjük.
 - Indikátor esemény várható értéke a valószínűsége, ezért a fenti érvelés a valószínűségekre is végiggondolható, ahol az $n$-ből adott $k$ db kiválasztásához kapcsolódó indikátor valószínűségi változó várható értékeként fogalmazzuk meg a megfelelő spawn chance-t.
