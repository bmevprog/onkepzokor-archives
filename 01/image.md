# 1721 A

Következő esetek lehetségesek:

- 4 különböző szín: 1-1-1-1
- 3 különböző szín: 1-1-2
- 2 különböző szín: 1-3 vagy 1-3
- 1 szín: 4

- A 4 különböző színnél az első lépés biztosan valakit átszínezni egy másik létező színre.
  - Tehát 1-1-1-1 -> 1-1-2 lépés.
- 1-1-2 esetén vagy a két egyedi színből csinálunk egyet (2-2) vagy valamelyik egyedit igazítjuk a 2-es csoport színéhez (1-3).
- Teljesen mindegy melyiket lépjük, mert utána 2-2 és 1-3 is egy lépésben megoldható (4).
- Tehát 1-1-2 két lépésben megoldható.
- Tehát 1-1-1-1 3 lépésben megoldható.

Tömören: 1-1-1-1 -> 1-1-2 -> 2-2 vagy 1-3 -> 4.

Ezen a ponton már le tudjuk kódolni.

Mitől lesz "oneliner":

- `c=input()+input()` kód 2 sort olvas be stdinről és konkatenálja őket (Python string konkatenáció: +), majd beteszi a 4 karaktert c-be.
- Nem számít mik a színek betűjelei, csak az, hogy hány darab van belőlük. Megszámolhatnánk őket a [Counter](https://docs.python.org/3/library/collections.html#collections.Counter) collection objecttel.
- De mégegy megfigyelés: A megoldáshoz vezető lépések száma pont az azonos színű csoportok darabszáma-1. Azaz `len(set(c))-1`-t kel kiírni.

Érdekes megoldások:

- https://codeforces.com/contest/1721/submission/169850259
  - The long one.
  - Még FastIO templatet is használ, de a main()-je is sok sor.

- https://codeforces.com/contest/1721/submission/169829511
  - The even longer one.
  - FastIO template itt is.

- https://codeforces.com/contest/1721/submission/169816722
  - Counteres megoldás.

- https://codeforces.com/contest/1721/submission/169848796
  - Ha nem ismered a Python trükkjeit.

- https://codeforces.com/contest/1721/submission/170245380
  - Adatszerkezet (set) nélkül.

- https://codeforces.com/contest/1721/submission/170773003
  - Szorzás = string konkatenáció annyiszor.
  - Exec végrehajtja a stringet.

- https://codeforces.com/contest/1721/submission/169932569
  - Golfozik?

- https://codeforces.com/contest/1721/submission/169854934
  - Am foreachet lehet 1 sorba is írni és akkor "oneliner" (én list comprehensiont használtam).

- https://codeforces.com/contest/1721/submission/169804030
  - Nyertes, C++-ban: sokkal több sor, de itt a nyelv miatt muszáj.
  - `#include <bits/stdc++.h>` -> Mindent importál.
  - `std::ios::sync_with_stdio(false); std::cin.tie(nullptr);` -> Gyorsabb IO, nem szinkronizál a C-s stdio függvényekkel meg az input/outputtal.
  - `while (t--)`: Casek darabszámából visszafele számol 0-ig. :)
