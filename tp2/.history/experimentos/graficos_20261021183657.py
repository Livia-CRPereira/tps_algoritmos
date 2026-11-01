"""Gera os gráficos comparativos a partir das tabelas de resultados.

Rodar a partir da pasta tp2/, depois de run_experimentos.py:
    python3 experimentos/graficos.py

Lê experimentos/resultados/tabela_*.csv e salva os gráficos em
experimentos/graficos/ (PNG e PDF). Requer matplotlib.
Pontos em que uma solução ultrapassou o timeout não são desenhados.
"""
import csv
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

RAIZ = Path(__file__).resolve().parent.parent
PASTA_RES = RAIZ / "experimentos" / "resultados"
PASTA_GRAF = RAIZ / "experimentos" / "graficos"

ESTILO = {
    "guloso":  {"label": "Guloso",              "marker": "o", "color": "#c0392b"},
    "divconq": {"label": "Divisão e conquista", "marker": "s", "color": "#2471a3"},
}


def ler(nome):
    caminho = PASTA_RES / f"tabela_{nome}.csv"
    if not caminho.exists():
        return None
    with open(caminho) as f:
        return list(csv.DictReader(f))


def serie(linhas, eixo_x, solucao):
    """Pares (x, tempo) da solução, ignorando execuções com timeout."""
    pontos = [(int(l[eixo_x]), float(l[f"{solucao}_s"]))
              for l in linhas if l[f"{solucao}_s"] != ""]
    return [p[0] for p in pontos], [p[1] for p in pontos]


def salvar(fig, nome):
    PASTA_GRAF.mkdir(parents=True, exist_ok=True)
    for ext in ("png", "pdf"):
        fig.savefig(PASTA_GRAF / f"{nome}.{ext}", bbox_inches="tight", dpi=200)
    plt.close(fig)
    print(f"-> experimentos/graficos/{nome}.png e .pdf")


def grafico_linha(nome, eixo_x, rotulo_x, titulo, log_x):
    linhas = ler(nome)
    if linhas is None:
        print(f"tabela_{nome}.csv não encontrada; gráfico ignorado")
        return
    fig, ax = plt.subplots(figsize=(6, 4))
    for solucao, estilo in ESTILO.items():
        x, y = serie(linhas, eixo_x, solucao)
        ax.plot(x, y, linewidth=1.5, markersize=5, **estilo)
    if log_x:
        ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel(rotulo_x)
    ax.set_ylabel("Tempo (s, mediana)")
    H = linhas[0]["H"]
    fixo = f"W = {linhas[0]['W']}" if eixo_x == "Q" else f"Q = {linhas[0]['Q']}"
    ax.set_title(f"{titulo} (H = {H}, {fixo})")
    #ax.grid(True, which="both", alpha=0.3)
    ax.legend()
    salvar(fig, f"grafico_{nome}")


def grafico_instancias():
    linhas = ler("instancias")
    if linhas is None:
        print("tabela_instancias.csv não encontrada; gráfico ignorado")
        return
    nomes = [l["instancia"] for l in linhas]
    rotulos = [f"{l['instancia']}\nH={l['H']} W={l['W']}\nQ={l['Q']}" for l in linhas]
    x = range(len(nomes))
    largura = 0.38
    sem_tempo = []
    fig, ax = plt.subplots(figsize=(8, 4.2))
    for i, (solucao, estilo) in enumerate(ESTILO.items()):
        tempos = [float(l[f"{solucao}_s"]) if l[f"{solucao}_s"] != "" else 0 for l in linhas]
        pos = [xi + (i - 0.5) * largura for xi in x]
        ax.bar(pos, tempos, largura, label=estilo["label"], color=estilo["color"])
        for p, l in zip(pos, linhas):
            if l[f"{solucao}_s"] == "":
                sem_tempo.append((p, estilo["color"]))
    ax.set_yscale("log")
    base = ax.get_ylim()[0]
    for p, cor in sem_tempo:
        ax.text(p, base * 1.3, "timeout", ha="center", va="bottom",
                fontsize=8, rotation=90, color=cor)
    ax.set_xticks(list(x))
    ax.set_xticklabels(rotulos, fontsize=8)
    ax.set_ylabel("Tempo (s, mediana)")
    ax.set_title("Tempo por instância")
    #ax.grid(True, axis="y", which="both", alpha=0.3)
    ax.legend()
    salvar(fig, "grafico_instancias")


if __name__ == "__main__":
    grafico_linha("varia_Q", "Q", "Número de consultas Q", "Tempo × Q", log_x=True)
    grafico_linha("varia_W", "W", "Número de colunas W", "Tempo × W", log_x=False)
    grafico_instancias()