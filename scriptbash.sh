#!/bin/bash

# Nome do arquivo de log
log_file="resultados.txt"

# Função para executar likwid-perfctr e adicionar a saída ao arquivo de log
run_likwid_perfctr() {
    metric="$1"
    size="$2"
    likwid-perfctr -C 3 -g "$metric" -m ./matmult "$size" >> "$log_file"
}

# Métricas ENERGY, FLOPS_DP e CACHE
metrics=("ENERGY" "FLOPS_DP" "CACHE")

# Tamanhos N
sizes=(64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000)

# Certifique-se de que o arquivo de log esteja vazio no início
> "$log_file"

# Loop para executar likwid-perfctr para cada métrica e tamanho
for metric in "${metrics[@]}"; do
    for size in "${sizes[@]}"; do
        run_likwid_perfctr "$metric" "$size"
    done
done
