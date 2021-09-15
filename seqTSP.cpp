#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::string;
using std::istream;
using std::ifstream;
using namespace std::chrono;

const int SIZE = 20;

/// a path sequence
struct Route {
    Route() { length = INT_MAX; }

    /// a path ordering
    int permutation[SIZE];
    /// length of the path
    int length;

    /// output the length and route assuming starting at 0 and 0 not in the permutation
    /// @param n total number of points in the path
    void outputRoute(int n) {
        cout << "length: " << length << endl << "0 ";
        for (auto i = 0; i < n - 1; ++i) {
            cout << permutation[i] << " ";
        }
        cout << endl;
    }

    /// compare two routes and return true if length of left operand < length of right operand
    bool operator<(const Route& rhs) {
        return length < rhs.length;
    }
};

// make an array containing struct so it is copyable
struct PermutationArray {
    PermutationArray() {
        for (int i = 0; i < SIZE; ++i) {
            permutation[i] = i + 1;
        }
    }
    int permutation[SIZE];
};

// global so simpler to convet to thread version
// distances between each stop
static int distances[SIZE][SIZE];

//--------------------------------------------------------------------

/// swap's the variables and b
/// @param a variable to swap with b
/// @param b variable to swap with a
inline void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

/// finds shortest path starting at location 0 through each other point in permutation
/// @param p initial call p.permutation should hold {1, 2, 3, 4, ..., n-1)
/// @param i inital call should be passed 0
/// @param n initial call should be with one fewer than numbers in permutation array
void permute(PermutationArray p, int i, int n, Route& route) {

    if (i != (n - 1)) {
        for (auto j = i; j < n; ++j) {
            swap(p.permutation[i], p.permutation[j]);
            permute(p, i + 1, n, route);
            // backtrack
            swap(p.permutation[i], p.permutation[j]);
        }
    }
    else {
        // accumulator for path length
        int length = 0;
        // start path at index 0
        int from = 0;
        // add up length of path through the permutation
        for (auto k = 0; k < n; ++k) {
            auto to = p.permutation[k];
            length += distances[from][to];
            from = to;
        }
        // include the length back to 0
        length += distances[from][0];

        // if shorter, copy the path and update the length
        if (length < route.length) {
            memcpy(route.permutation, p.permutation, sizeof(int) * n);
            route.length = length;
        }
    }
}

int main(int argc, const char* argv[]) {
    int i, j, n;
    string fname;
    if (argc == 2) {
        fname = string(argv[1]);
    }
    else {
        fname = "C:\\Users\\diono\\OneDrive\\Desktop\\cs\\traveling salesman\\tsp.txt";
    }
    ifstream ifs;
    ifs.open(fname.c_str());

    // read file
    ifs >> n;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            ifs >> distances[i][j];
        }
    }

    high_resolution_clock::time_point start = high_resolution_clock::now();

    PermutationArray p;
    Route bestRoute;
    permute(p, 0, n - 1, bestRoute);

    high_resolution_clock::time_point stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();
    cout << "sequential time: " << duration / 1000000.0 << endl;

    bestRoute.outputRoute(n);

    return 0;
}