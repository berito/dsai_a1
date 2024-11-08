#include <stdio.h>
#include <pthread.h>
#include <time.h>

// Constants for the problem
#define S 1000000   // Sequential execution time (S)
#define O 100       // Parallelization overhead
#define B 10        // Barrier cost
#define M 5         // Mutex cost per thread

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

int sum = 0;
int n = 1000; // Total number of elements (you can adjust this)

int Compute_next_value(int index)
{
    return index; // Example computation
}

// Parallel Sum with Mutex
void *ParallelSumWithMutex(void *arg)
{
    int thread_id = *((int *)arg);
    int items_per_task = n / 4;  // Fixed 4 threads, can be replaced with 't'
    if (items_per_task <= 0) items_per_task = 1;  // Prevent division by zero

    int start = thread_id * items_per_task;
    int my_sum = 0;
    for (int i = start; i < start + items_per_task && i < n; i++)  // Prevent exceeding n
    {
        int my_x = Compute_next_value(i);
        my_sum += my_x;
    }

    pthread_mutex_lock(&m);
    sum += my_sum;
    pthread_mutex_unlock(&m);

    return NULL;
}

// Parallel Sum with Barrier
void *ParallelSumWithBarrier(void *arg)
{
    int thread_id = *((int *)arg);
    int items_per_task = n / 4;  // Fixed 4 threads, can be replaced with 't'
    if (items_per_task <= 0) items_per_task = 1;  // Prevent division by zero

    int start = thread_id * items_per_task;
    int my_sum = 0;
    for (int i = start; i < start + items_per_task && i < n; i++)  // Prevent exceeding n
    {
        int my_x = Compute_next_value(i);
        my_sum += my_x;
    }

    pthread_mutex_lock(&m);
    sum += my_sum;
    pthread_mutex_unlock(&m);

    return NULL;
}

// Parallel Sum with Shared Array and Barrier
void *ParallelSumWithSharedArrayAndBarrier(void *arg)
{
    int thread_id = *((int *)arg);
    int items_per_task = n / 4;  // Fixed 4 threads, can be replaced with 't'
    if (items_per_task <= 0) items_per_task = 1;  // Prevent division by zero

    static int my_sum[4];  // Maximum of 4 threads
    int start = thread_id * items_per_task;
    for (int i = start; i < start + items_per_task && i < n; i++)  // Prevent exceeding n
    {
        int my_x = Compute_next_value(i);
        my_sum[thread_id] += my_x;
    }

    pthread_barrier_wait(&barrier);

    if (thread_id == 0)
    {
        sum = my_sum[0];
        for (int i = 1; i < 4; i++)  // For 4 threads
        {
            sum += my_sum[i];
        }
    }

    return NULL;
}

// Launch threads with Mutex
void LaunchThreadsWithMutex(int t)
{
    pthread_t thread[t];
    int thread_ids[t];

    for (int i = 0; i < t; i++)
    {
        thread_ids[i] = i;
        pthread_create(&thread[i], NULL, ParallelSumWithMutex, &thread_ids[i]);
    }

    for (int i = 0; i < t; i++)
    {
        pthread_join(thread[i], NULL);
    }
}

// Launch threads with Barrier
void LaunchThreadsWithBarrier(int t)
{
    pthread_t thread[t];
    int thread_ids[t];

    for (int i = 0; i < t; i++)
    {
        thread_ids[i] = i;
        pthread_create(&thread[i], NULL, ParallelSumWithBarrier, &thread_ids[i]);
    }

    for (int i = 0; i < t; i++)
    {
        pthread_join(thread[i], NULL);
    }
}

// Launch threads with Shared Array and Barrier
void LaunchThreadsWithSharedArrayAndBarrier(int t)
{
    pthread_t thread[t];
    int thread_ids[t];

    pthread_barrier_init(&barrier, NULL, t);

    for (int i = 0; i < t; i++)
    {
        thread_ids[i] = i;
        pthread_create(&thread[i], NULL, ParallelSumWithSharedArrayAndBarrier, &thread_ids[i]);
    }

    for (int i = 0; i < t; i++)
    {
        pthread_join(thread[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
}

// Measure execution times and save to CSV
void measure_and_save_to_csv(FILE *file, int t)
{
    clock_t start_time, end_time;
    double time_taken;

    // Version with Mutex
    sum = 0;
    start_time = clock();
    LaunchThreadsWithMutex(t);
    end_time = clock();
    time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    time_taken += M * t + O;  // Add Mutex cost and parallelization overhead
    fprintf(file, "%d, Mutex, %f\n", t, time_taken);

    // Version with Barrier
    sum = 0;
    start_time = clock();
    LaunchThreadsWithBarrier(t);
    end_time = clock();
    time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    time_taken += B + O;  // Add Barrier cost and parallelization overhead
    fprintf(file, "%d, Barrier, %f\n", t, time_taken);

    // Version with Shared Array and Barrier
    sum = 0;
    start_time = clock();
    LaunchThreadsWithSharedArrayAndBarrier(t);
    end_time = clock();
    time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    time_taken += B + O;  // Add Barrier cost and parallelization overhead
    fprintf(file, "%d, SharedArray+Barrier, %f\n", t, time_taken);
}

int main()
{
    int t = 12;  // Set the number of threads (can be changed)

    // Open CSV file for writing
    FILE *file = fopen("../csv/q2_a_results.csv", "a");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return 1;
    }

    // Write the header
    fprintf(file, "Threads, Version, ExecutionTime\n");

    // Run the measurements for t threads
    measure_and_save_to_csv(file, t);

    // Close the CSV file
    fclose(file);

    // Clean up
    pthread_mutex_destroy(&m);

    return 0;
}
