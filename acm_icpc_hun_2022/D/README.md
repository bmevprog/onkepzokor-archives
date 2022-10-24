# Devil’s Algorithm

ACM ICPC Hungary 2022

- [Feladat](https://domjudge.cms.inf.elte.hu/public/problems/42/text)

## Megoldás

- Három eset van:
- Ha Claire 5. eredménye lesz az eddigi legjobbja, akkor a 4 korábbi eredményből a 3 legkisebb átlaga kell, hogy legfeljebb annyi legyen mint az elérni kívánt cél.
  - Ha még így is az elérni kívánt célnál nagyobb az átlag, akkor lehetetlen olyan jó eredményt elérni, hogy a célt elérjük vele.
- Ha Claire 5. eredménye lesz az eddigi legrosszabbja, akkor a 4 korábbi eredményből a 3 legnagyobb átlaga kell, hogy legfeljebb annyi legyen, mint az elérni kívánt cél.
  - Ha ezeknek az átlaga eléri a célt, akkor bármi is az 5. eredmény, biztosan megoldható a feladat, ilyenkor írunk ki "infinite"-t.
- Egyéb esetben pedig az van, hogy az 5. eredmény szerepelni fog az átlagban, a 4 korábbi eredményből a 2 középső mellett.
  - Ilyenkor 1 ismeretlenes egyenlet megoldásával megadható minek kell az 5. eredménynek lennie.

## Észrevételek

- A feladat megoldásában fontos szerepet játszik, hogy ismerjük-e hogyan kell floating point számokkal számolni versenyeken.
- Minden olyan esetben amikor tört számokat hasonlítunk össze (`==`, `<=`, stb.), mindenképp jusson eszünkbe a floating point precision.
- Más jellegű feladatokban még az is előfordulhat, hogy egy `epsilon` pontossági hibát meg kell engedni az összehasonlításkor, ilyenkor a feladat szövege említi a megengedett pontossági hibát. Itt ez nem játszik.

### Trükk 1

- Amikor átlagolni akarunk 3 számot és azt összehasonlítani a target-el az problémás.
- Ilyenkor osztás helyett mindig vigyük át a másik oldalra és csináljunk belőle szorzást!
- Azaz `s[0] + s[1] + s[2] <= target / 3` helyett `3*(s[0] + s[1] + s[2]) <= target` legyen.

### Trükk 2

- Emellett pedig a bemeneti értékeket fel kell szorozni 100-al, így egész számok lesznek.
- Különben hiába olvasunk be például `6.38`-at, az valójában `6.2999999999999998`-ként fog tárolódni. Lásd az alábbi kód kimenetét:

```cpp
#include <iostream>
#include <limits>

using namespace std;

typedef std::numeric_limits<double> dbl;

int main() {
  double a = 6.3;
  cout.precision(dbl::max_digits10);
  cout << a << endl;
  return 0;
}
```

### Megjegyzés

- A feladat szövegéből nem derül ki egyértelműen, hogy az átlagolás művelet eredményét milyen pontossággal kell érteni. Mivel a be- és kimenetek is $2$ tizedesjegy pontossággal adandóak meg, ezért felmerülhet az a kérdés, hogy az átlag számítás eredményét is kerekíteni kellene.
  - Ha ez így lenne, akkor például a `0.50 1.00 1.00 2.00` eddigi eredmények és `1.00` target esetében megengedhető lenne az `1.01`-es 5. eredmény is, hiszen a `0.01` fölöslegből az átlagoláskor `0.00333333...` keletkezik, amit 2 tizedesjegyre kerekítve `0.00`-t kapunk.
  - Valószínűleg a feladat ezt nem így érti, mivel például az én mellékelt megoldásomat is elfogadta és a fenti logika nagyon sok bemenet esetén működik, ezért valószínűleg van ilyen hidden teszteset.

