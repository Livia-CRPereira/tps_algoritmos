#!/bin/sh
# Registra o ambiente de execução dos experimentos.
# Uso, a partir da pasta tp2/:  sh experimentos/ambiente.sh
mkdir -p experimentos/resultados
SAIDA=experimentos/resultados/ambiente.txt
{
    echo "== Data";             date
    echo; echo "== Sistema";    uname -a
    echo; echo "== CPU";        lscpu | grep -E "Model name|^CPU\(s\)|Thread|MHz" 
    echo; echo "== Memoria";    free -h | head -2
    echo; echo "== Compilador"; g++ --version | head -1
    echo; echo "== Python";     python3 --version
} > "$SAIDA"
echo "-> $SAIDA"