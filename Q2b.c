#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 24  // Maximum number of threads for comparison
#define N 100000000  // Total number of items to process

// Global variables
int sum = 0;  // Shared sum variable
pthread_mutex_t m;

// Function to simulate computation (Compute_next_value)
int Compute_next_value(int i) {
    return i * i;  // Simulated computation (e.g., square of the index)
}

// Function to calculate the serial sum (single-threaded)
int SerialSum() {
    int total = 0;
    for (int i = 0; i < N; i++) {
        total += Compute_next_value(i);
    }
    return total;
}

// Parallel function to sum with multiple threads
void *ParallelSumWithBarrier(void *arg) {
    int thread_id = *((int *)arg);
    int items_per_task = N / NUM_THREADS;
    if (items_per_task <= 0) items_per_task = 1;  // Prevent division by zero

    int start = thread_id * items_per_task;
    int my_sum = 0;

    for (int i = start; i < start + items_per_task && i < N; i++) {
        int my_x = Compute_next_value(i);
        my_sum += my_x;
    }

    pthread_mutex_lock(&m);
    sum += my_sum;
    pthread_mutex_unlock(&m);

    return NULL;
}

// Function to save results to CSV file
void save_results_to_csv(FILE *file, int threads, double T_serial, double T_parallel, double speedup) {
    fprintf(file, "%d,%.6f,%.6f,%.6f\n", threads, T_serial, T_parallel, speedup);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Initialize the mutex
    pthread_mutex_init(&m, NULL);

    // Open CSV file for writing
    FILE *file = fopen("q2_b_results.csv", "w");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    // Write header to CSV file
    fprintf(file, "Threads,T_serial,T_parallel,Speedup\n");

    // Measure serial execution time
    clock_t start_serial = clock();
    int serial_sum = SerialSum();
    clock_t end_serial = clock();
    double T_serial = (double)(end_serial - start_serial) / CLOCKS_PER_SEC;
    printf("Serial execution time: %.6f seconds\n", T_serial);

    // Loop over thread counts from 2 to 12
    for (int num_threads = 2; num_threads <= NUM_THREADS; num_threads+=2) {
        sum = 0;  // Reset the global sum

        // Measure parallel execution time
        clock_t start_parallel = clock();
        for (int i = 0; i < num_threads; i++) {
            thread_ids[i] = i;
            pthread_create(&threads[i], NULL, ParallelSumWithBarrier, &thread_ids[i]);
        }

        // Join threads
        for (int i = 0; i < num_threads; i++) {
            pthread_join(threads[i], NULL);
        }
        clock_t end_parallel = clock();
        double T_parallel = (double)(end_parallel - start_parallel) / CLOCKS_PER_SEC;
        printf("Parallel execution time with %d threads: %.6f seconds\n", num_threads, T_parallel);

        // Calculate speedup
        double speedup = T_serial / T_parallel;
        printf("Speedup: %.2f\n", speedup);

        // Save results to CSV
        save_results_to_csv(file, num_threads, T_serial, T_parallel, speedup);
    }

    // Close CSV file
    fclose(file);

    // Cleanup
    pthread_mutex_destroy(&m);

    printf("Results saved to 'profitability_data.csv'\n");

    return 0;
}
