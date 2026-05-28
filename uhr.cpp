/** uhr: generic time performance tester
 * Author: LELE
 *
 * Things to set up:
 * 0. Includes: include all files to be tested,
 * 1. Time unit: in elapsed_time,
 * 2. What to write on time_data,
 * 3. Data type and distribution of RNG,
 * 4. Additive or multiplicative stepping,
 * 5. The experiments: in outer for loop. */

#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

// Include to be tested files here

#include "utils.cpp"
#include "matrix_mul.h"

namespace {
    constexpr double kEpsilon = 1e-9;
    constexpr int kExperiment = 1; // 1: real [0,1], 2: int [-10,10], 3: identity, 4: constant
    constexpr bool kCheckCorrectness = true;
    constexpr bool kSweepN0 = false;
    constexpr int kN0Min = 2;
    constexpr int kN0Max = 128;

    bool matricesEqual(const Matriz& A, const Matriz& B) {
        int n = static_cast<int>(A.size());
        if (n != static_cast<int>(B.size())) {
            return false;
        }
        for (int i = 0; i < n; i++) {
            if (static_cast<int>(A[i].size()) != static_cast<int>(B[i].size())) {
                return false;
            }
            for (int j = 0; j < n; j++) {
                if (std::fabs(A[i][j] - B[i][j]) > kEpsilon) {
                    return false;
                }
            }
        }
        return true;
    }

    void fillMatrices(Matriz& A, Matriz& B, int n, std::mt19937_64& rng) {
        std::uniform_real_distribution<double> real_distr(0.0, 1.0);
        std::uniform_int_distribution<int> int_distr(-10, 10);

        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                if (kExperiment == 1) {
                    A[r][c] = real_distr(rng);
                    B[r][c] = real_distr(rng);
                } else if (kExperiment == 2) {
                    A[r][c] = static_cast<double>(int_distr(rng));
                    B[r][c] = static_cast<double>(int_distr(rng));
                } else if (kExperiment == 3) {
                    A[r][c] = (r == c) ? 1.0 : 0.0;
                    B[r][c] = (r == c) ? 1.0 : 0.0;
                } else if (kExperiment == 4) {
                    A[r][c] = 1.0;
                    B[r][c] = 2.0;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    // Validate and sanitize input
    std::int64_t runs, lower, upper, step;
    validate_input(argc, argv, runs, lower, upper, step);

    // Set up clock variables
    std::int64_t n, i, executed_runs;
    std::int64_t total_runs_additive = runs * (((upper - lower) / step) + 1);
    std::int64_t total_runs_multiplicative = runs * (floor(log(upper / double(lower)) / log(step)) + 1);
    std::vector<double> times(runs);
    std::vector<double> q;
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed_time = end_time - begin_time;

    // Set up random number generation
    std::random_device rd;
    std::mt19937_64 rng(rd());
    // File to write time data
    std::ofstream time_data;
    time_data.open(argv[1]);
    if (kSweepN0) {
        time_data << "n,n0_best,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4" << std::endl;
    } else {
        time_data << "n,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4" << std::endl;
    }

    // Begin testing
    std::cerr << "\033[0;36mRunning tests...\033[0m" << std::endl << std::endl;
    executed_runs = 0;
    for (n = lower; n <= upper; n *= step) {
        mean_time = 0;
        time_stdev = 0;

        // Test configuration goes here

        Matriz A = crearMatriz(static_cast<int>(n));
        Matriz B = crearMatriz(static_cast<int>(n));
        fillMatrices(A, B, static_cast<int>(n), rng);

        if (kCheckCorrectness && n <= 128) {
            Matriz C1 = multiplicacionClasica(A, B);
            Matriz C2 = strassent(A, B, 2);
            if (!matricesEqual(C1, C2)) {
                std::cerr << "Mismatch detected at n=" << n << std::endl;
                return 1;
            }
        }
        // Run to compute elapsed time
        if (kSweepN0) {
            int best_n0 = kN0Min;
            double best_mean = std::numeric_limits<double>::infinity();
            for (int n0 = kN0Min; n0 <= kN0Max; n0 *= 2) {
                mean_time = 0;
                for (i = 0; i < runs; i++) {
                    display_progress(++executed_runs, total_runs_multiplicative);

                    begin_time = std::chrono::high_resolution_clock::now();
                    Matriz C = strassent(A, B, n0);
                    volatile double sink = C[0][0];
                    (void)sink;
                    end_time = std::chrono::high_resolution_clock::now();

                    elapsed_time = end_time - begin_time;
                    times[i] = elapsed_time.count();
                    mean_time += times[i];
                }
                mean_time /= runs;
                if (mean_time < best_mean) {
                    best_mean = mean_time;
                    best_n0 = n0;
                }
            }
            mean_time = best_mean;

            for (i = 0; i < runs; i++) {
                dev = times[i] - mean_time;
                time_stdev += dev * dev;
            }
            time_stdev /= runs - 1;
            time_stdev = std::sqrt(time_stdev);
            quartiles(times, q);
            time_data << n << "," << best_n0 << "," << mean_time << "," << time_stdev << ",";
            time_data << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "," << q[4] << std::endl;
            continue;
        }

        for (i = 0; i < runs; i++) {
            // Remember to change total depending on step type
            display_progress(++executed_runs, total_runs_multiplicative);

            begin_time = std::chrono::high_resolution_clock::now();
            // Uncomment one algorithm to test
            Matriz C = multiplicacionClasica(A, B);
            //Matriz C = strassent(A, B, 2);
          //Matriz C = strassent(A, B, 64); //hybrid by changing n0
            volatile double sink = C[0][0];
            (void)sink;
            end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            times[i] = elapsed_time.count();

            mean_time += times[i];
        }

        // Compute statistics
        mean_time /= runs;

        for (i = 0; i < runs; i++) {
            dev = times[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= runs - 1; // Subtract 1 to get unbiased estimator
        time_stdev = std::sqrt(time_stdev);

        quartiles(times, q);

        time_data << n << "," << mean_time << "," << time_stdev << ",";
        time_data << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "," << q[4] << std::endl;
    }

    // This is to keep loading bar after testing
    std::cerr << std::endl << std::endl;
    std::cerr << "\033[1;32mDone!\033[0m" << std::endl;

    time_data.close();

    return 0;
}
