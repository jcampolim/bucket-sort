### Métricas Odd Even Sort

A análise dos testes do algoritmo Odd-Even Sort paralelo revela um ganho expressivo de desempenho em comparação com a versão sequencial. Essa melhoria é evidenciada pelas métricas de tempo de execução coletadas:

#### Tabela de Tempos de Execução

| Execução | Carregamento (s) | Ordenação (s) | Saída (s) | % Paralelizável |
|----------|------------------|---------------|-----------|-----------------|
| 1        | 0,050506         | 208,840107    | 0,061047  | 99,9466%        |
| 2        | 0,048604         | 211,932168    | 0,066675  | 99,9456%        |
| 3        | 0,066675         | 209,005305    | 0,076723  | 99,9314%        |
| 4        | 0,049614         | 208,384901    | 0,071159  | 99,9421%        |
| 5        | 0,052868         | 211,017792    | 0,076421  | 99,9388%        |
| 6        | 0,048122         | 213,023183    | 0,078211  | 99,9407%        |
| 7        | 0,060124         | 214,810552    | 0,070812  | 99,9391%        |
| 8        | 0,057143         | 216,173419    | 0,065333  | 99,9434%        |
| 9        | 0,050159         | 215,499776    | 0,069776  | 99,9444%        |
| 10       | 0,055208         | 216,001365    | 0,070207  | 99,9419%        |

**Média: % Paralelizável**: 99,9420%

Os resultados indicam que, para 1 e 3 cores, o algoritmo paralelo não apresentou grande eficiência, o que pode ser atribuído à forma como as tarefas são distribuídas pelo escalonador. No entanto, ao utilizar 2 cores, o algoritmo mostrou uma melhora significativa em relação ao código sequencial, que foi cerca de 30 segundos mais lento. E as execuções com 4 processadores se destacaram, com uma redução de três vezes no tempo total de execução em comparação ao código sequencial.

Ao analisar os tempos de execução das diferentes partes do código, observa-se que a porção que pode ser paralelizada, responsável pela ordenação, corresponde a aproximadamente 99% do tempo total de execução do algoritmo. Essa informação é crucial, pois possibilita a implementação da Lei de Amdahl para estimar o ganho de desempenho à medida que o número de processadores aumenta.

#### Tabela Amdahl vs Obtido

| Processadores | Amdahl              | Obtido          |
|---------------|---------------------|-----------------|
| 1             | 1                   | 0,9515080837    |
| 2             | 1,998841156         | 1,138971267     |
| 3             | 2,996525482         | 0,9280187217    |
| 4             | 3,993054985         | 3,127129349     |
| 8             | 7,967664788         | -               |
| 16            | 15,86205772         | -               |

Com base nos dados teóricos derivados da Lei de Amdahl, os valores de Speedup para 2 e 4 processadores são elevados, indicando que o desempenho do algoritmo está se aproximando de seu limite teórico de paralelização. No entanto, os valores de Speedup para 1 e 3 processadores sugerem que há potencial para uma utilização mais eficiente dos recursos computacionais, indicando que o algoritmo ainda pode possuir partes que podem ser paralelizadas para melhorar o desempenho.

A partir dos valores de Speedup, é possível calcular a eficiência do algoritmo, que reflete a porcentagem de uso de cada processador durante as execuções.

#### Tabela de Eficiência

| Processadores | Speed Up | Eficiência     |
|---------------|----------|----------------|
| 1             | 0,951508 | 0,9515080837   |
| 2             | 1,138971 | 0,5694856335   |
| 3             | 0,928018 | 0,3093395739   |
| 4             | 3,127129 | 0,7817823373   |

Os valores de eficiência indicam que as execuções com 2 e 3 processadores não fizeram um uso otimizado dos recursos disponíveis, especialmente com 3 processadores, onde a eficiência foi mais limitada. No entanto, com 4 processadores, a eficiência quase atinge 80%, o que demonstra que nessas condições, o algoritmo utiliza melhor os recursos disponíveis.

Para entender melhor o comportamento do algoritmo perante a uma variação de processadores, é preciso realizar testes mais extensos e com uma variação maior na quantidade de processadores.

<div align="center">
   ![Gráfico do tempo de execução do Odd Even Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/tempo-execucao/oddeven.png)
</div>

<div align="center">
   ![Gráfico do cálculo de Speedup do Odd Even Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/speedup/oddeven.png)
</div>

<div align="center">
   ![Gráfico do cálculo de eficiência do Odd Even Sort](https://github.com/jcampolim/bucket-sort/blob/main/assets/eficiencia/oddeven.png)
</div>
