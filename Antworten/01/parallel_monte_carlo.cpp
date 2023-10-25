#include <iostream>
#include <omp.h>
#include <random>

using namespace std;

int main() {
    unsigned int seed = 0;
    default_random_engine re{seed};
    uniform_real_distribution<double> zero_to_one{0.0, 1.0};

    int n = 100000000; // Anzahl der zu generierenden Punkte
    int counter = 0; // Zähler für Punkte im ersten Quadranten eines Einheitskreises
    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int chunk_size = n / num_threads;
        int local_counter = 0;

        for (int i = 0; i < chunk_size; ++i) {
            double x = zero_to_one(re);
            double y = zero_to_one(re);
            if (x * x + y * y <= 1.0) {
                ++local_counter;
            }
        }

        #pragma omp atomic
        counter += local_counter;
    }

    double run_time = omp_get_wtime() - start_time;
    double pi = 4 * (double(counter) / n);

    cout << "pi: " << pi << endl;
    cout << "run_time: " << run_time << " s" << endl;
    cout << "n: " << n << endl;
}
