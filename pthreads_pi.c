// /********************************************************************

//                   Example 3  (pthreads_pi.c)

// Objective                  : To calculate the value of pi using the given
//                              number of intervals.
//                              Demonstrates the usage of:
//                              pthread_create()
//                              pthread_join()
//                              pthread_mutex_t
//                              pthread_mutex_lock
//                              pthread_mutex_unlock()

// Input                      : The number of intervals.

// Output                     : Value of Pi using the given intervals.

//  ********************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Global Variables
double intervalWidth, area = 0.0;
int numberOfIntervals;

// Mutex for thread-safe access to `area`
pthread_mutex_t area_mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread Function
void* myPartOfCalc(void* threadID) {
    int myID = *(int*)threadID;  // Extract thread ID
    double myArea = 0.0;

    for (int myInterval = myID + 1; myInterval <= numberOfIntervals; myInterval += numberOfIntervals) {
        double myIntervalMidPoint = ((double)myInterval - 0.5) * intervalWidth;
        myArea += (4.0 / (1.0 + myIntervalMidPoint * myIntervalMidPoint));
    }

    pthread_mutex_lock(&area_mutex);
    area += myArea * intervalWidth;
    pthread_mutex_unlock(&area_mutex);

    printf("\nThread %d: Partial Area: %f", myID, myArea);
    return NULL;
}

// Main Function
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <number_of_intervals> <number_of_threads>\n", argv[0]);
        return -1;
    }

    numberOfIntervals = abs(atoi(argv[1]));
    int numThreads = numberOfIntervals;  // Use one thread per interval

    if (numberOfIntervals == 0) {
        numberOfIntervals = 50;
        numThreads = 50;
    }

    printf("\nNumber of Intervals: %d", numberOfIntervals);

    // Allocate memory for threads and thread IDs
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * numThreads);
    int* threadIDs = (int*)malloc(sizeof(int) * numThreads);

    // Calculate Interval Width
    intervalWidth = 1.0 / (double)numberOfIntervals;

    // Create Threads
    for (int i = 0; i < numThreads; i++) {
        threadIDs[i] = i;
        pthread_create(&threads[i], NULL, myPartOfCalc, &threadIDs[i]);
    }

    // Wait for Threads to Complete
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the Final Result
    printf("\n\nThe Computed Value of PI: %.15f\n", area);

    // Clean up resources
    free(threads);
    free(threadIDs);

    pthread_mutex_destroy(&area_mutex);
    return 0;
}
