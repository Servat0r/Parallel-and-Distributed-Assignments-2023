#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <numeric>
#include <algorithm>
#include <cmath>
#include "shared/utimer.cpp"
#include "shared/utils.cpp"

using namespace std;

int square(int n){return n*n;}

void thread_body(vector<int> src, vector<int>* dest, int start, int end, int func(int) = &square){
    /**
     * @brief Computes the given function func for all the elements in src[start:end] and
     * stores the results in dest[src:end].
    */
   //if (start > end) throw invalid_argument("It must be: start <= end (start = " + to_string(start) + ", end = " + to_string(end) + ")");
   utimer body("Body: ");
   for (int i = start; i < end; i++) (*dest)[i] = func(src[i]);
}

void sequential_program(vector<int> v, bool print_vectors = false){
    vector<int> w(v.capacity(), 0);
    {
        utimer seq("Sequential program: ");
        thread_body(v, &w, 0, v.size());
    }
    if (print_vectors){
        print_vector(v);
        print_vector(w);
    }
}


void parallel_program(vector<int> v, int nthreads, bool print_vectors = false){
    if (nthreads <= 0) throw invalid_argument("Number of threads must be at least 1 (got " + to_string(nthreads) + ")");
    int base_length = (int)ceil((double)v.size()/nthreads);
    int start, end;
    vector<int> w(v.capacity(), 0);
    {
        vector<thread> workers(nthreads);
        utimer par("Parallel program: ");
        for (int i = 0; i < nthreads; i++){
            start = i * base_length;
            end = min((i+1)*base_length, (int)v.size());
            cout << "Thread " << "#" << i << " between " << start << " and " << end << endl;
            //workers.push_back(thread(thread_body, v, &w, start, end));
            workers[i] = thread(thread_body, v, &w, start, end, &square);
        }
        utimer j("Join: ");
        for (int i = 0; i < nthreads; i++) workers[i].join();
    }
    if (print_vectors){
        print_vector(v);
        print_vector(w);
    }
}


int main(int argc, char* argv[]){
    /**
     * @brief Usage: <filename> [-help] N seed nthreads print_vectors, where:
     * -help = Shows help message
     * N = number of elements in the vectors
     * seed = seed of the function used to fill the vectors
     * nthreads = number of threads to use for parallel program (0 for sequential program)
     * print_vectors = whether or not to print the resulting vectors at the end of the program
    */
    const int N = argc >= 2 ? stoi(argv[1]) : 100;
    const bool print_vectors = argc >= 3 ? bool(stoi(argv[2])) : false;
    const int nthreads = argc >= 4 ? stoi(argv[3]) : 4;
    vector<int> v(N);
    iota(v.begin(), v.end(), 1);
    utimer t("Program timer: ");
    sequential_program(v, print_vectors);
    parallel_program(v, nthreads, print_vectors);
    return 0;
}