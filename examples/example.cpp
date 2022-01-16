// Copyright Davydov Nikolay 2021

//#include "unique_guard.hpp"
#include <thread>
#include <mutex>
#include <iostream>

#include "unique_guard.hpp"

std::mutex mtx;

void foo() {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    my_unique_guard::unique_lock<std::mutex> lock(mtx);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << std::this_thread::get_id() << "\n";
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    my_unique_guard::unique_lock<std::mutex> lock2(mtx);
    std::cout << "END THREAD " << std::this_thread::get_id() << "\n";
}

int main() {
    std::thread t1(foo);
    std::thread t2(foo);
    std::thread t3(foo);
    std::thread t4(foo);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
