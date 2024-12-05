### Métricas Merge Sort

A análise das métricas obtidas a partir dos testes realizados com a versão paralela do Merge Sort indica que a solução não apresentou ganhos significativos de eficiência. Esse comportamento é evidenciado pelas tabelas a seguir:

> tabela

O desempenho limitado do Merge Sort nas execuções com um número menor de processadores foi relacionado ao caráter altamente paralelo dos baldes e da ordenação. Esse tipo de paralelismo dentro de outro paralelismo pode acabar sobrecarregando os processadores do ambiente de teste, o que prejudica o desempenho geral.

Esse comportamento é refletido com Speedups abaixo de 1 para as execuções com 1, 2 e 3 processadores. Além disso, o Speedup de 1,07 com 4 processadores sugere apenas um leve aumento de eficiência da versão paralela em comparação com a versão sequencial. 

Analisando com mais afinco a falta de eficiência, principalmente nas execuções com 4 processadores, o grupo separou a parte do código que precisa ser executada e parte passível de paralelização. A partir desta análise, foi possível observar que apenas 38,5% do tempo total de execução está sendo utilizado na parte paralela. Isso indica que a porção paralela do algoritmo é relativamente curta em relação ao tempo total de execução, o que, por si só, não é suficiente para gerar um ganho significativo de desempenho.

> tabela

Essa conclusão é reforçada pela aplicação da Lei de Amdahl, que estima o Speedup máximo de um sistema baseado na proporção de código paralelizável. Ao aplicar essa lei aos tempos de execução das partes sequenciais e paralelas, observa-se que os valores de Speedup não apresentam variações significativas e são próximos dos valores obtidos. 

> tabela

Os valores de Speedup teóricos e obtidos através dos testes, mostra que podem existir pontos passíveis de paralelização, mas que não foram tratados. Além disso, o algoritmo poderia ter uma eficiência maior ao ser executado com uma entrada de dados maior. Uma maneira de aprofundar a análise do desempenho em cenários com maior volume de dados seria utilizar a Lei de Gustafson. Esta lei é mais adequada para avaliar a escalabilidade de algoritmos, já que ela considera a variação proporcional da entrada de dados em relação à quantidade de processadores.

Pelos valores de Speedup, é possível calcular também a eficiência dos algoritmos, para medir a porcentagem de uso de cada processador durante a execução.

> tabela

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
