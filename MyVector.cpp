
#include <cstdlib>
#include <iostream>

namespace vector {
    template<typename T, size_t CAPACITY>
    class MyVector {
    private:
        size_t mSize;
        T mArr[CAPACITY];
    public:
        MyVector() : mSize(0) {};

        ~MyVector() = default;

        T &operator[](int index) {
            if (index >= mSize) {
                throw std::exception();
            }
            return mArr[index];
        }

        void push_back(T &elem) {
            if (mSize == CAPACITY) {
                throw std::exception();
            }
            mArr[mSize++] = elem;
        }

        void push_back(T &&elem) {
            if (mSize == CAPACITY) {
                throw std::exception();
            }
            mArr[mSize++] = std::move(elem);
        }

        template<typename... Args>
        void emplace_back(Args &&... args) {
            if (mSize == CAPACITY) {
                throw std::exception();
            }
            mArr[mSize++] = T(std::forward<Args...>(args)...);
        }

        static void demo() {
            MyVector<int, 50> vec;
            vec.push_back(1);
            int a = 2;
            vec.push_back(a);
            std::cout << vec[0] << " " << vec[1] << std::endl;
        }
    };
}