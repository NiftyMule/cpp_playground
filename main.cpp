#include "TokenBucket.cpp"

int main() {
    TokenBucketMultiThread::demo();
    TokenBucketNormal::demo();
    RateLimiterQueue::demo();
}