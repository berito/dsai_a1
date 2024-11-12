#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

// Global Variables
double intervalWidth, area = 0.0;
long numberOfIntervals;
int numberOfThreads;
pthread_mutex_t area_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to get current time
double getTime() {
    const double kMicro = 1.0e-6;
    struct timeval TV;
    if (gettimeofday(&TV, NULL) == -1) {
        printf("ERROR: Bad call to gettimeofday\n");
        return -1;
    }
    return ((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec);
}

// Thread Function for Pthreads Parallel Calculation
void* calculatePartialSum(void* threadID) {
    int myID = *(int*)threadID;
    double myArea = 0.0;

    long intervalsPerThread = numberOfIntervals / numberOfThreads;
    long start = myID * intervalsPerThread + 1;
    long end = (myID == numberOfThreads - 1) ? numberOfIntervals : start + intervalsPerThread - 1;

    for (long myInterval = start; myInterval <= end; myInterval++) {
        double myIntervalMidPoint = ((double)myInterval - 0.5) * intervalWidth;
        myArea += (4.0 / (1.0 + myIntervalMidPoint * myIntervalMidPoint));
    }

    pthread_mutex_lock(&area_mutex);
    area += myArea * intervalWidth;
    pthread_mutex_unlock(&area_mutex);

    return NULL;
}

// Parallel Calculation of PI using Pthreads
double parallelCompute(long intervals, int threads) {
    pthread_t* threadHandles = (pthread_t*)malloc(sizeof(pthread_t) * threads);
    int* threadIDs = (int*)malloc(sizeof(int) * threads);

    numberOfThreads = threads;
    intervalWidth = 1.0 / (double)intervals;
    area = 0.0;

    for (int i = 0; i < threads; i++) {
        threadIDs[i] = i;
        pthread_create(&threadHandles[i], NULL, calculatePartialSum, &threadIDs[i]);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(threadHandles[i], NULL);
    }

    free(threadHandles);
    free(threadIDs);
    return area;
}

// Compute the difference from the actual value of PI
double getDifference(double calculatedPi) {
    const double PI = 3.14159265358979323846;
    return fabs(calculatedPi - PI);
}

// Main Function
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_intervals>\n", argv[0]);
        return -1;
    }

    numberOfIntervals = abs(atol(argv[1]));
    if (numberOfIntervals <= 0) {
        printf("Error: Number of intervals must be greater than 0.\n");
        return -1;
    }

    FILE* file = fopen("csv/q4_2_a_and_b_results.csv", "w");
    if (!file) {
        printf("Error: Unable to create results.csv file.\n");
        return -1;
    }

    // Write CSV header
    fprintf(file, "Threads,SequentialTime,ParallelTime,Speedup,Efficiency,Validation\n");

    // Perform sequential calculation
    double sequentialStart = getTime();
    double sequentialPi = parallelCompute(numberOfIntervals, 1); // Single-threaded computation
    double sequentialEnd = getTime();
    double sequentialTime = sequentialEnd - sequentialStart;

    printf("Sequential Calculation Time: %.6f seconds\n", sequentialTime);

    const double TOLERANCE = 1e-6;
    int threadCounts[] = {1, 2, 4, 8, 16};
    for (int i = 0; i < 5; i++) {
        int threads = threadCounts[i];

        double parallelStart = getTime();
        double parallelPi = parallelCompute(numberOfIntervals, threads);
        double parallelEnd = getTime();

        double parallelTime = parallelEnd - parallelStart;
        double speedup = sequentialTime / parallelTime;
        double efficiency = speedup / threads;

        int valid = (fabs(sequentialPi - parallelPi) < TOLERANCE);

        printf("Threads: %d, Sequential Time: %.6f, Parallel Time: %.6f, Speedup: %.6f, Efficiency: %.6f, Validation: %s\n",
               threads, sequentialTime, parallelTime, speedup, efficiency, valid ? "VALID" : "INVALID");

        // Write to CSV
        fprintf(file, "%d,%.6f,%.6f,%.6f,%.6f,%s\n",
                threads, sequentialTime, parallelTime, speedup, efficiency, valid ? "VALID" : "INVALID");
    }

    fclose(file);
    printf("Results saved to results.csv\n");
    return 0;
}
