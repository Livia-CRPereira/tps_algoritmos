"""Gerador de instâncias do TP2 (Rotas na Mina).

Uso pela linha de comando:
    python3 gen.py H W Q SEED [CUSTO_MAX] > entrada.txt

Custos sorteados uniformemente em [0, CUSTO_MAX] (padrão: 10^6, o limite do
enunciado); origem e destino de cada consulta sorteados uniformemente na grade.
A mesma seed gera sempre a mesma instância.
"""
import random
import sys


def gerar(H, W, Q, seed, custo_max=10**6):
    """Devolve o texto de uma instância no formato de entrada do TP."""
    rng = random.Random(seed)
    linhas = [f"{H} {W}"]
    for _ in range(H):
        linhas.append(" ".join(str(rng.randint(0, custo_max)) for _ in range(W)))
    linhas.append(str(Q))
    for _ in range(Q):
        linhas.append(f"{rng.randint(1, H)} {rng.randint(1, W)} "
                      f"{rng.randint(1, H)} {rng.randint(1, W)}")
    return "\n".join(linhas) + "\n"


if __name__ == "__main__":
    if len(sys.argv) not in (5, 6):
        sys.exit("uso: python3 gen.py H W Q SEED [CUSTO_MAX]")
    H, W, Q, seed = map(int, sys.argv[1:5])
    custo_max = int(sys.argv[5]) if len(sys.argv) == 6 else 10**6
    sys.stdout.write(gerar(H, W, Q, seed, custo_max))