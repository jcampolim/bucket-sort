#!/bin/bash

# Para rodar esse arquivo, use o comando:
# ./teste.sh <caminho_do_codigo.c> <numero_de_execucoes> <numero_de_cores>
# EX: ./teste.sh mergesort/mergesort.c 5 4 
# Isso roda mergesort.c que está na pasta mergesort 5 vezes, usando 4 cores.

# Caminho para o arquivo C a ser compilado
codigo_c=$1
# Número de execuções
num_execucoes=$2
# Número de cores
num_cores=$3
# Arquivo de entrada
input_file="input"

# Verifica se os parâmetros foram passados
if [[ -z $codigo_c || -z $num_execucoes || -z $num_cores ]]; then
    echo "Uso correto: ./teste.sh <caminho_do_codigo.c> <numero_de_execucoes> <numero_de_cores>"
    exit 1
fi

# Verifica se o arquivo C existe
if [[ ! -f $codigo_c ]]; then
    echo "Arquivo de código '$codigo_c' não encontrado!"
    exit 1
fi

# Verifica se o arquivo de entrada 'input' existe
if [[ ! -f $input_file ]]; then
    echo "Arquivo de entrada '$input_file' não encontrado!"
    exit 1
fi

# Extrai o diretório do arquivo C e o nome base do arquivo
diretorio_codigo=$(dirname "$codigo_c")
nome_codigo=$(basename "$codigo_c" .c)

# Cria o caminho do arquivo de log em formato CSV, incluindo o número de cores no nome
log_file="$diretorio_codigo/${nome_codigo}_cores_${num_cores}_log.csv"

# Cria o arquivo CSV com cabeçalhos
echo "Execução,Tempo (s)" > "$log_file"

# Compila o código C especificado, usando o nome do arquivo C como nome do executável
echo "Compilando '$codigo_c' com $num_cores cores..."
gcc "$codigo_c" -o "$diretorio_codigo/$nome_codigo" -O2 -fopenmp -lm

# Verifica se a compilação foi bem-sucedida
if [[ $? -ne 0 ]]; then
    echo "Erro na compilação do código C."
    exit 1
fi

# Configura o número de threads OpenMP
export OMP_NUM_THREADS=$num_cores

# Loop para executar o programa N vezes e calcular a soma dos tempos
echo "Executando o teste $num_execucoes vezes com $num_cores cores..."
soma_tempos=0
for ((i = 1; i <= num_execucoes; i++)); do
    echo "Executando o teste número $i..."
    # Usa o comando time para registrar o tempo
    tempo_execucao=$( { time "$diretorio_codigo/$nome_codigo" < "$input_file"; } 2>&1 | grep real | awk '{print $2}' )

    # Separa minutos e segundos usando regex
    if [[ $tempo_execucao =~ ([0-9]+)m([0-9]+)\.([0-9]+)s ]]; then
        minutos=${BASH_REMATCH[1]}
        segundos=${BASH_REMATCH[2]}
        milissegundos=${BASH_REMATCH[3]}
        # Converte para segundos com precisão decimal
        tempo_total=$(awk -v m="$minutos" -v s="$segundos" -v ms="$milissegundos" 'BEGIN {printf "%.3f", m * 60 + s + ms / 1000}')
    else
        tempo_total=$(echo "$tempo_execucao" | sed 's/,/./') # Usa o formato padrão de ponto decimal
    fi

    # Soma os tempos
    soma_tempos=$(awk -v soma="$soma_tempos" -v atual="$tempo_total" 'BEGIN {printf "%.3f", soma + atual}')

    # Registra no arquivo CSV
    echo "$i,$tempo_total" >> "$log_file"
    echo "Tempo da execução $i: ${tempo_total}s"
done

# Calcula a média usando awk
media=$(awk -v soma="$soma_tempos" -v execs="$num_execucoes" 'BEGIN {printf "%.3f", soma / execs}')

# Exibe a média no terminal
echo "--------------------------------"
echo "Média do tempo de execução: ${media}s"
echo "Número de cores utilizados: $num_cores"

# Adiciona as informações de média e número de cores ao final do arquivo CSV
echo "Média,$media" >> "$log_file"
echo "Número de cores,$num_cores" >> "$log_file"
