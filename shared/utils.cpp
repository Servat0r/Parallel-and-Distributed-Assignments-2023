// Utility functions
#include <iostream>
#include <vector>
#include <mutex>

using namespace std;
mutex mut;

template <typename S>


void print_vector(const vector<S>& vect, string sep = ", "){
    lock_guard guard(mut);
    cout << "[";
    for (int i = 0; i < vect.size(); i++){
        if (i < vect.size() - 1) cout << vect[i] << sep;
        else cout << vect[i] << "]" << endl;
    }
}