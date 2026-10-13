#ifndef BFS_HPP
#define BFS_HPP

#include <string>

const int MAXN = 10001;
const int MAXL = 101; 
const int INF = 1000000;
const long long MOD = 1000000007;

void bfsFas(const std::string mapa[], int L, int C,
            const int linhaF[], const int colunaF[], int totalF,
            int tFas[]);

void bfsArtista(const std::string mapa[], int L, int C, int V,
                int linA, int colA,
                const int tFas[], int passos[], long long caminhos[]);

#endif