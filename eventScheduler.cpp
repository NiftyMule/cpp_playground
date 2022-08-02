#include <thread>
#include <atomic>
#include <iostream>

using namespace std;

template<typename T, typename... Args>
thread *scheduleEvent(int time, T func, Args&&... args) {
    return new thread([&, time]{
        this_thread::sleep_for(chrono::milliseconds{time});
        func(forward<Args>(args)...);
    });
}

void demo() {
    auto t = scheduleEvent(5000, []{cout << "hello world";});
    t->join();
}