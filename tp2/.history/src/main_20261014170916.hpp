#ifndef MAIN_HPP
#define MAIN_HPP

#include <vector>
#include <utility>

typedef long long ll;

// Dados lidos da entrada padrão
struct Entrada {
    int H, W;
    std::vector<ll> custo;                       // custo[i*W + j], índices a partir de 0
    std::vector<std::pair<int, int>> consultas;  // (origem, destino) já convertidos para índice
};

// Lê um caso de teste completo da entrada padrão (implementada em main.cpp)
Entrada lerEntrada();

// Responde todas as consultas, na ordem da entrada.
// Implementada em guloso.cpp (tp02-guloso) e em divconq.cpp (tp02-divconq).
std::vector<ll> resolver(const Entrada& e);

#endif