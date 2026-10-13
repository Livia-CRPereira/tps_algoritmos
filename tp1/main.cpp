#include <iostream>
#include "bfs.hpp"


// tFas: número em cada nó representando em quanto tempo o grupo de Fãs mais próximo chega ao nó
// passos: número de movimentos do artista até cada célula
// caminhos: quantos caminhos mínimos chegam em cada célula (artista)

int tFas[MAXN];
int passos[MAXN];
long long caminhos[MAXN];

int main() {

    // Ler entrada
    int L, C, V;

    std::cin >> L >> C >> V;

    std::string mapa[MAXL];
    for (int i = 0; i < L; i++){
        std::cin >> mapa[i];
    }

    // Achar as posições de A, C e F
    int colA = -1, linA = -1, colCam = -1, linCam = -1;

    int linhaF[MAXN], colunaF[MAXN];
    int totalF = 0;

    for (int i = 0; i < L; i++){
        for (int j = 0; j < C; j++){
            if (mapa[i][j] == 'A'){
                linA = i;
                colA = j;
            }
            else if (mapa[i][j] == 'C'){
                linCam = i;
                colCam = j;
            }
            else if (mapa[i][j] == 'F'){
                linhaF[totalF] = i;
                colunaF[totalF] = j;
                totalF++;
            }
        }
    }
    
    bfsFas(mapa, L, C, linhaF, colunaF, totalF, tFas);
    bfsArtista(mapa, L, C, V, linA, colA, tFas, passos, caminhos);

    // Olhar se o artista chegou ao camarim
    int idxCamarim = linCam * C + colCam;

    if (passos[idxCamarim] == -1) {
        std::cout << "-1 0\n";
    } else {
        std::cout << passos[idxCamarim] << " " << caminhos[idxCamarim] << "\n";
    }

    return 0;
}
