#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_RADIX 256

typedef struct {
    long int *data;
    int length;
    long int total;
} bucket;

void sorted(char *a, int length, long int size) {
   for(long int i = 0; i < size - 1; i++) {
      if(strcmp(a + i * length, a + (i + 1) * length) > 0) {
         printf("não ordenou");
      }
   }
}

void radix(char *a, long int *data, int size, int position, int length) {
    long int *sortedData = (long int *) malloc(size * sizeof(long int));
    int *RADIXCOUNT = (int *) malloc(MAX_RADIX * sizeof(int));  

    for (int i = 0; i < MAX_RADIX; i++) {
        RADIXCOUNT[i] = 0;  
    }

    for (int i = 0; i < size; i++) {
        int valorPos = a[data[i] * length + position];
        RADIXCOUNT[valorPos]++;
    }

    for (int i = 1; i < MAX_RADIX; i++) {
        RADIXCOUNT[i] += RADIXCOUNT[i - 1];  
    }

    for (int i = size - 1; i >= 0; i--) {
        int valorPos = a[data[i] * length + position];
        sortedData[RADIXCOUNT[valorPos] - 1] = data[i]; 
        RADIXCOUNT[valorPos]--;
    }

    for (int i = 0; i < size; i++) {
        data[i] = sortedData[i];
    }

    free(sortedData);
    free(RADIXCOUNT);
}

void radixsort(char *a, long int *data, int size, int length) {
    for (int position = length - 1; position >= 0; position--) {
        radix(a, data, size, position, length);  
    }
}

void sort(char *a, bucket *bucket) {
    radixsort(a, bucket->data, bucket->total, bucket->length);
}

long int* bucket_sort(char *a, int length, long int size, int offset, int nbuckets) {
    long int i;
    bucket *buckets = NULL, *b = NULL;
    long int *returns = NULL;

    returns = (long int *) malloc(sizeof(long int) * size);
    buckets = (bucket *) malloc(sizeof(bucket) * nbuckets);

    for (i = 0; i < nbuckets; i++) {
        buckets[i].data = returns + i * (size / nbuckets);
        buckets[i].length = length;
        buckets[i].total = 0;
    }

    for (i = 0; i < size; i++) {
        b = &buckets[*(a + i * length) - offset];
        b->data[b->total++] = i;
    }

    for (i = 0; i < nbuckets; i++) {
        sort(a, &buckets[i]);
    }

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

    strings = (char*) malloc(n * len);

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
