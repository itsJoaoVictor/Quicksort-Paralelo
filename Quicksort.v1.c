#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 10

typedef struct {
    int* arr;
    int low;
    int high;
} QuickSortArgs;

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

void* quickSortThread(void* arg) {
    QuickSortArgs* args = (QuickSortArgs*) arg;
    quickSort(args->arr, args->low, args->high);
    return NULL;
}

void printArray(int arr[], int size){
   int i;
   for (i=0; i < size; i++)
      printf("%d ", arr[i]);
   printf("\n");
}

int main(){
    int arr[] = {10, 8, 11, 15, 7, 12, 33, 3};
    int n = sizeof(arr)/sizeof(arr[0]);

    // Initialize thread pool
    pthread_t threads[MAX_THREADS];
    int num_threads = 2; // Number of threads in the thread pool

    // Divide the list into num_threads parts and sort each part in a separate thread
    QuickSortArgs args[num_threads];
    int part_size = n / num_threads;
    for (int i = 0; i < num_threads; i++) {
        args[i].arr = arr;
        args[i].low = i * part_size;
        args[i].high = (i == num_threads - 1) ? n - 1 : (i + 1) * part_size - 1;
        pthread_create(&threads[i], NULL, quickSortThread, &args[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Merge the sorted parts
    int i, j, k;
    i = 0;
    j = part_size;
    k = 0;
    int temp[n];
    while (i < part_size && j < n) {
        if (arr[i] < arr[j]) {
            temp[k] = arr[i];
            i++;
        } else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

while (i < part_size) {
    temp[k] = arr[i];
    i++;
    k++;
}


while (j < n) {
    temp[k] = arr[j];
    j++;
    k++;
}


for (i = 0; i < n; i++) {
    arr[i] = temp[i];
}

printf("The sorted array is: \n");
printArray(arr, n);
return 0;
}