### Métricas Radix Sort

A análise das métricas obtidas indica que o algoritmo do Radix Sort possui uma alta eficiência, executando rapidamente na sua versão sequencial e obtendo um desempenho muito semelhante na sua versão paralela.

> tabela

Os valores de Speedup medidos para as execuções com 1, 2 e 3 processadores são valores abaixo de 1, indicando que, nestas condições, o algoritmo possui um desempenho inferior ao algoritmo paralelo. Esse baixo desempenho ocorre devido a uma sobrecarga dos recursos computacionais, causada pelo paralelismo na ordenação dos baldes e no próprio Radix Sort.

O Speedup obtido para 4 processadores foi de 1,171, representando um aumento de eficiência, porém este aumento é de apenas 17%. Sugerindo que a paralelização do algoritmo não trouxe ganhos substanciais no cenário testado.

No entanto, para entender melhor este resultado para 4 processadores, foram feitos outros testes com a versão sequencial e uma análise mais detalhada da parte do código que pode ser paralelizada revelou que apenas 20,7% do tempo de execução total corresponde à parte paralelizável.

Este valor justifica o baixo desempenho do algoritmo paralelo, uma vez que a parte paralelizável não representa uma porção significativa da execução do algoritmo.

> tabela

A partir destas medições, é possível aplicar a Lei de Amdahl para calcular os valores de Speedup teóricos conforme o aumento do número de processadores e os valores obtidos a partir dos testes:

> tabela

Os cálculos indicam que a solução paralela com 4 processadores deve ter um Speedup próximo a 1,184, valor que se aproxima do Speedup real obtido na implementação do algoritmo. Os dados também indicam que o número máximo eficiente de processadores para paralelizar este código seria de 4, e adicionar mais processadores não resultaria em ganhos significativos no desempenho do algoritmo.

A partir desses números, é possível realizar o cálculo da eficiência do algoritmo, que mostra o quanto de cada processador foi utilizado nas execuções.

> tabela

Nas execuções com 2 ou mais processadores, a eficiência do algoritmo é baixa, cerca de 30%. Esse desempenho pode ser explicado pela pequena proporção do código que pode ser paralelizada. Dessa forma, a parte paralelizada do código é distribuída entre vários processadores, o que reduz a carga de trabalho de cada um, mas também limita os ganhos de desempenho.

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/radixsort.png" alt="Gráfico do tempo de execução do Radix Sort">

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/radixsort.png" alt="Gráfico do cálculo de Speedup do Radix Sort">

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/radixsort.png" alt="Gráfico do cálculo de eficiência do Radix Sort">