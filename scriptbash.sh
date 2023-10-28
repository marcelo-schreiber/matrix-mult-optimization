#!/bin/bash

# Função para escrever o tamanho no arquivo _results.csv
write_size_to_results_file() {
    metric="$1"
    size="$2"
    echo "$size" >> "${metric}_results.csv"
}

# Função para executar likwid-perfctr e adicionar a saída ao arquivo de log correspondente à métrica
run_likwid_perfctr() {
    metric="$1"
    size="$2"
    log_file="${metric}.csv"

    # Se o arquivo ainda não existe, escreva o tamanho no início do arquivo
    if [ ! -e "${metric}_results.csv" ]; then
        write_size_to_results_file "$metric" "$size"
    fi

    likwid-perfctr -C 3 -g "$metric" -m ./matmult "$size" > "$log_file"

    # Grep para extrair linhas específicas dependendo da métrica e aplicar o awk para pegar a segunda coluna
    case "$metric" in
        "MEM")
            grep -i "Memory load bandwidth" "$log_file" | awk -F ' ' '{print $7}' >> "${metric}_results.csv"
            ;;
        "L2CACHE")
            grep -i "L2 miss ratio" "$log_file" | awk -F ' ' '{print $6}' >> "${metric}_results.csv"
            ;;
        "ENERGY")
            grep -i "Energy \[J\]" "$log_file" | awk -F ' ' '{print $5}' >> "${metric}_results.csv"
            ;;
        "FLOPS_DP")
            grep -i "MFLOP/s" "$log_file" | awk -F '|' '{print $3}' >> "${metric}_results.csv"
            ;;
        *)
            echo "Métrica não reconhecida: $metric"
            ;;
    esac
}

# Métricas ENERGY, FLOPS_DP e CACHE
metrics=("MEM" "L2CACHE" "ENERGY" "FLOPS_DP")

# Tamanhos N
sizes=(64 100 128 200 256 512 600 900 1024 2000 2048 3000 4000)

rm -f *.csv

# Loop para escrever o tamanho no início e executar likwid-perfctr para cada métrica e tamanho
for size in "${sizes[@]}"; do
    for metric in "${metrics[@]}"; do
        write_size_to_results_file "$metric" "$size"
        run_likwid_perfctr "$metric" "$size"
    done
done

rm -f ENERGY.csv FLOPS_DP.csv L2CACHE.csv MEM.csv
