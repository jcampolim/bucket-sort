#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  long int *data;
  int length;
  long int total;
} bucket;

void sorted(char *a, int length, long int size) {
  for (long int i = 0; i < size - 1; i++) {
    if (strcmp(a + i * length, a + (i + 1) * length) > 0) {
      printf("não ordenou");
    }
  }
}

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

void quicksort(char *a, long int *data, int low, int high, int length, int depth) {
  if (low < high) {
    int pi = partition(a, data, low, high, length);

    if (depth < 3) {
#pragma omp task shared(a, data, length) if (high - low > 1000)
      quicksort(a, data, low, pi - 1, length, depth + 1);

#pragma omp task shared(a, data, length) if (high - low > 1000)
      quicksort(a, data, pi + 1, high, length, depth + 1);

#pragma omp taskwait
    } else {
      quicksort(a, data, low, pi - 1, length, depth + 1);
      quicksort(a, data, pi + 1, high, length, depth + 1);
    }
  }
}

void sort(char *a, bucket *bucket) {
  #pragma omp parallel
  {
    #pragma omp single
    quicksort(a, bucket->data, 0, bucket->total - 1, bucket->length, 0);
  }
}

long int *bucket_sort(char *a, int length, long int size, int offset,
                      int nbuckets) {
  long int i;
  bucket *buckets = NULL;
  long int *returns = NULL;

  returns = (long int *)malloc(sizeof(long int) * size);
  buckets = (bucket *)malloc(sizeof(bucket) * nbuckets);

  for (i = 0; i < nbuckets; i++) {
    buckets[i].data = returns + i * size / nbuckets;
    buckets[i].length = length;
    buckets[i].total = 0;
  }

  for (i = 0; i < size; i++) {
    bucket *b = &buckets[*(a + i * length) - offset];
    b->data[b->total++] = i;
  }


  #pragma omp parallel for shared(a, buckets)
  for (i = 0; i < nbuckets; i++) {
    sort(a, &buckets[i]);
  }

  free(buckets);
  return returns;
}

void load(char **vString, long int *N, int *l, int *os, int *nb) {
  char *strings = NULL;
  int len, offset, nbucket;
  long int n;

  if (!fscanf(stdin, "%d", &len)) {
    printf("ERROR len\n");
    exit(1);
  }

  if (!fscanf(stdin, "%ld", &n)) {
    printf("ERROR n\n");
    exit(1);
  }

  if (!fscanf(stdin, "%d", &offset)) {
    printf("ERROR offset\n");
    exit(1);
  }

  if (!fscanf(stdin, "%d", &nbucket)) {
    printf("ERROR nbucket\n");
    exit(1);
  }

  nbucket = nbucket + 1;
  len = len + 1;

  strings = (char *)malloc(n * len);

  for (int i = 0; i < n; i++) {
    if (!fscanf(stdin, "%s", strings + (i * len))) {
      printf("ERROR %d\n", i);
      exit(1);
    }
  }

  *vString = strings;
  *N = n;
  *l = len;
  *os = offset;
  *nb = nbucket;
}

void save(char *strings, long int *index, long int n, int len) {
  for (int i = 0; i < n; i++) {
    printf("%s\n", strings + (index[i] * len));
  }
}

int main(int ac, char **av) {
    char *strings = NULL;
    long int N = 0;
    long int *index = NULL;
    int len = 0, offset, nbuckets;

    double startTime, endTime;
    double loadTime, sortTime, saveTime;

    // Medir o tempo de carregamento dos dados
    startTime = omp_get_wtime();
    load(&strings, &N, &len, &offset, &nbuckets);
    endTime = omp_get_wtime();
    loadTime = endTime - startTime;

    // Medir o tempo de ordenação
    startTime = omp_get_wtime();
    index = bucket_sort(strings, len, N, offset, nbuckets);
    endTime = omp_get_wtime();
    sortTime = endTime - startTime;

    // Medir o tempo de salvar/imprimir os dados
    startTime = omp_get_wtime();
    save(strings, index, N, len);
    endTime = omp_get_wtime();
    saveTime = endTime - startTime;

    // Exibir os tempos medidos
    printf("Tempo de carregamento: %f segundos\n", loadTime);
    printf("Tempo de ordenação: %f segundos\n", sortTime);
    printf("Tempo de salvamento/impressão: %f segundos\n", saveTime);

    // Liberar memória alocada
    free(strings);
    free(index);

    return EXIT_SUCCESS;
}
