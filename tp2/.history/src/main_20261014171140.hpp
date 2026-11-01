#ifndef MAIN_HPP
#define MAIN_HPP

#include <cstdint>
#include <vector>

typedef std::int64_t ll;

// Uma consulta: origem e destino já convertidos para índice (i*W + j)
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

// Responde todas as consultas, na ordem da entrada.
// Implementada em tp02_guloso.cpp e em tp02_divconq.cpp.
std::vector<ll> resolver(const Entrada& e);

#endif