### Métricas Merge Sort

A análise das métricas obtidas a partir dos testes realizados com a versão paralela do Merge Sort indica que a solução não apresentou ganhos significativos de eficiência. Esse comportamento é evidenciado pelas tabelas a seguir:

#### Tabela de Tempos de Execução
| Execução | Tempo sequencial | Algoritmo paralelo (1 core) | Tempo (s) | Algoritmo paralelo (2 cores) | Tempo (s) | Algoritmo paralelo (3 cores) | Tempo (s) | Algoritmo paralelo (4 cores) | Tempo (s) |
|----------|------------------|-----------------------------|-----------|-----------------------------|-----------|-----------------------------|-----------|-----------------------------|-----------|
| 1        | 0,195            | 1                           | 0,241     | 1                           | 0,236     | 1                           | 0,252     | 1                           | 0,205     |
| 2        | 0,237            | 2                           | 0,310     | 2                           | 0,274     | 2                           | 0,229     | 2                           | 0,254     |
| 3        | 0,246            | 3                           | 0,282     | 3                           | 0,230     | 3                           | 0,247     | 3                           | 0,229     |
| 4        | 0,215            | 4                           | 0,227     | 4                           | 0,191     | 4                           | 0,257     | 4                           | 0,215     |
| 5        | 0,225            | 5                           | 0,281     | 5                           | 0,266     | 5                           | 0,216     | 5                           | 0,211     |
| 6        | 0,233            | 6                           | 0,268     | 6                           | 0,219     | 6                           | 0,302     | 6                           | 0,211     |
| 7        | 0,236            | 7                           | 0,320     | 7                           | 0,261     | 7                           | 0,270     | 7                           | 0,208     |
| 8        | 0,239            | 8                           | 0,253     | 8                           | 0,323     | 8                           | 0,225     | 8                           | 0,230     |
| 9        | 0,306            | 9                           | 0,249     | 9                           | 0,290     | 9                           | 0,229     | 9                           | 0,210     |
| 10       | 0,257            | 10                          | 0,227     | 10                          | 0,381     | 10                          | 0,358     | 10                          | 0,260     |


O desempenho limitado do Merge Sort nas execuções com um número menor de processadores foi relacionado ao caráter altamente paralelo dos baldes e da ordenação. Esse tipo de paralelismo dentro de outro paralelismo pode acabar sobrecarregando os processadores do ambiente de teste, o que prejudica o desempenho geral.

Esse comportamento é refletido com Speedups abaixo de 1 para as execuções com 1, 2 e 3 processadores. Além disso, o Speedup de 1,07 com 4 processadores sugere apenas um leve aumento de eficiência da versão paralela em comparação com a versão sequencial. 

Analisando com mais afinco a falta de eficiência, principalmente nas execuções com 4 processadores, o grupo separou a parte do código que precisa ser executada e parte passível de paralelização. A partir desta análise, foi possível observar que apenas 38,5% do tempo total de execução está sendo utilizado na parte paralela. Isso indica que a porção paralela do algoritmo é relativamente curta em relação ao tempo total de execução, o que, por si só, não é suficiente para gerar um ganho significativo de desempenho.

| Execução | Carregamento | Ordenação | Saída  | % paralelizável   |
|----------|--------------|-----------|--------|-------------------|
| 1        | 0,051741     | 0,076326  | 0,062866 | 0,3997527929      |
| 2        | 0,055991     | 0,081531  | 0,076038 | 0,3817709309      |
| 3        | 0,050171     | 0,07337   | 0,065075 | 0,3889913899      |
| 4        | 0,049501     | 0,077732  | 0,077732 | 0,379245237       |
| 5        | 0,049722     | 0,074826  | 0,066345 | 0,3919787525      |
| 6        | 0,050162     | 0,074253  | 0,069888 | 0,3821505587      |
| 7        | 0,05114      | 0,079765  | 0,064718 | 0,4077485776      |
| 8        | 0,068393     | 0,08178   | 0,06776  | 0,3752529447      |
| 9        | 0,049377     | 0,073881  | 0,064865 | 0,3927270988      |
| 10       | 0,052064     | 0,077633  | 0,07516  | 0,37896191        |

**Média: % paralelizável** 0,3855709743


Essa conclusão é reforçada pela aplicação da Lei de Amdahl, que estima o Speedup máximo de um sistema baseado na proporção de código paralelizável. Ao aplicar essa lei aos tempos de execução das partes sequenciais e paralelas, observa-se que os valores de Speedup não apresentam variações significativas e são próximos dos valores obtidos. 

| Processadores | Amdahl         | Obtido            |
|---------------|----------------|-------------------|
| 1             | 1              | 0,8987960873      |
| 2             | 1,238828074    | 0,894421565       |
| 3             | 1,345980736    | 0,9241779497      |
| 4             | 1,406822418    | 1,069861173       |
| 8             | 1,509148312    | -                 |
| 16            | 1,566103968    | -                 |


Os valores de Speedup teóricos e obtidos através dos testes, mostra que podem existir pontos passíveis de paralelização, mas que não foram tratados. Além disso, o algoritmo poderia ter uma eficiência maior ao ser executado com uma entrada de dados maior. Uma maneira de aprofundar a análise do desempenho em cenários com maior volume de dados seria utilizar a Lei de Gustafson. Esta lei é mais adequada para avaliar a escalabilidade de algoritmos, já que ela considera a variação proporcional da entrada de dados em relação à quantidade de processadores.

Pelos valores de Speedup, é possível calcular também a eficiência dos algoritmos, para medir a porcentagem de uso de cada processador durante a execução.


| Processadores | Eficiência       |
|---------------|------------------|
| 1             | 0,8987960873     |
| 2             | 0,4472107825     |
| 3             | 0,3080593166     |
| 4             | 0,2674652933     |


A eficiência do Merge Sort diminuiu à medida que o número de processadores aumentou. Isso pode ser explicado pelo fato de que a parte paralelizável do algoritmo, que já era relativamente pequena, foi subdividida entre um número maior de processadores, resultando em maior ociosidade.

<div align="center">
   
   ![Gráfico do tempo de execução do Merge Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/mergesort.png)

</div>

<div align="center">
   
   ![Gráfico do cálculo de Speedup do Merge Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/mergesort.png)

</div>

<div align="center">
   
   ![Gráfico do tcálculo de eficiência do Merge Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/mergesort.png)

</div>
