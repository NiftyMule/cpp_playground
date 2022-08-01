#include <chrono>
#include <iostream>

namespace memory {
    using namespace std;

    void readArr(int arr[], size_t &size, int &step) {
        int j = 0;
        for (size_t i = 0; i < size; i += step) {
            j = arr[i];
        }
    }

    chrono::nanoseconds measure(int arr[], size_t &size, int &step) {
        auto before = chrono::steady_clock::now();
        readArr(arr, size, step);
        auto after = chrono::steady_clock::now();

        chrono::nanoseconds diff = after - before;
        return diff;
    }

    void cleanUpCache() {
        auto *arr = new int[4 * 1024 * 1024];
        for (int i = 0; i < 4 * 1024 * 1024; ++i) {
            arr[i] = 1;
        }
    }

    void demo() {
        int step = 1;
        size_t size = 4 * 1024 * 1024;

        for (int i = 0; i < 8; ++i) {
            step *= 2;
            int *arr = new int[size]{};

            auto before = chrono::steady_clock::now();
            readArr(arr, size, step);
            auto after = chrono::steady_clock::now();

            chrono::nanoseconds diff = after - before;
            cout << "step size: " << step * 4 << "  Bytes,  " << diff.count() << " ns" << endl;

            delete[] arr;

            cleanUpCache();
        }
    }
}