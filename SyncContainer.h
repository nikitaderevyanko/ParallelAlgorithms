//
// Created by derevo on 21.03.16.
//
#pragma once
#ifndef CSYNCCONTAINER_SYNCCONTAINER_H
#define CSYNCCONTAINER_SYNCCONTAINER_H
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class CSyncContainer{
private:
    std::mutex syncMutex;
    std::condition_variable cond_var;
    bool notify = false;
    T container;
public:
    CSyncContainer() {}
    int size(){
        std::unique_lock<std::mutex> lock(syncMutex);
        return container.size();
    }
    void push(const typename T::value_type& elem){
        std::unique_lock <std::mutex> lock(syncMutex);
        container.push_back(elem);
        cond_var.notify_one();
    }
    typename T::value_type popOrSleep(){
        std::unique_lock<std::mutex> lock(syncMutex);
        while(container.size() == 0 && !notify){
            cond_var.wait(lock);
        }
        typename T::value_type lastElem = container.back();
        container.pop_back();
        notify = false;
        return lastElem;
    }
    bool popNoSleep(){
        std::unique_lock<std::mutex> lock(syncMutex);
        if(container.size() > 0){
            container.pop_back();
            notify = false;
            return true;
        }
        return false;
    }
};
#endif //CSYNCCONTAINER_SYNCCONTAINER_H
