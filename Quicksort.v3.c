#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 8

typedef struct {
    int *arr;
    long int low;
    long int high;
} task_arg;

void parallel_quicksort(int arr[], long int low, long int high, int num_threads);

pthread_mutex_t mutex;

int partition(int arr[], long int low, long int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i+1];
    arr[i+1] = arr[high];
    arr[high] = temp;

    return i+1;
}

void sequential_quicksort(int arr[], long int low, long int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        sequential_quicksort(arr, low, pivot-1);
        sequential_quicksort(arr, pivot+1, high);
    }
}

void *thread_quicksort(void *arg) {
    task_arg *targ = (task_arg*) arg;
    int *arr = targ->arr;
    long int low = targ->low;
    long int high = targ->high;
    free(arg);

    if (low < high) {
        int pivot = partition(arr, low, high);
        task_arg *left_arg = (task_arg*) malloc(sizeof(task_arg));
        task_arg *right_arg = (task_arg*) malloc(sizeof(task_arg));
        left_arg->arr = right_arg->arr = arr;
        left_arg->low = low;
        left_arg->high = pivot-1;
        right_arg->low = pivot+1;
        right_arg->high = high;

        pthread_t left_thread, right_thread;
        pthread_create(&left_thread, NULL, thread_quicksort, (void*) left_arg);
        pthread_create(&right_thread, NULL, thread_quicksort, (void*) right_arg);
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);
    }

    pthread_exit(NULL);
}

void parallel_quicksort(int arr[], long int low, long int high, int num_threads) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        task_arg *left_arg = (task_arg*) malloc(sizeof(task_arg));
        task_arg *right_arg = (task_arg*) malloc(sizeof(task_arg));
        left_arg->arr = right_arg->arr = arr;
        left_arg->low = low;
        left_arg->high = pivot-1;
        right_arg->low = pivot+1;
        right_arg->high = high;

        if (num_threads > 1) {
            pthread_t left_thread, right_thread;
            pthread_create(&left_thread, NULL, thread_quicksort, (void*) left_arg);
            pthread_create(&right_thread, NULL, thread_quicksort, (void*) right_arg);
            pthread_join(left_thread, NULL);
            pthread_join(right_thread, NULL);
        } else {
            sequential_quicksort(arr, low, pivot-1);
            sequential_quicksort(arr, pivot+1, high);
         }
      } 
}

int main(int argc, char **argv) {
   int arr_size = 1000000;
   int num_threads = 2;
   if (argc > 1) {
    arr_size = atoi(argv[1]);
    if (argc > 2) {
        num_threads = atoi(argv[2]);
        if (num_threads > MAX_THREADS) {
            printf("Number of threads cannot be greater than %d. Setting to %d.\n", MAX_THREADS, MAX_THREADS);
            num_threads = MAX_THREADS;
        }
    }
}

   srand(time(NULL));
   int *arr = (int*) malloc(arr_size * sizeof(int));
   for (int i = 0; i < arr_size; i++) {
      arr[i] = rand();
   }

   printf("Sorting array of size %d with %d thread(s)\n", arr_size, num_threads);

   clock_t start_time = clock();

   pthread_mutex_init(&mutex, NULL);
   parallel_quicksort(arr, 0, arr_size-1, num_threads);
   pthread_mutex_destroy(&mutex);

   double elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
   printf("Elapsed time: %f\n", elapsed_time);

   free(arr);

   return 0;
}