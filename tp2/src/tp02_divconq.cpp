#include "main.hpp"
#include <functional>
#include <queue>

using namespace std;

// Dijkstra completo a partir de 'origem', andando só nas colunas [l, r].
// Devolve dist com índice LOCAL: (v / W) * largura + (v % W - l)
vector<ll> dijkstraFaixa(int origem, int l, int r, int W, const Grafo& adj) {
    int H = adj.size() / W;
    int largura = r - l + 1;
    vector<ll> dist(H * largura, INF); // tamanho da faixa

    // Converte o índice global de um vértice para o índice local da faixa
    auto local = [&](int v) { return (v / W) * largura + (v % W - l); };

    dist[local(origem)] = 0;

    // Fila de prioridade (min-heap) armazenando pares (distância, vértice)
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({0, origem});

    while (!pq.empty()) {
        ll d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Ignora entradas desatualizadas (quando achamos um caminho menor antes de processar esse)
        if (d > dist[local(u)]) {
            continue;
        }

        // Percorre os vizinhos de u
        for (const auto& aresta : adj[u]) {
            int v = aresta.first;
            ll w = aresta.second;

            // Pula vizinho fora da faixa de colunas [l, r]
            if (v % W < l || v % W > r) {
                continue;
            }

            // Relaxamento da aresta
            if (dist[local(u)] + w < dist[local(v)]) {
                dist[local(v)] = dist[local(u)] + w;
                pq.push({dist[local(v)], v});
            }
        }
    }

    // Distâncias da origem até todas as células da faixa (índice local)
    return dist;
}


void dividir(int l, int r, const vector<int>& ids,
             const Entrada& e, const Grafo& adj, vector<ll>& ans) {
    // caso-base: ids vazio ou l > r
    if (ids.empty() || l > r) {
        return;
    }

    // Encontrar a coluna do meio (mid)
    int mid = l + (r - l) / 2;

    // Converte o índice global para o índice local da faixa [l, r],
    // o mesmo usado pelo vetor devolvido por dijkstraFaixa
    int largura = r - l + 1;
    auto local = [&](int v) { return (v / e.W) * largura + (v % e.W - l); };

    // Para cada linha k, calcular o Dijkstra a partir da célula (k, mid)
    for (int k = 0; k < e.H; ++k) {
        int u_mid = k * e.W + mid; // Índice da célula na coluna mid e linha k

        // Distâncias de u_mid para todas as células da faixa de colunas [l, r]
        vector<ll> distK = dijkstraFaixa(u_mid, l, r, e.W, adj);

        // Atualiza a resposta de cada consulta da faixa com o melhor caminho
        // que passa por u_mid: custo(s..u_mid..t) = c[u_mid] + distK[s] + distK[t]
        // (a faixa é conexa, então distK[s] e distK[t] são sempre finitos)
        for (int q : ids) {
            int s = e.consultas[q].s;
            int t = e.consultas[q].t;
            ans[q] = min(ans[q], e.custo[u_mid] + distK[local(s)] + distK[local(t)]);
        }
    }

    // Separar as consultas (ids) em esq e dir
    vector<int> esq, dir;
    for (int q : ids) {
        int s = e.consultas[q].s;
        int t = e.consultas[q].t;

        // O índice da célula é (linha * W + coluna)
        int col_s = s % e.W;
        int col_t = t % e.W;

        if (col_s < mid && col_t < mid) {
            // Ambas as células estão estritamente à esquerda de mid
            esq.push_back(q);
        } else if (col_s > mid && col_t > mid) {
            // Ambas as células estão estritamente à direita de mid
            dir.push_back(q);
        }
        // Se a consulta cruza ou toca a coluna mid, todo caminho entre s e t
        // passa por mid e já foi considerado acima: a consulta não desce.
    }

    // Chamadas recursivas dividindo o problema
    dividir(l, mid - 1, esq, e, adj, ans);
    dividir(mid + 1, r, dir, e, adj, ans);
}

vector<ll> resolver(const Entrada& e, const Grafo& adj) {
    int Q = e.consultas.size();
    vector<ll> ans(Q, INF);

    vector<int> ids(Q);
    for (int q = 0; q < Q; q++) ids[q] = q;

    dividir(0, e.W - 1, ids, e, adj, ans);
    return ans;
}