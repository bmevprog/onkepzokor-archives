# Low-cost Transformation

ACM ICPC Hungary 2022

- [Feladat](https://domjudge.cms.inf.elte.hu/public/problems/50/text)

## Megoldás

- Amíg $A$ nagyobb mint $B$, addig muszáj osztani, mert másképp nem tudjuk csökkenteni az értékét.
- Ha $A$ páratlan akkor csak úgy tudjuk leosztani, ha előtte hozzáadunk $1$-et.
- Ha még fogunk később osztani, akkor nem érdemes egymás után többször is hozzáadni $1$-et $A$-hoz, hiszen az osztás után $1$-el kevesebb művelettel ugyanazt az eredményt érjük el.
- Ha $A$ kisebb mint $B$, akkor addig adunk hozzá $1$-et, amíg $A=B$ nem lesz.
