### Métricas Quick Sort

A análise das métricas revela que a versão sequencial do algoritmo Quick Sort teve um desempenho inferior do que o esperado para um algoritmo com complexidade O(n log n). Este comportamento foi causado pela entrada de dados que está sendo utilizada para teste, que está ordenada ao contrário, representando o pior caso do Quick Sort e aumentando a complexidade do algoritmo para O(n^2).

| Execução | Tempo (sequencial) | Algoritmo paralelo (1 core) | Tempo (s) | Algoritmo paralelo (2 cores) | Tempo (s) | Algoritmo paralelo (3 cores) | Tempo (s) | Algoritmo paralelo (4 cores) | Tempo (s) |
|----------|---------------------|-----------------------------|-----------|-----------------------------|-----------|-----------------------------|-----------|-----------------------------|-----------|
| 1        | 207,224             | 1                           | 218,825   | 1                           | 120,865   | 1                           | 89,464    | 1                           | 71,508    |
| 2        | 213,252             | 2                           | 247,908   | 2                           | 113,812   | 2                           | 98,979    | 2                           | 73,179    |
| 3        | 210,428             | 3                           | 220,399   | 3                           | 112,843   | 3                           | 85,483    | 3                           | 73,742    |
| 4        | 215,791             | 4                           | 218,823   | 4                           | 109,079   | 4                           | 94,423    | 4                           | 72,120    |
| 5        | 209,843             | 5                           | 219,299   | 5                           | 110,549   | 5                           | 92,185    | 5                           | 72,417    |
| 6        | 212,355             | 6                           | 223,290   | 6                           | 111,827   | 6                           | 81,966    | 6                           | 73,097    |
| 7        | 215,687             | 7                           | 207,256   | 7                           | 110,559   | 7                           | 87,497    | 7                           | 65,219    |
| 8        | 218,934             | 8                           | 215,548   | 8                           | 108,069   | 8                           | 95,381    | 8                           | 65,038    |
| 9        | 213,786             | 9                           | 219,130   | 9                           | 109,683   | 9                           | 86,758    | 9                           | 64,453    |
| 10       | 212,958             | 10                          | 235,130   | 10                          | 114,504   | 10                          | 87,225    | 10                          | 64,150    |


Entretanto, apesar de não ser o resultado esperado, a versão paralela do Quick Sort obteve um desempenho significativamente melhor quando comparada à versão sequencial. Os testes do algoritmo com 1 processador, mostraram uma execução inferior ao tempo sequencial, provavelmente por conta da sobrecarga gerada por um alto grau de paralelismo de funções. 

Os demais processadores apresentaram um ganho significativo com relação à versão sequencial, todos reduziram expressivamente o tempo de execução com relação ao código sequencial, demonstrando o impacto positivo na paralelização.

Para entender melhor o comportamento do algoritmo, foi analisada a proporção de tempo dedicada à parte paralelizada em comparação com a parte sequencial. Os resultados mostraram que 99,9% do tempo de execução do Bucket Sort, que utiliza o Quick Sort, é dedicado à parte paralela. Isso indica que o algoritmo possui um grande potencial de paralelização, já que a maior parte do trabalho pode ser distribuída entre múltiplos processadores.

| Execução | Carregamento | Ordenação  | Saída    | % paralelizável    |
|----------|--------------|------------|----------|--------------------|
| 1        | 0,048746     | 206,709555 | 0,069498 | 0,9994282974       |
| 2        | 0,050361     | 211,727691 | 0,083308 | 0,9993690732       |
| 3        | 0,057816     | 209,676416 | 0,066115 | 0,9994092908       |
| 4        | 0,049144     | 219,1597   | 0,079347 | 0,9994140542       |
| 5        | 0,057711     | 209,567081 | 0,072964 | 0,9993768412       |
| 6        | 0,072964     | 209,567081 | 0,072964 | 0,9993041538       |
| 7        | 0,0494       | 205,825667 | 0,087684 | 0,9993344233       |
| 8        | 0,087684     | 206,929982 | 0,072011 | 0,9992288606       |
| 9        | 0,050175     | 215,969094 | 0,074868 | 0,9994213494       |
| 10       | 0,054482     | 211,620057 | 0,061215 | 0,9994535784       |

**Média: % paralelizável** 0,999393066


A aplicação da Lei de Amdahl à proporção de 99,9% de paralelização e 0,1% de sequencialidade permite estimar os valores teóricos de Speedup.

| Processadores | Amdahl         | Obtido            |
|---------------|----------------|-------------------|
| 1             | 1              | 0,9571577744      |
| 2             | 1,998786868    | 1,898981093       |
| 3             | 2,996362811    | 2,368635064       |
| 4             | 3,992730029    | 3,065459051       |
| 8             | 7,966155485    | -                 |
| 16            | 15,85565       | -                 |


O valor de Speedup alcançado na implementação do algoritmo paralelo foi próximo ao valor obtido pelos testes realizados. Em todos eles, o Speedup se mostrou elevado e próximo ao limite máximo, com destaque para a execução com 4 processadores, que conseguiu um Speedup de 3,065. Indicando que, sob estas condições, a versão paralela do algoritmo é três vezes mais rápida que a versão sequencial.

> tabela

Os valores de eficiência demonstram um alto grau de uso dos processadores com 1 e 2 processadores. Possuindo um desempenho menor com 3 e 4 processadores, mas ainda sendo um valor elevado.

<div align="center">
   
   ![Gráfico do tempo de execução do Quick Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/quicksort.png)

</div>

<div align="center">
   
   ![Gráfico do cálculo de Speedup do Quick Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/quicksort.png)

</div>

<div align="center">
   
   ![Gráfico do tcálculo de eficiência do Quick Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/quicksort.png)

</div>