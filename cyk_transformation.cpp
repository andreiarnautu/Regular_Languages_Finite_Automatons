/**
  *  Worg
  */

#include <set>
#include <cstdio>
#include <vector>
#include <fstream>

std::ifstream fin("cyk.in"); std::ofstream fout("cyk.out");

const int inf = 1e9;
const int max_n = 100;
const int sigma = 26;

/*-------- Data --------*/
std::set<std::pair<int, int> > prod_term;
std::vector<std::vector<std::pair<int, int > > > prod_nonterm;
std::string str;

int dp[max_n][max_n][sigma];
/*-------- --------*/

int value(char c) {
    if ('a' <= c && c <= 'z') {
        return c - 'a';
    } else {
        return c - 'A';
    }
}

void ReadInput() {
    int term; fin >> term;
    for (int i = 0; i < term; i++) {
        char a, b; fin >> a >> b;
        prod_term.emplace(value(a), value(b));
    }

    prod_nonterm.resize(sigma);

    int nonterm; fin >> nonterm;
    for (int i = 0; i < nonterm; i++) {
        char a, b, c; fin >> a >> b >> c;
        prod_nonterm[value(a)].emplace_back(value(b), value(c));
    }

    fin >> str;
}

void ComputeDp() {
    const int n = (int)str.size();

    for (int i = 0; i < max_n; i++) {
        for (int j = 0; j < max_n; j++) {
            for (int k = 0; k < sigma; k++) {
                dp[i][j][k] = inf;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < sigma; j++) {
            dp[i][i][j] = 1;  //  adaugam o litera
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < sigma; j++) {
            if (prod_term.find(std::make_pair(j, value(str[i]))) != prod_term.end()) {
                dp[i][i + 1][j] = 0;  //  nu trebuie sa facem schimbare
                fout << "ok: " << i << " " << (char)('a' + j) << '\n';
            } else {
                dp[i][i + 1][j] = 1;  //  facem o schimbare
            }
        }
    }

    for (int i = n - 2; i >= 0; i--) {
        for (int j = i + 2; j <= n; j++) {
            for (int k = 0; k < sigma; k++) {
                //  primul tip de operatie: stergeri
                dp[i][j][k] = std::min(dp[i][j][k], 1 + std::min(dp[i + 1][j][k], dp[i][j - 1][k]));

                //  al doilea tip de operatie: schimbari
                for (int x = i; x <= j; x++) {
                    for (auto& p : prod_nonterm[k]) {
                        dp[i][j][k] = std::min(dp[i][j][k], dp[i][x][p.first] + dp[x][j][p.second]);
                    }
                }
            }

            //  al treilea tip de operatii: adaugari
            for (int k = 0; k < sigma; k++) {
                for (auto& p : prod_nonterm[k]) {
                    dp[i][j][k] = std::min(dp[i][j][k], dp[i][j][p.first] + dp[j][j][p.second]);
                }
            }
        }
    }

    fout << dp[0][n][0];
}

int main() {
    ReadInput();

    ComputeDp();

    return 0;
}
