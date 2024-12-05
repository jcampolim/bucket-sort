### Métricas Quick Sort

A análise das métricas revela que a versão sequencial do algoritmo Quick Sort teve um desempenho inferior do que o esperado para um algoritmo com complexidade O(n log n). Este comportamento foi causado pela entrada de dados que está sendo utilizada para teste, que está ordenada ao contrário, representando o pior caso do Quick Sort e aumentando a complexidade do algoritmo para O(n^2).

> tabela

Entretanto, apesar de não ser o resultado esperado, a versão paralela do Quick Sort obteve um desempenho significativamente melhor quando comparada à versão sequencial. Os testes do algoritmo com 1 processador, mostraram uma execução inferior ao tempo sequencial, provavelmente por conta da sobrecarga gerada por um alto grau de paralelismo de funções. 

Os demais processadores apresentaram um ganho significativo com relação à versão sequencial, todos reduziram expressivamente o tempo de execução com relação ao código sequencial, demonstrando o impacto positivo na paralelização.

Para entender melhor o comportamento do algoritmo, foi analisada a proporção de tempo dedicada à parte paralelizada em comparação com a parte sequencial. Os resultados mostraram que 99,9% do tempo de execução do Bucket Sort, que utiliza o Quick Sort, é dedicado à parte paralela. Isso indica que o algoritmo possui um grande potencial de paralelização, já que a maior parte do trabalho pode ser distribuída entre múltiplos processadores.

> tabela

A aplicação da Lei de Amdahl à proporção de 99,9% de paralelização e 0,1% de sequencialidade permite estimar os valores teóricos de Speedup.

> tabela

O valor de Speedup alcançado na implementação do algoritmo paralelo foi próximo ao valor obtido pelos testes realizados. Em todos eles, o Speedup se mostrou elevado e próximo ao limite máximo, com destaque para a execução com 4 processadores, que conseguiu um Speedup de 3,065. Indicando que, sob estas condições, a versão paralela do algoritmo é três vezes mais rápida que a versão sequencial.

> tabela

Os valores de eficiência demonstram um alto grau de uso dos processadores com 1 e 2 processadores. Possuindo um desempenho menor com 3 e 4 processadores, mas ainda sendo um valor elevado.

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/quicksort.png" alt="Gráfico do tempo de execução do Quick Sort">

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/quicksort.png" alt="Gráfico do cálculo de Speedup do Quick Sort">

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/quicksort.png" alt="Gráfico do cálculo de eficiência do Quick Sort">