# Bucket Sort Paralelo
> Autores: [Enzo Guarnieri](https://github.com/enzo-guarnieri), [Erika Borges Piaui](https://github.com/ErikaBP), [Júlia Campolim de Oste](https://github.com/jcampolim) e [Otávio Augusto Freire de Andrade Bruzadin](https://github.com/OtavioBruzadin)

### `1. Descrição do problema`

O Bucket sort é um algoritmo de ordenação, cuja estratégia para ordenar é dividir o vetor em `buckets` (baldes) com um número finito de elementos. A vantagem dos baldes é a sua complexidade O(n), onde “n” é o tamanho do vetor. Entretanto, a complexidade linear é atingida apenas quando os números estão distribuídos uniformemente.

A entrada consiste em duas partes: os parâmetros e o conteúdo que será ordenado. Os primeiros quatro números são a quantidade de caracteres do valores, o tamanho do vetor, o offset e o número de buckets. O restante dos parâmetros são os números inteiros para serem ordenados.

A saída consiste nos números ordenados.

<div align="center">
   
   ![Exemplo de entrada e saída do programa](https://github.com/jcampolim/bucket-sort/blob/main/assets/entrada-saida.png)

</div>

### `2. Solução serial`

#### `2.1 Fluxo`

O fluxo do código pode ser dividido em quatro partes: carregamento dos dados, distribuição dos dados em cada balde, ordenação dos baldes e exibição dos resultados.

O carregamento dos dados é a primeira etapa do programa, os dados incluem os parâmetros especificados no enunciado do problema e uma sequência de números inteiros que serão alocados em um vetor para serem ordenados. Para manipular os números, a estratégia usada foi separar um espaço na memória para cada elemento e trabalhar com ponteiros para obter o índice do primeiro caractere de cada número, para isto, os números são tratados como um vetor de caracteres.

```c
void load(char **vString, long int *N, int *l, int *os, int *nb){
   char *strings = NULL;
   int      len,
            offset,
            nbucket;
   long int n;

   if (!fscanf(stdin, "%d", &len)) {
      printf ("ERROR len\n");
      exit (1);
   }

   if (!fscanf(stdin, "%ld", &n)) {
      printf ("ERROR n\n");
      exit (1);
   }

   if (!fscanf(stdin, "%d", &offset)) {
      printf ("ERROR offset\n");
      exit (1);
   }

   if (!fscanf(stdin, "%d", &nbucket)) {
      printf ("ERROR nbucket\n");
      exit (1);
   }

   nbucket = nbucket + 1;
   len = len + 1;

   strings = (char*) malloc(n * len);

   for (int i = 0; i < n; i++){
      if (!fscanf(stdin, "%s", strings + (i * len))) {
      printf ("ERROR %d\n",i);
      exit (1);
      }
   }

   *vString = strings;
   *N = n;
   *l = len;
   *os = offset;
   *nb = nbucket;
}
```

Com os dados já carregados, a próxima etapa é criar os baldes e distribuir os elementos do vetor entre eles. O número de baldes é um dos parâmetros pré-definidos na entrada do problema e, para cada balde criado, o índice do primeiro caractere de cada número é armazenado como referência, juntamente com o tamanho de cada balde.

```c
long int* bucket_sort(char *a, int length, long int size, int offset, int nbuckets) {

    long int i;
    bucket   *buckets = NULL,
             *b = NULL;
    long int *returns = NULL;

    returns = (long int *) malloc(sizeof(long int) * size);
    buckets = (bucket   *) malloc(sizeof(bucket) * nbuckets);


    for (i = 0; i < nbuckets; i++) {
        buckets[i].data = returns + i * size / nbuckets;
        buckets[i].length = length;
        buckets[i].total = 0;
    }

    for (i = 0; i < size; i++) {
        b = &buckets[*(a + i * length) - offset];
        b->data[b->total++] = i;
    }

    for (i = 0; i < nbuckets; i++)
        sort(a, &buckets[i]);

    return returns;
}
```

Em seguida, é preciso ordenar cada balde; para isto, é preciso usar um algoritmo de ordenação auxiliar. No código original, o algoritmo auxiliar é o Insertion Sort, que se baseia em comparações para colocar cada elemento em sua respectiva posição.

Neste caso, comparamos os elementos lexicograficamente e de forma sequencial:

```c
void sort(char *a, bucket *bucket) {
    int j, i, length;
    long int key;
    length = bucket->length;

    for (j = 1; j < bucket->total; j++) {
        key = bucket->data[j];
        i = j - 1;
        while (i >= 0 && strcmp(a + bucket->data[i] * length, a + key * length) > 0) {
            bucket->data[i + 1] = bucket->data[i];
            i--;
        }
        bucket->data[i + 1] = key;
    }
}
```

A última etapa é a de exibição dos resultados. Originalmente o vetor já ordenado era exibido na saída padrão, mas essa parte foi alterada para salvar os resultados no arquivo output.txt, permitindo uma melhor visualização dos dados.

```c
void save(char *strings, long int *index, long int n, int len){
   for (int i = 0; i < n; i++){
      printf("%s\n", strings + (index[i] * len));
   }
}
```

Antes de finalizar o código, todas as memórias alocadas são liberadas.


#### `2.2 Oportunidades de paralelismo`

Durante a análise do código sequencial, o grupo identificou que duas das quatro partes são inerentemente sequências, o carregamento dos dados e salvar os resultados. As outras duas partes, no entanto, poderiam ser paralelizadas para melhorar o desempenho.

##### `2.2.1 Ordenação dos baldes`

Na abordagem tradicional, a ordenação dos baldes no Bucket Sort é realizada em um laço de repetição de forma sequencial, onde cada balde é ordenado individualmente, um após o outro. Esta etapa pode ser paralelizada, sendo o grão um laço de repetição, para melhorar o desempenho.

Com este objetivo, foi utilizada a anotação `parallel for` da biblioteca `OpenMP`, que cria uma região paralela para executar as iterações do laço de forma simultânea. Permitindo que cada balde seja ordenado de forma independente e aproveitando ao máximo o uso dos recursos computacionais.

```c
#pragma omp parallel for shared(a, buckets)
for (i = 0; i < nbuckets; i++)
    sort(a, &buckets[i]);
```

Outras abordagens foram consideradas para paralelizar a ordenação dos baldes, como o uso de um saco de tarefas, onde cada balde seria uma tarefa. Porém, estas abordagens se mostraram menos eficientes em testes que envolviam outros algoritmos de ordenação nos baldes.

##### `2.2.2 Algoritmo de ordenação auxiliar`

O Insertion Sort, usado como algoritmo auxiliar do Bucket Sort no código base, possui complexidade O(n2) e, embora teoricamente seja possível paralelizá-lo, duas abordagens principais apresentam limitações práticas:

* Divisão em sub vetores menores: consiste em dividir o vetor em partes menores e ordená-las separadamente. Isto pode ser feito utilizando o Merge (típico do Merge Sort) para juntar os sub vetores ordenados ou o Partition (típico do Quick Sort) para auxiliar na separação dos sub vetores. No entanto, esta abordagem foi descartada porque seria necessário introduzir um terceiro algoritmo no código do Bucket Sort, adicionando uma complexidade desnecessária.

* Busca paralela pela posição do elemento: se baseia em utilizar a busca (binária ou linear) pela posição do elemento no vetor de forma paralela. Contudo, após determinar esta posição, seria necessário percorrer a extensão do vetor ajustando a posição dos demais elementos, processo que não pode ser paralelizado.

Por estas razões, a ideia de paralelizar o Insertion Sort foi descartada, abrindo espaço para o uso de outros algoritmos de ordenação que possuem melhor desempenho quando paralelizados.


### `3. Análise dos algoritmos de ordenação`

Na busca pelo algoritmo de ordenação que apresenta maior eficiência no cenário retratado, são explorados os quatro algoritmos clássicos: Odd Even Sort, Merge Sort, Quick Sort e Radix Sort. Estes algoritmos foram escolhidos por sua eficiência e pela possibilidade de serem paralelizados.

Os testes de cada algoritmo foram realizados 10 vezes em condições controladas, utilizando um ambiente com 4 CPUs e 8 GB de memória RAM, onde todos os recursos computacionais foram dedicados exclusivamente para a execução dos algoritmos de ordenação. Cada algoritmo também foi testado variando o número de processadores disponíveis em 1, 2, 3 e 4 cores com a variável de ambiente OMP_NUM_THREADS da biblioteca `OpenMP`.

A entrada utilizada em cada teste também foi a mesma, composta por um vetor de 100.000 dados.

Para uma análise abrangente e justa entre os algoritmos foram utilizadas as métricas: 

* Média: reflete o tempo médio de execução do algoritmo durante os testes, fornecendo uma visão geral do algoritmo.

* Mediana: apresenta o valor de execução central, eliminando possíveis distorções.

* Desvio padrão: mede a consistência do desempenho, indicando o grau de variação entre os tempos de execução.

* Speedup: avalia a eficiência da paralelização, comparando a velocidade da versão paralela com a versão sequencial. 

Essas métricas permitem uma análise detalhada do comportamento de cada algoritmo, em termos de tempo de execução, escalabilidade e eficiência.

Todavia, após os primeiros testes, os números de Speedup estavam “estranhos” nos algoritmos utilizando o Merge Sort  e o Radix Sort. Isso porque os algoritmos estavam com tempos rápidos, mas a versão paralela estava executando em um tempo quase igual ao tempo da versão sequencial.

Visando entender o que estava acontecendo, foram feitas algumas análises seguindo a Lei de Amdahl, responsável pela análise como uma mesma entrada de dados pode variar com relação ao número de processadores.

#### `3.1 Algoritmo original`

O algoritmo original do Bucket Sort está usando o Insertion Sort como auxiliar para a ordenação dos baldes. As métricas obtidas ao executar o programa original foram:

| Execução | Tempo (s) |
| --- | --- |
| 1 | 204,897 |
| 2 | 204,323 |
| 3 | 208,977 |
| 4 | 198,082 |
| 5 | 202,422 |
| 6 | 201,020 |
| 7 | 200,976 |
| 8 | 195,338 |
| 9 | 198,322 |
| 10 | 201,157 |

#### `3.2 Odd-Even Sort`

O algoritmo Odd-Even Sort é uma variação do algoritmo do Bubble Sort, um dos algoritmos de ordenação mais simples. O Bubble Sort tem seu funcionamento baseado na comparação dos pares adjacentes de um vetor e na troca de posições quando necessário, visando “empurrar” o menor elemento gradativamente para o início do vetor. Apesar de ser um algoritmo simples, possui complexidade O(n2), não sendo eficiente para grandes conjuntos de dados; além disso, as trocas e comparações dificultam na sua paralelização, pois cada iteração depende diretamente dos resultados obtidos na iteração anterior.

O Odd-Even Sort tem como objetivo quebrar as dependências entre cada iteração, facilitando a sua paralelização. Dessa forma, ao invés de percorrer todos os elementos do vetor em uma única “passada” para as comparações, o algoritmo realiza duas etapas separadas:

1. Comparação e, se necessário, troca dos elementos de índice ímpar (odd) e seus sucessores.

2. Comparação e, se necessário, troca dos elementos de índice par (even) e seus sucessores.

Este processo alternado permite múltiplas comparações independentes que podem ser executadas de forma paralela. Isso porque, cada comparação na “passada ímpar” é independente das outras, o mesmo ocorre na “passada par”. Essa independência fica evidente no esquema abaixo:

<div align="center">
   
   ![Exemplo do funcionamento do Odd Even Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/oddeven.png)

</div>

Dessa forma, o grão da paralelização seria o laço de repetição responsável por paralelizar cada passada. 

* [Algoritmo sequencial](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/oddeven/oddeven.c)
* [Algoritmo paralelo](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/oddeven/oddevenParallel.c)
* [Métricas](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/oddeven/metricas.md)

#### `3.3 Merge Sort`

O Merge Sort é um algoritmo de ordenação eficiente, de complexidade O(n log n), que utiliza a estratégia de divisão e conquista. Sua abordagem é dividir o vetor recursivamente em subvetores menores até que cada subvetor contenha apenas um elemento que, por definição, está ordenado. Em seguida, os subvetores são intercalados de forma ordenada.

<div align="center">
   
   ![Exemplo do funcionamento do Merge Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/mergesort.png)

</div>

Para intercalar os elementos do vetor, utiliza-se uma função chamada Merge, que recebe duas listas ordenadas e combina em uma única lista também ordenada. O algoritmo Merge que foi utilizado no código do Bucket Sort é o mais tradicional:

```c
void merge(char *a, long int *data, int left, int middle, int right, int length) {
    long int n1 = middle - left + 1;
    long int n2 = right - middle;

    long int *L = (long int *) malloc(n1 * sizeof(long int));
    long int *R = (long int *) malloc(n2 * sizeof(long int));

    for (int i = 0; i < n1; i++) {
        L[i] = data[left + i];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = data[middle + 1 + j];
    }

    long int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (strcmp(a + (L[i] * length), a + (R[j] * length)) < 0) {
            data[k] = L[i];
            i++;
        } else {
            data[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        data[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        data[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}
```

O Merge é um algoritmo sequencial; entretanto, é possível paralelizar o Merge Sort aproveitando a independência dos subvetores em cada nível da árvore de recursão, sendo cada subvetor o grão da paralelização no nível de tarefas. 

Uma abordagem eficiente para paralelizar o Merge Sort é realizar subdivisões de forma paralela até uma determinada profundidade da árvore, permitindo que os subvetores gerados sejam processados simultaneamente. Para a ordenação simultânea, cada subvetor é colocado dentro de uma tarefa e, para prosseguir para o Merge, a ordenação de cada vetor precisa ser concluída.

No caso, a profundidade escolhida foi 3, o que resulta na divisão do vetor original em 8 subvetores que serão ordenados sequencialmente. Essa profundidade foi definida após testes rápidos, considerando o tempo limitado para a experimentação.

Outra abordagem possível para restringir o paralelismo é limitar pela quantidade de elementos no subvetor, mas esta estratégia demandaria mais testes aprofundados para identificar o ponto de paralelismo. Além disso, é preciso levar em conta que a melhor estratégia depende da arquitetura e dos recursos computacionais utilizados, não havendo uma resposta “certa”.

* [Algoritmo sequencial](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/mergesort/mergesort.c)
* [Algoritmo paralelo](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/mergesort/mergesortParallel.c)
* [Métricas](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/mergesort/metricas.md)

#### `3.4 Quick Sort`

Assim como o Merge Sort, o Quick Sort também utiliza a técnica de divisão e conquista para ordenar um vetor. O Quick Sort, em geral,  é um algoritmo eficiente, com complexidade O(n log n) no seu caso médio; no entanto, em seu pior caso a complexidade aumenta significativamente, passando a ser O(n^2).

A divisão do vetor no Quick Sort, é realizada por uma função chamada Partition, onde um pivô é escolhido e todos os elementos menores que o pivô são rearranjados para o início do vetor e os elementos maiores são rearranjados para o final. Após todos os elementos serem separados entre menores e maiores que o pivô, o pivô já está ordenado na sua posição correta e o processo é repetido recursivamente nos subvetores que se formaram à esquerda e à direita do pivô, até que todo o vetor esteja ordenado.

<div align="center">
   
   ![Exemplo do funcionamento do Quick Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/quicksort.png)

</div>

Dentro do Partition, existem várias formas de escolher um pivô, a abordagem utilizada foi a de escolher o primeiro elemento do vetor.

```c
int partition(char *a, long int *data, int low, int high, int length) {
    long int pivot = data[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (strcmp(a + data[j] * length, a + pivot * length) <= 0) {
            i++;
            long int temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    long int temp = data[i + 1];
    data[i + 1] = data[high];
    data[high] = temp;

    return i + 1;
}
```

É possível paralelizar o Quick Sort modificando o processo de Partition para selecionar vários pivôs, dividindo o vetor em vários subvetores para ordená-los simultaneamente. Entretanto, essa abordagem pode acarretar um custo computacional significativo, principalmente ao selecionar mais de 3 pivôs. Teoricamente, essa abordagem é promissora, mas exige mais tempo para testes de eficiência e otimização do algoritmo.

Dessa forma, a abordagem adotada foi simplificar o paralelismo, limitando a árvore de recursão até a profundidade 3 (semelhante ao que foi feito no Merge Sort). Neste ponto, o vetor original é dividido em 8 subvetores e esses subvetores são ordenados de forma sequencial, mas simultânea, onde cada um é designado a uma tarefa (grão do paralelismo). 

Esta abordagem reduz o custo computacional, uma vez que o paralelismo é aplicado apenas até uma profundidade limitada da recursão. A profundidade 3 foi escolhida mediante a testes rápidos, mas que poderiam ser aprofundados para identificar o momento ideal de paralelismo.

* [Algoritmo sequencial](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/quicksort/quicksort.c)
* [Algoritmo paralelo](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/quicksort/quicksortParallel.c)
* [Métricas](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/quicksort/metricas.md)

#### `3.5 Radix Sort`

Ao contrário dos outros algoritmos de ordenação que foram citados, o Radix Sort não se baseia na comparação dos elementos de um vetor para realizar a ordenação. Ao invés disso, ele ordena os elementos com base nos seus dígitos, um de cada vez, começando pelo dígito de maior ordem até o de menor ordem. Isso faz do Radix Sort uma excelente escolha para ordenar números inteiros ou conjuntos de caracteres com tamanho fixo, sendo vantajoso no cenário testado.

A ordenação é realizada passo-a-passo com o auxílio de um algoritmo de ordenação estável. No caso, foi utilizado o counting Sort, um algoritmo eficiente quando se trata da ordenação de elementos em um intervalo pequeno e com muitas repetições de valores.

<div align="center">
   
   ![Exemplo do funcionamento do Radix Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/radixsort.png)

</div>
Dentro deste algoritmo, foram encontrados alguns laços de repetição passíveis de paralelização, como a contagem de ocorrências de cada dígito, a distribuição dos elementos já ordenados e a cópia dos valores ordenados para o vetor original. Estes laços correspondem ao grão de paralelismo do Radix Sort.

* [Algoritmo sequencial](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/radixsort/radixsort.c)
* [Algoritmo paralelo](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/radixsort/radixsortParallel.c)
* [Métricas](https://github.com/jcampolim/bucket-sort/blob/main/algoritmos/radixsort/metricas.md)

### `4. Escalabilidade e desempenho`

#### `4.1 Speedup`

Os testes realizados em cada variação dos algoritmos permitiram calcular o Speedup com base na média de 10 execuções sequenciais e 10 execuções paralelas com diferentes cores. Os resultados obtidos foram os seguintes para 4 processadores (versão mais eficiente testada):

<table>
    <tr>
        <td></td>
        <td>Merge Sort</td>
        <td>Radix Sort</td>
        <td>Quick Sort</td>
        <td>Odd Even Sort</td>
    </tr>
    <tr>
        <td>Speedup</td>
        <td>1,069861173</td>
        <td>1,171161255</td>
        <td>3,065459051</td>
        <td>3,127129349</td>
    </tr>
</table>
<br></br>

<div align="center">
   
   ![Gráfico comparativo dos valores de Speedup obtidos](https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/comparacao.png)

</div>

Os algoritmos que utilizaram o Merge Sort e o Radix Sort apresentam um baixo ganho de tempo, com a diferença entre as execuções sequenciais e paralelas sendo quase nula. Entretanto, este comportamento foi atribuído ao tamanho da entrada de dados utilizada, uma vez que o tempo de execução da parte paralela dos algoritmos representava apenas 38,5% no caso do Merge Sort e 20,7% no caso do Radix Sort, em relação ao tempo total de execução. Como a maior parte do tempo de execução é dedicada à parte sequencial do algoritmo, a paralelização não gerou um ganho expressivo de desempenho.

Dessa forma, o tempo gasto pela parte sequencial acaba sendo o principal gargalo. A partir disso, com o auxílio da Lei de Amdahl, foi possível estimar o limite máximo de processadores em que o Speedup poderia aumentar de forma significativa. Esse limite se aproximou dos valores de Speedup observados nos testes.

Para validar esta teoria, seria necessário testar os princípios da Lei de Gustafson, aumentando progressivamente a entrada de dados, permitindo observar se esse aumento, proporcional ao aumento no número de processadores, resulta em um ganho de desempenho mais expressivo.

Por outro lado, os algoritmos do Odd-Even Sort e do Quick Sort apresentaram um Speedup superior que 3, evidenciando que as suas versões paralelas são expressivamente mais performáticas do que as versões sequenciais.

Todos os valores de Speedup obtidos se mostraram próximos aos valores teóricos calculados,  que confirma que os algoritmos desenvolvidos possuem um alto grau de paralelismo. Porém, todos os Speedup estavam abaixo do dos valores esperados, o que sugere que existem pontos de paralelismo que ainda não foram completamente explorados, como a parte de distribuição dos dados nos baldes, que está sendo realizada de forma sequencial.

#### `4.2 Eficiência`

A eficiência dos algoritmos pode ser medida pelo número de Speedup obtido e da quantidade de processadores utilizados e é responsável por determinar o uso efetivo dos processadores alocados. Para realizar os cálculos, foram utilizados os valores de Speedup relacionados a 4 processadores, já que esta configuração se mostrou mais eficiente.

Os dados de eficiência obtidos foram:

<table>
    <tr>
        <td></td>
        <td>Merge Sort</td>
        <td>Radix Sort</td>
        <td>Quick Sort</td>
        <td>Odd Even Sort</td>
    </tr>
    <tr>
        <td>Eficiência</td>
        <td>0,2674652933</td>
        <td>0,2927903137</td>
        <td>0,7663647627</td>
        <td>0,7817823373</td>
    </tr>
</table>
<br></br>

<div align="center">
   
   ![Gráfico comparativo dos valores de eficiência obtidos](https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/comparacao.png)

</div>

O ideal é que os valores de eficiência estejam o mais próximo possível de 100%, indicando que todos os processadores alocados estão usando sua capacidade total.

As medições indicam que os algoritmos utilizando o Odd-Even Sort e o Quick Sort apresentaram uma eficiência semelhante, ambas se aproximaram de 80%. Isso sugere que esses algoritmos paralelos fazem um bom uso dos recursos computacionais disponíveis, aproveitando adequadamente o paralelismo e apresentando alta escalabilidade.

Por outro lado, os algoritmos com Merge Sort e Radix Sort apresentaram uma eficiência baixa, em torno de 25%, o que indica uma distribuição inadequada dos recursos computacionais. Essa baixa eficiência pode ser explicada pela característica desses algoritmos, que possuem um tempo gasto na parte paralela proporcionalmente menor em relação ao tempo da parte sequencial, o que limita os ganhos de desempenho.

Essa teoria da má distribuição dos recursos poderia ser validada por meio de uma bateria de testes aplicando a Lei de Gustafson, que permite uma análise mais detalhada do comportamento do paralelismo com diferentes entradas que variam proporcionalmente à quantidade de processadores.

### `5. Solução final`

Com base nas análises de desempenho realizadas dos algoritmos paralelos, é possível concluir que cada algoritmo possui características próprias que influenciam diretamente na sua paralelização e eficiência. Apesar dessas diferenças, algumas conclusões gerais podem ser feitas sobre o desempenho observado durante os testes.

Os algoritmos que utilizam o Quick Sort e o Odd-Even Sort apresentaram um Speedup significativo, com ganhos de 3 vezes em comparação com suas versões sequenciais. Esse desempenho indica que os algoritmos possuem um bom potencial para paralelização, aproveitando a adição de processadores. 

Porém, eles são consideravelmente mais lentos quando comparados com o Merge Sort e o Radix Sort, que apresentaram limitações de paralelização, ambos com um ganho menor de Speedup. No entanto, a tendência destes algoritmos é apresentar uma maior escalabilidade quando testados em conjuntos maiores de dados.

Portanto, conclui-se que, mesmo com índices menores de paralelismo, o uso do Bucket Sort aliado ao Merge Sort ou ao Radix Sort é mais vantajoso em termos de tempo de execução e de escalabilidade. O Quick Sort provavelmente também teria um bom desempenho com entradas do seu caso médio, mas para comprovar seriam necessários mais testes.

#### `5.1 Desenvolvimento do Bash`

Durante o desenvolvimento do trabalho, identificou-se a necessidade de uma ferramenta que auxiliasse na medição de desempenho dos algoritmos de forma eficiente e uniforme. Neste contexto, o desenvolvimento de um script Bash trouxe praticidade e precisão ao processo, garantindo que os testes fossem conduzidos de maneira consistente entre todos os integrantes da equipe.

Esse script automatiza as execuções do código C e a medição dos tempos de execução em milissegundos, registrando os resultados em um arquivo CSV. Esse formato estruturado não só facilita a análise estatística dos dados, como também permite a fácil importação dos resultados para ferramentas de análise de dados, oferecendo suporte para a criação de gráficos e relatórios detalhados sobre o desempenho dos algoritmos.

Em resumo, o script Bash contribuiu significativamente para a eficiência e confiabilidade do trabalho, reduzindo o risco de erro humano, acelerando o processo de coleta de dados, e possibilitando a reprodutibilidade dos experimentos. Possibilitando que o foco principal da equipe estivesse na análise dos resultados, ao invés de na execução manual dos testes.
