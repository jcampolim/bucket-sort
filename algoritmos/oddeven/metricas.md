### Métricas Odd Even Sort

A análise dos testes do algoritmo Odd-Even Sort paralelo revela um ganho expressivo de desempenho em comparação com a versão sequencial. Essa melhoria é evidenciada pelas métricas de tempo de execução coletadas:

> tabela

Os resultados indicam que, para 1 e 3 cores, o algoritmo paralelo não apresentou grande eficiência, o que pode ser atribuído à forma como as tarefas são distribuídas pelo escalonador. No entanto, ao utilizar 2 cores, o algoritmo mostrou uma melhora significativa em relação ao código sequencial, que foi cerca de 30 segundos mais lento. E as execuções com 4 processadores se destacaram, com uma redução de três vezes no tempo total de execução em comparação ao código sequencial.

Ao analisar os tempos de execução das diferentes partes do código, observa-se que a porção que pode ser paralelizada, responsável pela ordenação, corresponde a aproximadamente 99% do tempo total de execução do algoritmo. Essa informação é crucial, pois possibilita a implementação da Lei de Amdahl para estimar o ganho de desempenho à medida que o número de processadores aumenta.

> tabela

Com base nos dados teóricos derivados da Lei de Amdahl, os valores de Speedup para 2 e 4 processadores são elevados, indicando que o desempenho do algoritmo está se aproximando de seu limite teórico de paralelização. No entanto, os valores de Speedup para 1 e 3 processadores sugerem que há potencial para uma utilização mais eficiente dos recursos computacionais, indicando que o algoritmo ainda pode possuir partes que podem ser paralelizadas para melhorar o desempenho.

A partir dos valores de Speedup, é possível calcular a eficiência do algoritmo, que reflete a porcentagem de uso de cada processador durante as execuções.

> tabela

Os valores de eficiência indicam que as execuções com 2 e 3 processadores não fizeram um uso otimizado dos recursos disponíveis, especialmente com 3 processadores, onde a eficiência foi mais limitada. No entanto, com 4 processadores, a eficiência quase atinge 80%, o que demonstra que nessas condições, o algoritmo utiliza melhor os recursos disponíveis.

Para entender melhor o comportamento do algoritmo perante a uma variação de processadores, é preciso realizar testes mais extensos e com uma variação maior na quantidade de processadores.

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/oddeven.png" alt="Gráfico do tempo de execução do Odd Even Sort">

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/oddeven.png" alt="Gráfico do cálculo de Speedup do Odd Even Sort">

<img src="https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/oddeven.png" alt="Gráfico do cálculo de eficiência do Odd Even Sort">