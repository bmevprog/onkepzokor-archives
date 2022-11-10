#include <bits/stdc++.h>

#ifdef LOCAL
#include "../algo/debug.h"
#else
#define debug(...) 42
#endif

using namespace std;

// Edmonds-Karp maximális folyam / minimális vágás kereső algoritmus.
class EdmondsKarp {
  // Source = forrás és target = nyelő csúcsok indexei.
  int s, t;

  // Eredeti hálózat:
  // - Szomszédossági mátrix az irányított élekhez, egész kapacitásokkal.
  // - capacity[i][j] az i->j él kapacitása.
  vector<vector<int>> capacity;

  // Csúcsok darabszáma = vertex count.
  int v;

  // Aktuális folyam értékei az egyes éleken.
  vector<vector<int>> flow;

  // Eddigi megtalált legnagyobb s->t folyam összértéke.
  // Ugyanaz, mint a flow mátrixban a flow[s][..] vagy a flow[..][t] élek összege.
  int maxflow;

  // Maradékok hálózata = residual flow network.
  //
  // Az aktuális folyam mellett karbantartott hálózat
  // (maradék kapacitásos irányított szomszédossági mátrix).
  //
  // Amikor s -> t úton új "vizet" folyatunk, akkor
  // azokat az értékeket levonjuk az útvonal menti élekből és
  // ellentétes irányban pedig felvesszük.
  //
  // Ebben keresünk s->t utat BFS-el amíg van. Ha már nincs, akkor
  // megtaláltuk a minimális vágást = maximális folyamot.
  vector<vector<int>> residual;

  // Segédmátrix amit a BFS használ, ebben tárolja a szélességi keresés
  // során megtalált fában az elért csúcsok szüleit.
  vector<int> parent;

  // Futás elején inicializáljuk a segédváltozókat.
  void init(int s, int t) {
    // Reseteljük a folyam értékét 0-ra.
    this->maxflow = 0;
    // Elmentjük a source és target indexeket.
    this->s = s;
    this->t = t;

    // A maradékhálózat és a folyam inicializálása.
    // Kezdetben a maradékhálózat egyezik a kiinduló kapacitásokkal.
    this->residual = capacity;
    // Kezdetben a folyam azonosan 0 értékű minden élen.
    this->flow = vector<vector<int>>(v, vector<int>(v));
  }

  // Szélességi keresés a maradékok hálózatában (residual),
  // a source csúcsból kiindulva próbáljuk a target-et elérni.
  int BFS() {
    // Kezdetben -1-re állítunk minden szülőt.
    // -1 = még keressük a szülőjét a BFS-ben.
    parent = vector<int>(v, -1);
    
    // A BFS során a source-ból az adott i csúcsba küldhető max folyam érték
    // az s->i megtalált út mentén.
    // A BFS fában az s->i úton az élek maradék kapacitásának a minimuma.
    // Kezdetben mindenhol 0, mert még nem találtunk utat.
    vector<int> residual_capacity_of_path(v);

    // A source csúcsba végtelen folyam küldhető, innen továbbmenve
    // korlátozódik majd a kimenő élek maradék kapacitásai szerint.
    residual_capacity_of_path[s] = INT_MAX;
    
    // Mivel a -1 azt jelenti, hogy még szülőt keresünk egy csúcshoz,
    // a source csúcsnak nincs szülője és nem is keresünk neki, ezért ő
    // egy kitüntetett -2-es indexet kap.
    //
    // Ez akkor jön jól, ha olyan a gráf hogy a source-ba vannak benne
    // bemenő élek, mert akkor kapnánk egy kört, így ezt a BFS ignorálja.
    parent[s] = -2;

    // BFS keresés "széle"/várakozási sora, amelyik csúcsokat már
    // megtaláltunk de még nem fejtettünk ki.
    queue<int> frontier;

    // A source-ból indulunk.
    frontier.push(s);

    // Amíg van kifejtendő csúcs, addig azt kifejtjük.
    while (!frontier.empty()) {

      // Aktuálisan kifejtendő csúcs = curr, kivesszük a várakozási sorból.
      int curr = frontier.front(); frontier.pop();
      debug(curr);
      
      // Végigmegyünk a maradékok hálózatában a curr csúcs szomszédain.
      for (int next = 0; next < v; ++next) {
        // Valaki (a next csúcs) akkor szomszéd ha pozitív maradék kapacitással elérhető curr-ból.
        // Ha a next csúcsnak még nincs szülője a BFS-ben, csak akkor fejtjük ki.
        if (residual[curr][next] > 0 && parent[next] == -1) {
          // Most megtaláltuk a next-et, curr-ból, ezért curr a szülője a BFS fában.
          parent[next] = curr;
          debug(curr, next);

          // Next-be küldhető maximális folyam érték a curr-be küldhető max folyam
          // és a curr->next maradék kapacitás él minimuma.
          residual_capacity_of_path[next] = min(residual[curr][next], residual_capacity_of_path[curr]);

          // Ha elértük a target-et a BFS-el, akkor visszatérünk az elérhető max folyamértékkel,
          // kint majd ez alapján frissítjük a flow-t és a residual-t.
          if (next == t) {
            return residual_capacity_of_path[t];
          } else {
            // Ha nem értük el a target-et, akkor a next is egy kifejtendő csúcs,
            // tehát ő is bekerül a várakozási sor végére.
            // BFS ettől szélességi keresés, hogy a megtalálás sorrendjében
            // fejtjük ki a csúcsokat, amihez egy queue = sor adatszerkezet passzol.
            frontier.push(next);
          }
        }
      }
    }
    debug("BFS END", parent);
    // Ha ide eljutottunk, akkor nem találtuk meg a target-et,
    // tehát a max elérhető s->t út értéke 0 lett.
    return 0;
  }

public:
  // Az osztály létrehozásakor elmenti a kapacitás mátrixot
  // és hogy hány csúcsa van a gráfnak.
  EdmondsKarp(const vector<vector<int>> &capacity) {
    this->capacity = capacity;
    this->v = this->capacity.size();
  }

  // s->t folyam keresése.
  int run(int s, int t) {
    // Resetelünk minden segédváltozót.
    // Így a run függvény többször is hívható
    // ugyanarra a gráfra, más source és target csúcsokra.
    // Pl. a pingvines feladatban is több targeten végig kell menni.
    init(s, t);

    // A BFS mindig s->t utat keres és visszatér ezen út mentén
    // elküldhető folyam értékével.
    while (int addflow = BFS()) {
      debug("AFTER BFS");
      // Ezt hozzáadjuk az aktuálisan elért maximális folyamértékhez.
      maxflow += addflow;
      debug(parent);
      // Majd a BFS parent táblája alapján viszamászunk
      // a target-ből kezdve a source-ig az s->t út élein.
      int curr = t;
      while (curr != s) {
        // Egy élre az alábbi frissítéseket végezzük:
        int pcurr = parent[curr];
        // A maradék hálózat karbantartása:
        // A most átküldött folyamértéket levonjuk az él kapacitásából.
        residual[pcurr][curr] = residual[pcurr][curr] - addflow;
        // Visszairányban pedig felvesszük, ettől lesz visszavonható ez a döntésünk.
        residual[curr][pcurr] = residual[curr][pcurr] + addflow;
        // A folyamot is karbantartjuk.
        // Ezt külön érdemes, mert a maradékok hálózatában nehéz követni, hogy
        // melyik él eredeti és melyik "visszaél", a visszavonhatósághoz.
        flow[pcurr][curr] = flow[pcurr][curr] + addflow;
        // Megyünk a szülővel tovább, föl egészen a source-ig.
        curr = pcurr;
        debug(curr);
      }
      debug(flow);
      debug(residual);
    }
    debug("DONE FLOW");
    // A futás végén visszatérünk az elérhető max s->t folyam értékével.
    return maxflow;
  }

  // A run függvény hívása után hívhatjuk ezt a függvényt.
  //
  // A megtalált max folyamhoz tartozó minimális vágást fogja megadni.
  //
  // Vágás = a source-ból még elérhető csúcsok a maradékok hálózatában.
  // A target biztosan nem elérhető, mert akkor áll meg az algoritmus,
  // amikor már nincs s->t út.
  // 
  // Ilyenkor a minimális vágáshoz tartozó élek a vágás-ból
  // mutatnak ki a (teljes csúcshalmaz \ vágás)-ba, ezek alkotják a
  // hálózatban a "bottleneck"-et, ami miatt nem elérhető nagyobb folyam.
  //
  // Mivel az utolsó BFS futtatása után a parent tömbben a source-ból
  // elérhető csúcsoknak != -1 az értékük, ezért ők vannak pont a vágásban.
  vector<int> cut() {
    vector<int> result;
    for (int i = 0; i < v; ++i) {
      if (parent[i] != -1) {
        result.push_back(i);
      }
    }
    return result;
  }
};

// Egy testcase megoldása.
void solve() {

  // A jégtáblák darabszáma.
  // 1 <= ... <= 100
  int floes; 

  // Az összes pingvin darabszáma,
  // majd összeszámoljuk ebbe a változóba.
  int penguins = 0;

  // Max ugorható távolság.
  // 0 <= ... <= 100,000
  double maxd;

  cin >> floes >> maxd;

  vector<int> x(floes), y(floes), n(floes), m(floes);

  for(int i=0; i<floes; ++i) {
    // Az i. jégtábla x és y koordinátái.
    // -10,000 <= ... <= 10,000
    int xi, yi;
    
    // Az i. jégtáblán álló pingvinek száma.
    // 0 <= ... <= 10
    int ni;

    // Hány pingvin tud elugrani a jégtábláról.
    // 1 <= ... <= 200
    int mi;

    cin >> xi >> yi >> ni >> mi;
    x[i] = xi; y[i] = yi; n[i] = ni; m[i] = mi;

    // Számoljuk az összes pingvint.
    penguins += ni;
  }

  debug(x);
  debug(y);
  debug(n);
  debug(m);

  // Hálózat konstrukciója a következő módon:
  // 
  // Az i. jégtáblára (i = 0..floes-1):
  // 1. Csúcskapacitások miatt széthúzunk minden csúcsot egy élre.
  //    - Eredetileg i. csúcs helyett egy 2*i -> 2*i+1 él fog mutatni.
  //    - Ennek a kapacitása az mi "hány pingvin tud elugrani az i. tábláról" értékkel egyezik.
  // 2. Amelyik jégtáblák elég közel vannak, hogy át lehessen köztük ugrani:
  //    - A j. jégtáblára (i!=j):
  //        - 2*i+1 -> 2*j él mutat INT_MAX kapacitással.
  //        - 2*j+1 -> 2*i él mutat INT_MAX kapacitással.
  //       Ettől az eredeti i-j jégtáblák között oda-vissza van keletkezett végtelen kapacitású út.
  //       (A végpontját kötöttük a "csúcskapacitás" élének a másik kezdőpontja irányába,
  //        tehát tovább lehet menni majd a csúcskapacitáson.)
  // 3. +1 mesterséges source:
  //    Ő fogja biztosítani a most épp a jégtáblán álló pingvineket.
  //    - Az i. jégtáblán ni db pingvin van, ez megfelel egy source -> 2*i élnek.
  //      (Ha ezek a pingvinek el akarnak ugrani, akkor át kell menniük a csúcskapacitáson.)
  //    - A source indexe legyen 2*jégtáblák száma = 2*floes.
  // 4. Targetek:
  //    - Mivel a feladat azt kérdezi melyik táblákra tudnak összegyűlni a pingvinek,
  //      ezért minden táblát ki kell próbálni target-ként. Vagyis n db folyamot kell futtatni/testcase.
  //    - Az i. jégtáblához a target a 2*i lesz, mivel a megérkezés kell csak, nem kell elugraniuk,
  //      nem kell átmenniük a csúcskapacitáson.

  int v = 2*floes + 1;
  vector<vector<int>> capacity(v, vector<int>(v));
  int s = 2*floes;

  for(int i=0; i<floes; ++i) {
    // Csúcskapacitások.
    capacity[2*i][2*i+1] = m[i];
    // Kiindulási pingvineket a mesterséges source biztosítja.
    capacity[s][2*i] = n[i];

    // Minden i-j párra (i!=j):
    for(int j=0; j<floes; ++j) if (i!=j) {
      // Pitagorasz tétel: elég közel vannak-e a jégtáblák.
      //
      // Fontos, hogy ne akarjunk gyököt vonni, dolgozzunk a négyzetekkel
      // és hasonlítsuk azokat össze, így floating point pontosságból nem vesztünk.
      //
      // A feladat nem adott meg epsilon határértéket amin belül elfogadható lenne
      // a pontossági hiba, ezért biztosan nem lehet gyököt vonni!
      auto distsq = (x[i]-x[j]) * (x[i]-x[j]) + (y[i]-y[j]) * (y[i]-y[j]);

      // Ha elérhetőek a jégtáblák akkor összekötjük őket, oda-vissza.
      if(distsq <= maxd * maxd) {
        capacity[2*i+1][2*j] = INT_MAX;
        capacity[2*j+1][2*i] = INT_MAX;
      }
    }
  }

  debug(capacity);
  EdmondsKarp flow(capacity);

  bool found = false;
  // Végigmegyünk minden jégtáblán mint lehetséges target.
  for(int i=0; i<floes; ++i) {
    int t = 2*i;
    debug("BEGIN TO TARGET:", t);
    // s,t,v = source, target, num of vertices

    // Futtatjuk a folyam algot.
    auto maxflow = flow.run(s, t);
    debug(maxflow);
    // Amúgy a vágásra nincs szükségünk, de azt ezen a ponton lehet elkérni.
    auto cut = flow.cut();
    debug(cut);

    // Ha minden pingvin el tud menni a jégtáblára, akkor ez egy jó megoldás.
    if (maxflow == penguins) {
      cout << (found ? " ": "") << i;
      found = true;
    }
  }

  if(!found) {
      cout << -1;
    }

  cout << endl;
}

int main() {
  int cases; cin>>cases;

  while(cases--) {
    solve();
  }

  return 0;
}
