#include "main.hpp"
#include <iostream>

using namespace std;

Entrada lerEntrada() {
    Entrada e;
    cin >> e.H >> e.W;

    // A grade é lida linha por linha, então a k-ésima leitura
    // corresponde exatamente à posição i*W + j
    e.custo.resize(e.H * e.W);
    for (int k = 0; k < e.H * e.W; k++) {
        cin >> e.custo[k];
    }

    int Q;
    cin >> Q;
    e.consultas.resize(Q);
    for (int k = 0; k < Q; k++) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        // Coordenadas da entrada começam em 1, vetor começa em 0 > subtrair 1
        int s = (r1 - 1) * e.W + (c1 - 1);
        int t = (r2 - 1) * e.W + (c2 - 1);
        e.consultas[k].s = s;
        e.consultas[k].t = t;
    }
    return e;
}

Grafo construirGrafo(int H, int W, const vector<ll>& c) {
    Grafo adj(H * W);
    
    // Vetores de deslocamento para as 4 direções: cima, baixo, esquerda, direita
    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int u = i * W + j;
            
            for (int k = 0; k < 4; ++k) {
                int ni = i + di[k];
                int nj = j + dj[k];
                
                // Verifica se as novas coordenadas estão dentro dos limites da grade
                if (ni >= 0 && ni < H && nj >= 0 && nj < W) {
                    int v = ni * W + nj;
                    adj[u].push_back({v, c[v]});
                }
            }
        }
    }
    
    return adj;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Entrada e = lerEntrada();
    vector<ll> respostas = resolver(e);

    for (ll r : respostas) {
        cout << r << '\n';
    }
    return 0;
}