# Budapest Hold’em

ACM ICPC Hungary 2022

- [Feladat](https://domjudge.cms.inf.elte.hu/public/problems/40/text)

## Megoldás

- Beolvassuk a kártyákat, csak az első karakter számít mindegyikből.
- Megszámoljuk melyikből mennyi van.
- Kiírjuk a legnagyobb darabszámot.

## Észrevételek

- Pythonban a [Counter](https://docs.python.org/3/library/collections.html#collections.Counter) osztály használható elemek számolására beépítetten.
  - Ezen a `most_common` függvényt hívva csökkenő sorredbe állítjuk a kulcsokat.
  - A konstruktorának meg lehet adni bármilyen iterálható inputot.
