#include <thread>
#include <iostream>
#include <atomic>
#include <chrono>

using namespace std;

template<typename T>
void measure(T&& func) {
    auto before = chrono::steady_clock::now();
    func();
    auto diff = chrono::steady_clock::now() - before;
    cout << "elapsed time: " << diff.count() << "ns" << endl;
}

class TokenBucketMultiThread {
public:
    explicit TokenBucketMultiThread(int rate)
        : mToken(0)
        , mRate(rate)
        , mThread(updateBucket, std::ref(mToken), std::ref(mRate), std::ref(mStopFlag)){}

    ~TokenBucketMultiThread() {
        mStopFlag.test_and_set();
        if (mThread.joinable()) mThread.join();
    }

    bool request() {
        if (mToken > 0) {
            mToken--;
            return true;
        }
        return false;
    }

    static void updateBucket(atomic<int> &token, int &rate, atomic_flag &stopFlag) {
        static auto interval = chrono::nanoseconds{1000000000 / rate};
        while (!stopFlag.test()) {
            this_thread::sleep_for(interval);
            if (token < rate) {
                token += 1;
            }
        }
    }

    static void demo() {
        cout << "Token Bucket MultiThread" << endl;
        TokenBucketMultiThread tb{100};
        cout << tb.mToken << endl;
        measure([&]{cout << boolalpha << tb.request() << endl;});
        cout << tb.mToken << endl;
        this_thread::sleep_for(chrono::milliseconds{1000});
        cout << tb.mToken << endl;
        measure([&]{cout << boolalpha << tb.request() << endl;});
        cout << tb.mToken << endl;
    }

    // how many requests can pass in a second
    atomic<int> mToken;
    atomic_flag mStopFlag = ATOMIC_FLAG_INIT;
    int mRate;
    thread mThread;
};

class TokenBucketNormal {
public:
    explicit TokenBucketNormal(int rate)
        : mToken(0)
        , mRate(rate)
        , mIntervalNs(1000000000 / rate)
        , mLastRequest(chrono::steady_clock::now()){}

    bool request() {
        auto now = chrono::steady_clock::now();
        chrono::nanoseconds diff = now - mLastRequest;

        mLastRequest = now - chrono::nanoseconds(diff.count() % mIntervalNs);

        mToken += diff.count() / mIntervalNs;
        mToken = mToken > mRate ? mRate : mToken;

        if (mToken > 0) {
            mToken--;
            return true;
        }
        return false;
    }

    static void demo() {
        cout << "Token Bucket Normal" << endl;
        TokenBucketNormal tb{100};
        cout << tb.mToken << endl;
        measure([&]{cout << boolalpha << tb.request() << endl;});
        cout << tb.mToken << endl;
        this_thread::sleep_for(chrono::milliseconds{1000});
        cout << tb.mToken << endl;
        measure([&]{cout << boolalpha << tb.request() << endl;});
        cout << tb.mToken << endl;
    }

    long long mToken;
    int mRate;
    int mIntervalNs;
    chrono::steady_clock::time_point mLastRequest;
};

class RateLimiterQueue {
public:
    explicit RateLimiterQueue(int rate)
        : mRate(rate)
        , index(0) {
        mRequestTime = new long long[rate]{};
    }

    bool request() {
        auto now = chrono::steady_clock::now().time_since_epoch().count();
        if (now - mRequestTime[index] > 1000000000) {
            mRequestTime[index++] = now;
            return true;
        } else {
            return false;
        }
    }

    static void demo() {
        cout << "Rate limiter queue" << endl;
        RateLimiterQueue tb{100};
        measure([&]{cout << boolalpha << tb.request() << endl;});
        this_thread::sleep_for(chrono::milliseconds{1000});
        measure([&]{cout << boolalpha << tb.request() << endl;});
    }

    int mRate;
    int index;
    long long *mRequestTime;
};