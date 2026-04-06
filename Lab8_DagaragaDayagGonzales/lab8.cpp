/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

April 6, 2026
*/

/*
We have not discussed the C++ language code and Bash scripting in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code and Bash scripting obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code and Bash scripting or documentation of either were used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <cctype>       // for isdigit(), in input validation
#include <pthread.h>    // for pthread_create, pthread_join, pthread_mutex_t, etc...
using namespace std;

// This mutex acts like a lock so that only one thread prints at a time.
pthread_mutex_t print_mutex;

/*
 * This struct holds the parameters we need to pass into each thread, since
 * pthread_create only lets us pass one argument. We also store the hit count
 * here so main() can read it after the thread finishes.
 */
struct ThreadParams {
    int thread_id;
    long long trialcount;
    long long progressreport;
    long long hits; // how many points landed within the circle
};

/*
 * This is the function each thread runs. It generates random (x, y) points
 * and checks if they fall inside the unit circle to estimate π. Each thread
 * has its own random number generator so they don't interfere with each other.
 */
void *monte_carlo_pi_calc(void* ptr) {
    ThreadParams* params = (ThreadParams*) ptr;
    
    // Random generator code
    // Each thread gets its own seeded generator
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<long double> distribution(-1.0L, 1.0L);

    long long hits = 0;

    for (long long t = 1; t <= params->trialcount; t++) {
        long double x = distribution(generator);
        long double y = distribution(generator);
        long double r = x * x + y * y;

        if (r <= 1.0L) hits++;

        if (t % params->progressreport == 0) {
            long double estpi = (long double) hits / (long double) t * 4.0L;

            // Lock before printing so the threads don't overwrite
            pthread_mutex_lock(&print_mutex);
            cout << "Thread " << params->thread_id << ": "
                 << hits << " / " << t
                 << " (current estimate: "
                 << fixed << setprecision(18) << estpi << ")" << endl;
            pthread_mutex_unlock(&print_mutex);
        }
    }

    // Store the final hit count back into the struct for main() to aggregate
    params->hits = hits;
    pthread_exit(NULL);
}

/*
 * main() validates the command line arguments, then creates and launches all
 * the threads. Once all threads are done, it adds up their hits and computes
 * the final π estimate from the combined results.
 */
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <num_threads> <trials_per_thread> <progress_report>" << endl;
        return 1;
    }

    // Checks if all characters in each argument are digits. Otherwise, return 0.
    for (int i = 1; i <= 3; i++) {
        for (int j = 0; argv[i][j] != '\0'; j++) {
            if (!isdigit(argv[i][j])) {
                cerr << "Error: all arguments must be positive integers." << endl;
                return 1;
            }
        }
    }

    int threadcount = atoi(argv[1]);
    long long trialcount = atoll(argv[2]);
    long long progressreport = atoll(argv[3]);

    // Other edge cases
    if (threadcount <= 0) {
        cerr << "Error: number of threads must be at least 1." << endl;
        return 1;
    }
    if (trialcount <= 0) {
        cerr << "Error: number of trials must be at least 1." << endl;
        return 1;
    }
    if (progressreport <= 0) {
        cerr << "Error: progress report interval must be at least 1 (cannot be 0, would cause division by zero)." << endl;
        return 1;
    }
    if (progressreport > trialcount) {
        cerr << "Warning: Current trial is not divisible by p. No progress lines will be printed." << endl;
    }

    pthread_mutex_init(&print_mutex, NULL);

    vector<pthread_t> threads(threadcount);
    vector<ThreadParams> params(threadcount);

    // Start all threads
    for (int i = 0; i < threadcount; i++) {
        params[i].thread_id = i;
        params[i].trialcount = trialcount;
        params[i].progressreport = progressreport;
        params[i].hits = 0;

        int ret = pthread_create(&threads[i], NULL, monte_carlo_pi_calc, (void*) &params[i]);
        if (ret) {
            cerr << "Error: pthread_create() return code: " << ret << endl;
            return 1;
        }
    }

    // Waits for all threads to finish before aggregating
    for (int i = 0; i < threadcount; i++) {
        pthread_join(threads[i], NULL);
    }

    // Aggregate the hits and total trials across all threads
    long long total_hits = 0;
    long long total_trials = (long long) threadcount * trialcount;
    for (int i = 0; i < threadcount; i++) {
        total_hits += params[i].hits;
    }

    // Computes the final π estimate from all threads combined
    long double final_pi = (long double) total_hits / (long double) total_trials * 4.0L;
    cout << "Final result: " << fixed << setprecision(17) << final_pi << endl;

    pthread_mutex_destroy(&print_mutex);
    return 0;
}
