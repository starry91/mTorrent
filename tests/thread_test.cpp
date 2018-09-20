#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <deque>
#include <functional>
#include <chrono>

void print(int i)
{
    std::cout << "Hello " << i << std::endl;
}

int main()
{
    // queue a bunch of "work items"
    int i = 0;
    while(true)//for (int i = 0; i < 8; ++i)
    {
        std::thread t1(print, i);
        t1.detach();
        std::cout << "Hello thread" << std::endl;       
        i++;
    }
    std::cout << "Helloending" << std::endl;
    return 0;
}