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
    if (strcmp(a + i * length, a + (i + 1) * length) < 0) {
      printf("não ordenou");
    }
  }
}

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

void mergesort(char *a, long int *data, int left, int right, int length,
               int depth) {
  if(left < right) {
      int middle = (left + right) / 2;

      if(depth < 3) {
          #pragma omp task shared(a, data, length)
          mergesort(a, data, left, middle, length, depth + 1);
          #pragma omp task shared(a, data, length)
          mergesort(a, data, middle + 1, right, length, depth + 1);
      } else {
          mergesort(a, data, left, middle, length, depth + 1);
          mergesort(a, data, middle + 1, right, length, depth + 1);
      }

      #pragma omp taskwait
      merge(a, data, left, middle, right, length);
  }
}

void sort(char *a, bucket *bucket) {
  mergesort(a, bucket->data, 0, bucket->total - 1, bucket->length, 0);
}

long int *bucket_sort(char *a, int length, long int size, int offset,
                      int nbuckets) {
  long int i;
  bucket *buckets = NULL, *b = NULL;
  long int *returns = NULL;

  returns = (long int *)malloc(sizeof(long int) * size);
  buckets = (bucket *)malloc(sizeof(bucket) * nbuckets);

  for (i = 0; i < nbuckets; i++) {
    buckets[i].data = returns + i * size / nbuckets;
    buckets[i].length = length;
    buckets[i].total = 0;
  }

  for (i = 0; i < size; i++) {
    b = &buckets[*(a + i * length) - offset];
    b->data[b->total++] = i;
  }

  #pragma omp parallel for shared(a, buckets)
  for (i = 0; i < nbuckets; i++)
    sort(a, &buckets[i]);

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
  FILE *file = fopen("output.txt", "w");
  for (int i = 0; i < n; i++) {
    fprintf(file, "%s\n", strings + (index[i] * len));
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

  // sorted(strings, len, N);
  //  Medir o tempo de salvar/imprimir os dados
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
