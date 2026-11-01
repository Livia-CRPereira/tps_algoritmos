"""Executa os experimentos do TP2 e gera as tabelas de resultados.

Rodar a partir da pasta tp2/, depois de compilar com make:
    python3 experimentos/run_experimentos.py

Opções:
    --repeticoes N   execuções por configuração (padrão: 5; usa-se a mediana)
    --timeout S      tempo máximo por execução, em segundos (padrão: 300)
    --apenas NOMES   roda só alguns experimentos: instancias, varia_W, varia_Q

Para cada configuração, a instância é gerada uma única vez (seed fixa) e salva
em experimentos/instancias/, de modo que as duas soluções recebem exatamente a
mesma entrada. O tempo de parede é medido externamente (time.perf_counter em
torno do subprocesso), sem nenhuma impressão de medição pelos programas.
As saídas das duas soluções são comparadas em toda configuração.
"""
import argparse
import csv
import statistics
import subprocess
import sys
import time
from pathlib import Path

from gen import gerar

RAIZ = Path(__file__).resolve().parent.parent
PASTA_EXP = RAIZ / "experimentos"
PASTA_INST = PASTA_EXP / "instancias"
PASTA_RES = PASTA_EXP / "resultados"
EXECUTAVEIS = {
    "guloso": RAIZ / "tp02-guloso",
    "divconq": RAIZ / "tp02-divconq",
}
SEED = 2026

# ---------------------------------------------------------------------------
# Configurações (H, W, Q)
# ---------------------------------------------------------------------------

# Experimento 1: cinco instâncias de tamanhos e combinações diferentes
INSTANCIAS = [
    ("pequena",    3,   100,   100),
    ("media",      4,  1000,  1000),
    ("grande_W",   5, 10000,  1000),   # W máximo, poucas consultas
    ("grande_Q",   5,  2000, 50000),   # Q máximo, W moderado
    ("grande_max", 5, 10000, 50000),   # restrições máximas
]

# Experimento 2: H pequeno e Q fixos, W variando
VARIA_W = [(5, W, 1000) for W in (500, 1000, 2000, 4000, 6000, 8000, 10000)]

# Experimento 3: H pequeno e W fixos, Q variando (inclui Q pequeno para
# localizar o ponto em que a divisão e conquista passa a ser vantajosa)
VARIA_Q = [(5, 2000, Q) for Q in (1, 10, 50, 100, 500, 1000, 5000, 10000, 25000, 50000)]


# ---------------------------------------------------------------------------
# Funções auxiliares
# ---------------------------------------------------------------------------

def instancia(nome, H, W, Q):
    """Gera (se ainda não existir) e devolve o caminho da instância."""
    PASTA_INST.mkdir(parents=True, exist_ok=True)
    caminho = PASTA_INST / f"{nome}_H{H}_W{W}_Q{Q}.txt"
    if not caminho.exists():
        caminho.write_text(gerar(H, W, Q, SEED))
    return caminho


def medir(executavel, entrada, repeticoes, timeout):
    """Executa o programa várias vezes e devolve (mediana em s, saída).

    Devolve (None, None) se alguma execução ultrapassar o timeout.
    """
    tempos = []
    saida = None
    for _ in range(repeticoes):
        with open(entrada, "rb") as f_in:
            inicio = time.perf_counter()
            try:
                proc = subprocess.run([str(executavel)], stdin=f_in,
                                      stdout=subprocess.PIPE, timeout=timeout,
                                      check=True)
            except subprocess.TimeoutExpired:
                return None, None
            tempos.append(time.perf_counter() - inicio)
        saida = proc.stdout
    return statistics.median(tempos), saida


def rodar(nome_exp, configs, args):
    """Mede as duas soluções em cada configuração e salva as tabelas."""
    linhas = []
    for nome, H, W, Q in configs:
        entrada = instancia(nome, H, W, Q)
        print(f"[{nome_exp}] {nome}: H={H} W={W} Q={Q}", flush=True)
        t_g, out_g = medir(EXECUTAVEIS["guloso"], entrada, args.repeticoes, args.timeout)
        t_d, out_d = medir(EXECUTAVEIS["divconq"], entrada, args.repeticoes, args.timeout)

        if out_g is None or out_d is None:
            iguais = "n/a"
        else:
            iguais = "sim" if out_g == out_d else "NAO"
            if iguais == "NAO":
                print("  ATENCAO: saidas diferentes!", flush=True)

        razao = t_g / t_d if (t_g is not None and t_d) else None
        linhas.append({
            "instancia": nome, "H": H, "W": W, "Q": Q,
            "guloso_s": t_g, "divconq_s": t_d,
            "razao": razao, "saidas_iguais": iguais,
        })
        print(f"  guloso: {fmt(t_g, args.timeout)}  divconq: {fmt(t_d, args.timeout)}"
              f"  razao: {fmt_razao(razao)}", flush=True)

    salvar_csv(nome_exp, linhas)
    salvar_tex(nome_exp, linhas, args.timeout)


def fmt(t, timeout):
    return f"> {timeout:g}" if t is None else f"{t:.4f}"


def fmt_razao(r):
    return "--" if r is None else f"{r:.1f}"


def salvar_csv(nome_exp, linhas):
    PASTA_RES.mkdir(parents=True, exist_ok=True)
    caminho = PASTA_RES / f"tabela_{nome_exp}.csv"
    with open(caminho, "w", newline="") as f:
        escritor = csv.DictWriter(f, fieldnames=list(linhas[0].keys()))
        escritor.writeheader()
        for linha in linhas:
            escritor.writerow({k: ("" if v is None else v) for k, v in linha.items()})
    print(f"  -> {caminho.relative_to(RAIZ)}")


def salvar_tex(nome_exp, linhas, timeout):
    """Tabela pronta para \\input{} no relatório (requer o pacote booktabs)."""
    caminho = PASTA_RES / f"tabela_{nome_exp}.tex"
    with open(caminho, "w") as f:
        f.write("\\begin{tabular}{lrrrrrr}\n\\toprule\n")
        f.write("Instância & $H$ & $W$ & $Q$ & Guloso (s) & D\\&C (s) & Razão \\\\\n\\midrule\n")
        for l in linhas:
            nome = l["instancia"].replace("_", "\\_")
            f.write(f"{nome} & {l['H']} & {l['W']} & {l['Q']} & "
                    f"{fmt(l['guloso_s'], timeout).replace('>', '$>$')} & "
                    f"{fmt(l['divconq_s'], timeout)} & {fmt_razao(l['razao'])} \\\\\n")
        f.write("\\bottomrule\n\\end{tabular}\n")
    print(f"  -> {caminho.relative_to(RAIZ)}")


# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--repeticoes", type=int, default=5)
    parser.add_argument("--timeout", type=float, default=300)
    parser.add_argument("--apenas", nargs="+", choices=["instancias", "varia_W", "varia_Q"],
                        default=["instancias", "varia_W", "varia_Q"])
    args = parser.parse_args()

    for nome, caminho in EXECUTAVEIS.items():
        if not caminho.exists():
            sys.exit(f"Executável {caminho} não encontrado. Rode 'make' na pasta tp2/.")

    if "instancias" in args.apenas:
        rodar("instancias", INSTANCIAS, args)
    if "varia_W" in args.apenas:
        rodar("varia_W", [(f"varia_W", H, W, Q) for H, W, Q in VARIA_W], args)
    if "varia_Q" in args.apenas:
        rodar("varia_Q", [(f"varia_Q", H, W, Q) for H, W, Q in VARIA_Q], args)


if __name__ == "__main__":
    main()