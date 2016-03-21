//
// Created by derevo on 21.03.16.
//
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"
#include "SyncContainer.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

void pushtimesv( CSyncContainer< std::vector<int> >& cont, int length){
    for(int i = 0; i < length; ++i){
        cont.push(i);
    }
}

void popStimesv( CSyncContainer< std::vector<int> >& cont, int length){
    for(int i = 0; i < length; ++i)
        cont.popOrSleep();
}

void popNotimesv( CSyncContainer< std::vector<int> >& cont, int length, int& fails){
    bool arg = true;
    fails = 0;
    for(int i = 0; i < length; ++i) {
        arg = cont.popNoSleep();
        if(!arg)
            fails++;
    }
}

void pushtimesl( CSyncContainer< std::list<int> >& cont, int length){
    for(int i = 0; i < length; ++i){
        cont.push(i);
    }
}

void popStimesl( CSyncContainer< std::list<int> >& cont ,int length){
    for(int i = 0; i < length; ++i)
        cont.popOrSleep();
}

void popNotimesl( CSyncContainer< std::list<int> >& cont ,int length, int& fails){
    bool arg = true;
    fails = 0;
    for(int i = 0; i < length; ++i) {
        arg = cont.popNoSleep();
        if(!arg)
            fails++;
    }
}

void pushtimesd( CSyncContainer< std::deque<int> >& cont, int length){
    for(int i = 0; i < length; ++i){
        cont.push(i);
    }
}

void popStimesd( CSyncContainer< std::deque<int> >& cont, int length){
    for(int i = 0; i < length; ++i)
        cont.popOrSleep();
}

void popNotimesd( CSyncContainer< std::deque<int> >& cont, int length, int& fails){
    bool arg = true;
    fails = 0;
    for(int i = 0; i < length; ++i) {
        arg = cont.popNoSleep();
        if(!arg)
            fails++;
    }
}

TEST(vector, small) {
    CSyncContainer< std::vector<int> > a;
    std::thread first(std::ref(pushtimesv), std::ref(a), 30);
    std::thread second(std::ref(popStimesv), std::ref(a), 20);

    first.join();
    second.join();

    EXPECT_EQ(10, a.size());
}

TEST(list, small) {
    CSyncContainer< std::list<int> > a;
    std::thread first(std::ref(pushtimesl), std::ref(a), 30);
    std::thread second(std::ref(popStimesl), std::ref(a), 20);

    first.join();
    second.join();

    EXPECT_EQ(10, a.size());
}

TEST(deque, small) {
    CSyncContainer< std::deque<int> > a;
    std::thread first(std::ref(pushtimesd), std::ref(a), 30);
    std::thread second(std::ref(popStimesd), std::ref(a), 20);

    first.join();
    second.join();

    EXPECT_EQ(10, a.size());
}

TEST(vector, bigtest) {
    CSyncContainer< std::vector<int> > a;
    std::thread first(std::ref(pushtimesv), std::ref(a), 30000);
    std::thread second(std::ref(popStimesv), std::ref(a), 20000);

    first.join();
    second.join();

    EXPECT_EQ(10000, a.size());
}

TEST(list, bigtest) {
    CSyncContainer< std::list<int> > a;
    std::thread first(std::ref(pushtimesl), std::ref(a), 30000);
    std::thread second(std::ref(popStimesl), std::ref(a), 20000);

    first.join();
    second.join();

    EXPECT_EQ(10000, a.size());
}

TEST(deque, bigtest) {
    CSyncContainer< std::deque<int> > a;
    std::thread second(std::ref(popStimesd), std::ref(a), 20000);
    std::thread first(std::ref(pushtimesd), std::ref(a), 30000);

    first.join();
    second.join();

    EXPECT_EQ(10000, a.size());
}

TEST(vector, smallWithNoW8) {
    int fails;
    CSyncContainer< std::vector<int> > a;
    std::thread f(std::ref(pushtimesv), std::ref(a), 15);
    std::thread s(std::ref(popNotimesv), std::ref(a), 10, std::ref(fails));
    std::thread first(std::ref(pushtimesv), std::ref(a), 30);
    std::thread second(std::ref(popStimesv), std::ref(a), 20);

    f.join();
    s.join();
    first.join();
    second.join();

    EXPECT_EQ(15, a.size() - fails);
}

TEST(list, smallWithNoW8) {
    int fails;
    CSyncContainer< std::list<int> > a;
    std::thread f(std::ref(pushtimesl), std::ref(a), 15);
    std::thread s(std::ref(popNotimesl), std::ref(a), 10, std::ref(fails));
    std::thread first(std::ref(pushtimesl), std::ref(a), 30);
    std::thread second(std::ref(popStimesl), std::ref(a), 20);

    f.join();
    s.join();
    first.join();
    second.join();

    EXPECT_EQ(15, a.size() - fails);
}

TEST(deque, smallWithNoW8) {
    int fails;
    CSyncContainer< std::deque<int> > a;
    std::thread f(std::ref(pushtimesd), std::ref(a), 15);
    std::thread s(std::ref(popNotimesd), std::ref(a), 10, std::ref(fails));
    std::thread first(std::ref(pushtimesd), std::ref(a), 30);
    std::thread second(std::ref(popStimesd), std::ref(a), 20);

    f.join();
    s.join();
    first.join();
    second.join();

    EXPECT_EQ(15, a.size() - fails);
}

TEST(vector, bigWithNoW8) {
    int fails;
    CSyncContainer< std::vector<int> > a;
    std::thread f(std::ref(pushtimesv), std::ref(a), 15000);
    std::thread s(std::ref(popNotimesv), std::ref(a), 10000, std::ref(fails));
    std::thread first(std::ref(pushtimesv), std::ref(a), 30000);
    std::thread second(std::ref(popStimesv), std::ref(a), 20000);

    f.join();
    s.join();
    first.join();
    second.join();

    EXPECT_EQ(15000, a.size() - fails);
}

TEST(list, bigWithNoW8) {
    int fails;
    CSyncContainer< std::list<int> > a;
    std::thread f(std::ref(pushtimesl), std::ref(a), 15000);
    std::thread s(std::ref(popNotimesl), std::ref(a), 10000, std::ref(fails));
    std::thread first(std::ref(pushtimesl), std::ref(a), 30000);
    std::thread second(std::ref(popStimesl), std::ref(a), 20000);

    f.join();
    s.join();
    first.join();
    second.join();

    EXPECT_EQ(15000, a.size() - fails);
}

TEST(deque, bigWithNoW8) {
    int fails;
    CSyncContainer< std::deque<int> > a;
    std::thread f(std::ref(pushtimesd), std::ref(a), 15000);
    std::thread s(std::ref(popNotimesd), std::ref(a), 10000, std::ref(fails));
    std::thread first(std::ref(pushtimesd), std::ref(a), 30000);
    std::thread second(std::ref(popStimesd), std::ref(a), 20000);

    f.join();
    s.join();
    first.join();
    second.join();

    EXPECT_EQ(15000, a.size() - fails);
}



