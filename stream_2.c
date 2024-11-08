#include <omp.h> // Include OpenMP header for parallelization functions

int main(int argc, char *argv[])
{
    int quantum, checktick();
    int BytesPerWord;
    int k, num_threads;
    ssize_t j;
    STREAM_TYPE scalar;
    double t, times[4][NTIMES];

    // Check if the number of threads is specified as a command-line argument
    if (argc > 1)
    {
        num_threads = atoi(argv[1]);
        omp_set_num_threads(num_threads); // Set the number of threads
    }
    else
    {
        num_threads = 1; // Default to 1 thread if no argument is provided
        omp_set_num_threads(num_threads);
    }

    printf(HLINE);
    printf("STREAM version $Revision: 5.10 $\n");
    printf(HLINE);
    BytesPerWord = sizeof(STREAM_TYPE);
    printf("This system uses %d bytes per array element.\n", BytesPerWord);
    printf(HLINE);

    printf("Number of Threads requested = %i\n", num_threads);

#ifdef _OPENMP
#pragma omp parallel
    {
#pragma omp master
        {
            printf("Number of Threads counted = %i\n", omp_get_num_threads());
        }
    }
#endif

    // Initialize arrays
#pragma omp parallel for
    for (j = 0; j < STREAM_ARRAY_SIZE; j++)
    {
        a[j] = 1.0;
        b[j] = 2.0;
        c[j] = 0.0;
    }

    printf(HLINE);

    // Timing loop for performance measurement
    scalar = 3.0;
    for (k = 0; k < NTIMES; k++)
    {
        times[0][k] = mysecond();
#pragma omp parallel for
        for (j = 0; j < STREAM_ARRAY_SIZE; j++)
            c[j] = a[j];
        times[0][k] = mysecond() - times[0][k];

        times[1][k] = mysecond();
#pragma omp parallel for
        for (j = 0; j < STREAM_ARRAY_SIZE; j++)
            b[j] = scalar * c[j];
        times[1][k] = mysecond() - times[1][k];

        times[2][k] = mysecond();
#pragma omp parallel for
        for (j = 0; j < STREAM_ARRAY_SIZE; j++)
            c[j] = a[j] + b[j];
        times[2][k] = mysecond() - times[2][k];

        times[3][k] = mysecond();
#pragma omp parallel for
        for (j = 0; j < STREAM_ARRAY_SIZE; j++)
            a[j] = b[j] + scalar * c[j];
        times[3][k] = mysecond() - times[3][k];
    }

    // Output performance results
    for (k = 1; k < NTIMES; k++)
    {
        for (j = 0; j < 4; j++)
        {
            avgtime[j] += times[j][k];
            mintime[j] = MIN(mintime[j], times[j][k]);
            maxtime[j] = MAX(maxtime[j], times[j][k]);
        }
    }
    // modified the code
    FILE *file = fopen("stream_benchmark.csv", "a"); // Open file for writing

    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    printf("Function    Best Rate MB/s  Avg time     Min time     Max time\n");
    fprintf(file, "Threads,Function,Best Rate MB/s,Avg time,Min time,Max time\n");
    for (j = 0; j < 4; j++)
    {
        avgtime[j] /= (double)(NTIMES - 1);
        printf("%s%12.1f  %11.6f  %11.6f  %11.6f\n", label[j],
               1.0E-06 * bytes[j] / mintime[j],
               avgtime[j], mintime[j], maxtime[j]);
        //    new added
        fprintf(file, "%d,%s,%.1f,%.6f,%.6f,%.6f\n", num_threads, label[j],
                1.0E-06 * bytes[j] / mintime[j], // Best Rate MB/s
                avgtime[j], mintime[j], maxtime[j]);
    }
    printf(HLINE);
    fclose(file);  // Close the file
    printf("Data written to output.csv successfully.\n");
    // Check results
    checkSTREAMresults();
    printf(HLINE);

    return 0;
}
