#include <stdio.h>
#include <math.h>

int main() {
    // Define arrays for n and p
    int n_values[] = {10, 20, 40, 80, 160, 320};
    int p_values[] = {1, 2, 4, 8, 16, 32, 64, 128};
    int n_count = sizeof(n_values) / sizeof(n_values[0]);
    int p_count = sizeof(p_values) / sizeof(p_values[0]);
    
    // Open a file to store results
    FILE *file = fopen("q1_a_results.csv", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return 1;
    }
    // Write header to the CSV file
    fprintf(file, "n,p,Tserial,Tparallel,Speedup,Efficiency\n");

    // Display table header
    printf("n\tp\tTserial\tTparallel\tSpeedup\tEfficiency\n");

    // Loop over values of n
    for (int i = 0; i < n_count; i++) {
        int n = n_values[i];
        double Tserial = n * n; // Tserial = n^2

        // Loop over values of p
        for (int j = 0; j < p_count; j++) {
            int p = p_values[j];
            double Tparallel = (n * n) / (double)p + log2(p); // Tparallel = n^2 / p + log2(p)
            double speedup = Tserial / Tparallel;             // Speedup = Tserial / Tparallel
            double efficiency = speedup / p;                 // Efficiency = Speedup / p

            // Print results
            printf("%d\t%d\t%.2f\t%.2f\t\t%.2f\t%.2f\n", n, p, Tserial, Tparallel, speedup, efficiency);
            // Write results to the file
            fprintf(file, "%d,%d,%.2f,%.2f,%.2f,%.2f\n", n, p, Tserial, Tparallel, speedup, efficiency);
        }

    }
    fclose(file);
    printf("Results written to results.csv\n");
    return 0;
}
