#include <iomanip>
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int num_steps = 100000000; // Anzahl der Rechtecke
    double width = 1.0 / double(num_steps); // Breite eines Rechtecks
    double sum = 0.0; // Zur Summierung aller Höhen der Rechtecke
    double start_time = omp_get_wtime(); // Wanduhrzeit in Sekunden
    int num_threads = omp_get_max_threads(); // Die maximale Anzahl von Threads
    double pi = 0.0; // Variable zum Sammeln von Teilergebnissen

    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        int chunk_size = num_steps / num_threads;
        int start = thread_id * chunk_size;
        int end = (thread_id == num_threads - 1) ? num_steps : start + chunk_size;
        double local_sum = 0.0;

        for (int i = start; i < end; i++) {
            double x = (i + 0.5) * width;
            local_sum += 1.0 / (1.0 + x * x);
        }

        #pragma omp critical
        {
            sum += local_sum;
        }
    }

    pi = sum * 4 * width; // Pi berechnen

    double run_time = omp_get_wtime() - start_time;
    cout << "Pi mit " << num_steps << " Schritten beträgt " << setprecision(17)
         << pi << " in " << setprecision(6) << run_time << " Sekunden\n";
}

