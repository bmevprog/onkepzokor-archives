# Grand Scheme of Things

ACM ICPC Hungary 2022

- [Feladat](https://domjudge.cms.inf.elte.hu/public/problems/45/text)

## Megoldás

- Kulcsmondat: It is guaranteed that the sum over all $m_i$ is at most $200 000$.
- Reprezentáljuk a feladatot páros gráfként!
- Legyenek az A színosztály csúcsai az emberek, a B színosztály csúcsai pedig a különböző számaik (amik majd közösek lehetnek).
- Kössön össze akkor él $a\in{}A$ elemet $b\in{}B$-vel ha az $a$ csúcshoz tartozó embernek száma a $b$ csúcshoz tartozó szám.
- Emberből legfeljebb $2\cdot{}10^5$ van, a számok ugyan lehetnek legfeljebb $10^9$ értékűek, de legfeljebb azokból is csak $2\cdot{}10^5$ lesz a fenti kulcsmondatból következően.
- A kulcsmondat miatt ebben a gráfban legfeljebb $2\cdot{}10^5$ él van.
- A gráfot bejárjuk, például szélességi kereséssel $O(|V|+|E|)$, kb $const\cdot{}10^5$ lépésben. Ha összefüggő akkor van megoldás, ha nem összefüggő akkor nincs.
- Ha van megoldás, akkor azt a szélességi bejárás közben írhatjuk fel:
  - Amikor egy $a_1 \in{} A$ csúcsból $b\in{}B$-be lépünk, majd onnan $a_2\in{}A$-ba, akkor az " $a_2$ $a_1$ $b$ " sort jegyezzük meg az $a_2$-höz.

## Észrevételek

- Más gráfreprezentáció is eszünkbe juthat, például egy olyan ahol az emberek a csúcsok és akkor megy él két csúcs között, ha van közös számuk.
- Ebben az esetben figyelni kell arra, hogy az éleket nem fogjuk tudni előre kiszámolni, mivel ahhoz ~ $10^{10}$ lépés kellene, ezért trükkösen kell csinálni, hogy a BFS beleférjen. 
- Jó trükk ezért ilyen feladatokban, hogy többféle gráfreprezentációt is kipróbálunk és megnézzük melyiket könnyebb-nehezebb használni, kihasználni vele a feladat korlátait.
