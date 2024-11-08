#include <stdio.h>
#include <math.h>

double calculate_efficiency(double T_serial, double T_overhead, int p);

int main() {
    int p = 4; // Fixed number of processors
    int max_problem_size = 5000; // Increased range for problem size
    int step_size = 500; // Larger steps for better visualization

    // Open a CSV file for writing
    FILE *file = fopen("q1_b_results.csv", "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return 1;
    }

    // Write the CSV header
    fprintf(file, "Problem Size,T_serial,T_overhead (slow growth),Efficiency (slow growth),T_overhead (fast growth),Efficiency (fast growth)\n");

    for (int problem_size = step_size; problem_size <= max_problem_size; problem_size += step_size) {
        double T_serial = problem_size;

        // Exaggerated slow growth: Logarithmic
        double T_overhead_slow = log(problem_size + 1);
        double efficiency_slow = calculate_efficiency(T_serial, T_overhead_slow, p);

        // Exaggerated fast growth: Cubic
        double T_overhead_fast = pow(problem_size, 3) * 0.00001; // Scaled down for realism
        double efficiency_fast = calculate_efficiency(T_serial, T_overhead_fast, p);

        // Write results to CSV
        fprintf(file, "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                problem_size, T_serial, T_overhead_slow, efficiency_slow, T_overhead_fast, efficiency_fast);
    }

    fclose(file);
    printf("Updated data written to parallel_efficiency_analysis_updated.csv.\n");

    return 0;
}

double calculate_efficiency(double T_serial, double T_overhead, int p) {
    double T_parallel = T_serial / p + T_overhead;
    return T_serial / (p * T_parallel);
}
