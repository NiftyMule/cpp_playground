#include <chrono>
#include <iostream>

namespace memory {
    using namespace std;

    void writeArr(int arr[], size_t &size, int &step) {
        for (size_t i = 0; i < size; i += step) {
            arr[i] = 1;
        }
    }

    chrono::nanoseconds measure(int arr[], size_t &size, int &step) {
        auto before = chrono::steady_clock::now();
        writeArr(arr, size, step);
        auto after = chrono::steady_clock::now();

        chrono::nanoseconds diff = after - before;
        return diff;
    }

    void cleanUpCache() {
        auto *arr = new int[256 * 1024 * 1024];
        for (int i = 0; i < 256 * 1024 * 1024; ++i) {
            arr[i] = 1;
        }
    }

    void demo() {
        int step = 0;
        size_t size = 256 * 1024 * 1024;

        for (int i = 0; i < 32; ++i) {
            step += 2;
            int *arr = new int[size]{};

            auto before = chrono::steady_clock::now();
            writeArr(arr, size, step);
            auto after = chrono::steady_clock::now();

            chrono::nanoseconds diff = after - before;
            cout << "step size: " << step << " & " << step * 4 << "  Bytes,  " << diff.count() << " ns" << endl;

            delete[] arr;

            cleanUpCache();
        }
    }
}