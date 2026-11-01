#include "main.hpp"
#include <functional>
#include <queue>
#include <utility>

using namespace std;

typedef vector<vector<pair<int, ll>>> Grafo; //lista de adjacencia!

const ll INF = INT64_MAX;

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

vector<ll> resolver(const Entrada& e) {
    Grafo adj = construirGrafo(e.H, e.W, e.custo);

    vector<ll> respostas;
    for (const Consulta& q : e.consultas) {
        respostas.push_back(dijkstra(q.s, q.t, adj, e.custo));
    }
    return respostas;
}