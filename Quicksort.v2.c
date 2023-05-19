#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 4

typedef struct {
    int* arr;
    long int low;
    long int high;
} thread_data_t;

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high- 1; j++){
        if (arr[j] <= pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], long int low, long int high){
    if (low < high){
        long int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*) arg;
    quickSort(data->arr, data->low, data->high);
    return NULL;
}

void parallelQuickSort(int arr[], long int low, long int high, int n_threads) {
    pthread_t threads[MAX_THREADS];
    thread_data_t thread_data[MAX_THREADS];
    long int interval_size = (high - low + 1) / n_threads;
    long int remainder = (high - low + 1) % n_threads;
    long int next_low = low;
    long int next_high;

    for (int i = 0; i < n_threads; i++) {
        next_high = next_low + interval_size - 1;
        if (remainder > 0) {
            next_high++;
            remainder--;
        }

        thread_data[i].arr = arr;
        thread_data[i].low = next_low;
        thread_data[i].high = next_high;

        pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);

        next_low = next_high + 1;
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}

void printArray(int arr[], int size){
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(){
    long int n = 10000;
    int *arr = (int*)malloc(n * sizeof(int));
    srand(0);
    long int i;
    for (i = 0; i < n; i++) {
        arr[i] = rand() % n;
    }

    parallelQuickSort(arr, 0, n-1, 4);

    printf("The sorted array is: \n");
    printArray(arr, n);
    return 0;
}
