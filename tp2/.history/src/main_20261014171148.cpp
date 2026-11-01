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
        // Coordenadas da entrada começam em 1
        int s = (r1 - 1) * e.W + (c1 - 1);
        int t = (r2 - 1) * e.W + (c2 - 1);
        e.consultas[k].s = s;
        e.consultas[k].t = t;
    }
    return e;
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