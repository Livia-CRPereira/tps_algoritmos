#include "main.hpp"
#include <iostream>

using namespace std;

Entrada lerEntrada() {
    Entrada ent;
    cin >> ent.H >> ent.W;

    // A grade é lida linha por linha, então a k-ésima leitura
    // corresponde exatamente à posição i*W + j
    ent.custo.resize(ent.H * ent.W);
    for (int k = 0; k < ent.H * ent.W; k++) {
        cin >> ent.custo[k];
    }

    int Q;
    cin >> Q;
    ent.consultas.resize(Q);
    for (int k = 0; k < Q; k++) {
        int linhaOrigem, colunaOrigem, linhaDestino, colunaDestino;
        cin >> linhaOrigem >> colunaOrigem >> linhaDestino >> colunaDestino;
        // Coordenadas da entrada começam em 1, vetor começa em 0 > subtrair 1
        int s = (linhaOrigem - 1) * ent.W + (colunaOrigem - 1);
        int t = (linhaDestino - 1) * ent.W + (colunaDestino - 1);
        ent.consultas[k].s = s;
        ent.consultas[k].t = t;
    }
    return ent;
}

Grafo construirGrafo(int H, int W, const std::vector<ll>& c) {
    Grafo adj(H * W);
    
    // Vetores de deslocamento para as 4 direções: cima, baixo, esquerda, direita
    // i é linha, j é coluna
    int deslocamento_i[] = {-1, 1, 0, 0};
    int deslocamento_j[] = {0, 0, -1, 1};

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int u = i * W + j;
            
            for (int k = 0; k < 4; ++k) {
                int novo_i = i + deslocamento_i[k];
                int novo_j = j + deslocamento_j[k];
                
                // Verifica se as novas coordenadas estão dentro dos limites da grade
                if (novo_i >= 0 && novo_i < H && novo_j >= 0 && novo_j < W) {
                    int v = novo_i * W + novo_j;
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
    cin.tie(nullptr);

    Entrada e = lerEntrada();
    Grafo adj = construirGrafo(e.H, e.W, e.custo);
    vector<ll> respostas = resolver(e, adj);

    for (ll r : respostas) {
        cout << r << '\n';
    }
    return 0;
}