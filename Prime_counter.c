#include <stdio.h>
        #include <stdlib.h>
        #include <pthread.h>
        #include <stdbool.h>
        #include <time.h>

        // Global shared counter and mutex
        int prime_count = 0;
        pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

        // Thread argument structure
        typedef struct {
            int start;
            int end;
        } thread_args_t;

        // Function to check if a number is prime
        bool is_prime(int n) {
            if (n <= 1) return false;
            if (n <= 3) return true;
            if (n % 2 == 0 || n % 3 == 0) return false;

            for (int i = 5; i * i <= n; i += 6) {
                if (n % i == 0 || n % (i + 2) == 0)
                    return false;
            }
            return true;
        }

        // Thread function to count primes in a range
        void* count_primes_in_range(void* arg) {
            thread_args_t* args = (thread_args_t*)arg;
            int local_count = 0;

            // Count primes in assigned range
            for (int i = args->start; i <= args->end; i++) {
                if (is_prime(i)) {
                    local_count++;
                }
            }

            // Update shared counter with mutex protection
            pthread_mutex_lock(&count_mutex);
            prime_count += local_count;
            pthread_mutex_unlock(&count_mutex);

            free(args);
            return NULL;
        }

        int main(int argc, char* argv[]) {
            if (argc != 3) {
                fprintf(stderr, "Usage: %s <upper_bound> <num_threads>\n", argv[0]);
                return 1;
            }

            int upper_bound = atoi(argv[1]);
            int num_threads = atoi(argv[2]);

            if (upper_bound <= 0 || num_threads <= 0) {
                fprintf(stderr, "Arguments must be positive integers\n");
                return 1;
            }

            // Start timing
            clock_t start_time = clock();

            pthread_t* threads = malloc(num_threads * sizeof(pthread_t));

            // Calculate range per thread
            int range_per_thread = upper_bound / num_threads;

            // Create threads
            for (int i = 0; i < num_threads; i++) {
                thread_args_t* args = malloc(sizeof(thread_args_t));
                args->start = i * range_per_thread + 1;
                args->end = (i == num_threads - 1) ? upper_bound : (i + 1) * range_per_thread;

                pthread_create(&threads[i], NULL, count_primes_in_range, args);
            }

            // Join all threads
            for (int i = 0; i < num_threads; i++) {
                pthread_join(threads[i], NULL);
            }

            // End timing
            clock_t end_time = clock();
            double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

            // Print results
            printf("Found %d prime numbers between 1 and %d\n", prime_count, upper_bound);
            printf("Execution time: %.4f seconds\n", elapsed);

            // Cleanup
            free(threads);
            pthread_mutex_destroy(&count_mutex);

            return 0;
        }
