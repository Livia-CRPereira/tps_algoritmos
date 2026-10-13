#include "bfs.hpp"

static bool vizinhoValido(const std::string mapa[], int L, int C, int idx) {
    if (idx < 0 || idx >= L*C) return false;
    int i = idx / C;
    int j = idx % C;
    return mapa[i][j] != '#';
}

void bfsFas(const std::string mapa[], int L, int C,
            const int linhaF[], const int colunaF[], int totalF,
            int tFas[]) {

    // marcar todos como não visitados
    for (int i = 0; i < L*C; i++) {
        tFas[i] = INF;
    }
    
    // colocar todos os fãs na fila 
    int atual = 0, fim = 0;
    int fila[MAXN];

    for (int i = 0; i < totalF; i++) {
        // índice da célula onde começa esse grupo de fãs
        int idx = linhaF[i] * C + colunaF[i];
        tFas[idx] = 0;
        fila[fim] = idx;
        fim++;
    }

    while (atual < fim){
        int no = fila[atual];
        atual++;

        int j = no % C;

        int vizinhos[4];
        vizinhos[0] = no - C;                          // cima
        vizinhos[1] = no + C;                          // baixo
        vizinhos[2] = (j + 1 < C) ? no + 1 : -1;        // direita
        vizinhos[3] = (j - 1 >= 0) ? no - 1 : -1;       // esquerda
        
        for (int k = 0; k < 4; k++){
            if(vizinhoValido(mapa, L, C, vizinhos[k]) && tFas[vizinhos[k]] == INF){
                tFas[vizinhos[k]] = tFas[no] + 1;
                fila[fim] = vizinhos[k];
                fim++;
            }
        }
    }

}

void bfsArtista(const std::string mapa[], int L, int C, int V,
                int linA, int colA,
                const int tFas[], int passos[], long long caminhos[]){

    // marcar todos como não visitados
    for (int i = 0; i < L*C; i++) {
        passos[i] = -1;
        caminhos[i] = 0;
    }

    // colocar o artista na fila
    int atual = 0, fim = 0;
    int fila[MAXN];

    int idx = linA * C + colA;
    passos[idx] = 0;
    caminhos[idx] = 1;
    fila[fim] = idx;
    fim++;
    
    while (atual < fim){
        int no = fila[atual];
        atual++;

        int j = no % C;

        int vizinhos[4];
        vizinhos[0] = no - C;                          // cima
        vizinhos[1] = no + C;                          // baixo
        vizinhos[2] = (j + 1 < C) ? no + 1 : -1;        // direita
        vizinhos[3] = (j - 1 >= 0) ? no - 1 : -1;       // esquerda
        
        for (int k = 0; k < 4; k++){
            int viz = vizinhos[k];

            if (!vizinhoValido(mapa, L, C, viz)) continue;
            if (passos[no] + 1 >= V * tFas[viz]) continue;
            
            if (passos[viz] == -1) {
                passos[viz] = passos[no] + 1;
                caminhos[viz] = caminhos[no];
                fila[fim] = viz;
                fim++;
            } else if (passos[viz] == passos[no] + 1) {
                caminhos[viz] = (caminhos[viz] + caminhos[no]) % MOD;
            }

        }

    }
}