#include "main.hpp"
#include <functional>
#include <queue>

using namespace std;

ll dijkstra(int s, int t, const Grafo& adj, const vector<ll>& c) {
    int num_vertices = adj.size(); // Corresponde a H*W
    vector<ll> dist(num_vertices, INF);
    dist[s] = 0;

    // Fila de prioridade (min-heap) armazenando pares {distância, vértice}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push({0, s});

    while (!pq.empty()) {
        ll d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Ignora entradas desatualizadas (quando achamos um caminho menor antes de processar esse)
        if (d > dist[u]) {
            continue;
        }

        // Se chegamos no destino, a distância dist[t] já é definitiva
        if (u == t) {
            break;
        }

        // Percorre os vizinhos de u
        for (const auto& aresta : adj[u]) {
            int v = aresta.first;
            ll w = aresta.second;

            // Relaxamento da aresta
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    // Retorna o custo de s + a distância mínima calculada até t
    return c[s] + dist[t];
}

vector<ll> resolver(const Entrada& e, const Grafo& adj) {

    vector<ll> respostas;
    for (const Consulta& q : e.consultas) {
        respostas.push_back(dijkstra(q.s, q.t, adj, e.custo));
    }
    return respostas;
}