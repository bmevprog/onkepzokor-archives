#include <cassert>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <optional>
#include <iomanip>

using namespace std;

struct Owner {
    int owner;
    double share;
};

class SccFinder {
  public:
    vector<vector<int>> find(const vector<vector<Owner>>& graf) {
        vector<vector<int>> comps;
        int n = graf.size();
        low.assign(n, 0);
        dfs_num.assign(n, 0);
        component.assign(n, -1);

        for (int i = 0; i < n; ++i)
            if (dfs_num[i] == 0)
                scc_internal(i, graf, comps);
        return comps;
    }

  private:
    void scc_internal(int u, const vector<vector<Owner>>& graf,
                      vector<vector<int>>& comps) {
        int n = graf.size();
        low[u] = dfs_num[u] = dfs_num_counter++;
        S.push(u);

        for (const auto& v : graf[u]) {
            if (v.owner >= n)
                continue;
            if (dfs_num[v.owner] == 0) // not visited yet
                scc_internal(v.owner, graf, comps);
            if (dfs_num[v.owner] != -1) // not popped yet
                low[u] = min(low[u], low[v.owner]);
        }

        if (low[u] == dfs_num[u]) { // extract the component
            int cnum = comps.size();
            comps.push_back({}); // start new component
            int w;
            do {
                w = S.top();
                S.pop();
                comps.back().push_back(w);
                component[w] = cnum;
                dfs_num[w] = -1; // mark popped
            } while (w != u);
        }
    }

    int dfs_num_counter = 1;
    vector<int> low;
    vector<int> dfs_num;
    stack<int> S;

  public:
    vector<int> component; // maps vertex to its component
};

struct LUSolver;
class Matrix {
  public:
    Matrix() : n_(0), m_(0), data_() {}
    Matrix(int n, int m) : n_(n), m_(m), data_(n * m) {}
    double operator()(int i, int j) const {
        assert(0 <= i);
        assert(i < n_);
        assert(0 <= j);
        assert(j < m_);
        return data_[i * m_ + j];
    }
    double& operator()(int i, int j) {
        assert(0 <= i);
        assert(i < n_);
        assert(0 <= j);
        assert(j < m_);
        return data_[i * m_ + j];
    }
    Matrix operator*(const Matrix& other) const {
        assert(m_ == other.n_);
        Matrix result(n_, other.m_);
        for (int i = 0; i < n_; ++i) {
            for (int j = 0; j < other.m_; ++j) {
                for (int k = 0; k < m_; ++k) {
                    result(i, j) += this->operator()(i, k) * other(k, j);
                }
            }
        }
        return result;
    }
    Matrix inverse() const;

    int n() const { return n_; }
    int m() const { return m_; }
 private:

    optional<LUSolver> inplaceLUP() const;
    friend LUSolver;
  private:
    int n_, m_;
    vector<double> data_;
};

struct LUSolver {
    Matrix LU;
    vector<int> pivot;

    optional<vector<double>> solve(vector<double> b) {
        int n = LU.n_;
        assert(n == b.size());
        vector<double> x(n);
        int i, k;
        double* p_k;

        for (k = 0, p_k = &LU.data_[0]; k < n; p_k += n, k++) {
            if (pivot[k] != k) std::swap(b[k], b[pivot[k]]);
            x[k] = b[k];
            for (i = 0; i < k; i++) x[k] -= x[i] * p_k[i];
            x[k] /= p_k[k];
        }

        for (k = n - 1, p_k = &LU.data_[0] + n * (n - 1); k >= 0; k--, p_k -= n) {
            if (pivot[k] != k) std::swap(b[k], b[pivot[k]]);
            for (i = k + 1; i < n; i++) x[k] -= x[i] * p_k[i];
            if (p_k[k] == 0.0) return std::nullopt;
        }

        return {x};
    }
};
optional<LUSolver> Matrix::inplaceLUP() const {
    Matrix A = *this;
    assert(A.n_ == A.m_);
    int n = A.n_;
    vector<int> pivot(n);

    int i, j, k;
    double *p_k, *p_row, *p_col;

    for (k = 0, p_k = &A.data_[0]; k < n; p_k += n, k++) {
        pivot[k] = k;
        double max_val = abs(p_k[k]);
        for (j = k + 1, p_row = p_k + n; j < n; j++, p_row += n) {
            if (max_val < abs(p_row[k])) {
                max_val = abs(p_row[k]);
                pivot[k] = j;
                p_col = p_row;
            }
        }

        if (pivot[k] != k) {
            for (j = 0; j < n; j++) {
                max_val = p_k[j];
                p_k[j] = p_col[j];
                p_col[j] = max_val;
            }
        }

        if (p_k[k] == 0.0) return std::nullopt;
        for (j = k + 1; j < n; j++) p_k[j] /= p_k[k];
        for (i = k + 1, p_row = p_k + n; i < n; p_row += n, i++)
            for (j = k + 1; j < n; j++)
                p_row[j] -= p_row[k] * p_k[j];
    }
    return {{.LU = std::move(A), .pivot = std::move(pivot)}};
}
Matrix Matrix::inverse() const {
    assert(n_ == m_);
    optional<LUSolver> solver = inplaceLUP();
    assert(solver);

    Matrix result(m_, n_);
    for (int i = 0; i < n_; ++i) {
        vector<double> b(n_);
        b[i] = 1;
        optional<vector<double>> x = solver->solve(std::move(b));
        assert(x);
        for (int j = 0; j < n_; ++j) {
            result(j, i) = x->operator[](j);
        }
    }
    return result;
}

ostream& operator<<(ostream& os, const Matrix& A) {
    for (int i = 0; i < A.n(); ++i) {
        for (int j = 0; j < A.m(); ++j) {
            os << A(i, j) << ' ';
        }
        os << '\n';
    }
    return os;
}
bool isIdentity(const Matrix& A) {
    for (int i = 0; i < A.n(); ++i) {
        for (int j = 0; j < A.m(); ++j) {
            if (abs(A(i, j) - (i == j)) > 1e-5) {
                return false;
            }
        }
    }
    return true;
}


int main() {
    cin.sync_with_stdio(false);
    cin.tie(nullptr);

    int c, p, k;
    cin >> c >> p;
    assert(c >= 1);
    assert(p >= 1);
    assert(c <= 1000);
    assert(p <= 1000);

    char ch, colon, dot;
    int id, sh1, sh2;

    vector<vector<Owner>> owners(c);
    int ks = 0;
    for (int i = 0; i < c; ++i) {
        cin >> k;
        assert(k >= 1);
        ks += k;

        set<int> input_owners;
        for (int j = 0; j < k; ++j) {
            cin >> std::ws >> ch >> id >> colon >> sh1 >> dot >> sh2;
            --id;
            assert(ch == 'P' || ch == 'C');
            assert(colon == ':');
            int owner_id = ch == 'P' ? id + c : id;
            assert(input_owners.count(owner_id) == 0);
            input_owners.insert(owner_id);
            assert(dot == '.');
            assert(0 <= sh1);
            assert(sh1 <= 100);
            assert(0 <= sh2);
            assert(sh2 <= 9);
            int share = 10*sh1 + sh2;
            assert(share > 0);
            assert(share <= 1000);
            owners[i].push_back({owner_id, share / 1000.0});
        }
    }
    assert(ks <= 10000);

    bool hooman = false;
    for (int i = 0; i < c; ++i) {
        double s = 0;
        for (const Owner& o : owners[i]) {
            s += o.share;
            if (o.owner >= c) {
                hooman = true;
            }
        }
        assert(std::abs(s - 1) < 1e-6 && "shares don't sum to 1");
    }
    // not exhaustive, but required
    assert(hooman && "No human owners");

    SccFinder finder;
    vector<vector<int>> comps = finder.find(owners);
    int nc = comps.size();
    vector<int> local(c);
    for (int i = 0; i < nc; ++i) {
        int s = comps[i].size();
        for (int j = 0; j < s; ++j) {
            local[comps[i][j]] = j;
        }
    }
    vector<Matrix> comp_matrices(nc);
    for (int i = 0; i < nc; ++i) {
        int s = comps[i].size();
        Matrix M(s, s);
        for (int node : comps[i]) {
            for (const Owner& o : owners[node]) {
                if (o.owner >= c)
                    continue;
                if (finder.component[o.owner] != i)
                    continue;
                //                  cout << "i: " << i << "ow: " << o.owner <<
                //                  endl;
                M(local[o.owner], local[node]) = o.share;
            }
        }
        bool some_out = false;
        for (int j = 0; j < s; ++j) {
            double ss = 0;
            for (int k = 0; k < s; ++k) {
                ss += M(k, j);
            }
            if (ss + 1e-6 < 1) {
                some_out = true;
            }
            for (int k = 0; k < s; ++k) {
                M(j, k) = (j == k) - M(j, k);
            }
        }
        assert(some_out && "there must be some real ownership");
        comp_matrices[i] = M.inverse();
        assert(isIdentity(comp_matrices[i] * M));
    }


    vector<vector<double>> ownership_ratio(c, vector<double>(p, 0.0));
    for (int i = 0; i < nc; ++i) {
        int s = comps[i].size();
        Matrix L(c + p, s);
        for (int node : comps[i]) {
            for (const Owner& o : owners[node]) {
                if (o.owner < c && finder.component[o.owner] == i)
                    continue;
                L(o.owner, local[node]) = o.share;
            }
        }
        Matrix R = L * comp_matrices[i];
        for (int j = 0; j < s; ++j) {
            int f = comps[i][j];
            for (int x = 0; x < p; ++x) {
                ownership_ratio[f][x] += R(c + x, j);
            }
            for (int y = 0; y < c; ++y) {
                if (!R(y, j)) continue;
                for (int x = 0; x < p; ++x) {
                    ownership_ratio[f][x] += ownership_ratio[y][x] * R(y, j);
                }
            }
        }
    }

    cout << std::fixed << std::setprecision(15);
    for (int y = 0; y < c; ++y) {
        for (int x = 0; x < p; ++x) {
            cout << ownership_ratio[y][x];
            if (x != p-1) cout << ' ';
        }
        cout << '\n';
    }

    return 0;
}
