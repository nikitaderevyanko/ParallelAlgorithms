#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

class futex{
    std::atomic<int> fut;
public:
    futex(){
        fut.store(0);
    }
    void lock( int curid ){
        int x = 0;
        while(!fut.compare_exchange_weak(x, curid)){
            std::this_thread::yield();
        }
    }
    void unlock( int curid ){
        while(!fut.compare_exchange_weak(curid, 0));
    }
};

void incr(int thr_id, long potolok, long& a, futex& myfutex){
    bool flag = false;
    int counter = 0;
    while(1) {
        myfutex.lock(thr_id);
        if(a >= potolok) {
            //cout << "Thread:" << thr_id << ".The number of incr:" << counter << std::endl;
            myfutex.unlock(thr_id);
            return;
        }
        a++;
        counter++;
        myfutex.unlock(thr_id);
    }
}

void increm(int thr_id, long potolok, long& a, mutex& mymutex){
    bool flag = false;
    int counter = 0;
    while(1) {
        mymutex.lock();
        if(a >= potolok) {
            //cout << "Thread:" << thr_id << ".The number of incr:" << counter << std::endl;
            mymutex.unlock();
            return;
        }
        a++;
        counter++;
        mymutex.unlock();
    }
}


int main() {
    futex myfutex;
    mutex mymutex;
    int thrNumber = std::thread::hardware_concurrency() / 2;
    long a = 0;

    auto start = std::chrono::high_resolution_clock::now();

    vector<thread> v;
    for(int i = 1; i <= thrNumber; ++i)
        v.push_back(std::thread(incr, i, 10000000, std::ref(a), std::ref<futex>(myfutex)));
    for(int i = 0; i < thrNumber; ++i)
        v[i].join();

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    cout << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();

    vector<thread> v2;
    long b = 0;
    for(int i = 1; i <= thrNumber; ++i)
        v2.push_back(std::thread(increm, i, 10000000, std::ref(b), std::ref<mutex>(mymutex)));
    for(int i = 0; i < thrNumber; ++i)
        v2[i].join();

    auto elapsed2 = std::chrono::high_resolution_clock::now() - start2;
    cout << std::chrono::duration_cast<std::chrono::microseconds>(elapsed2).count() << std::endl;
    return 0;
}