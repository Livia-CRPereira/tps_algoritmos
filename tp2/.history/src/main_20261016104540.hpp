#ifndef MAIN_HPP
#define MAIN_HPP

#include <utility>
#include <cstdint>
#include <vector>

typedef std::int64_t ll;

const ll INF = INT64_MAX;
typedef std::vector<std::vector<std::pair<int, ll>>> Grafo; //lista de adjacencia

// Origem e destino com índice (i*W + j)
struct Consulta {
    int s, t;
};

// Dados lidos da entrada padrão
struct Entrada {
    int H, W;
    std::vector<ll> custo;            // custo[i*W + j], índices a partir de 0
    std::vector<Consulta> consultas;  // na ordem da entrada
};

// Lê um caso de teste completo da entrada padrão (implementada em main.cpp)
Entrada lerEntrada();

Grafo construirGrafo(int H, int W, const vector<ll>& c);

// Responde todas as consultas, na ordem da entrada.
// Implementada em 2 versões: em tp02_guloso.cpp e em tp02_divconq.cpp.

std::vector<ll> resolver(const Entrada& e, const Grafo& adj);

#endif