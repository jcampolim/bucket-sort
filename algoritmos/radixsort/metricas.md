### Métricas Radix Sort

A análise das métricas obtidas indica que o algoritmo do Radix Sort possui uma alta eficiência, executando rapidamente na sua versão sequencial e obtendo um desempenho muito semelhante na sua versão paralela.

| Execução | Tempo sequencial | Algoritmo paralelo (1 core) | Tempo (1 core) | Algoritmo paralelo (2 cores) | Tempo (2 cores) | Algoritmo paralelo (3 cores) | Tempo (3 cores) | Algoritmo paralelo (4 cores) | Tempo (4 cores) |
|----------|------------------|-----------------------------|----------------|-----------------------------|-----------------|-----------------------------|-----------------|-----------------------------|-----------------|
| 1        | 0,151            | 1                           | 0,204          | 1                           | 0,164           | 1                           | 0,256           | 1                           | 0,160           |
| 2        | 0,182            | 2                           | 0,181          | 2                           | 0,184           | 2                           | 0,274           | 2                           | 0,162           |
| 3        | 0,280            | 3                           | 0,162          | 3                           | 0,268           | 3                           | 0,184           | 3                           | 0,182           |
| 4        | 0,188            | 4                           | 0,225          | 4                           | 0,384           | 4                           | 0,210           | 4                           | 0,169           |
| 5        | 0,234            | 5                           | 0,295          | 5                           | 0,336           | 5                           | 0,205           | 5                           | 0,179           |
| 6        | 0,236            | 6                           | 0,261          | 6                           | 0,413           | 6                           | 0,198           | 6                           | 0,182           |
| 7        | 0,187            | 7                           | 0,223          | 7                           | 0,437           | 7                           | 0,213           | 7                           | 0,195           |
| 8        | 0,230            | 8                           | 0,195          | 8                           | 0,346           | 8                           | 0,205           | 8                           | 0,196           |
| 9        | 0,169            | 9                           | 0,220          | 9                           | 0,356           | 9                           | 0,216           | 9                           | 0,198           |
| 10       | 0,271            | 10                          | 0,232          | 10                          | 0,400           | 10                          | 0,230           | 10                          | 0,194           |


Os valores de Speedup medidos para as execuções com 1, 2 e 3 processadores são valores abaixo de 1, indicando que, nestas condições, o algoritmo possui um desempenho inferior ao algoritmo paralelo. Esse baixo desempenho ocorre devido a uma sobrecarga dos recursos computacionais, causada pelo paralelismo na ordenação dos baldes e no próprio Radix Sort.

O Speedup obtido para 4 processadores foi de 1,171, representando um aumento de eficiência, porém este aumento é de apenas 17%. Sugerindo que a paralelização do algoritmo não trouxe ganhos substanciais no cenário testado.

No entanto, para entender melhor este resultado para 4 processadores, foram feitos outros testes com a versão sequencial e uma análise mais detalhada da parte do código que pode ser paralelizada revelou que apenas 20,7% do tempo de execução total corresponde à parte paralelizável.

Este valor justifica o baixo desempenho do algoritmo paralelo, uma vez que a parte paralelizável não representa uma porção significativa da execução do algoritmo.

| Execução | Carregamento | Ordenação  | Saída    | % paralelizável    |
|----------|--------------|------------|----------|--------------------|
| 1        | 0,051792     | 0,031445   | 0,06734  | 0,2088300338       |
| 2        | 0,052284     | 0,031633   | 0,066741 | 0,2099656175       |
| 3        | 0,055961     | 0,030875   | 0,072519 | 0,1937498039       |
| 4        | 0,052979     | 0,032874   | 0,07387  | 0,2058188238       |
| 5        | 0,052979     | 0,030894   | 0,066765 | 0,2050876937       |
| 6        | 0,049599     | 0,030656   | 0,069545 | 0,2046461949       |
| 7        | 0,049666     | 0,031008   | 0,066072 | 0,2113038856       |
| 8        | 0,050016     | 0,031629   | 0,064528 | 0,2163805901       |
| 9        | 0,085883     | 0,038132   | 0,09441  | 0,174577086        |
| 10       | 0,052031     | 0,031719   | 0,064904 | 0,2133746821       |

**Média: % paralelizável** 0,2073244288


A partir destas medições, é possível aplicar a Lei de Amdahl para calcular os valores de Speedup teóricos conforme o aumento do número de processadores e os valores obtidos a partir dos testes:

| Processadores | Amdahl         | Obtido            |
|---------------|----------------|-------------------|
| 1             | 1              | 0,9681528662      |
| 2             | 1,115650836    | 0,6472019465      |
| 3             | 1,160383961    | 0,9712460064      |
| 4             | 1,184123259    | 1,171161255       |
| 8             | 1,221611095    | -                 |
| 16            | 1,241259442    | -                 |


Os cálculos indicam que a solução paralela com 4 processadores deve ter um Speedup próximo a 1,184, valor que se aproxima do Speedup real obtido na implementação do algoritmo. Os dados também indicam que o número máximo eficiente de processadores para paralelizar este código seria de 4, e adicionar mais processadores não resultaria em ganhos significativos no desempenho do algoritmo.

A partir desses números, é possível realizar o cálculo da eficiência do algoritmo, que mostra o quanto de cada processador foi utilizado nas execuções.

| Processadores | Eficiência      |
|---------------|-----------------|
| 1             | 0,9681528662    |
| 2             | 0,3236009732    |
| 3             | 0,3237486688    |
| 4             | 0,2927903137    |


Nas execuções com 2 ou mais processadores, a eficiência do algoritmo é baixa, cerca de 30%. Esse desempenho pode ser explicado pela pequena proporção do código que pode ser paralelizada. Dessa forma, a parte paralelizada do código é distribuída entre vários processadores, o que reduz a carga de trabalho de cada um, mas também limita os ganhos de desempenho.

<div align="center">
   
   ![Gráfico do tempo de execução do Radix Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/radixsort.png)

</div>

<div align="center">
   
   ![Gráfico do cálculo de Speedup do Radix Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/radixsort.png)

</div>

<div align="center">
   
   ![Gráfico do tcálculo de eficiência do Radix Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/radixsort.png)

</div>